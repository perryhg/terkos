# Basic VEX Bot #


![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/themes/terk/img/icon_time.gif](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/themes/terk/img/icon_time.gif) 1 Hour ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/themes/terk/img/icon_cost.gif](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/themes/terk/img/icon_cost.gif) ~$100 ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/themes/terk/img/icon_level.gif](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/themes/terk/img/icon_level.gif) Beginner

![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image11_ready.jpg](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image11_ready.jpg)

The Vex bot is the most basic robot you can build with the Vex robotics kit. The Omni wheel at the back lets it make sharp turns, while still enabling it to move forwards and backwards.


---


### Table of Contents ###
  * **[Introduction](RobotRecipesBasicVEXBot#Introduction.md)**
  * **[Build the Frame](RobotRecipesBasicVEXBot#Build_the_Frame.md)**
  * **[Attach the Motors](RobotRecipesBasicVEXBot#Attach_the_Motors.md)**
  * **[Attach the Encoders](RobotRecipesBasicVEXBot#Attach_the_Encoders.md)**
  * **[Wheels and Gears](RobotRecipesBasicVEXBot#Wheels_and_Gears.md)**
  * **[Attach the Caster](RobotRecipesBasicVEXBot#Attach_the_Caster.md)**
  * **[Attach the Battery](RobotRecipesBasicVEXBot#Attach_the_Battery.md)**


---


## Introduction ##

### Required Tools ###

| Image | Tool | Source | Part Number | Price |
|:------|:-----|:-------|:------------|:------|
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/tools/hex_key_3_32-100.jpg](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/tools/hex_key_3_32-100.jpg) | 3/32" Allen Wrench | [McMaster](http://www.mcmaster.com/) | 7391A51     | $1.04 |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/tools/564allen-100.jpg](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/tools/564allen-100.jpg) | 5/64" Allen Wrench | [McMaster](http://www.mcmaster.com/) | 7122A15     | $0.14 |

### Required Parts ###

| Image | Part | Quantity | Part Number |
|:------|:-----|:---------|:------------|
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/c-channel_25x2-100.jpg](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/c-channel_25x2-100.jpg) | C-Channel, 1x2x1x25 | {qty}    | VL-CHAN-121-25 |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/bumper_15-100.jpg](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/bumper_15-100.jpg) | Bumper 25x25x2 | {qty}    | CHASSIS-25x25x2 |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/plate_5x15-100.jpg](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/plate_5x15-100.jpg) | Plate, 5x15 | {qty}    | VL-CHAN-121-25 |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/4in_wheel-100.jpg](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/4in_wheel-100.jpg) | 4-inch Wheel | {qty}    | 276-2164    |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/omni_wheel-100.jpg](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/omni_wheel-100.jpg) | Large Omni Wheel | {qty}    | 276-2185    |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/optical-shaft-encoder_100.png](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/optical-shaft-encoder_100.png) | Encoder | {qty}    | 276-2156    |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/vex-motor_100.png](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/vex-motor_100.png) | Motor Kit | {qty}    | 276-2163    |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/clutch-100.jpg](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/clutch-100.jpg) | Clutch | {qty}    | 276-2163    |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/clutch_post-100.jpg](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/clutch_post-100.jpg) | Clutch Post | {qty}    | 276-2163    |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/vex-84-tooth-gear_100.png](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/vex-84-tooth-gear_100.png) | 84-tooth Gear | {qty}    | 276-2169    |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/vex-36-tooth-gear_100.png](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/vex-36-tooth-gear_100.png) | 36-tooth Gear | {qty}    | 276-2169    |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/12tooth_gear-100.jpg](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/12tooth_gear-100.jpg) | 12-tooth Gear | {qty}    | 276-2169    |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/battery_strap-100.jpg](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/battery_strap-100.jpg) | Battery Strap | {qty}    | 276-2219    |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/battery-100.jpg](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/battery-100.jpg) | 7.2v Battery | {qty}    | 276-2183    |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/bearing-flat_100.png](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/bearing-flat_100.png) | Delrin Bearing | {qty}    | BEARING-FLAT-10PK |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/pillow_block-100.jpg](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/pillow_block-100.jpg) | Pillow Block | {qty}    | BEARING-BLOCK-LOCK-BAR |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/shaft-3000_100.png](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/shaft-3000_100.png) | 3-inch Shaft | {qty}    | SQR-BAR-4PK |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/shaft-2000_100.png](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/shaft-2000_100.png) | 2-inch Shaft | {qty}    | SQR-BAR-4PK |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/spacer-0500-0500_100.png](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/spacer-0500-0500_100.png) | 1/2-inch Spacer | {qty}    | SPACER-4X-PACK-10 |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/spacer-0500-0250_100.png](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/spacer-0500-0250_100.png) | 1/4-inch Spacer | {qty}    | SPACER-4X-PACK-10 |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/spacer-0500-0125_100.png](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/spacer-0500-0125_100.png) | 1/8-inch Spacer | {qty}    | SPACER-4X-PACK-10 |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/shaft-collar_100.png](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/shaft-collar_100.png) | Collar | {qty}    | COLLAR-PK16 |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/screw-632-0500_100.png](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/screw-632-0500_100.png) | 6-32 1/2-inch Bolt | {qty}    | SCREW-632-12-10PK |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/screw-832-0500_100.png](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/screw-832-0500_100.png) | 8-32 1/2-inch Bolt | {qty}    | SCREW-832-12-28PK |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/screw-832-0375_100.png](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/screw-832-0375_100.png) | 8-32 3/8-inch Bolt | {qty}    | SCREW-832-38-28PK |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/nut-832-keps_100.png](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/nut-832-keps_100.png) | 8-32 Keps Nuts | {qty}    | NUT-LW-832-PACK-100 |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/washer-steel_100.png](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/washer-steel_100.png) | Steel Washers | {qty}    | WASHER-STEEL-PACK-200 |


