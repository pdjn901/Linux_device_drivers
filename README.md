# Linux_device_drivers
14-02-2024
this repo created on above date.
this repo simply having drivers like 1.UART, 2.I2C, 3.SPI, 4.USB, 5.wifi, 6.blutooth.

Q) how to compile device driver code? 
A) $ make -C /lib/modules/$(uname -r)/build M=$(pwd) modules

Q) how to insmod with example? 
Ans) sudo insmod my_usb_driver.ko

Q) how to check after insnod
Ans) dmesg
    or dmesg | tail -10
    
Q) how to unload the above driver? 
   $sudo rmmod my_usb_driver

Q) how to debug the above code? 
Ans) 
1.1
$ printk(KERN_DEBUG "Debug message: ...\n");
1.2 
pr_debug("Debug message: ...\n");
1.3 Kernel Debugging Tools:-
$ kgdb, kdb, or kdbx. 
1.4
Static Analysis:- Use static analysis tools like sparse, cppcheck, or clang
1.5 
Kernel Profiling:-
$'perf'

1.6 Kernel Module Parameters:
Add module parameters to control the behavior of the module at runtime. You can pass parameters to the module when loading it with insmod or modprobe, allowing you to enable/disable debug features or customize behavior.
Ex:-
 static int debug = 0;
module_param(debug, int, 0); // Create module parameter

1.7 
$ dmesg | grep my_usb_driver

1.8 Dynamic Debugging (pr_debug):
Use pr_debug statements to enable dynamic debugging for specific functions or modules. This allows you to selectively enable debug messages at runtime without recompiling the kernel module.
Enable dynamic debugging support in the kernel configuration (CONFIG_DYNAMIC_DEBUG) and use the DEBUG macro in your code. For example:
$ pr_debug("Debug message: ...\n");

1.9

Q) How to debug my_usb_driver.ko file with gdb? 
Ans) 
1.1) Prepare the Environment:
Enable kernel debugging options (CONFIG_DEBUG_INFO, CONFIG_KALLSYMS, etc.) in your kernel configuration and rebuild the kernel if necessary.
1.2. 
Load the Module with Debug Symbols:

Load the kernel module (my_usb_driver.ko) into the kernel with debug symbols enabled (-g flag). You can do this by compiling the module with debug symbols:

1.2. Load the Module with Debug Symbols:

Load the kernel module (my_usb_driver.ko) into the kernel with debug symbols enabled (-g flag). You can do this by compiling the module with debug symbols:

$ make -C /lib/modules/$(uname -r)/build M=$(pwd) modules EXTRA_CFLAGS="-g"

1.3
Obtain the Module's Address:

Use the lsmod command to obtain the address of the loaded module (my_usb_driver).
Attach gdb to the Kernel:

Start gdb and attach it to the running kernel:
$ sudo gdb /usr/src/linux/vmlinux /proc/kcore

Replace /usr/src/linux/vmlinux with the path to your kernel image. You can find it in /boot/vmlinuz-* or the directory where your kernel is installed.
Load kernel symbols:
$add-symbol-file /path/to/my_usb_driver.ko <address>
Replace /path/to/my_usb_driver.ko with the path to your module file, and <address> with the address obtained from lsmod.
Set Breakpoints and Debug:

Set breakpoints on functions or lines of code in your module using gdb commands.
Use gdb commands (next, step, continue, print, etc.) to navigate through the code and inspect variables
View Kernel Log Messages:

View kernel log messages (dmesg) to monitor the behavior of your module and debug output generated by printk statements.

Q) How to debug my_usb_driver.ko file with kdb? 
Ans) 
1.1
$sudo insmod my_usb_driver.ko
1.2 Trigger the Kernel Debugger (KDB):
You can trigger KDB in several ways, depending on your kernel configuration and setup:
SysRq Key: If your kernel is configured to allow the use of the SysRq key, you can trigger KDB by pressing the SysRq key combination (usually Alt + SysRq + G).

Magic SysRq: If the SysRq key is enabled, you can also trigger KDB using the "magic SysRq" feature. Press Alt + SysRq, release both keys, then press G.

Panic: If your module triggers a kernel panic, you may enter KDB automatically if it's configured to do so.

1.3

Navigate to the Module Code:

Once KDB is active, you'll be dropped into the KDB prompt. Use the lsmod command to list loaded modules and find the address of your module:
$lsmod
Note the address of your module (my_usb_driver.ko).

1.4
Set Breakpoints:

Use the b command in KDB to set breakpoints at specific lines of code within your module.
Replace my_function_name with the name of the function where you want to set the breakpoint.

1.5
Inspect Variables and Memory:

Use KDB commands to inspect variables, memory, and registers, and step through code execution
$ p my_variable

Replace my_variable with the name of the variable you want to inspect.

1.6 continue execution 
$c
1.7 quite the execution 
$q

###Some references for code
//https://elixir.bootlin.com/linux/latest/source/include/linux/spi/spi.h




