/*
  ESP32_SEA_SPIFFS_Loader
  Version 1.1
  Loads a bitstream named "default.bit" from the SPIFFS to the Spartan-7 FPGA on the Spartan Edge Accelerator Board.
  This makes the board usable without a sd card.
*/

#include "FS.h"
#include "SPIFFS.h"

#define XFPGA_CCLK_PIN 17
#define XFPGA_DIN_PIN 27
#define XFPGA_PROGRAM_PIN 25
#define XFPGA_INTB_PIN 26
#define XFPGA_DONE_PIN 34

char buf[8192];

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("ESP32-SEA-SPIFFS-Loader V1.1");
  if(!SPIFFS.begin(true)){
      Serial.println("SPIFFS Mount Failed!");
      while(true){ 
        Serial.println("Please upload a valid SPIFFS filesystem!");
        delay(1000); 
      }
  }
  
  Serial.println();
  
  pinMode(XFPGA_INTB_PIN, INPUT);
  pinMode(XFPGA_DONE_PIN, INPUT);
  pinMode(XFPGA_PROGRAM_PIN, OUTPUT);

  Serial.print("FPGA: Resetting ... ");
  // reset FPGA configuration logic
  digitalWrite(XFPGA_PROGRAM_PIN, LOW);
  pinMode(XFPGA_CCLK_PIN, OUTPUT);
  digitalWrite(XFPGA_CCLK_PIN, LOW);
  digitalWrite(XFPGA_PROGRAM_PIN, HIGH);

  // wait until FPGA reports reset complete
  while(digitalRead(XFPGA_INTB_PIN) == 0) {}
  Serial.println("DONE!");

  loadBitstream();
}

void loop() {

}

void loadBitstream(){
  Serial.println("Opening bitstream file: default.bit");
  
  File file = SPIFFS.open("/default.bit");
  if(!file || file.isDirectory()){
      Serial.println("Could not open default.bit!");
      while(true){}
  }

  long startTime = millis();

  // we dont need to skip the header in the bit file, the configuration logic will ignore data until the sync word 0xAA995566 is received

  Serial.print("Loading FPGA ... ");
  pinMode(XFPGA_DIN_PIN, OUTPUT);

  while(file.available()){
     int cnt = file.readBytes(buf, sizeof(buf));
     for(int i=0; i<cnt; i++){ 
      shiftOut(XFPGA_DIN_PIN, XFPGA_CCLK_PIN, MSBFIRST, buf[i]);
     }
  }
  
  Serial.println("DONE!");
  digitalWrite(XFPGA_CCLK_PIN, LOW); 
  
  file.close();

  long endTime = millis();

  if(digitalRead(XFPGA_DONE_PIN) == 0) {
    Serial.println("FPGA Configuration FAILED!");
  } else {
    Serial.println("FPGA Configuration SUCCESS!");
  }

  long loadingTime = endTime - startTime;
  Serial.print("Loading took ");
  Serial.print(loadingTime);
  Serial.println("ms");
}
