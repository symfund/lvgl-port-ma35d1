PATH=%~dp0;C:\Program Files\qemu;%PATH%
qemu-system-x86_64.exe -M pc -kernel bzImage -drive file=rootfs.ext2,if=virtio,format=raw -append "rootwait root=/dev/vda console=tty1 console=ttyS0" -net nic,model=virtio -net user -device virtio-vga,max_outputs=2 --full-screen
