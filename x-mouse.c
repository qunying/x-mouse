// clang-format off

// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: Copyright (C) 2024, Zhu, Qun-Ying <zhu.qunying*gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

// clang-format on
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>

#include <windef.h>
#include <winuser.h>

typedef struct x_mouse_st {
    bool  win_tracking;
    bool  win_raising;
    DWORD delay;
} x_mouse_t;

static void        get_current_info(x_mouse_t *st);
static inline void print_current_info(x_mouse_t *st);
static void        print_usage(void);

int
main(int argc, char *argv[])
{
    struct option opts[] = {
        {"help", no_argument, NULL, 'h'},
        {"win-tracking", required_argument, NULL, 1000},
        {"win-raising", required_argument, NULL, 1001},
        {"delay", required_argument, NULL, 1002},
        {NULL, 0, NULL, 0},
    };
    int opt;
    int opt_index;

    x_mouse_t xm_st = {
        .win_tracking = false,
        .win_raising  = false,
    };
    x_mouse_t xm_opt;

    get_current_info(&xm_st);
    xm_opt = xm_st;

    while ((opt = getopt_long(argc, argv, "h?", opts, &opt_index)) != -1) {
        switch (opt) {
        case 'h':
        case '?':
            print_usage();
            exit(0);
            break;
        case 1000:
            if (!strcmp(optarg, "on")) {
                xm_opt.win_tracking = true;
            } else if (!strcmp(optarg, "off")) {
                xm_opt.win_tracking = false;
            } else {
                printf("invalid argument for %s: %s\n",
                       opts[opt_index].name,
                       optarg);
                exit(EXIT_FAILURE);
            }
            break;
        case 1001:
            if (!strcmp(optarg, "on")) {
                xm_opt.win_raising = true;
            } else if (!strcmp(optarg, "off")) {
                xm_opt.win_raising = false;
            } else {
                printf("invalid argument for %s: %s\n",
                       opts[opt_index].name,
                       optarg);
                exit(EXIT_FAILURE);
            }
            break;
        case 1002: {
            long  val;
            char *endptr;
            val = strtol(optarg, &endptr, 10);
            if ((errno == ERANGE && (val == LONG_MIN || val == LONG_MAX))
                || val < 0 || val > 3000) {
                printf("invalid argument for %s: %s\n",
                       opts[opt_index].name,
                       optarg);
                exit(EXIT_FAILURE);
            }
            xm_opt.delay = (DWORD)val;
            break;
        }
        default:
            printf("unknown arguement %s\n", optarg);
            exit(EXIT_FAILURE);
            break;
        } /* switch */
    } /* while */
    if (memcmp(&xm_opt, &xm_st, sizeof(xm_st))) {
        // some changes given, set it properly
        BOOL  on = TRUE;
        PVOID val;
        if (xm_opt.win_tracking != xm_st.win_tracking) {
            val = xm_opt.win_tracking ? &on : NULL;
            (void)!SystemParametersInfoW(SPI_SETACTIVEWINDOWTRACKING,
                                         0,
                                         val,
                                         SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
        }
        if (xm_opt.win_raising != xm_st.win_raising) {
            val = xm_opt.win_raising ? &on : NULL;
            (void)!SystemParametersInfoW(SPI_SETACTIVEWNDTRKZORDER,
                                         0,
                                         val,
                                         SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
        }
        if (xm_opt.delay != xm_st.delay) {
            val = (LPVOID)(uintptr_t)xm_opt.delay;
            (void)!SystemParametersInfoW(SPI_SETACTIVEWNDTRKTIMEOUT,
                                         0,
                                         val,
                                         SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
        }
        get_current_info(&xm_st);
    }
    print_current_info(&xm_st);

    return 0;
}

static void
get_current_info(x_mouse_t *st)
{
    BOOL ret;
    BOOL value = FALSE;

    ret = SystemParametersInfoW(SPI_GETACTIVEWINDOWTRACKING, 0, &value, 0);
    if (ret == TRUE) {
        st->win_tracking = value == TRUE ? true : false;
    }
    ret = SystemParametersInfoW(SPI_GETACTIVEWNDTRKZORDER, 0, &value, 0);
    if (ret == TRUE) {
        st->win_raising = value == TRUE ? true : false;
    }
    SystemParametersInfoW(SPI_GETACTIVEWNDTRKTIMEOUT, 0, &st->delay, 0);
}

static void
print_current_info(x_mouse_t *st)
{
    printf("Window tracking: %s\n"
           "Window raising : %s\n"
           "Tracking Delay : %lums\n",
           st->win_tracking ? "on" : "off",
           st->win_raising ? "on" : "off",
           st->delay);
}
static void
print_usage(void)
{
    puts("Usage: x-mouse [arguments]\n"
         "   --win-tracking on/off   Set Windows tracking on/off.\n"
         "   --win-raising  on/off   Set Windows raising on/off.\n"
         "   --delay        NUM      Set Windows tracking delay "
         "(0~3000ms).\n"
         "\n"
         "-h/?,--help                This help.");
}
/* vim: set ts=8 sts=4 sw=4 smarttab expandtab spell : */
