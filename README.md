[RU](README_ru.md)

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
![GitHub repo size](https://img.shields.io/github/repo-size/OpenIPC/openipc-2.1)
![GitHub issues](https://img.shields.io/github/issues/OpenIPC/openipc-2.1)
![GitHub pull requests](https://img.shields.io/github/issues-pr/OpenIPC/openipc-2.1)
[![License](https://img.shields.io/github/license/OpenIPC/openipc-2.1)](https://opensource.org/licenses/MIT)

-----

### Supporting

If you like our work, please consider supporting us on [Opencollective](https://opencollective.com/openipc/contribute/backer-14335/checkout) or [PayPal](https://www.paypal.com/donate/?hosted_button_id=C6F7UJLA58MBS) or [YooMoney](https://openipc.org/donation/yoomoney.html). 

[![Backers](https://opencollective.com/openipc/tiers/backer/badge.svg?label=backer&color=brightgreen)](https://opencollective.com/openipc)
[![Backers](https://opencollective.com/openipc/tiers/badge.svg)](https://opencollective.com/openipc)

[![Backers](https://opencollective.com/openipc/tiers/backer.svg?avatarHeight=36)](https://opencollective.com/openipc#support)

### Thanks a lot !!!

<p align="center">
<a href="https://opencollective.com/openipc/contribute/backer-14335/checkout" target="_blank"><img src="https://opencollective.com/webpack/donate/button@2x.png?color=blue" width="300" alt="OpenCollective donate button" /></a>
<a href="https://www.paypal.com/donate/?hosted_button_id=C6F7UJLA58MBS"><img src="https://www.paypalobjects.com/en_US/IT/i/btn/btn_donateCC_LG.gif" alt="PayPal donate button" /> </a>
<a href="https://openipc.org/donation/yoomoney.html"><img src="https://yoomoney.ru/transfer/balance-informer/balance?id=596194605&key=291C29A811B500D7" width="100" alt="YooMoney donate button" /> </a>
</p>
