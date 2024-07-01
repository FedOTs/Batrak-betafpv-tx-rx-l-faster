 #include "CRSF.h"
#include <Streaming.h>
//https://github.com/janelia-arduino/Streaming

#include "sbus.h"

bfs::SbusRx sbus_rx(&Serial1, 14, 12, true);


bfs::SbusTx sbus_tx(&Serial1, 14, 12, true);

/* SBUS data */
bfs::SbusData data;

#define DEBUG // if not commented out, Serial.print() is active! For debugging only!!

const bool testAutoSendMode = false;
#define FRESH_TIME_US 4000  // Update every 4ms
unsigned long int currentMicros = 0;

int packetPerSec;
unsigned long int currentMillis;

#define SECOND 1000

CRSF crsf;

void setup(){
  crsf.begin();

  sbus_rx.Begin();
  sbus_tx.Begin();

  #ifdef DEBUG
    Serial2.begin(115200);
    Serial.begin(115200);
    packetPerSec = 0;
    currentMillis = 0;
  #endif
  delay(1000);
}

void loop(){
  //delay(5);
  
  if (micros() - currentMicros >= FRESH_TIME_US) {
    crsf.GetCrsfPacket();
    if(crsf.crsfData[1]==24){ //data packet
      crsf.UpdateChannels();
    }
    currentMicros = micros();
  }
    int rcCH2; // Левый стик вверх - вниз
    int rcCH0; // Правый стик вправо - влево
    int rcCH6; // Левый триггер нажат или нет

    int checkBuffer; // Проверка буффера. Если данных нет, то связь потеряна.

    checkBuffer = crsf.inBuffer[10]+crsf.inBuffer[11]+crsf.inBuffer[12];

    data.ch[0] = 1000;
    data.ch[1] = 1003;
    data.ch[2] = 1004;
    data.ch[3] = 996;
    data.ch[4] = 1000;
    data.ch[5] = 1000;
    data.ch[6] = 200;
    data.ch[7] = 200;
    

    if (checkBuffer > 2) {
        
        rcCH0 = map(crsf.channels[0], 191, 1809, 601, 1398);
        rcCH6 = map(crsf.channels[5], 191, 1792, 0, 100);
    
        if (rcCH6 > 90) {
              rcCH2 = map(crsf.channels[2], 214, 1811, 200, 1800);
          }
        else {
              rcCH2 = map(crsf.channels[2], 214, 1811, 600, 1400);
          } 
        data.ch[0] = rcCH0;
        data.ch[1] = rcCH2;
         
    }

    sbus_tx.data(data);

     
    //Serial.print(crsf.channels[0]); 
    //Serial.print(crsf.channels[0]); 
    //Serial.print(crsf.channels[1]); 
    //Serial.print(crsf.channels[2]); 
    //Serial.println();
    Serial<<crsf.channels[0]<<","<<crsf.channels[1]<<","<<crsf.channels[2]<<","<<crsf.channels[3]<<","<<crsf.channels[4]<<","<<crsf.channels[5]<<","<<crsf.channels[6]<<","<<crsf.channels[7]<<","<<rcCH2<<","<<rcCH0<<","<<rcCH6<<"\r\n";
    // Serial<<crsf.channels[0]<<","<<crsf.channels[1]<<","<<crsf.channels[2]<<","<<crsf.channels[3]<<","<<crsf.channels[6]<<","<<crsf.channels[10]<<","<<crsf.channels[11]<<","<<crsf.channels[12]<<","<<crsf.channels[13]<<"\r\n";
    
   // Serial<<crsf.crsfData[0]<<","<<crsf.crsfData[1]<<","<<crsf.crsfData[2]<<","<<crsf.crsfData[3]<<"\r\n";
   // Serial<<crsf.channels[0]<<","<<crsf.channels[1]<<","<<crsf.channels[2]<<","<<crsf.channels[3]<<","<<crsf.inBuffer[4]<<","<<crsf.inBuffer[5]<<","<<crsf.inBuffer[24]<<","<<crsf.inBuffer[25]<<"\r\n";
   //Serial.println(startMillis);
  
}
