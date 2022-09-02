# LVGL for MA35D1 (AARCH64)

For capacitive touchscreen, LVGL uses the evdev driver to talk to touch panel. However, it is still possible to use the deprecated repository
https://github.com/symfund/lv_port_linux_frame_buffer.git

For resistive and capacitive touchscreens, use this repository lv-port-ma35d1 instead.

To get driver implementation for the resistive touchscreen, execute the command
`git submodule update --init --recursive --remote`

# Compiling LVGL with tslib backend for resistive touchscreen through the internal toolchain generated by buildroot
1. Add GCC cross toolchain path into the environment variable PATH

   `$ export PATH=${BR2_DIR}/output/host/usr/bin:$PATH`
   
   where **BR2_DIR** denotes the path to the directory of buildroot.
   
2. Launch the make command to compile LVGL

   `$ make CC=aarch64-nuvoton-linux-gnu-gcc LDFLAGS="-lts $LDFLAGS"`
   
3. To deploy the LVGL demo, copy the out executable binary file 'demo' to target device.

# For using Wayland on MA35D1, please contact Nuvoton sales representative
![Wayland Protocols Code Generator](/wayland-protocol-code-generator.png)

# LVGL with Wayland on MA35D1

![LVGL with Wayland](/ma35d1.png)

# 2D Transformation Software Implementaton Wayland Based Analog Clock LVGL Demo
![Analog Clock](/egl-960.gif)

# LVGL in QEMU
1. Install the **QEMU64 for Windows 64 bit** from site **https://qemu.weilnetz.de/w64/**
2. Double click the batch file **lvgl-port-ma35d1\qemu\launcher.bat** to run LVGL with Wayland backend in QEMU Linux virtual machine.
3. Lauch the **weston-terminal** by clicking the terminal icon in title bar, execute the following demos from the command line
```
   # calendar &
   # sticky-notes &
   # analog-clock &
   # weston-simple-egl &
```

The login user name is **root**.

![LVGL in QEMU](/lvgl-in-qemu.png)
