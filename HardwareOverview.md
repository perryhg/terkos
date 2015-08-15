# Hardware Overview #

The VEXPro ARM9 Microcontroller began as the popular Qwerk robot controller, the result of a collaboration between Charmed Labs and the [Carnegie Mellon CREATE Lab](http://cmucreatelab.org/). The VEXPro ARM9 is a redesign of Qwerk and intended to be used by advanced users at the high school and university level.

The VEXPro ARM9 is a fully-integrated robot controller.  It combines the power system, motor controllers, motor amplifiers, sensor inputs, RISC processor, multitasking OS, wireless networking, text LCD, speaker and keypad into one device.  Furthermore, what makes the VEXPro ARM9 well-suited for robotics is the tightly-integrated FPGA (field-programmable gate array).  The FPGA acts as robot ASIC (application-specific integrated circuit) that can be programmed with peripherals and co-processors specific to robotics.  In particular, it provides closed-loop multi-axis motor control that uses a special method for motion feedback unique to the VEXPro.

![http://lh3.googleusercontent.com/-7IfEFWTdxOI/Tpizso9k0ZI/AAAAAAAABls/9oBIXuBhrjI/s653/vexprodiagram.gif](http://lh3.googleusercontent.com/-7IfEFWTdxOI/Tpizso9k0ZI/AAAAAAAABls/9oBIXuBhrjI/s653/vexprodiagram.gif)

![http://lh4.googleusercontent.com/-tf-s2n7Us6Y/Tpixz754MTI/AAAAAAAABlQ/9n30CYHeGEA/s672/vexprosidediagram.gif](http://lh4.googleusercontent.com/-tf-s2n7Us6Y/Tpixz754MTI/AAAAAAAABlQ/9n30CYHeGEA/s672/vexprosidediagram.gif)

The VEXPro ARM9 features the following:
  * 200 MHz ARM9 RISC processor
  * 16 Mbytes of flash, 32 Mbytes of RAM
  * Supports 802.11g (WiFi), VEXnet
  * 2 USB 2.0 host ports
  * Mini USB serial port for accessing Linux console
  * 16 digital I/O ports
  * 16 analog input ports
  * 4 "smart" 2-wire motor ports
  * 12 servo motor ports
  * UART port
  * I2C port
  * 16 x 2 character LCD screen
  * Power and battery backup ports
  * Keypad
  * Internal speaker


## Back-EMF Control, aka Smart 2-Wire Motor Control ##

Many robot controllers offer open-loop motor control.  Open-loop simply means that the motor controller doesn't have knowledge of the motor's motion.  However, if a robot is expected to move accurately within its environment and deal effectively with random factors such as bumps or inclines, closed-loop motor control is much more useful. A closed-loop motor controller can automatically increase or decrease power to the motors in order to maintain the desired velocity or position in the presence of bumps or inclines.

Closed-loop motor control requires some form of motion feedback.
Back-EMF feedback is based on the principle that a motor is also a generator producing a voltage that's proportional to its velocity. This
voltage, called the back-EMF voltage, can be used to infer a motor's motion if measured periodically.

The VEXPro uses its FPGA to implement a custom four-axis
back-EMF feedback controller. This controller talks directly to
a 12-bit A/D converter, which converts back-EMF
voltages into measurements. To achieve the best signal
to noise ratio, the controller takes thousands of back-EMF
measurements per second. In this way the VEXPro is able to provide accurate closed-loop motor control using simple motors without extra sensors.

## Basic Operation ##
The VEXPro requires an external battery for power.  Before turning the VEXPro on, plug in a fully charged 7.2V VEX battery pack into the main battery port.  Hold down the power button (the one with the check mark next to it -- refer to figure) until you see the LCD screen turn on.  Similarly, to power off, hold down the power button for several seconds until you see the LCD screen dim.

The VEXPro takes several seconds to finish booting.  The VEXPro is finished booting when you see the message "Welcome to TerkOS" on the LCD.

![http://lh4.ggpht.com/_AaoS_28Sdhw/S6Jt-e3Q6UI/AAAAAAAAAC0/fT-G8Mr6_8Q/VEXPro.png](http://lh4.ggpht.com/_AaoS_28Sdhw/S6Jt-e3Q6UI/AAAAAAAAAC0/fT-G8Mr6_8Q/VEXPro.png)

---

[previous](VEXProTerkOSUserGuide.md) | [next](LCDMenu.md)