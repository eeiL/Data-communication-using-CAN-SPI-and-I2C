#include <t3spi.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>

#define CTAR0 0
#define CTAR1 1

T3SPI SPI_MASTER; //Initialize T3SPI class as SPI_MASTER

FlexCAN CAN_Transfer(500000);
static CAN_message_t txmsg,rxmsg;

//The number of integers per data packet
#define dataLength0 9
#define dataLength1 9
#define dataLength2 9

//Initialize the arrays for outgoing data
volatile uint8_t  data0[dataLength0] = {};
volatile uint8_t  data1[dataLength1] = {};
volatile uint8_t  data2[dataLength2] = {};

//Initialize the arrays for incoming data
volatile uint8_t returnData0[dataLength0] = {}; 
volatile uint8_t returnData1[dataLength1] = {};
volatile uint8_t returnData2[dataLength2] = {}; 

void setup(){
  Serial.begin(115200);
  CAN_Transfer.begin();
  
  //setup for SPI master
  SPI_MASTER.begin_MASTER(SCK, MOSI, MISO, CS0, CS_ActiveLOW);
  SPI_MASTER.setCTAR(CTAR0,8,SPI_MODE0,LSB_FIRST,SPI_CLOCK_DIV128);

  pinMode(15,OUTPUT);
  pinMode(20,OUTPUT);
  pinMode(21,OUTPUT);
  pinMode(10,OUTPUT);
  
  //delay(500);
  Serial.println("CAN SPI Umsteigen ready");
}

void loop(){
  if( CAN_Transfer.read(rxmsg)){ //from ECU
    Serial.println("Receiving from CAN Master: ");
    Serial.print("ID= ");
    Serial.println(rxmsg.id,HEX);
    for(int i=0; i<rxmsg.len; i++) {
      data0[i+1] = rxmsg.buf[i];
      data1[i+1] = rxmsg.buf[rxmsg.len-1-i];
      data2[i+1] = rxmsg.buf[i]+1;
      Serial.print(rxmsg.buf[i]); Serial.print(" ");
    }
    Serial.println("");
    Serial.println("");
  }
  
  //Capture the time before sending data
  //SPI_MASTER.timeStamp1 = micros();

  //Send n number of data packets
  for (int i=0; i<1; i++) {
    digitalWrite(21,LOW);
    SPI_MASTER.txrx8(data0, returnData0, dataLength0,CTAR0,CS3);
    delayMicroseconds(10);
    digitalWrite(21,HIGH);
    SPI_MASTER.packetCT=0;}//Reset the packet count 
    
  for (int i=0; i<1; i++) {
    digitalWrite(20,LOW);
    SPI_MASTER.txrx8(data1, returnData1, dataLength1,CTAR0,CS2);
    delayMicroseconds(10);
    digitalWrite(20,HIGH);SPI_MASTER.packetCT=0;}
    
  for (int i=0; i<1; i++) {
    digitalWrite(10,LOW);
    SPI_MASTER.txrx8(data2, returnData2, dataLength2,CTAR0,CS0); 
    delayMicroseconds(10);
    digitalWrite(10,HIGH);SPI_MASTER.packetCT=0;}
    
  //Capture the time when transfer is done
  //SPI_MASTER.timeStamp2 = micros();
  
  for (int i=0; i<dataLength0; i++){
    Serial.print("returnData from SPI Slave[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.print(returnData0[i]);
    Serial.print(", ");
    Serial.print(returnData1[i]);
    Serial.print(", ");
    Serial.println(returnData2[i]);
    Serial.flush();}
    
  Serial.println("");
  //Print statistics for the previous transfer
  //SPI_MASTER.printStatistics(dataLength);
  
  Serial.println("Sending to CAN Master: ");
  
  Serial.print("Data from Slave0: ");
  txmsg.id = 0x200;
  txmsg.len = 8;
  for(int i=0; i<txmsg.len; i++) { 
    txmsg.buf[i] = returnData0[i+1];
    Serial.print(txmsg.buf[i]); Serial.print(" ");}
  Serial.println("");
  CAN_Transfer.write(txmsg);

  Serial.print("Data from Slave1: ");
  txmsg.id = 0x210;
  txmsg.len = 8;
  for(int i=0; i<txmsg.len; i++) { 
    txmsg.buf[i] = returnData1[i+1];
    Serial.print(txmsg.buf[i]); Serial.print(" ");}
  Serial.println("");
  CAN_Transfer.write(txmsg);
  
  Serial.print("Data from Slave2: ");
  txmsg.id = 0x220;
  txmsg.len = 8;
  for(int i=0; i<txmsg.len; i++) { 
    txmsg.buf[i] = returnData2[i+1];
    Serial.print(txmsg.buf[i]); Serial.print(" ");}
  Serial.println("");
  CAN_Transfer.write(txmsg);

  Serial.println("");
  Serial.println("CAN SPI Umsteigen");
  Serial.println("");
  delay(2000);
}
