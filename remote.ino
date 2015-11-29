/*
SP8XCE & SQ9MDD
REMOTE odbior, potwierdzenie odbioru + obsluga dekodera BCD 5 pinow

CHANGELOG nowe na górze
 2015.11.29 - SQ9MDD dodałem gaszenie wyjść 5sec po ostatnim wysterowaniu
 2015.11.29 - uruchomienie sterowania diodami podpietymi do wyjscia dekodera BCD  
 2015.11.26 - dodanie konfiguracji adresu (programowo), dodanie reakcji na adres

*/
#define ver 1.3

#include <SPI.h>
#include <RF22.h>

//inicjalizacja
RF22 rf22;

//zmienne
int net_addr = 11;
unsigned long time_to_gaszenie = 0;   //pomocnicza do wygaszania wejść
int bcd_E1 = 5;
int bcd_A0 = 6;
int bcd_A1 = 7;
int bcd_A2 = 8;
int bcd_A3 = 9;

//funkcje
void rcv_data(){
  //TX
  //rf22.send((uint8_t*)&TX, sizeof(TX));
  //rf22.waitPacketSent();
  //RX
  uint8_t buf[RF22_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  if (rf22.waitAvailableTimeout(500)){ 
  if (rf22.recv(buf, &len)){
      int RX = (int&)buf;
      //przychodzi ramka XXYYZ na przykład: 11101 dzielimy to bez reszty przez 1000 by oddzielić część z adresem
      int tmp_addr = RX / 1000; 
      int reszta = RX % 1000;
      //jeśli adres jest poprawny to proceduje reszte
      if(net_addr == tmp_addr){
        //wyciagam adres wyjścia
        int adres_wyjscia = reszta / 10;
        //wyciagam polecenie 1 - ON, 0 - OFF
        int polecenie = reszta % 10; 
          //jak już mam adres wyjscia to moge nimi sterowac
          switch(adres_wyjscia){
            case 10: // sprawdzenie czy zyje odbiornik
              Serial.println("sprawdzenie czy odbiornik zyje");
            break;
            case 11:
              if(polecenie == 1){
              Serial.println("odpalenie wyjscia 1"); 
              digitalWrite(bcd_E1,HIGH);
              digitalWrite(bcd_A0,LOW);
              digitalWrite(bcd_A1,LOW);
              digitalWrite(bcd_A2,LOW);
              digitalWrite(bcd_A3,LOW);
              }break;
            case 12:
              if(polecenie == 1){
              Serial.println("odpalenie wyjscia 2");
              digitalWrite(bcd_E1,HIGH);
              digitalWrite(bcd_A0,HIGH);
              digitalWrite(bcd_A1,LOW);
              digitalWrite(bcd_A2,LOW);
              digitalWrite(bcd_A3,LOW);
              }break;
            case 13:
              if(polecenie == 1){
              Serial.println("odpalenie wyjscia 3");
              digitalWrite(bcd_E1,HIGH);
              digitalWrite(bcd_A0,LOW);
              digitalWrite(bcd_A1,HIGH);
              digitalWrite(bcd_A2,LOW);
              digitalWrite(bcd_A3,LOW);
              }break; 
          }
        
      }

      //debugowanie printujemy wszystko co odbieramy
      Serial.print("mamy odpowiedz: ");
      Serial.print(RX);
      }
   }
}

void odpalanie_wyjscia(int wejscie){
  //sterowanie BCD
  switch(wejscie){
     case 1:
        digitalWrite(bcd_E1,HIGH);
        digitalWrite(bcd_A0,LOW);
        digitalWrite(bcd_A1,LOW);
        digitalWrite(bcd_A2,LOW);
        digitalWrite(bcd_A3,LOW);
     break;
     case 2:
        digitalWrite(bcd_E1,HIGH);
        digitalWrite(bcd_A0,HIGH);
        digitalWrite(bcd_A1,LOW);
        digitalWrite(bcd_A2,LOW);
        digitalWrite(bcd_A3,LOW);
      break;
      case 3:
        digitalWrite(bcd_E1,HIGH);
        digitalWrite(bcd_A0,LOW);
        digitalWrite(bcd_A1,HIGH);
        digitalWrite(bcd_A2,LOW);
        digitalWrite(bcd_A3,LOW); 
      break;
      case 4:
        digitalWrite(bcd_E1,HIGH);
        digitalWrite(bcd_A0,HIGH);
        digitalWrite(bcd_A1,HIGH);
        digitalWrite(bcd_A2,LOW);
        digitalWrite(bcd_A3,LOW);   
     break;
     case 5:
        digitalWrite(bcd_E1,HIGH);
        digitalWrite(bcd_A0,LOW);
        digitalWrite(bcd_A1,LOW);
        digitalWrite(bcd_A2,HIGH);
        digitalWrite(bcd_A3,LOW);
     break;
     case 6:
        digitalWrite(bcd_E1,HIGH);
        digitalWrite(bcd_A0,HIGH);
        digitalWrite(bcd_A1,LOW);
        digitalWrite(bcd_A2,HIGH);
        digitalWrite(bcd_A3,LOW);
     break;
     case 7:
        digitalWrite(bcd_E1,HIGH);
        digitalWrite(bcd_A0,LOW);
        digitalWrite(bcd_A1,HIGH);
        digitalWrite(bcd_A2,HIGH);
        digitalWrite(bcd_A3,LOW);
     break;
     case 8:
        digitalWrite(bcd_E1,HIGH);
        digitalWrite(bcd_A0,HIGH);
        digitalWrite(bcd_A1,HIGH);
        digitalWrite(bcd_A2,HIGH);
        digitalWrite(bcd_A3,LOW);
     break;
     case 9:
        digitalWrite(bcd_E1,HIGH);
        digitalWrite(bcd_A0,LOW);
        digitalWrite(bcd_A1,LOW);
        digitalWrite(bcd_A2,LOW);
        digitalWrite(bcd_A3,HIGH);
     break;
     case 10:
        digitalWrite(bcd_E1,HIGH);
        digitalWrite(bcd_A0,HIGH);
        digitalWrite(bcd_A1,LOW);
        digitalWrite(bcd_A2,LOW);
        digitalWrite(bcd_A3,HIGH);
     break;
     case 11:
        digitalWrite(bcd_E1,HIGH);
        digitalWrite(bcd_A0,LOW);
        digitalWrite(bcd_A1,HIGH);
        digitalWrite(bcd_A2,LOW);
        digitalWrite(bcd_A3,HIGH);
     break;
     case 12:
        digitalWrite(bcd_E1,HIGH);
        digitalWrite(bcd_A0,HIGH);
        digitalWrite(bcd_A1,HIGH);
        digitalWrite(bcd_A2,LOW);
        digitalWrite(bcd_A3,HIGH);
     break;
     case 13:
        digitalWrite(bcd_E1,HIGH);
        digitalWrite(bcd_A0,LOW);
        digitalWrite(bcd_A1,LOW);
        digitalWrite(bcd_A2,HIGH);
        digitalWrite(bcd_A3,HIGH);
     break;
     case 14:
        digitalWrite(bcd_E1,HIGH);
        digitalWrite(bcd_A0,HIGH);
        digitalWrite(bcd_A1,LOW);
        digitalWrite(bcd_A2,HIGH);
        digitalWrite(bcd_A3,HIGH);
     break;
     case 15:
        digitalWrite(bcd_E1,HIGH);
        digitalWrite(bcd_A0,LOW);
        digitalWrite(bcd_A1,HIGH);
        digitalWrite(bcd_A2,HIGH);
        digitalWrite(bcd_A3,HIGH);
     break;
     case 16:
        digitalWrite(bcd_E1,HIGH);
        digitalWrite(bcd_A0,HIGH);
        digitalWrite(bcd_A1,HIGH);
        digitalWrite(bcd_A2,HIGH);
        digitalWrite(bcd_A3,HIGH);
     break;
  }
  //a tutaj ustalamy czas gaszenia wyjsc
  time_to_gaszenie = millis() + 5000;
}

void gaszenie_wyjsc(){
  if(millis() >= time_to_gaszenie){
        digitalWrite(bcd_E1,LOW);
        digitalWrite(bcd_A0,LOW);
        digitalWrite(bcd_A1,LOW);
        digitalWrite(bcd_A2,LOW);
        digitalWrite(bcd_A3,LOW); 
  } 
}

//startup sequence
void setup(){
  Serial.begin(9600);
  
  pinMode(bcd_E1,OUTPUT);  
  pinMode(bcd_A0,OUTPUT);
  pinMode(bcd_A1,OUTPUT);
  pinMode(bcd_A2,OUTPUT);
  pinMode(bcd_A3,OUTPUT);
  digitalWrite(bcd_E1,LOW);
  digitalWrite(bcd_A0,LOW);
  digitalWrite(bcd_A1,LOW);
  digitalWrite(bcd_A2,LOW);
  digitalWrite(bcd_A3,LOW);
  
  if (!rf22.init()){
    Serial.println("RF22 init failed");
  }
  rf22.setFrequency(434.50);
  rf22.setTxPower(RF22_TXPOW_17DBM);  
  Serial.print("startup");
}

//infinity loop
void loop(){
  //tutaj wysylka danych przez radio

  rcv_data();
  gaszenie_wyjsc();
  //Serial.print(" infinity ");
  //ulatwia synchronizowanie odbiornika po zaniku sygnalu
  //delay(100); 
}
