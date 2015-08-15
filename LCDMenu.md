# LCD Menu #

The VEXPro's LCD Menu provides quick and easy access to both diagnostics and configuration.  Following is an outline of the various menu items, with descriptions where appropriate/necessary:

  * **System Info**
    * **Power Status** - main battery voltage/state, backup battery voltage/state, breaker states, etc.
    * **Memory Usage** - total, used, and free memory
    * **Version Info** - Linux, firmware, and hardware versions
  * **Audio**
    * **Set Volume** - set the speaker volume
    * **Toggle Audio Alerts** - set whether audio alerts should be played for various warning/error conditions
    * **Reset Audio** - reset the volume and audio alert settings to the factory defaults
  * **LCD**
    * **Set Backlight Timeout** - set the timeout for the backlight, or turn it off altogether
    * **Reset LCD** - reset the LCD settings to the factory defaults
  * **Wireless Networking**
    * **Wireless Status** - provides ability to turn wireless on/off and--if wireless is currently enabled--display the wireless network name (SSID) and IP address
    * **List Wireless Networks** - list all visible wireless networks.  Press the OK button to join the currently selected network.
    * **Ad-hoc Wifi Networking** - turn on/off ad-hoc wireless networking
    * **Wifi Adapter MAC Address** - display the wifi adapter's MAC address
    * **Start Wireless Upon Boot** - specify whether wireless networking should be started upon system boot
    * **Reset Wireless Settings** - reset the wireless networking settings to the factory defaults
  * **User Programs**
    * **Set Program to Run On Boot** - choose a program to auto-run upon bootup
    * **Reset User Programs** - reset the user program settings to the factory defaults
  * **System Settings**
    * **Save Prefs to USB Drive** - saves system preferences to a file on a USB drive
    * **Load Prefs from USB Drive** - loads system preferences from a file on a USB drive
    * **Reset System Preferences** - resets system preferences to the factory defaults.

When resetting system preferences, the root user password is reset to **qwerk** and the web control panel login is set to username **admin** and password **password**.

Advanced users may appreciate knowing that the LCD Menu is based on a plugin architecture for easy expansion and customization of the options.  Also, the menu hierarchy is defined in XML, so it is easy to reorganize.  Finally, all text labels in the menu can be overridden to allow for internationalization.  We will provide more information on the LCD Menu plugin architecture and customization options in the future.

---

[previous](HardwareOverview.md) | [next](WirelessNetworking.md)