---


## Build the Frame ##

### Required Tools ###

| Image | Tool |
|:------|:-----|
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/tools/hex_key_3_32-300.jpg](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/tools/hex_key_3_32-300.jpg) | 3/32" Allen Wrench |

### Required Parts ###

| Image | Part |
|:------|:-----|
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/c-channel_25x2-300.jpg](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/c-channel_25x2-300.jpg) | C-Channel, 1x2x1x25 |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/bumper_15-300.jpg](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/bumper_15-300.jpg) | Bumper 25x25x2 |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/plate_5x15-300.jpg](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/plate_5x15-300.jpg) | Plate, 5x15 |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/screw-832-0375_300.png](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/screw-832-0375_300.png) | 8-32 3/8-inch Bolt |
| ![http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/nut-832-keps_300.png](http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/nut-832-keps_300.png) | 8-32 Keps Nuts |

### Steps ###

  1. Place a bumper over the c-channel as shown. <br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/c-channel_bumper.jpg' />
<ol><li>Use two 8-32 3/8-inch bolts and two keps nuts to attach the bumper to the c-channel. Use the 3/32-inch Allen wrench to tighten the bolts.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/c-channel_bumper_join.jpg' />
</li><li>Repeat steps 1 and 2 to create another L-shaped structure.<br>
</li><li>Align the two L-structures to form a rectangle. Remember to keep the bumpers on top of the c-channels<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/full_frame.jpg' />
</li><li>Use the 8-32 3/8-inch bolts and keps nuts to secure the two L-structures together.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/full_frame_2.jpg' />
</li><li>Rest the plate on the two c-channels. There should be seven holes to the right of the plate, and nine to the left.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/plate_frame.jpg' />
</li><li>Use four 8-32 3/8-inch bolts and four keps nuts to secure the plate to the frame.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/frame_final.jpg' /></li></ol>

<hr />

<h2>Attach the Motors</h2>

<h3>Required Tools</h3>

<table><thead><th> Image </th><th> Tool </th></thead><tbody>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/tools/564allen-300.jpg' /> </td><td> 5/64" Allen Wrench </td></tr></tbody></table>

<h3>Required Parts</h3>

<table><thead><th> Image </th><th> Part </th></thead><tbody>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/vex-motor_300.png' /> </td><td> Motor Kit </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/clutch-300.jpg' /> </td><td> Clutch </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/clutch_post-300.jpg' /> </td><td> Clutch Post </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/bearing-flat_300.png' /> </td><td> Delrin Bearing </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/shaft-collar_300.png' /> </td><td> Collar </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/screw-632-0500_300.png' /> </td><td> 6-32 1/2-inch Bolt </td></tr></tbody></table>

<h3>Steps</h3>

<ol><li>Flip the frame. Keep the side with more room between the frame and bumper to the left. This is the front of the robot.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image8_flip_frame.jpg' />
</li><li>Insert one end of the clutch post into one side of the clutch.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image9_clutch_and_post.jpg' />
</li><li>Insert the other end of the clutch post into one of the motors.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image10_clutch_motor.jpg' />
</li><li>Insert two 6-32 1/2-inch bolts into a Delrin bearing.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image11_bolts_bearing.jpg' />
</li><li>Align the Delrin bearing to the c-channel. The first bolt should go through the third hole on the channel.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image12_bearing_c-channel.jpg' />
</li><li>Use the bolts to attach a motor to the c-channel. Insert the 5/64-inch Allen wrench through the other side of the c-channel and tighten the bolt.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image13_motor_c-channel.jpg' />
</li><li>Insert a 3-inch square post through the hole across the clutch. Don’t insert it all the way through yet.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image14_post_motor.jpg' />
</li><li>Place a collar on the post. Don’t tighten it yet.<br>
</li><li>Push the post all the way into the clutch.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image15_tighten_collar.jpg' />
</li><li>Press the collar firmly against the outer edge of the c-channel.<br>
</li><li>Use the 5/34-inch Allen wrench to tighten it.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image16_repeat.jpg' />
</li><li>Repeat the process to attach the second motor to the other side of the frame.</li></ol>

