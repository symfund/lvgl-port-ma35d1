# LVGL for frame buffer device

LVGL configured to work with /dev/fb0 on Linux.

When cloning this repository, also make sure to download submodules (`git submodule update --init --recursive`) otherwise you will be missing key components.

Check out this blog post for a step by step tutorial:
https://blog.lvgl.io/2018-01-03/linux_fb

# Note

For capacitive touchscreen, LVGL uses the evdev driver to read mouse pointer position, you can still use the deprecated repository
https://github.com/symfund/lv_port_linux_frame_buffer.git

For resistive and capacitive touchscreens, use this repository lv-port-ma35d1 instead.

To get driver implementation for the resistive touchscreen, execute the following command on the command line
`git submodule update --init --recursive --remote`

# Compiling LVGL in Buildroot via internal toolchain
1. Add GCC cross toolchain path into the environment variable PATH

   `$ export PATH=${BR2_DIR}/output/host/usr/bin:$PATH`
   
2. Launch the make command to compile LVGL

   `$ make CC=aarch64-nuvoton-linux-gnu-gcc LDFLAGS="-lts $LDFLAGS"`
   
3. To deploy the LVGL demo, copy the out executable binary file 'demo' to target device.
 
