# Getting Started with the VEXPro ARM9 #

This Quickstart Guide serves as a step-by-step guide to get your VEXPro ARM9 up and running.  And it points you in the the direction to program and debug your VEXPro.

  1. The VEXPro ARM9 needs a 7.2V battery and a VEXnet USB Adapter Key. Please refer to the [VEXPro ARM9 Page](http://www.vexrobotics.com/217-2180.html) for information about recommend and required accessories.
  1. Plug the VEXnet Key into your VEXPro (refer to figure below for location.) ![http://lh3.googleusercontent.com/-7IfEFWTdxOI/Tpizso9k0ZI/AAAAAAAABls/9oBIXuBhrjI/s653/vexprodiagram.gif](http://lh3.googleusercontent.com/-7IfEFWTdxOI/Tpizso9k0ZI/AAAAAAAABls/9oBIXuBhrjI/s653/vexprodiagram.gif)
  1. As with any battery, plug the battery into the battery charger and run it through a charge cycle to make sure that the battery is fully charged and fully capable to operate your VEXPro.
  1. Plug the 7.2V battery into your VEXPro (refer to figure below for location.) ![http://lh4.googleusercontent.com/-tf-s2n7Us6Y/Tpixz754MTI/AAAAAAAABlQ/9n30CYHeGEA/s672/vexprosidediagram.gif](http://lh4.googleusercontent.com/-tf-s2n7Us6Y/Tpixz754MTI/AAAAAAAABlQ/9n30CYHeGEA/s672/vexprosidediagram.gif)
  1. Turn on your VEXPro ARM9, ie press the OK button – the button in the upper right of the keypad with the “check” icon next to it.  After pressing this button, you should see the LCD illuminate and display  “Booting...” When the VEXPro is finished booting, you should see the first item of the LCDMenu program “System Info”.
  1. The VEXPro ARM9 will automatically connect to the the strongest wireless router signal that **does not have encryption**.  If this describes your networking situation, continue reading.  If not, skip to the next step.  Verify that the VEXPro has connected to your network.  Use the arrow keys on the keypad to scroll to “Wireless Networking” and press the OK button.  “Wireless Status” should then be displayed.  Press OK again.  “Wireless is on” should be displayed with “Info” checked.  Press OK again.  Your SSID should then be displayed with the DHCP-assigned IP address of your VEXPro.  If you wish, you can try to ping this address from your desktop computer for further verification that your VEXPro is connected and up and running.
  1. If you wish to connect to a wireless network that is protected/encrypted, or establish your own direct connect ad-hoc network between your VEXPro ARM9 and your computer, please refer to the [networking page](WirelessNetworking.md) to set up your network.
  1. You are now ready to connect to the VEXPro ARM9 and program it with the TerkIDE.  Refer to the [VEXPro IDE Guide](http://code.google.com/p/terk-ide/wiki/InstallationInstructions).
  1. Once you've run through the [VEXPro IDE Guide](http://code.google.com/p/terk-ide/wiki/InstallationInstructions), you are ready to start developing robotics programs with your VEXPro.  Be sure to check out the [libqwerk API documentation](libqwerkAPI.md) as well as the following pages in this Wiki.


---

previous | [next](VEXProTerkOSUserGuide.md)