#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <linux/uinput.h>

static inline int clampi(int v, int lo, int hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static int write_full(int fd, const void *buf, size_t len) {
    const unsigned char *p = (const unsigned char *)buf;
    while (len) {
        ssize_t w = write(fd, p, len);
        if (w < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        p += (size_t)w;
        len -= (size_t)w;
    }
    return 0;
}

static int setup_uinput(int fu, const char *devname, const struct input_absinfo *ax, const struct input_absinfo *ay) {
    if (ioctl(fu, UI_SET_EVBIT, EV_SYN) < 0) return -1;
    if (ioctl(fu, UI_SET_EVBIT, EV_KEY) < 0) return -1;
    if (ioctl(fu, UI_SET_EVBIT, EV_ABS) < 0) return -1;

    if (ioctl(fu, UI_SET_KEYBIT, BTN_TOUCH) < 0) return -1;

    if (ioctl(fu, UI_SET_ABSBIT, ABS_X) < 0) return -1;
    if (ioctl(fu, UI_SET_ABSBIT, ABS_Y) < 0) return -1;

    struct uinput_user_dev uidev;
    memset(&uidev, 0, sizeof(uidev));

    snprintf(uidev.name, sizeof(uidev.name), "touchyfix-%s", devname);
    uidev.id.bustype = BUS_VIRTUAL;
    uidev.id.vendor = 0x1234;
    uidev.id.product = 0x5678;
    uidev.id.version = 1;

    uidev.absmin[ABS_X] = ax->minimum;
    uidev.absmax[ABS_X] = ax->maximum;
    uidev.absmin[ABS_Y] = ay->minimum;
    uidev.absmax[ABS_Y] = ay->maximum;

    if (write_full(fu, &uidev, sizeof(uidev)) < 0) return -1;
    if (ioctl(fu, UI_DEV_CREATE) < 0) return -1;

    return 0;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        return 2;
    }

    const char *devpath = argv[1];
    int y_delta = atoi(argv[2]);

    int fd_in = open(devpath, O_RDONLY);
    if (fd_in < 0) {
        return 3;
    }

    struct input_absinfo ax, ay;
    if (ioctl(fd_in, EVIOCGABS(ABS_X), &ax) < 0) {
        close(fd_in);
        return 4;
    }
    if (ioctl(fd_in, EVIOCGABS(ABS_Y), &ay) < 0) {
        close(fd_in);
        return 4;
    }

    if (ioctl(fd_in, EVIOCGRAB, (void *)1) < 0) {
        close(fd_in);
        return 5;
    }

    int fd_u = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd_u < 0) {
        ioctl(fd_in, EVIOCGRAB, (void *)0);
        close(fd_in);
        return 6;
    }

    if (setup_uinput(fd_u, devpath, &ax, &ay) < 0) {
        ioctl(fd_u, UI_DEV_DESTROY);
        close(fd_u);
        ioctl(fd_in, EVIOCGRAB, (void *)0);
        close(fd_in);
        return 7;
    }

    struct input_event ev;
    while (read(fd_in, &ev, sizeof(ev)) == (ssize_t)sizeof(ev)) {
        if (ev.type == EV_ABS) {
            if (ev.code == ABS_Y) {
                ev.value = clampi(ev.value + y_delta, ay.minimum, ay.maximum);
            } else if (ev.code == ABS_X) {
                ev.value = clampi(ev.value, ax.minimum, ax.maximum);
            }
        }

        if (write_full(fd_u, &ev, sizeof(ev)) < 0) {
            break;
        }
    }

    ioctl(fd_u, UI_DEV_DESTROY);
    ioctl(fd_in, EVIOCGRAB, (void *)0);
    close(fd_u);
    close(fd_in);
    return 0;
}
