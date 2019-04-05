#include <Ultrasonic.h>
#include <SoftwareSerial.h>
#include <Servo.h>

Ultrasonic ultrasonic(28, 29); // (Trig PIN,Echo PIN)
SoftwareSerial BT(11, 12); // RX, TX
Servo Servo1;

int secuencia = 0;
int Pinanalogo = A3;    // coloco en una variable el pin analogo
int valor = 0;  // variable que almacenará la lectura analóga

int pinServo = 25;
int pulsomin = 501;
int pulsomax = 2301;
int vfin = 0;

int ENI = 5;
int IN1 = 6;
int IN2 = 7;
int IN3 = 9;
int IN4 = 8;
int END = 10;

int haypeso = 0;

String val2;
String val3;
int esp;

int VCCL = 30;

int GND1 = 50;
int GND2 = 52;
int VCC1 = 51;
int VCC2 = 53;


int GNDL = 31;

int ACNY = A0;
int BCNY = A1;

String inicio = "";
String esperar = "";
String automatico = "";
String datain;
String vector[2];

int SETDISTANCIA = 15;
int SETTESPERA = 3000;

int CNEGROMIN = 0;
int CNEGROMAX = 10;
int CROJOMIN = 0;
int CROJOMAX = 10;
int CAMARMIN = 0;
int CAMARMAX = 10;
int VROJO = 0;
int VAMAR = 0;

