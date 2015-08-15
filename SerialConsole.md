# VEXPro USB Serial Console Setup #

The VEXPro has a USB mini-B port on the side labeled "Console".  This port allows access to the VEXPro's main Linux console, which is sometimes useful for configuring the VEXPro's Linux settings, etc.

Before using the VEXPro USB serial console, you must have the correct drivers.  The drivers are required to communicate with the USB serial controller on the VEXPro (FTDI FT232R).

### If you are running Linux ###
It is likely that your kernel has the driver built in.  You can verify this by plugging in the mini-B USB cable (provided with the VEXPro) into your VEXPro and your PC's USB port.  Then look for the device in the /dev directory -- it should be ttyUSB0.  Note, you can try this with the VEXPro powered on or off.  If the ttyUSB0 device fails to show up, you can obtain the kernel module or upgrade the kernel.  Related information can be found [here](http://elinux.org/Hammer_How_to_USB_FTDI).

Now bring up a terminal program (such as minicom) and set the communication parameters to:
  * device = /dev/ttyUSB0
  * baudrate = 460800
  * stop bits = 1
  * parity = none
  * flow-control = off

You should see the Linux login prompt when pressing return, assuming the VEXPro has finished booting.

### If you are running Windows ###
Obtain the device driver [here](http://www.ftdichip.com/Drivers/VCP.htm).  Unzip the driver into a convenient location on your hard drive.  Then plug in the mini-B USB cable (provided with the VEXPro) into your VEXPro and your PC's USB port.  The Hardware Update Wizard should ask you if Windows can "connect to Windows Update to search for the software".  Select "No", and click on next. Then when asked, select "Install from a list or specific location (Advanced)." and click on next.  When asked where to search, select “Include the location in the search” and browse to the directory where you unzipped the driver.  After clicking Next, the driver will automatically install on your system.  You can verify that the driver is up and running by looking in the Device Manager under "Ports (COM & LPT)".  You should see something like "USB Serial Port (COM4)."

Now bring up a terminal program, such as Hyperterminal (recommanded for XP) or [Tera Term](http://ttssh2.sourceforge.jp/index.html.en) (recommended for Win7) or [Realterm](http://realterm.sourceforge.net/) and set the communication parameters to:
  * device = COM2, COM3, ..., or COM9 depending on your system.  Choose the highest comport number typically.
  * baudrate = 460800
  * stop bits = 1
  * parity = none
  * flow-control = off

### Logging in ###
When seeing the Linux login prompt "qwerk login:" type in "root".  The default password is "qwerk".  You should then see the command prompt "root@qwerk:~#", assuming your VEXPro's name is "qwerk".  You can now interact with the VEXPro's Linux OS.

---

[previous](libqwerkAPI.md) | [next](FirmwareUpload.md)