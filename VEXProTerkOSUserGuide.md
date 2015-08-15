# VEXPro ARM9 and TerkOS User Guide: Welcome and Introduction #

Welcome to VEXPro ARM9 and TerkOS!  This user guide explains the basics of using the VEXPro hardware and the TerkOS.

TerkOS is a Google Code project that contains a bunch of open-source code for the VEXPro ARM9 Linux operating system as well as the documentation you are reading right now.  If you are here to familiarize yourself with the software for your VEXPro ARM9, you're at the right place.  TerkOS includes:

  * Linux kernel and disk image, based on OpenEmbedded distro
  * LCD daemon program (LCDMenu) that runs on the VEXPro automatically.  It is used for checking battery status, network connection info, setting audio volume, testing motors, sensors, launching applications, etc.
  * The onboard webserver on the VEXPro for configuring network settings, robot name, password, etc.
  * Drivers for the motor control, digital interrupts, sensor input, audio, voice synthesis, webcam/camera, etc.
  * VEXPro ARM9 User Code package, includes sensor/motor interface library (libqwerk) and examples.

So in other words, TerkOS is everything that runs onboard the VEXPro (embedded Linux OS, drivers, servers, programs).  If you are interested in learning more about the Integrated Development Environment (IDE) for the VEXPro ARM9, jump to the [TerkIDE project](http://code.google.com/p/terk-ide/wiki/InstallationInstructions), which is designed to work with TerkOS, but is maintained separately.  The [TerkIDE](http://code.google.com/p/terk-ide/wiki/InstallationInstructions) is the best place to get started developing programs for the VEXPro ARM9.

---

[previous](Quickstart.md) | [next](HardwareOverview.md)