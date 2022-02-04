 #include <TM1637.h>

//Viel Spaß beim nachmachen und verlinkt gerne diesen Beitrag bei euren Bildern oder Videos  
//ESP32: https://www.amazon.de/AZDelivery-ESP32-NodeMCU-gratis-eBook/dp/B07Z83MF5W/ref=sr_1_5?keywords=esp32&qid=1643144310&sr=8-5&th=1
//Relais: https://www.amazon.de/gp/product/B07BVXT1ZK/ref=ppx_yo_dt_b_asin_image_o03_s00?ie=UTF8&psc=1
//4 Bit Digital Tube LED 7-Segment Display Modul: https://www.amazon.de/gp/product/B06X952QXS/ref=ppx_yo_dt_b_asin_image_o05_s00?ie=UTF8&th=1
//Elektronische Summer: https://www.amazon.de/gp/product/B07MJ6ZRLB/ref=ppx_yo_dt_b_asin_image_o05_s00?ie=UTF8&psc=1
//Kabel: https://www.amazon.de/gp/product/B07KYHBVR7/ref=ppx_yo_dt_b_asin_image_o04_s00?ie=UTF8&th=1
//Pin Vebindungen: G4-->G22     G16-->G23     G17-->G21     G18-->G19 

const int Cable1 = 22; //G22
const int Cable2 = 16; //G16
const int Cable3 = 17; //G17
const int Cable4 = 18; //G18 --> Das Kabel zum Defusen 

const int Cable5 = 19; //G19
const int Cable6 = 21; //G21
const int Cable7 = 23; //G23
const int Cable8 = 4; //4
const int Buzzer1 = 32; //G23 -->Kabel zum Relai
int timer = 5999; // Timer zum einstellen --> Berechnung unten
int first = true; //überprüfung ob einkabel schon gezogen wurde
int CLK = 25; //G25 -->Display Modul
int DIO = 26; //G26 -->Display Modul

TM1637 timedisplay(CLK,DIO); //Display Modul


void setup() {
  pinMode(Cable1, OUTPUT); 
  pinMode(Cable2, OUTPUT);
  pinMode(Cable3, OUTPUT);
  pinMode(Cable4, OUTPUT);
  pinMode(Cable5, OUTPUT);
  pinMode(Cable6, OUTPUT);
  pinMode(Cable7, OUTPUT);
  pinMode(Cable8, OUTPUT);
  digitalWrite(Cable1, LOW);
  digitalWrite(Cable2, LOW);
  digitalWrite(Cable3, HIGH);
  digitalWrite(Cable4, LOW);
  digitalWrite(Cable5, HIGH);
  digitalWrite(Cable6, LOW);
  digitalWrite(Cable7, HIGH);
  digitalWrite(Cable8, HIGH);
  pinMode(Buzzer1, OUTPUT);
  digitalWrite(Buzzer1, LOW);
  timedisplay.init();
  timedisplay.set(2);
  Serial.begin(9600); //Kann höher gestellt werden oder niedriger, umso schneller wird auf Kabel Durchtrennung reagiert

}

void loop() {
  Bomb();

}

void Bomb(){
  
  if(timer == 0){// wenn Timer gleich 0 dann explodieren
      digitalWrite(Buzzer1, HIGH);
      Serial.println("BOOM"); //Eigentlich unwichtig, kann aber zur Überprüfung genutzt werden
      displayTime(0);
      delay(20000); // 20sek dann Buzzer wieder aus
      digitalWrite(Buzzer1, LOW); 
      Serial.println("RESET");
      delay(3600000); // 1 Stunde dann reset
      timer=120;
  }
  else if(timer>0){
      for (timer; timer>=1 ; timer--){
          int state1 = digitalRead(Cable1);
          int state2 = digitalRead(Cable2);
          int state3 = digitalRead(Cable3);
          int state4 = digitalRead(Cable4);
      
          if ((state1== 1) && (state2== 1) && (state3== 0) && (state4==1)){ //Startet nur den Timer, wenn alle Kabel verbunden sind
                Serial.println(timer);
                displayTime(timer);
          }
          if (state4 != 1){ //Bombe entschärft
                defused();
          }
          if ((state1 !=1 || state2 != 1 || state3 !=0) && (first==true)){ //Erstes kabel falsch durchtrennt halbiert die zeit
                Serial.println("Zweite Chance");
                timer=timer/2;
                first = false;
          }
          if (first==false){
                Serial.println(timer);
                displayTime(timer);
                 
                if (state4 != 1){
                      defused();
                }
                else if ((state1!= 1) && (state2!= 1)){
                    timer=0;
                    break;
                }
                else if ((state2!= 1) && (state3!= 0)){
                    timer=0;
                    break;
                }
                else if ((state1!= 1) && (state3!= 0)){
                    timer=0;
                    break;
                }
          }
       delay(1000); //jede Sekunde
       }
   }
 }

void displayTime(int sec){
    int minutes = sec / 60;
    int seconds = sec % 60;
    timedisplay.point(1);
    timedisplay.display(3, seconds % 10);
    timedisplay.display(2, seconds / 10 % 10);
    timedisplay.display(1, minutes % 10);
    timedisplay.display(0, minutes / 10 % 10);
}

void defused(){
    int timerset=timer; 
    timer=-1;
    Serial.println("Entschärft");
    displayTime(timerset); //Stopt die Zeit
}
 
