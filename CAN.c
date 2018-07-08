#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include <TimerOne.h>

int led = 13; //using the on-board led to indicate
// create CAN object
FlexCAN CANReceiver(500000);
static CAN_message_t msg;

void setup() {
  
  Serial.begin(9600);
  pinMode(led, OUTPUT); 
  
  // init CAN bus and Timer1 for interrupt 
  CANReceiver.begin();
  Timer1.initialize(10000);
  Timer1.attachInterrupt(ECU);
  delay(1000);
  Serial.println("CAN Receiver Initialized");
}

//Interrupt for every 100ms
void ECU(void){
  while( CANReceiver.read(msg)) {
    digitalWrite(led, !digitalRead(led));
    
    // CAN date ID=0x100
    if(msg.id == 0x100){
      Serial.print("ID=0x100: ");
      for(int i=0; i<msg.len; i++) {
      Serial.print(msg.buf[i], HEX); Serial.print(", "); //print date on the screen
    }
    Serial.println("");
    }
    
    // CAN date ID=0x101
    else if(msg.id == 0x101){
      Serial.print("ID=0x101: ");
      for(int i=0; i<msg.len; i++) {
      Serial.print(msg.buf[i], HEX); Serial.print(", ");
    }
    Serial.println("");
    }
    
    // CAN date ID=0x102
    else if(msg.id == 0x102){
      Serial.print("ID=0x102: ");
      for(int i=0; i<msg.len; i++) {
      Serial.print(msg.buf[i], HEX); Serial.print(", ");
    }
    Serial.println("");
    }
    Serial.println("");
 }
}

void loop() {
  
}

