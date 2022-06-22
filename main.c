#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#if USE_TSLIB
#include "lv_drivers/indev/tslib_drv.h"
#endif
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <poll.h>
#include <limits.h>
#include <errno.h>

#define DISP_BUF_SIZE (128 * 1024)

int main(void)
{
#if USE_WAYLAND
    struct pollfd pfd;
    uint32_t time_till_next;
    int sleep;
    lv_disp_t *disp;
    lv_obj_t *label;
#endif

    /*LittlevGL init*/
    lv_init();

#if USE_WAYLAND
    lv_wayland_init();

    disp = lv_wayland_create_window(320, 240, "MA35D1 SOM Board", NULL);
	
    label = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);     /*Break the long lines*/
    lv_label_set_recolor(label, true);                      /*Enable re-coloring by commands in the text*/
    lv_label_set_text(label, "#0000ff Re-color# #ff00ff words# #ff0000 of a# label, align the lines to the center "
                      "and wrap long text automatically.\n\n LVGL with Wayland on MA35D1, brought to you by Nuvoton.");
    lv_obj_set_width(label, 150);  /*Set smaller width to make the lines wrap*/
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -40);

    while (1) {
        time_till_next = lv_wayland_timer_handler();
        
        /* check that there's still a window to wait on */
        if (!lv_wayland_window_is_open(NULL)) {
            break;
        }
        
        /* Wait for something interesting to happen */
        if (time_till_next == LV_NO_TIMER_READY) {
            sleep = -1;
        } else if (time_till_next > INT_MAX) {
            sleep = INT_MAX;
        } else {
            sleep = time_till_next;
        }
        
        while ((poll(&pfd, 1, sleep) < 0) && (errno == EINTR));
    }
    
    lv_wayland_deinit();
	
#else
    /*Linux frame buffer device init*/
    fbdev_init();

    /*A small buffer for LittlevGL to draw the screen's content*/
    static lv_color_t buf[DISP_BUF_SIZE];

    /*Initialize a descriptor for the buffer*/
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    /*Initialize and register a display driver*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &disp_buf;
    disp_drv.flush_cb   = fbdev_flush;
    disp_drv.hor_res    = 800;
    disp_drv.ver_res    = 480;
    lv_disp_drv_register(&disp_drv);

#if (USE_TSLIB)
    tslib_init();
#else
    evdev_init();
#endif

    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;

    /*This function will be called periodically (by the library) to get the mouse position and state*/
#if (USE_TSLIB)
    indev_drv_1.read_cb = tslib_read;
#else
    indev_drv_1.read_cb = evdev_read;
#endif

    lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);


    /*Set a cursor for the mouse*/
    LV_IMG_DECLARE(mouse_cursor_icon)
    lv_obj_t * cursor_obj = lv_img_create(lv_scr_act()); /*Create an image object for the cursor */
    lv_img_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
    lv_indev_set_cursor(mouse_indev, cursor_obj);             /*Connect the image  object to the driver*/


    /*Create a Demo*/
    lv_demo_widgets();

    /*Handle LitlevGL tasks (tickless mode)*/
    while(1) {
        lv_timer_handler();
        usleep(5000);
    }

#endif


    return 0;
}

/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}
