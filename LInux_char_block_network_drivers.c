
//char driver step by step.
//https://www.apriorit.com/dev-blog/195-simple-driver-for-linux-os#:~:text=There%20are%20two%20ways%20of%20a%20Linux%20device,case%20you%20won%E2%80%99t%20need%20to%20recompile%20the%20kernel.


Char, block, network drivers with interrupts,ioctl, spinlocks.
//char,block,network drivers
https://linux-kernel-labs.github.io/refs/heads/master/labs/device_drivers.html
//pdf:bootlin
  //https://bootlin.com/doc/training/linux-kernel/linux-kernel-slides.pdf

  //debugging
  https://linux-kernel-labs.github.io/refs/heads/master/lectures/debugging.html

//system calls
https://linux-kernel-labs.github.io/refs/heads/master/lectures/syscalls.html


//memory management
https://linux-kernel-labs.github.io/refs/heads/master/lectures/memory-management.html


//can i2c implemented as char driver?
https://stackoverflow.com/questions/41492850/does-i2c-driver-need-to-be-implemented-just-like-any-other-character-device-driv

//i2c hw monitoring temp sensor, kernel project
https://www.kernel.org/doc/Documentation/hwmon/hwmon-kernel-api.txt

//how many types to access i2c drivers?
    i)builtin kernel and rebuild in kernel
    ii) sysfs/bus/i2c/driver_name
    iii) added char driver
   //https://stackoverflow.com/questions/23979129/i2c-device-linux-driver

//how many type of devices in devices in linux?
Under Linux, there are essentially 3 types of devices:
▶ Network devices. They are represented as network interfaces, visible in user
space using ip a
▶ Block devices. They are used to provide user space applications access to raw
storage devices (hard disks, USB keys). They are visible to the applications as
device files in /dev.
▶ Character devices. They are used to provide user space applications access to all
other types of devices (input, sound, graphics, serial, etc.). They are also visible
to the applications as device files in /dev.

//Major and minor number?
  Within the kernel, all block and character devices are identified using a major and
a minor number.
▶ The major number typically indicates the family of the device.
▶ The minor number allows drivers to distinguish the various devices they manage
