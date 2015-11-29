/*
SP8XCE & SQ9MDD
REMOTE odbior, potwierdzenie odbioru + obsluga dekodera BCD 5 pinow

CHANGELOG nowe na górze
 2015.11.29 - SQ9MDD v.1.4 dodanie funkcji do obsługi dipswitch adresy
 2015.11.29 - SQ9MDD dodałem gaszenie wyjść 5sec po ostatnim wysterowaniu
 2015.11.29 - uruchomienie sterowania diodami podpietymi do wyjscia dekodera BCD  
 2015.11.26 - dodanie konfiguracji adresu (programowo), dodanie reakcji na adres

*/
#define ver 1.4

#include <SPI.h>
#include <RF22.h>

//inicjalizacja
RF22 rf22;

//zmienne
int net_addr = 0;
int wejscie_pomiarowe_adresu = A0;
unsigned long time_to_gaszenie = 0;   //pomocnicza do wygaszania wejść
int bcd_E1 = 5;
int bcd_A0 = 6;
int bcd_A1 = 7;
int bcd_A2 = 8;
int bcd_A3 = 9;

//funkcje

//ustawienie adresu
int set_addres(){
  int addres = 0;
  int pomiar = analogRead(wejscie_pomiarowe_adresu);
  //Serial.println(pomiar); //odkomentuj do kalibracji
  if(pomiar <= 5) addres = 0;
  if(pomiar >= 10 && pomiar <= 30) addres = 1; 
  if(pomiar >= 30 && pomiar <= 46) addres = 2; 
  if(pomiar >= 47 && pomiar <= 66) addres = 3;
  if(pomiar >= 67 && pomiar <= 88) addres = 4; 
  if(pomiar >= 87 && pomiar <= 105) addres = 5; 
  if(pomiar >= 106 && pomiar <= 115) addres = 6; 
  if(pomiar >= 116 && pomiar <= 131) addres = 7; 
  if(pomiar >= 132 && pomiar <= 150) addres = 8; 
  if(pomiar >= 151 && pomiar <= 160) addres = 9; 
  if(pomiar >= 161 && pomiar <= 175) addres = 10; 
  if(pomiar >= 176 && pomiar <= 190) addres = 11; 
  if(pomiar >= 191 && pomiar <= 205) addres = 12; 
  if(pomiar >= 206 && pomiar <= 220) addres = 13; 
  if(pomiar >= 221 && pomiar <= 230) addres = 14; 
  if(pomiar >= 231 && pomiar <= 241) addres = 15; 
  return(addres+10); 
}

//odbieranie danych
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
                odpalanie_wyjscia(1);
              }
            break;
            case 12:
              if(polecenie == 1){
                Serial.println("odpalenie wyjscia 2");
                odpalanie_wyjscia(2);
              }
            break;
            case 13:
              if(polecenie == 1){
                Serial.println("odpalenie wyjscia 3");
                odpalanie_wyjscia(3);
              }
            break; 
            case 14:
              if(polecenie == 1){
                Serial.println("odpalenie wyjscia 4");
                odpalanie_wyjscia(4);
              }
            break; 
            case 15:
              if(polecenie == 1){
                Serial.println("odpalenie wyjscia 5");
                odpalanie_wyjscia(5);
              }
            break; 
            case 16:
              if(polecenie == 1){
                Serial.println("odpalenie wyjscia 6");
                odpalanie_wyjscia(6);
              }
            break; 
            case 17:
              if(polecenie == 1){
                Serial.println("odpalenie wyjscia 7");
                odpalanie_wyjscia(7);
              }
            break; 
            case 18:
              if(polecenie == 1){
                Serial.println("odpalenie wyjscia 8");
                odpalanie_wyjscia(8);
              }
            break; 
            case 19:
              if(polecenie == 1){
                Serial.println("odpalenie wyjscia 9");
                odpalanie_wyjscia(9);
              }
            break; 
            case 20:
              if(polecenie == 1){
                Serial.println("odpalenie wyjscia 10");
                odpalanie_wyjscia(10);
              }
            break; 
            case 21:
              if(polecenie == 1){
                Serial.println("odpalenie wyjscia 11");
                odpalanie_wyjscia(11);
              }
            break; 
            case 22:
              if(polecenie == 1){
                Serial.println("odpalenie wyjscia 12");
                odpalanie_wyjscia(12);
              }
            break; 
            case 23:
              if(polecenie == 1){
                Serial.println("odpalenie wyjscia 13");
                odpalanie_wyjscia(13);
              }
            break; 
            case 24:
              if(polecenie == 1){
                Serial.println("odpalenie wyjscia 14");
                odpalanie_wyjscia(14);
              }
            break; 
            case 25:
              if(polecenie == 1){
                Serial.println("odpalenie wyjscia 15");
                odpalanie_wyjscia(15);
              }
            break; 
            case 26:
              if(polecenie == 1){
                Serial.println("odpalenie wyjscia 16");
                odpalanie_wyjscia(16);
              }
            break;
          }
        
      //debugowanie printujemy wszystko co odbieramy
      Serial.print("mamy odpowiedz: ");
      Serial.println(RX);
      //odsyłamy potwierdzenie do nadawcy
      rf22.send((uint8_t*)&RX, sizeof(RX));
      rf22.waitPacketSent();      
      }
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
  net_addr = set_addres();
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
  //aby skalibrować drabinke z adresem odkomentuj i czytaj wartości
  net_addr = set_addres();
  
  //tutaj wysylka danych przez radio
  rcv_data();
  gaszenie_wyjsc();
  //Serial.print(" infinity ");
  //ulatwia synchronizowanie odbiornika po zaniku sygnalu
  //delay(100); 
}
