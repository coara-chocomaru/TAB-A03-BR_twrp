#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <sys/types.h>
#include <sys/stat.h>

#define clampi(v, lo, hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))
#define BUF_SIZE 64

int main(int argc, char **argv) {
    if (argc < 3) return 2;
    const char *devpath = argv[1];
    const char *mode = argv[2];
    int fd_in = open(devpath, O_RDONLY);
    if (fd_in < 0) return 3;

    int use_affine = 0;
    int use_delta = 0;
    long post_off = 0;
    int out_h = 800;
    int pad_top = 10;
    int pad_bottom = 10;
    int delta = 0;
    int raw_min = 0, raw_max = 65535;

    if (strcmp(mode, "delta") == 0) {
        if (argc < 4) { close(fd_in); return 2; }
        delta = atoi(argv[3]);
        use_delta = 1;
    } else if (strcmp(mode, "affine_auto") == 0) {
        if (argc < 6) { close(fd_in); return 2; }
        out_h = atoi(argv[3]);
        pad_top = atoi(argv[4]);
        pad_bottom = atoi(argv[5]);
        if (argc >= 7) post_off = atol(argv[6]);
        struct input_absinfo absy;
        if (ioctl(fd_in, EVIOCGABS(ABS_Y), &absy) < 0) { close(fd_in); return 4; }
        raw_min = absy.minimum;
        raw_max = absy.maximum;
        use_affine = 1;
    } else if (strcmp(mode, "affine_fixed") == 0) {
        if (argc < 8) { close(fd_in); return 2; }
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

    struct input_absinfo abs_x, abs_y;
    int have_x = (ioctl(fd_in, EVIOCGABS(ABS_X), &abs_x) == 0);
    int have_y = (ioctl(fd_in, EVIOCGABS(ABS_Y), &abs_y) == 0);
    int x_min = have_x ? abs_x.minimum : 0;
    int x_max = have_x ? abs_x.maximum : 1280;
    int y_min = have_y ? abs_y.minimum : 0;
    int y_max = have_y ? abs_y.maximum : 65535;

    if (use_affine) {
        if (raw_max <= raw_min) { close(fd_in); return 5; }
    }

    if (ioctl(fd_in, EVIOCGRAB, (void*)1) < 0) {
        close(fd_in);
        return 5;
    }

    int fd_u = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd_u < 0) {
        ioctl(fd_in, EVIOCGRAB, (void*)0);
        close(fd_in);
        return 6;
    }

    ioctl(fd_u, UI_SET_EVBIT, EV_SYN);
    ioctl(fd_u, UI_SET_EVBIT, EV_KEY);
    ioctl(fd_u, UI_SET_EVBIT, EV_ABS);
    ioctl(fd_u, UI_SET_KEYBIT, BTN_TOUCH);
    ioctl(fd_u, UI_SET_ABSBIT, ABS_X);
    ioctl(fd_u, UI_SET_ABSBIT, ABS_Y);

    struct uinput_user_dev uidev;
    memset(&uidev, 0, sizeof(uidev));
    snprintf(uidev.name, sizeof(uidev.name), "calibrated-combined-%s", devpath);
    uidev.id.bustype = BUS_VIRTUAL;
    uidev.id.vendor = 0x1234;
    uidev.id.product = 0x5678;
    uidev.absmin[ABS_X] = x_min;
    uidev.absmax[ABS_X] = x_max;
    if (use_affine) {
        uidev.absmin[ABS_Y] = 0;
        uidev.absmax[ABS_Y] = out_h;
    } else {
        uidev.absmin[ABS_Y] = y_min;
        uidev.absmax[ABS_Y] = y_max;
    }

    if (write(fd_u, &uidev, sizeof(uidev)) < 0) {
        ioctl(fd_in, EVIOCGRAB, (void*)0);
        close(fd_u);
        close(fd_in);
        return 7;
    }
    if (ioctl(fd_u, UI_DEV_CREATE) < 0) {
        ioctl(fd_in, EVIOCGRAB, (void*)0);
        close(fd_u);
        close(fd_in);
        return 7;
    }

    long a_num = 1;
    long a_den = 1;
    long b = 0;
    if (use_affine) {
        a_num = (out_h - pad_top - pad_bottom);
        a_den = (raw_max - raw_min);
        b = pad_top - (a_num * raw_min) / a_den;
    }

    struct input_event buf[BUF_SIZE];
    ssize_t r;
    while (1) {
        r = read(fd_in, buf, sizeof(buf));
        if (r <= 0) break;
        if (r % sizeof(struct input_event) != 0) continue;
        size_t nev = r / sizeof(struct input_event);
        for (size_t i = 0; i < nev; i++) {
            struct input_event *ev = &buf[i];
            if (ev->type == EV_ABS) {
                if (ev->code == ABS_Y) {
                    if (use_affine) {
                        long raw = ev->value;
                        long scaled = (a_num * raw) / a_den + b + post_off;
                        int v = (int)scaled;
                        v = clampi(v, 0, out_h);
                        ev->value = v;
                    } else if (use_delta) {
                        int val = ev->value + delta;
                        val = clampi(val, y_min, y_max);
                        ev->value = val;
                    }
                } else if (ev->code == ABS_X) {
                    int vx = ev->value;
                    vx = clampi(vx, x_min, x_max);
                    ev->value = vx;
                }
            }
        }
        write(fd_u, buf, r);
    }

    ioctl(fd_u, UI_DEV_DESTROY);
    ioctl(fd_in, EVIOCGRAB, (void*)0);
    close(fd_u);
    close(fd_in);
    return 0;
}
