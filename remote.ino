/*
SP8XCE & SQ9MDD
REMOTE odbior, potwierdzenie odbioru + obsluga dekodera BCD 5 pinow

CHANGELOG nowe na górze
 2015.12.05 - SP8XCE skrocenie czasu gasenia wyjsc do 0,5 sekundy
 2015.11.29 - SP8XCE skalibrowanie dipswitchy
 2015.11.29 - SQ9MDD v.1.4 dodanie funkcji do obsługi dipswitch adresy
 2015.11.29 - SQ9MDD dodałem gaszenie wyjść 5sec po ostatnim wysterowaniu
 2015.11.29 - SP8XCE uruchomienie sterowania diodami podpietymi do wyjscia dekodera BCD  
 2015.11.26 - dodanie konfiguracji adresu (programowo), dodanie reakcji na adres
*/
#define ver 1.4

#include <SPI.h>
#include <RF22.h>

//inicjalizacja
RF22 rf22;

//zmienne
int net_addr = 0;
//int czestotliwosc = 0;
int wejscie_pomiarowe_adresu = A0;    //pomiar na dzielniku napiecia
//int wejscie_pomiarowe_czestotliwosci = A1; //pomiar na dzielniku napiecia
unsigned long time_to_gaszenie = 0;   //pomocnicza do wygaszania wejść
int bcd_E1 = 5;
int bcd_A0 = 6;
int bcd_A1 = 7;
int bcd_A2 = 8;
int bcd_A3 = 9;
int led_act = 4;
int led_state = 0;

unsigned long time_to_change_led = 0;

//funkcje

//ustawienie adresu
int set_addres(){
  int addres = 0;
  int pomiar = analogRead(wejscie_pomiarowe_adresu);
  //Serial.println(pomiar); //odkomentuj do kalibracji
  if(pomiar <= 0) addres = 0;
  if(pomiar >= 30 && pomiar <= 90) addres = 1; 
  if(pomiar >= 90 && pomiar <= 120) addres = 2; 
  if(pomiar >= 120 && pomiar <= 160) addres = 3;
  if(pomiar >= 160 && pomiar <= 190) addres = 4; 
  if(pomiar >= 190 && pomiar <= 235) addres = 5; 
  if(pomiar >= 235 && pomiar <= 270) addres = 6; 
  if(pomiar >= 270 && pomiar <= 300) addres = 7; 
  if(pomiar >= 300 && pomiar <= 330) addres = 8; 
  if(pomiar >= 330 && pomiar <= 355) addres = 9; 
  if(pomiar >= 355 && pomiar <= 380) addres = 10; 
  if(pomiar >= 380 && pomiar <= 400) addres = 11; 
  if(pomiar >= 400 && pomiar <= 415) addres = 12; 
  if(pomiar >= 415 && pomiar <= 435) addres = 13; 
  if(pomiar >= 435 && pomiar <= 455) addres = 14; 
  if(pomiar >= 455 && pomiar <= 480) addres = 15; 
  return(addres+10); 
}
/*ustawienie czestotliowosci
int set_czestotliwosc(){
  int czestotliwosc = 0;
  int pomiar1 = analogRead(wejscie_pomiarowe_adresu_1);
  if(pomiar1 <= 0) czestotliwosc = 0;
  if(pomiar1 >= 30 && pomiar1 <= 90) czestotliwosc = 1; 
  if(pomiar1 >= 90 && pomiar1 <= 120) czestotliwosc = 2; 
  if(pomiar1 >= 120 && pomiar1 <= 160) czestotliwosc = 3;
  if(pomiar1 >= 160 && pomiar1 <= 190) czestotliwosc = 4; 
  if(pomiar1 >= 190 && pomiar1 <= 235) czestotliwosc = 5; 
  if(pomiar1 >= 235 && pomiar1 <= 270) czestotliwosc = 6; 
  if(pomiar1 >= 270 && pomiar1 <= 300) czestotliwosc = 7; 
  if(pomiar1 >= 300 && pomiar1 <= 330) czestotliwosc = 8; 
  if(pomiar1 >= 330 && pomiar1 <= 355) czestotliwosc = 9; 
  if(pomiar1 >= 355 && pomiar1 <= 380) czestotliwosc = 10; 
  if(pomiar1 >= 380 && pomiar1 <= 400) czestotliwosc = 11; 
  if(pomiar1 >= 400 && pomiar1 <= 415) czestotliwosc = 12; 
  if(pomiar1 >= 415 && pomiar1 <= 435) czestotliwosc = 13; 
  if(pomiar1 >= 435 && pomiar1 <= 455) czestotliwosc = 14; 
  if(pomiar1 >= 455 && pomiar1 <= 480) czestotliwosc = 15; 
  return(czestotliwosc+10); 
}
*/

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
      if(net_addr = 99){
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
      }
   }
}
}


void odpalanie_wyjscia(int wyjscie){
  //sterowanie BCD
  switch(wyjscie){
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
  time_to_gaszenie = millis() + 500;
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
  pinMode(led_act,OUTPUT);
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
  //ustawianie adresu
  net_addr = set_addres();
  
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
  //net_addr = set_addres();
  //delay(1000);
  //tylko do kalibracji
  
  rcv_data();
  gaszenie_wyjsc();

    if(millis() >= time_to_change_led){
      if(led_state == 0){
          led_state = 1;
          digitalWrite(led_act,HIGH);
      }else{
          led_state = 0;
          digitalWrite(led_act,LOW);        
    }
    time_to_change_led = millis() + 1000;
  }
}