<hr />

<h2>Attach the Encoders</h2>

<h3>Required Tools</h3>

<table><thead><th> Image </th><th> Tool </th></thead><tbody>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/tools/564allen-300.jpg' /> </td><td> 5/64" Allen Wrench </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/tools/hex_key_3_32-300.jpg' /> </td><td> 3/32" Allen Wrench </td></tr></tbody></table>

<h3>Required Parts</h3>

<table><thead><th> Image </th><th> Part </th></thead><tbody>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/optical-shaft-encoder_300.png' /> </td><td> Encoder </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/vex-36-tooth-gear_300.png' /> </td><td> 36-tooth Gear </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/bearing-flat_300.png' /> </td><td> Delrin Bearing </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/shaft-2000_300.png' /> </td><td> 2-inch Shaft </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/shaft-collar_300.png' /> </td><td> Collar </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/screw-832-0500_300.png' /> </td><td> 8-32 1/2-inch Bolt </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/nut-832-keps_300.png' /> </td><td> 8-32 Keps Nuts </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/washer-steel_300.png' /> </td><td> Steel Washers </td></tr></tbody></table>

<h3>Steps</h3>

<ol><li>Attach a collar to the end of a 2-inch square bar. Use the 5/64-inch Allen wrench to tighten it.<br>
</li><li>Slide a 36-tooth gear on the bar. Slide on a washer after it.<br>
</li><li>Insert the square bar through the front of the encoder.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image13_shaft_encoder.jpg' />
</li><li>On the other side, place a steel washer, followed by a collar.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image14_collar_encoder.jpg' />
</li><li>Secure and tighten the collar such that none of the components are loose.<br>
</li><li>Insert two 8-32 1/2-inch bolts through the Delrin bearing.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image15_bolt_bearing.jpg' />
</li><li>Flip the frame.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image16_bearing_c-channel.jpg' />
</li><li>Align the bearing inside the c-channel, two holes away from the motor.<br>
</li><li>Secure the encoder to the bolts using keps nuts. Use the 5/64-inch Allen wrench to tighten the bolts.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image17_secure_encoder.jpg' />
</li><li>Repeat this procedure to attach the second encoder to the other side of the frame.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image18_repeat.jpg' /></li></ol>

<hr />

<h2>Wheels and Gears</h2>

<h3>Required Tools</h3>

<table><thead><th> Image </th><th> Tool </th></thead><tbody>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/tools/564allen-300.jpg' /> </td><td> 5/64" Allen Wrench </td></tr></tbody></table>

<h3>Required Parts</h3>

<table><thead><th> Image </th><th> Part </th></thead><tbody>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/4in_wheel-300.jpg' /> </td><td> 4-inch Wheel </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/vex-84-tooth-gear_300.png' /> </td><td> 84-tooth Gear </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/vex-36-tooth-gear_300.png' /> </td><td> 36-tooth Gear </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/12tooth_gear-300.jpg' /> </td><td> 12-tooth Gear </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/shaft-3000_300.png' /> </td><td> 3-inch Shaft </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/spacer-0500-0500_300.png' /> </td><td> 1/2-inch Spacer </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/spacer-0500-0125_300.png' /> </td><td> 1/8-inch Spacer </td></tr></tbody></table>

<h3>Steps</h3>

