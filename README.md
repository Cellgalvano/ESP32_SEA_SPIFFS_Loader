# ESP32_SEA_SPIFFS_Loader
SPIFFS Bitstreamloader for the Spartan Edge Accelerator Board (no SD Card required)

The Spartan Edge Accelerator Boards loads the bitstream into the fpga via the onboard ESP32.  
Default Sketch:  
Micro SD Card --> ESP32 --> Spartan-7 (via Slave Serial Configuration Mode)  

This sketch simply uses the onboard SPI flash chip (W25Q32JV, 4MB) to store the bitstream next to the ESP32 code.  
By default the ESP32 allows you to use the normaly unused space on the flash to store files using [SPIFFS](https://github.com/pellepl/spiffs).  
ESP32_SEA_SPIFFS_Loader:  
SPI flash (SPIFFS) --> ESP32 --> Spartan-7 (via Slave Serial Configuration Mode)  


### Installation
1. Clone the repository to your Arduino Sketchbook folder
2. Install the [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
3. Install the [ESP32 SPIFFS Upload Plugin](https://github.com/me-no-dev/arduino-esp32fs-plugin)
4. Compile the Sketch and upload it to the Spartan Edge Board (select the "DOIT ESP32 DEVKIT" board from the tools menu)
5. Set the switch "K5" to the "Slave" position
6. Create a folder called "data" within the Sketch folder
7. Place your bitstream for the Spartan-7 FPGA in the "data" folder an name it "default.bit"
8. Upload the SPIFFS filesystem to the ESP32 via the Arduino IDE Plugin in the tools menu

If you want to update the bitstream just replace the file in the data folder and reupload the filesystem.

### Sources
* https://github.com/sea-s7/spartan-edge-esp32-boot
* https://www.xilinx.com/support/documentation/user_guides/ug470_7Series_Config.pdf
* https://github.com/espressif/arduino-esp32/tree/master/libraries/SPIFFS
