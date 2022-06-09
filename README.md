# LVGL for frame buffer device

LVGL configured to work with /dev/fb0 on Linux.

When cloning this repository, also make sure to download submodules (`git submodule update --init --recursive`) otherwise you will be missing key components.

Check out this blog post for a step by step tutorial:
https://blog.lvgl.io/2018-01-03/linux_fb

# Note

For capacitive touchscreen, LVGL uses the evdev driver to read mouse pointer position, you should clone the following repository (deprecated)
https://github.com/symfund/lv_port_linux_frame_buffer.git

For resistive touchscreen, use this repository lv-port-ma35d1.
