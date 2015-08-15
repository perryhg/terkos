# Wireless Networking #
The VEXPro is capable of 802.11g wireless networking (WiFi) when equipped with the [VEXnet USB Adapter](http://www.vexrobotics.com/276-2245.html).  The VEXPro supports open (unencrypted) networks, WEP and WPA encrypted networks, and ad-hoc networking.

By default, the VEXPro will connect to the first open network it finds (trying them in order of signal strength).  However, you can configure the VEXPro to connect to encrypted networks and/or define the set of networks it should try connecting to. There are four different ways to configure the VEXPro's wireless networking settings.  This guide will discuss each in turn and describe situations where one is preferable to the others.

**Table of Contents**
  * **[LCD Menu](WirelessNetworking#LCD_Menu.md)**
  * **[Ad Hoc Networking](WirelessNetworking#Ad_Hoc_Networking.md)**
  * **[Serial Console](WirelessNetworking#Serial_Console.md)**
  * **[Web Site](WirelessNetworking#Web_Site.md)**


---


### LCD Menu ###

You can list and join open networks and start/stop ad-hoc networking via the VEXPro's [LCD Menu](LCDMenu.md).  The Wireless Networking section of the LCD menu contains the following submenus:

  * **Wireless Networking**
    * **Wireless Status** - provides ability to turn wireless on/off and--if wireless is currently enabled--display the wireless network name (SSID) and IP address
    * **List Wireless Networks** - list all visible wireless networks.  Press the OK button to join the currently selected network.
    * **Ad-hoc Wifi Networking** - turn on/off ad-hoc wireless networking (see [below](WirelessNetworking#Ad_Hoc_Networking.md) for usage details)
    * **Wifi Adapter MAC Address** - display the wifi adapter's MAC address
    * **Start Wireless Upon Boot** - specify whether wireless networking should be started upon system boot
    * **Reset Wireless Settings** - reset the wireless networking settings to the factory defaults

Since it's always available and easiest to access, choosing the network via the LCD menu is the preferred choice when connecting to an open network.  Since network preference changes made via the LCD Menu are not persisted beyond reboot, this option is often best when you want to quickly connect to an open network that you are not likely to use in the future, such as at a cafe or a friend's house.  If you want the VEXPro to always connect to the same network(s), then you should configure wireless networking by using the [Serial Console](WirelessNetworking#Serial_Console.md) or on-board [Web Site](WirelessNetworking#Web_Site.md).


---


### Ad Hoc Networking ###

Configuring your computer for ad hoc networking can be a little more involved, but the benefit is that you can connect to your VEXPro wirelessly and with no additional infrastructure such as a networking router.  Once connected, you can configure the VEXPro's wireless setting as described below in the [web site](WirelessNetworking#Web_Site.md) section below

When you turn on ad hoc networking on the VEXPro, it automatically chooses a network name and IP address.  The network name will be of the form `VEXProNNNN` where `NNNN` is a constant identifier for that particular VEXPro.  The IP address will be 169.254.226.42.

Once ad hoc networking is enabled on the VEXPro, you can connect to the network using your computer. You computer's wireless networking control panel should have some way of listing all the networks it sees.  Find your VEXPro's network in the list and connect to it.  If it's not obvious which VEXPro is yours (e.g. there are more than one, such as in a classroom, go to **Wireless Networking --> Wireless Status** in the LCD menu to see what network name your VEXPro is using.  Once your computer is connected to the ad hoc network, you'll most likely need to manually configure your computer's IP address.  Since the VEXPro's IP address is 169.254.226.42, we'll simply use 169.254.226.43 for the computer.  The procedure for setting the IP address differs depending on what operating system your using.  See below for details

#### Manually Setting the IP Address on Mac OS X or Linux ####

To manually set your computer's IP address on Mac OS X or Linux, first open a terminal (command prompt) window.  You'll need to know the name of your wireless interface--on a Mac, it's typically `en1`, but on Linux it might be something different such as `wlan0`.  To find out, type `ifconfig` at the command prompt.  That should list all the various interfaces that your computer knows about.  Consult your system documentation if you're still not sure.

Once you know the name of your wifi interface, type the following in the terminal window, making sure to replace `en1` with the name of your wifi interface:

> `sudo ifconfig en1 169.254.226.43`

Enter the sudo password when prompted.  You should now be able to connect to the VEXPro's on-board web site by opening a browser and going to http://169.254.226.42/

#### Manually Setting the IP Address on Windows ####

To manually set the IP address on Windows, begin by clicking on the wifi icon in the task bar.  That should pull up a menu showing that you're currently connected to your VEXPro's ad hoc network, as well as listing other nearby wifi networks.  Right click-on the VEXPro's network and choose **Status** from the submenu:

![http://cmucreatelab.org/www.terk.ri.cmu.edu/vexpro/AdHoc0.png](http://cmucreatelab.org/www.terk.ri.cmu.edu/vexpro/AdHoc0.png)

You should now see the Wireless Network Connection Status window.  Click the **Properties** button.

![http://cmucreatelab.org/www.terk.ri.cmu.edu/vexpro/AdHoc1.png](http://cmucreatelab.org/www.terk.ri.cmu.edu/vexpro/AdHoc1.png)

You should now see the Wireless Network Connection Properties window.  Select **Internet Protocol Version 4 (TCP/IPv4)** from the list and then click the **Properties** button.

![http://cmucreatelab.org/www.terk.ri.cmu.edu/vexpro/AdHoc2.png](http://cmucreatelab.org/www.terk.ri.cmu.edu/vexpro/AdHoc2.png)

You should now see the Internet Protocol Version 4 (TCP/IPv4) Properties window.  Edit it to look like this:

![http://cmucreatelab.org/www.terk.ri.cmu.edu/vexpro/AdHoc3.png](http://cmucreatelab.org/www.terk.ri.cmu.edu/vexpro/AdHoc3.png)

When you're done, click OK on the various windows to close them.  You should now be able to connect to the VEXPro's on-board web site by opening a browser and going to http://169.254.226.42/


---


### Serial Console ###

You can configure wireless networking by running a configuration program via the serial console.  The configuration program lets you specify either an open, unencrypted network, or a network encrypted with WEP or WPA.  You'll need to use this serial console configuration program instead of the LCD Menu if there are no open networks available, or if you want your preferences to persist beyond reboot.

To use the serial console configuration program, first connect to the VEXPro as described in the [USB Serial Console](SerialConsole.md) instructions.  Once you are at the VEXPro command prompt, launch the wireless networking configuration utility like this:

> `root@qwerk:~# /opt/scripts/setupWirelessNetworking.pl`

The program will guide you through the setup.  Once complete, just reboot your VEXPro and it should connect to the network you specified.


---


### Web Site ###

The VEXPro's [on-board web site](WebSite.md) provides the most user-friendly interface for configuring wireless networking.  You can use it to specify the set of wireless networks to which the VEXPro should try to connect, and the order in which the VEXPro should try to connect to them.  However, since it's a web site, accessing it requires that the VEXPro already has wireless connectivity.

When you first access the Wireless Networking tab in the on-board web site, you should see something similar to the following.  The upper section shows you the name of the network to which you're currently connected, the VEXPro's IP address, and its MAC address.  The lower section is where you can configure whether the VEXPro should start wireless networking on boot up, and the set of preferred networks:
![http://cmucreatelab.org/www.terk.ri.cmu.edu/vexpro/WebSite15_Wifi8.png](http://cmucreatelab.org/www.terk.ri.cmu.edu/vexpro/WebSite15_Wifi8.png)<br>
To add a network to the list, click the plus (+) button in the lower-left corner.  Doing so will bring up a dialog box for adding a new network to your set of preferred networks.   You can enter the network's name (SSID) here, as well and specify the encryption details (if any).  The following three screenshots show what the dialog looks like for open, WEP, and WPA networks:<br>
<img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/vexpro/WebSite12_Wifi5.png' /><br>
<img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/vexpro/WebSite13_Wifi6.png' /><br>
<img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/vexpro/WebSite14_Wifi7.png' /><br>
Instead of manually entering the network name and encryption type, you can scan for available networks.  If you click the Scan button in the lower-left corner, the VEXPro will look for all available networks--this usually takes a few seconds:<br>
<img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/vexpro/WebSite09_Wifi2.png' /><br>
Once scanning is complete, you should see a list of all available networks.  Networks with a lock icon are encrypted networks:<br>
<img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/vexpro/WebSite10_Wifi3.png' /><br>
Selecting an encrypted network in the list will expand the dialog, allowing you to enter the encryption details:<br>
<img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/vexpro/WebSite11_Wifi4.png' /><br>
Click the Add button to add the network to your list of preferred networks:<br>
<img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/vexpro/WebSite04_WirelessNetworking.png' /><br>
Once you have added some networks, you can drag them in the list to change the order in which the VEXPro will try to connect to them.  Click the minus (-) button to delete the selected network from the list.  Click the pencil button to edit the selected network.  Make sure you save your changes by clicking the Save button.<br>
<br>
<hr />

<a href='LCDMenu.md'>previous</a> | <a href='WebSite.md'>next</a>