#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <linux/uinput.h>

#define clampi(v, lo, hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))
#define BUF_SIZE 64

static int write_full(int fd, const void *buf, size_t len)
{
    const unsigned char *p;
    ssize_t w;

    p = (const unsigned char *)buf;
    while (len) {
        w = write(fd, p, len);
        if (w < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        p += (size_t)w;
        len -= (size_t)w;
    }
    return 0;
}

static int setup_uinput(int fd_u, const char *devname,
                        int x_min, int x_max, int y_min, int y_max)
{
    struct uinput_user_dev uidev;

    if (ioctl(fd_u, UI_SET_EVBIT, EV_SYN) < 0) return -1;
    if (ioctl(fd_u, UI_SET_EVBIT, EV_KEY) < 0) return -1;
    if (ioctl(fd_u, UI_SET_EVBIT, EV_ABS) < 0) return -1;
    if (ioctl(fd_u, UI_SET_KEYBIT, BTN_TOUCH) < 0) return -1;
    if (ioctl(fd_u, UI_SET_ABSBIT, ABS_X) < 0) return -1;
    if (ioctl(fd_u, UI_SET_ABSBIT, ABS_Y) < 0) return -1;

    memset(&uidev, 0, sizeof(uidev));
    snprintf(uidev.name, sizeof(uidev.name), "calibrated-%s", devname);
    uidev.id.bustype = BUS_VIRTUAL;
    uidev.id.vendor = 0x1234;
    uidev.id.product = 0x5678;
    uidev.id.version = 1;
    uidev.absmin[ABS_X] = x_min;
    uidev.absmax[ABS_X] = x_max;
    uidev.absmin[ABS_Y] = y_min;
    uidev.absmax[ABS_Y] = y_max;

    if (write_full(fd_u, &uidev, sizeof(uidev)) < 0) return -1;
    if (ioctl(fd_u, UI_DEV_CREATE) < 0) return -1;

    return 0;
}

int main(int argc, char **argv)
{
    const char *devpath;
    const char *mode;
    int fd_in;
    int fd_u;
    int use_affine;
    int use_delta;
    long post_off;
    int out_h;
    int pad_top;
    int pad_bottom;
    int delta;
    int raw_min;
    int raw_max;
    struct input_absinfo abs_x;
    struct input_absinfo abs_y;
    int have_x;
    int have_y;
    int x_min;
    int x_max;
    int y_min;
    int y_max;
    int y_min_out;
    int y_max_out;
    long a_num;
    long a_den;
    long b;
    struct input_event buf[BUF_SIZE];
    ssize_t r;
    size_t nev;
    size_t i;

    if (argc < 3) return 2;

    devpath = argv[1];
    mode = argv[2];
    fd_in = open(devpath, O_RDONLY);
    if (fd_in < 0) return 3;

    use_affine = 0;
    use_delta = 0;
    post_off = 0;
    out_h = 800;
    pad_top = 10;
    pad_bottom = 10;
    delta = 0;
    raw_min = 0;
    raw_max = 65535;

    if (strcmp(mode, "delta") == 0) {
        if (argc < 4) {
            close(fd_in);
            return 2;
        }
        delta = atoi(argv[3]);
        use_delta = 1;
    } else if (strcmp(mode, "affine_auto") == 0) {
        struct input_absinfo absy;

        if (argc < 6) {
            close(fd_in);
            return 2;
        }
        out_h = atoi(argv[3]);
        pad_top = atoi(argv[4]);
        pad_bottom = atoi(argv[5]);
        if (argc >= 7) post_off = atol(argv[6]);
        if (ioctl(fd_in, EVIOCGABS(ABS_Y), &absy) < 0) {
            close(fd_in);
            return 4;
        }
        raw_min = absy.minimum;
        raw_max = absy.maximum;
        use_affine = 1;
    } else if (strcmp(mode, "affine_fixed") == 0) {
        if (argc < 8) {
            close(fd_in);
            return 2;
        }
        out_h = atoi(argv[3]);
        pad_top = atoi(argv[4]);
        pad_bottom = atoi(argv[5]);
        raw_min = atoi(argv[6]);
        raw_max = atoi(argv[7]);
        if (argc >= 9) post_off = atol(argv[8]);
        use_affine = 1;
    } else {
        close(fd_in);
        return 2;
    }

    have_x = (ioctl(fd_in, EVIOCGABS(ABS_X), &abs_x) == 0);
    have_y = (ioctl(fd_in, EVIOCGABS(ABS_Y), &abs_y) == 0);
    x_min = have_x ? abs_x.minimum : 0;
    x_max = have_x ? abs_x.maximum : 1280;
    y_min = have_y ? abs_y.minimum : 0;
    y_max = have_y ? abs_y.maximum : 65535;

    if (use_affine) {
        if (raw_max <= raw_min) {
            close(fd_in);
            return 5;
        }
    }

    if (ioctl(fd_in, EVIOCGRAB, (void *)1) < 0) {
        close(fd_in);
        return 5;
    }

    fd_u = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd_u < 0) {
        ioctl(fd_in, EVIOCGRAB, (void *)0);
        close(fd_in);
        return 6;
    }

    if (use_affine) {
        y_min_out = 0;
        y_max_out = out_h;
    } else {
        y_min_out = y_min;
        y_max_out = y_max;
    }

    if (setup_uinput(fd_u, devpath, x_min, x_max, y_min_out, y_max_out) < 0) {
        ioctl(fd_u, UI_DEV_DESTROY);
        close(fd_u);
        ioctl(fd_in, EVIOCGRAB, (void *)0);
        close(fd_in);
        return 7;
    }

    a_num = 1;
    a_den = 1;
    b = 0;
    if (use_affine) {
        a_num = (long)(out_h - pad_top - pad_bottom);
        a_den = (long)(raw_max - raw_min);
        b = (long)pad_top - (a_num * (long)raw_min) / a_den;
    }

    while (1) {
        r = read(fd_in, buf, sizeof(buf));
        if (r <= 0) break;
        if (r % (ssize_t)sizeof(struct input_event) != 0) continue;

        nev = (size_t)r / sizeof(struct input_event);
        for (i = 0; i < nev; i++) {
            struct input_event *ev;

            ev = &buf[i];
            if (ev->type == EV_ABS) {
                if (ev->code == ABS_Y) {
                    if (use_affine) {
                        long raw;
                        long scaled;
                        int v;

                        raw = (long)ev->value;
                        scaled = (a_num * raw) / a_den + b + post_off;
                        v = (int)scaled;
                        v = clampi(v, 0, out_h);
                        ev->value = v;
                    } else if (use_delta) {
                        int val;

                        val = ev->value + delta;
                        val = clampi(val, y_min, y_max);
                        ev->value = val;
                    }
                } else if (ev->code == ABS_X) {
                    int vx;

                    vx = ev->value;
                    vx = clampi(vx, x_min, x_max);
                    ev->value = vx;
                }
            }
        }

        if (write_full(fd_u, buf, (size_t)r) < 0) break;
    }

    ioctl(fd_u, UI_DEV_DESTROY);
    ioctl(fd_in, EVIOCGRAB, (void *)0);
    close(fd_u);
    close(fd_in);
    return 0;
}