void setup() {
  pinMode(pinServo, OUTPUT);
  BT.begin(9600);
  Serial.begin(9600);
  Servo1.attach(pinServo, pulsomin, pulsomax);
  pinMode(Pinanalogo,INPUT);
  
  
  pinMode(ENI, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(END, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(VCCL, OUTPUT); // VCC pin
  pinMode(GNDL, OUTPUT); // GND ping
  
  pinMode(GND1, OUTPUT); 
  pinMode(GND2, OUTPUT); 
  pinMode(VCC1, OUTPUT);
  pinMode(VCC2, OUTPUT);  
  digitalWrite(GND1, LOW);
  digitalWrite(GND2, LOW);
  digitalWrite(VCC1, HIGH);
  digitalWrite(VCC2, HIGH);
  
  pinMode(ACNY, INPUT);
  pinMode(BCNY, INPUT);
  digitalWrite(VCCL, HIGH); // VCC +5V mode
  digitalWrite(GNDL, LOW);  // GND mode
  
}

void loop() {
  //BT.println("#antes#");
  Servo1.write(0);
  int sensorValueA = analogRead(ACNY); //Lectura de entrada analogica 0
  int sensorValueB = analogRead(BCNY); //Lectura de entrada analogica 1

  //MBrandon();

  if (BT.available() > 0){ 
    datain = BT.readStringUntil('\n');
    if (datain.charAt(0) == '#' && datain.charAt(datain.length() - 1) == '#') {      
      if (datain.indexOf("ESP") >= 0) {

        val2 = getValue(datain, '|', 1);
        val3 = getValue(datain, '|', 2);

        SETDISTANCIA = val2.toInt();
        esp = val3.toInt();
        SETTESPERA = esp * 1000;
        esperar="esperar";              
      }

      if (datain.indexOf("INICIO") >= 0) {
        inicio="inicio";
      }
      if (datain.indexOf("FIN") >= 0) {
        inicio="fin";
      }

      if (datain.indexOf("AUT") >= 0) {
        val2 = getValue(datain, '|', 1);
        val3 = getValue(datain, '|', 2);

        SETDISTANCIA = val2.toInt();
        esp = val3.toInt();
        SETTESPERA = esp * 1000;

        automatico="automatico";
      }

      Serial.println(datain);
      BT.println(datain);      
    }
    //delay(1000);
  }
  
  if(inicio.equals("inicio"))
  {
    if(haypeso == 0){
      BT.println("#inicio#");
      vfin = 0;
      //Mninio();
      haypeso++;
    }
  
    if((sensorValueA < CNEGROMAX) && (sensorValueB < CNEGROMAX))
    {
      if(ultrasonic.Ranging(CM)>SETDISTANCIA)
      {
        Adelante();
      }   
      else
      {
        if(esperar.equals("esperar"))
        {
          BT.println("#esperar#");
          Detener();
          delay(SETTESPERA);
          //esperar = "";
        }
        if(automatico.equals("automatico"))
        {
          BT.println("#automatico#");
          Evadir();          
          //automatico = "";
        }     
      }
    }
    else if(sensorValueA < CNEGROMAX)
    {
      if(ultrasonic.Ranging(CM)>SETDISTANCIA)
      {
        Derecha();
      }
      else
      {
        Detener();
      }
    }
    else if(sensorValueB < CNEGROMAX)
    {
      if(ultrasonic.Ranging(CM)>SETDISTANCIA)
      {
        Izquierda();
      }
      else
      {
        Detener();
      }
    }
    else
    {
      Detener();
      //BT.println("#fin#");
      if(vfin == 0){vfin++;}
      
    }
  }
  else if(inicio.equals("fin"))
  {
    Serial.println("termina-----------");
    
    Detener();   
    //AbrirPuerta();
    //if(haypeso == 0){Mninio();haypeso++;}
    //CerrarPuerta();
    haypeso = 0;
    GiroRet();
    inicio = "";
    BT.println("#fin#");
    delay(1000);
  }
    
  //Serial.print(sensorValueB);
  //Serial.print(" ");
  //Serial.println(sensorValueA);
  //Serial.println("Prueva");
  //Serial.println(ultrasonic.Ranging(CM));
  //Serial.println(" cm" );
  //Adelante();
  //Derecha();  
  
}

void GiroRet()
{
  Derecha2();
  delay(1000);
  Detener();
}
void Detener()
{
  digitalWrite(ENI, LOW);
  digitalWrite(END, LOW);
}
void Adelante()
{
  analogWrite(ENI, 175);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(END, 150);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void Atras()
{
  analogWrite(ENI, 175);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(END, 150);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void Izquierda()
{
  analogWrite(ENI, LOW);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(END, 150);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void Derecha()
{
  analogWrite(ENI, 175);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(END, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void Izquierda2()
{
  digitalWrite(ENI, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(END, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void Derecha2()
{
  digitalWrite(ENI, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(END, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void Evadir()
{
  Derecha2();
  delay(500);
  Adelante();
  delay(1000);
  Izquierda2();
  delay(500);
  Adelante();
  delay(2000);
  Izquierda2();
  delay(500);
  Adelante();
  delay(700);
  Derecha2();
  delay(500);
  Detener();
  //delay(5000);  
}

void Mninio()
{
  for (secuencia=0;secuencia<3;secuencia++)
    {
      valor = analogRead(Pinanalogo);
      //Serial.println(valor);
      if (valor == 0){
        Serial.println("0 kg");
      }
      else{
        if(valor>=100 && valor<199){
          Serial.println("1.941 kg");  
        }
        if(valor>=200 && valor<299){
          Serial.println("2.2 kg");  
        }
        if(valor>=300 && valor<399){
          Serial.println("2.58 kg");  
        }
        if(valor>=400 && valor<499){
          Serial.println("3.21 kg");  
        }
        if(valor>=500 && valor<599){
          Serial.println("3.46 kg");  
        }
        if(valor>=600 && valor<699){
          Serial.println("3.6 kg");  
        }
        if(valor>=700 && valor<799){
          Serial.println("3.675 kg");  
        }
      }
      delay(500);
      Serial.println("Haypseo");
    }     
  secuencia=0;
  //delay(1000);
}

void AbrirPuerta() {
  Servo1.write(0);
  delay(1000);
  Servo1.write(0);
  delay(1000);
  Servo1.write(90);
  delay(1000);
  Servo1.write(180);
  delay(10000);

}



void CerrarPuerta() {
  Servo1.write(0);
  delay(1000);


}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  } return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
