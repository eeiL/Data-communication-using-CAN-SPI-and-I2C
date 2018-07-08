#include <Wire.h>

#define dataLength  8

int led = 13;
int Data[8];
int returnData[8];// = {11,22,33,44,55,66,77,88};
int flag_receive = 0;


void setup() {
  //Wire.begin(8);                // Slave1 join i2c bus with COM 10
  Wire.begin(10);                // Slave2 join i2c bus with COM 12
  //Wire.begin(12);               // Slave3 join i2c bus with COM 6
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent);
  Serial.begin(115200);           // start serial for output
  pinMode(led,OUTPUT);
}

void loop() {
  if(flag_receive){
    Serial.println("Data receive;");
    for(int i=0; i<dataLength; i++){
      Serial.print("Data[");
       Serial.print(i);
       Serial.print("]: ");
       Serial.println(Data[i],HEX);
      }
     Serial.println("");
    flag_receive = 0;
  }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  for(int i=0; i<dataLength; i++){
    Data[i] = Wire.receive();    // receive byte as an integer
    returnData[i] = Data[i];   //for COM 10
  }
  flag_receive = 1;  
  digitalWrite(led, LOW);
}

void requestEvent(){
  for(int i=0; i<8; i++){
    Wire.send(returnData[i]);
  }
  digitalWrite(led, HIGH);
}

