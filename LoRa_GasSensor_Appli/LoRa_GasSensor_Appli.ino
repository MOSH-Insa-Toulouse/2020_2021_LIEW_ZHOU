#include <SoftwareSerial.h>
#include <rn2xx3.h>

#define LORA_TX         10
#define LORA_RX         11
#define LORA_RST        12

#define BAUD_RATE_DEBUG 57600
#define BAUD_RATE_LORA  9600

const char *devEUI = "EEEEEDB1212E24DE"; 
const char *appEUI = "70B3D57ED0038A97"; 
const char *devAddr = "26013775";
const char *nwkSKey = "9DD70B8DF7D64F5732CFB2FBFBE67D08";
const char *appSKey = "7C4C2E41BA487F5A688387C5B52D88B7";


SoftwareSerial loraSerial(LORA_TX,LORA_RX); // (ARDUINO_RX -- LORA_TX, ARDUINO_TX -- LORA_RX)

rn2xx3 lora(loraSerial);

void serial_test(rn2xx3* lora) {
  
  Serial.println("Testing Connection with RN2483A...");
  Serial.print("My LoRa module hweui : ");
  Serial.println(lora->hweui());
  Serial.print("RN2483A firmware version : ");
  Serial.println(lora->sysver());  
  
}

void TTN_config (rn2xx3* lora) {
  
  bool join_res = false;
  
  Serial.println("Configuring TTN...");
  
  Serial.println("Joining TTN first try ...");
  join_res = lora->initABP(devAddr, appSKey,nwkSKey);
  
  while(!join_res){
    Serial.println("Retrying to join TTN ...");
    delay(30000); //delay 30s
    join_res = lora->initABP(devAddr, appSKey,nwkSKey);   
  }

  Serial.println("Successfully join TTN ...");
  
}

void lora_setup(rn2xx3* lora, SoftwareSerial* loraSerial) {

  Serial.println("Setting up LoRa...");
  lora->autobaud(); //Autobaud the rn2483 module to BAUD_RATE_LORA(9600). Otherwise the default baud rate will stay at 57600.
  serial_test(lora);
  TTN_config(lora);
  
}

void data_tx_test (rn2xx3* lora) {
  TX_RETURN_TYPE tx_res;
  const char *data_test = "48656c6C6F";
  
  Serial.print("Transmitting data... : ");
  Serial.println(data_test);
  
  tx_res = lora->txCnf(data_test);
  
  if(tx_res == 0){
    Serial.println("Transmission Failed :(");
  }
  else if(tx_res == 1){
    Serial.println("Transmission Succeed :)");
  }
  else if(tx_res == 2){
    Serial.println("Downlink message was received after transmission");
  }
  else{
    Serial.println("Error!!!");
  }

  delay(10000); //delay 10s
}

void data_rx_test (rn2xx3* lora, SoftwareSerial* loraSerial) {
    
  Serial.println("Receiving data...");
  lora->txCnf("mac pause");
  lora->txCnf("radio rx 0");
  
//  loraSerial.println("mac pause");
//  delay(10);
//  loraSerial.println("radio rx 0");
  
  Serial.println("Data received : ");
  while (loraSerial->available()) {
     Serial.write(loraSerial->read());
  }
  
  delay(10000); // delay 10s
}

void setup() {
  
  Serial.begin(BAUD_RATE_DEBUG);  
  loraSerial.begin(BAUD_RATE_LORA);
  
  Serial.println("Starting...");
  lora_setup(&lora, &loraSerial);
  
}

void loop() {
  data_tx_test(&lora);
//  data_rx_test(&lora, &loraSerial);
}
