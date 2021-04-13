# hisi-trace
HiSilicon tracer

A utility to run Sofia from XM in a non-stock environment. Uses linux kernel function "ptrace". Allows porting stock Sofia functions to the target system without loading in the original firmware.

Work description

The utility needs to specify the path to Sofia in its arguments, example run: hisi-tracer /path/to/Sofia.

Next, the utility will create a child process of Sofia and start capturing system calls. Intercepts calls to open /proc/mtd ; /dev/mtd ; /dev/mtd0 ... all the mtd blocks. Sophia will return 0 (SUCCESS) on ioctl calls to these files with custom XMMTD_ commands. Captured file names will be changed so that the kernel opens mtd files from the original firmware. The naming scheme is simple - the fourth character of the name is changed to 'i'. I.e. for /proc/mtd it will be /pric/mtd, for /dev/mtd it will be /dei/mtd, etc. That is, before launching the utility, you need to create files /pric/mtd, /dei/mtd... They should be downloaded from the original firmware where Sofia was taken from.

The linux kernel should be built with the ptrace option. Include strace in your build as well, it will come in handy. For Sofia to work correctly you also need HIMPP modules from the original firmware, they must also be loaded.

Assembly:

The cmake build system is used. Execute in the same place where openipc is built:

```
make -j1 V=s package/feeds/glutinium/hisi-trace/{clean,compile,install}
```
