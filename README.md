This is a Community Device for Mobiflight that uses and ESP32 S3 and a Waveshare 4" LCD. I used [this] (https://www.aliexpress.us/item/3256807167751643.html) but I'm now realizing what a one-off piece of hardware it really is. Probably a rip-off of this: https://www.waveshare.com/wiki/ESP32-S3-LCD-Driver-Board and the demos from that size work on the board. It uses standard ST7701 parallel LCD driver and an ESP32-S3 N8R8. I'm going to try some other, more common devices. Probably 
https://www.makerfabs.com/esp32-s3-parallel-tft-with-touch-4-inch.html

Key to getting this working was  https://github.com/lovyan03/LovyanGFX/tree/master for the LCD library and https://github.com/elral/MF_CommunityTemplate for the ESP32 version of the MobiFlight custom device library. I had to create a custom header with the right settings for the board. You can see that file as "4inchLCDConfig.h" in the repo. 

There is no special wiring required if you're using the hardware above. Maybe when this is complete I'll add an encoder for the EGT marker, but for now it's output only. No configuration, although I've not stripped that out yet. 

Known issues:
-- I need to clean up the graphics. Not sure if I'll stick with my own drawn gauges or move to my typical "Screen shot with movable elements" technique. 
-- Need to add the rest of the guages for the "4-pack" of instruments. (Fuel L&R, Oil Press & Temp, EGT & Fuel Flow, Ammeter & Vacuum.) 
-- Mobiflight is inconsistent about connecting to the device. I'm not sure why, but if this isn't resolved it could be a deal-killer. I hate futzing with all my peripherals when all I want to do is fly.
-- If you pass an empty string during testing it crashes. I've put in a lot of defensive code in my tests. Need to debug.

