#include <Wire.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include <TimerOne.h>
#include <TimerThree.h>

#define dataLength  8

FlexCAN CAN_Umsteigen(500000);
static CAN_message_t txmsg,rxmsg;

int led = 13;
int Data[dataLength];
int Data0[dataLength];//={11,22,33,44,55,66,77,88};
int Data1[dataLength];//={180,190,200,210,220,230,240,250};
int Data2[dataLength];//={180,190,200,210,220,230,240,250};
int flag_ecu = 0;
int flag_i2c = 0;
int returnData0[dataLength], returnData1[dataLength],returnData2[dataLength];

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  CAN_Umsteigen.begin();
  pinMode(led,OUTPUT);
  Serial.begin(115200);
  void Write(int ch);
  
  Timer1.initialize(10000);
  Timer1.attachInterrupt(ECU);
  Timer3.initialize(10000);
  Timer3.attachInterrupt(I2C);
  Serial.println("Master ready");
}


void ECU(void){
  while(CAN_Umsteigen.read(rxmsg)){   
      if(rxmsg.id == 0x175){
        for(int i=0; i<rxmsg.len; i++) {
        Data0[i] = rxmsg.buf[i];}

        Wire.beginTransmission(8); // transmit to device #8
        for(int i=0; i<dataLength; i++){
        Wire.send(Data0[i]);
        }
        Wire.endTransmission();    // stop transmitting   
      }
      
      if(rxmsg.id == 0x179){
        for(int i=0; i<rxmsg.len; i++) {
        Data1[i] = rxmsg.buf[i];}

        Wire.beginTransmission(10); // transmit to device #8
        for(int i=0; i<dataLength; i++){
        Wire.send(Data1[i]);
        }
        Wire.endTransmission();    // stop transmitting    
      }
      
      if(rxmsg.id == 0x17D){
        for(int i=0; i<rxmsg.len; i++) {
        Data2[i] = rxmsg.buf[i];}
        
        Wire.beginTransmission(12); // transmit to device #8
        for(int i=0; i<dataLength; i++){
        Wire.send(Data2[i]);
        }
        Wire.endTransmission();    // stop transmitting
      }
  }
  //flag_ecu = 1;
  digitalWrite(led, LOW);
}

void I2C(void){
  txmsg.len = 8;
  
  Wire.requestFrom(8,8);
  while(Wire.available()) {
    txmsg.id = 0x285;
    for(int i=0; i<dataLength; i++){
      returnData0[i]= Wire.receive();
      txmsg.buf[i]= returnData0[i];}
    CAN_Umsteigen.write(txmsg);
  }

  Wire.requestFrom(10,8);
  while(Wire.available()) {
    txmsg.id = 0x289;
    for(int i=0; i<dataLength; i++){
      returnData1[i]= Wire.receive();
      txmsg.buf[i]= returnData1[i];}
    CAN_Umsteigen.write(txmsg);
  }

  Wire.requestFrom(12,8);
  while(Wire.available()) {
    txmsg.id = 0x28D;
    for(int i=0; i<dataLength; i++){
      returnData2[i]= Wire.receive();
      txmsg.buf[i]= returnData2[i];}
    CAN_Umsteigen.write(txmsg);
  }
 
  flag_i2c = 1;
  digitalWrite(led, HIGH);
  //digitalWrite(led, !digitalRead(led));
}

void loop() {
  if(flag_ecu){
    Serial.println("Command from ECU:");
    Serial.print("");
  }
  if(flag_i2c){
    Serial.println("Data from SPI Slave:");
    for(int i=0; i<dataLength; i++){
      //Serial.print(returnData0[i],HEX);
      //Serial.print(", ");
      Serial.print(returnData1[i],HEX);
      Serial.print(", ");
      Serial.println(returnData2[i],HEX);
    }
    Serial.println("");
    flag_i2c = 0;
  }
}


void Write(int ch){
  while(CAN_Umsteigen.read(rxmsg)){
    if(ch == 8){
      if(rxmsg.id == 0x175){
        for(int i=0; i<rxmsg.len; i++) {
        Data[i] = rxmsg.buf[i];}
      }
      Wire.beginTransmission(ch); // transmit to device #8
      for(int i=0; i<dataLength; i++){
      Wire.send(Data[i]);
      }
      Wire.endTransmission();    // stop transmitting
    }
    
    if(ch == 10){
      if(rxmsg.id == 0x179){
        for(int i=0; i<rxmsg.len; i++) {
        Data[i] = rxmsg.buf[i];}
      }
      Wire.beginTransmission(ch); // transmit to device #8
      for(int i=0; i<dataLength; i++){
      Wire.send(Data[i]);
      }
      Wire.endTransmission();    // stop transmitting
    }
    
    if(ch == 12){
      if(rxmsg.id == 0x17D){
        for(int i=0; i<rxmsg.len; i++) {
        Data[i] = rxmsg.buf[i];}
      }
      Wire.beginTransmission(ch); // transmit to device #8
      for(int i=0; i<dataLength; i++){
      Wire.send(Data[i]);
      }
      Wire.endTransmission();    // stop transmitting
    }
  }
}
