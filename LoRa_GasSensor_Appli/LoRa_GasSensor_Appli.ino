#include <SoftwareSerial.h>
#include <rn2xx3.h>
#include <string.h>
#include "analogComp.h"

#define LORA_TX         10
#define LORA_RX         11
#define LORA_RST        12

#define TEMP_PIN        A1
#define REF_PIN         AIN0  // Connect the reference pin to digital input pin 6 for analog interrupt reference 
#define TEST_REF_PIN   A5 // To see reference voltage value, remember to connect the cable to pin A5

#define LED_PIN         7

#define GAS_SENSOR_PIN  A1 

#define BAUD_RATE_DEBUG 57600
#define BAUD_RATE_LORA  9600

volatile int count = 0;


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
  
  //always reset rn2483 before configuration, if not the command to lora module won't work (remark: Lora RST pin is active-low)
  pinMode(LORA_RST, OUTPUT);
  digitalWrite(LORA_RST, LOW);
  delay(500);
  digitalWrite(LORA_RST, HIGH);

  delay(100); //wait for the RN2xx3's startup message
  loraSerial->flush();
  
  lora->autobaud(); //Autobaud the rn2483 module to BAUD_RATE_LORA(9600). Otherwise the default baud rate will stay at 57600.
  serial_test(lora);
  TTN_config(lora);
  
}

void tx_data (rn2xx3* lora, byte* data_bytes, int size_of_data_byte) {
  TX_RETURN_TYPE tx_res;

  
  Serial.println("Transmitting data... : ");

  tx_res = lora -> txBytes(data_bytes, size_of_data_byte);
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
  
  delay(1000); // delay 10s
}

/* getVoltage() – returns the voltage on the analog input defined by pin */
float getVoltage(int pin){
  return (analogRead(pin) * .004882814); //converting from a 0 to 1023 digital range
  // to 0 to 5 volts (each 1 reading equals ~ 5 mV)
}

int retrieve_temp_int(){
  float temperature = getVoltage(TEMP_PIN); //getting the voltage reading from the temperature sensor
  temperature = (temperature - 0.5) * 100; //converting from 10mV/°C with 500 mV
  //offset to degrees ((voltage – 500mV)*100)
  int temperature_int =  (int)temperature;
  Serial.println(temperature_int); //printing the result
  return temperature_int;
}


void exceed_thr (){
   digitalWrite(LED_PIN, HIGH);
   count++;
   
}

void analogReadnPrintVolt_REF_TEST (){
  
  float ref_value;
  float ref_volt ;
  ref_value = analogRead(TEST_REF_PIN);
  ref_volt = ref_value/1024*5.0;
  Serial.print("ref_volt = ");
  Serial.print(ref_volt);
  Serial.println("V");

}

//convert int to table of 2 bytes
byte* int_to_byte(int val_int){
//  for debugging purpose:
//  Serial.print("val_int : ");
//  Serial.println(val_int);

  byte* tab_byte = (byte*)malloc(2*sizeof(byte));
  memset(tab_byte, 0, 2);
  
  tab_byte[0] = val_int >> 8;
  tab_byte[1] = val_int & 0xFF;
  
//  for debugging purpose:
//  Serial.print("byte : 0x ");
//  Serial.print(tab_byte[0]);
//  Serial.print(" ");
//  Serial.println(tab_byte[1]);
  
  return tab_byte;

}

int retrieve_gas_sensor_val(){
    
    int sensorValue;
    float sensor_volt;
    
    sensorValue = analogRead(GAS_SENSOR_PIN);
    Serial.print("sensorValue = ");
    Serial.println(sensorValue);
      
    //print the voltage
    sensor_volt = (float)sensorValue/1024*5.0;
    Serial.print("sensor_volt = ");
    Serial.print(sensor_volt);
    Serial.println("V");

    return sensorValue;
}

void setup() {
  
  Serial.begin(BAUD_RATE_DEBUG);  
  loraSerial.begin(BAUD_RATE_LORA);
  
  Serial.println("Starting...");
  lora_setup(&lora, &loraSerial);
  
  pinMode(LED_PIN, OUTPUT);
  
//  analogComparator.setOn(REF_PIN, GAS_SENSOR_PIN);
//  analogComparator.enableInterrupt(exceed_thr, CHANGE);
}



void loop() {

//  data_rx_test(&lora, &loraSerial);
  
  Serial.println("############loop############");
  
  Serial.println("Counter : ");
  Serial.println(count);

//  tansmit only 1 byte for temperature o save energy since tempearture in degree celcius does not exceed 255(0xFF)
//  byte* data_byte = int_to_byte(retrieve_temp_int()); 
//  tx_data(&lora, data_byte, 1);
 
  byte* data_byte = int_to_byte(retrieve_gas_sensor_val());
  tx_data(&lora, data_byte, 2);
  
  free(data_byte);  
  delay(1000); //waiting a second  
  

}
