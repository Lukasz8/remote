/*
 
 SP8XCE & SQ9MDD

*/

//tablice kolejności i zwłok czasowych
int tablica[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
int interwa[16] = {1000,1000,3000,1000,500,1000,100,100,1000,1000,3000,1000,500,1000,100,10};

int reczne_odpalenie = 3;

//podlaczenie BCD do arduino i kabelkologia
int bcd_E1 = 5;
int bcd_A0 = 6;
int bcd_A1 = 7;
int bcd_A2 = 8;
int bcd_A3 = 9;
int play_button = 3;
int reczny_button = 4;

//zmienne pomcnicze
int flaga  = 0;
int start_pgm = 0;

//**************************************************************************************************//
//funkcje
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
}


//funkcja odtwarzania pokazu
void play_show(){
  if(flaga == 0){
    Serial.println("Start Pokazu");
    for(int licznik = 0; licznik < 16; licznik++){
       Serial.print(tablica[licznik]);
       Serial.print(";");
       Serial.println(interwa[licznik]); 
       
       //wywolanie procedury odpalenia odpowiedniego wyjscia
       odpalanie_wyjscia(tablica[licznik]);
       delay(interwa[licznik]);
    } 
   flaga = 1; 
   Serial.println("Koniec Pokazu");
  } 
}

//**************************************************************************************************//
//funkcja odpalana przy starcie
void setup(){
  Serial.begin(9600); 
  pinMode(play_button,INPUT_PULLUP);
  pinMode(reczny_button,INPUT_PULLUP);
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
  
  delay(3000); 
  Serial.println("GOTOWY");
  delay(1000);
}

//**************************************************************************************************//
//petla główna 
void loop(){
  int stan_przycisku = digitalRead(play_button);
  if (stan_przycisku == LOW){
    play_show();  
  }
  int stan_reczny_button = digitalRead(reczny_button);
  if(stan_reczny_button == LOW){
    odpalanie_wyjscia(reczne_odpalenie);
  }
  
}

//**************************************************************************************************//

//EOF
