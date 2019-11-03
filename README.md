# Heltec_CubeCell
Documents and Code for  use with Heltec CubeCell LoraWAN Node Boards, Capsule, and Modules

The Heltec CubeCell product line is a good product but as is often the case the actual use, show differences to the design. In addition Docuemntation is not always as readable as needed.

This  Repository will hold the current notes on the product  and as tested, working sensor test and LoRaWAN  sketches for other sensors not provided within the Heltec Example Code. It should be noted that each sensor has its own .c/.h in the folder so that if you have files of the same name the correct library is used, even though this is wastefull in storage space it provides a controlled use of libraries.

The CubeCell product is very good in that it has:
•	Low standby power consumption,
•	Arduino IDE configured and using simple script and settings in the board manager
•	Ability to be configured by AT commands such as keys
•	Has a convenient way to handle Sensor types and code
•	Very small 41.5 x 24.1 x 7.2 mm
•	It is based on the Semtech LoRaMac-node reference implementation and documentation in GitHub, LoRa-Net network node. Release Version 4.4.1 Release date  2018.03.07 which is LoRaWAN version 1.0.2.

This guide is design to assist with the issues that may arrise or information on actual experienced  in testing. It is recommended that the user fully follows the Heltec Software download and installing process, this seems to work for most people.

You can comment or email to Simon At Securethings dot UK, I will try to respond but this is not a help service, and aimed at just providing a real user experience and information, along with tested  Additional Sensor Code.  
