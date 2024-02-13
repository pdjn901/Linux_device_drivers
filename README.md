# Linux_device_drivers
14-02-2024
this repo created on above date.
this repo simply having drivers like 1.UART, 2.I2C, 3.SPI, 4.USB, 5.wifi, 6.blutooth.

Q) how to compile device driver code? 
A) make -C /lib/modules/$(uname -r)/build M=$(pwd) modules

Q) how to insmod with example? 
Ans) sudo insmod my_usb_driver.ko

Q) how to check after insnod
Ans) dmesg
    or dmesg | tail -10
    
Q) how to unload the above driver? 
   sudo rmmod my_usb_driver

Q) how to debug the above code? 
Ans) 
1.1
printk(KERN_DEBUG "Debug message: ...\n");
1.2 
pr_debug("Debug message: ...\n");
1.3 Kernel Debugging Tools:-
kgdb, kdb, or kdbx. 
1.4
Static Analysis:- Use static analysis tools like sparse, cppcheck, or clang
1.5 
Kernel Profiling:-
'perf'

1.6 Kernel Module Parameters:
Add module parameters to control the behavior of the module at runtime. You can pass parameters to the module when loading it with insmod or modprobe, allowing you to enable/disable debug features or customize behavior.
Ex:-
 static int debug = 0;
module_param(debug, int, 0); // Create module parameter

