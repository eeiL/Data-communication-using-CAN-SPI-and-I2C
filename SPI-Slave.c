#include <t3spi.h>

T3SPI SPI_SLAVE;  //Initialize T3SPI class as SPI_SLAVE
//The number of integers per data packet MUST be the same as defined on the MASTER device
#define dataLength  9

volatile uint8_t data[dataLength] = {}; //Initialize the arrays for incoming data
volatile uint8_t returnData[dataLength] = {}; //Initialize the arrays for outgoing data
const uint8_t Data[]={11,220,225,230,235,240,245,250,253}; //COM12

void setup(){
  Serial.begin(115200);
 
  //Setup for SPI-Slave
  SPI_SLAVE.begin_SLAVE(SCK, MOSI, MISO, CS0); //Begin SPI in SLAVE (SCK pin, MOSI pin, MISO pin, CS pin)
  SPI_SLAVE.setCTAR_SLAVE(8, SPI_MODE0); //Set the CTAR0_SLAVE0 (Frame Size, SPI Mode)
  
  NVIC_ENABLE_IRQ(IRQ_SPI0); //Enable the SPI0 Interrupt
  
  for (int i=0; i<dataLength; i++){
    returnData[i]= Data[i];}
  
  delay(10);  
  Serial.println("Slave is ready");
}

void spi0_isr(void){ //Interrupt Service Routine to handle incoming data
  SPI_SLAVE.rxtx8 (data, returnData, dataLength);
}

void loop(){
  if (SPI_SLAVE.packetCT==1){  
    for (int i=0; i<dataLength; i++){
       Serial.print("data[");
       Serial.print(i);
       Serial.print("]: ");
       Serial.println(data[i],HEX);}
    
     //SPI_SLAVE.printStatistics(dataLength);
     Serial.println("Slave");
     
     SPI_SLAVE.packetCT=0;}
}