<ol><li>Place a collar at the end of a 3-inch square bar. Tighten it using the Allen wrench.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image10_collar_bar.jpg' />
</li><li>Slide a 12-tooth gear on to the bar.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image11_12-t_gear_post.jpg' />
</li><li>Slide the bar through the c-channel and into the middle of the Delrin bearing.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image12_post_c-channel.jpg' />
</li><li>Slide a 1/2-inch spacer and a 1/8-inch spacer on the other side of the bar.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image13_spacer_post.jpg' />
</li><li>Slide a 36-tooth gear on to the bar. Turn it so that the teeth align properly with the gear on the encoder.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image14_gear_post.jpg' />
</li><li>Place a collar at the end of the post. Use the Allen wrench to tighten it.<br>
</li><li>Slide a 1/8-inch spacer on the bar of the motor.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image15_spacer_front.jpg' />
</li><li>Slide an 84-tooth gear on to the bar. Make sure that the small post on the gear is facing outward.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image16_gear_front.jpg' />
</li><li>Turn the gear to align it with the 12-tooth gear on the encoder.<br>
</li><li>Slide another 1/8-inch spacer on to the bar.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image17_spacer_gear.jpg' />
</li><li>Slide a 4-inch wheel on to the bar, such that the arrows on the wheel point to the back of the frame.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image18_wheel.jpg' />
</li><li>Place a collar at the end of the bar. Push it against the wheel and tighten it using the Allen wrench.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image19_wheel_collar.jpg' />
</li><li>Repeat the procedure to attach a wheel to the other side of the robot.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image20_repeat.jpg' /></li></ol>

<hr />

<h2>Attach the Caster</h2>

<h3>Required Tools</h3>

<table><thead><th> Image </th><th> Tool </th></thead><tbody>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/tools/hex_key_3_32-300.jpg' /> </td><td> 3/32" Allen Wrench </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/tools/564allen-300.jpg' /> </td><td> 5/64" Allen Wrench </td></tr></tbody></table>

<h3>Required Parts</h3>

<table><thead><th> Image </th><th> Part </th></thead><tbody>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/omni_wheel-300.jpg' /> </td><td> Large Omni Wheel </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/pillow_block-300.jpg' /> </td><td> Pillow Block </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/shaft-2000_300.png' /> </td><td> 2-inch Shaft </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/spacer-0500-0250_300.png' /> </td><td> 1/4-inch Spacer </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/shaft-collar_300.png' /> </td><td> Collar </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/screw-832-0375_300.png' /> </td><td> 8-32 3/8-inch Bolt </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/nut-832-keps_300.png' /> </td><td> 8-32 Keps Nuts </td></tr></tbody></table>

<h3>Steps</h3>

<ol><li>Flip the frame.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image10_pillowblock_bumper.jpg' />
</li><li>Align the pillow block to the center of the back bumper.<br>
</li><li>Use two 8-32 3/8-inch bolts and keps nuts to secure the pillow block to the bumper. Use the 5/64-inch Allen wrench to tighten the bolts.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image11_secure_pillowblock.jpg' />
</li><li>Attach a collar to the end of the 2-inch square bar. Tighten it using the 5/64-inch Allen wrench.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image11_post_collar.jpg' />
</li><li>Slide the omni wheel on to the bar, followed by a 1/4-inch spacer.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image13_post_wheel_spacer.jpg' />
</li><li>Slide the assembly through the bumper and into the pillow block.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image14_wheel_pillowblock.jpg' />
</li><li>Place a collar on the other side of the bar. Tighten it using the 5/64-inch Allen wrench.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image15_secure_wheel.jpg' /></li></ol>

<hr />

<h2>Attach the Battery</h2>

<h3>Required Tools</h3>

<table><thead><th> Image </th><th> Tool </th></thead><tbody>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/tools/hex_key_3_32-300.jpg' /> </td><td> 3/32" Allen Wrench </td></tr></tbody></table>

<h3>Required Parts</h3>

<table><thead><th> Image </th><th> Part </th></thead><tbody>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/battery_strap-300.jpg' /> </td><td> Battery Strap </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/battery-300.jpg' /> </td><td> 7.2v Battery </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/screw-832-0375_300.png' /> </td><td> 8-32 3/8-inch Bolt </td></tr>
<tr><td> <img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/vex_parts/nut-832-keps_300.png' /> </td><td> 8-32 Keps Nuts </td></tr></tbody></table>

<h3>Steps</h3>

<ol><li>On the underside of the plate, align a battery strap one hole away from the side, with the strap pointing to the back of the robot.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image6_strap1.jpg' />
</li><li>Use 8-32 1/8-inch bolts and keps nuts to secure the strap.<br>
</li><li>Repeat this for the second strap.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image7_strap2.jpg' />
</li><li>Place the battery on the underside of the plate.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image8_fasten_battery.jpg' />
</li><li>Loop the straps around the battery, and insert the ends through the slots near the base.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image9_loop_straps.jpg' />
</li><li>Tighten the straps to secure the battery firmly.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image10_tighten_straps.jpg' />
</li><li>Flip the robot. It is now complete, and ready for its processor.<br><img src='http://cmucreatelab.org/www.terk.ri.cmu.edu/wp-content/uploads/basic-vex-bot/image11_ready.jpg' /></li></ol>

<hr />
<a href='RobotRecipes.md'>previous</a> | next