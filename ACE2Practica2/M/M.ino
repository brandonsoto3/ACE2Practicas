#include <Ultrasonic.h>
#include <SoftwareSerial.h>
#include <Servo.h>
Ultrasonic ultrasonic(28, 29); // (Trig PIN,Echo PIN)
SoftwareSerial BT(11, 12); // RX, TX
Servo Servo1;
int pinServo = 13;
int pulsomin = 501;
int pulsomax = 2301;
int ENI = 5;
int IN1 = 6;
int IN2 = 7;
int IN3 = 9;
int IN4 = 8;
int END = 10;
String val2;
String val3;
int esp;

String espaut="";

int VCCL = 30;
int GNDL = 31;
int ACNY = A0;
String esperar = "";
String automatico = "";

int SETDISTANCIA = 15;
int SETTESPERA = 3000;
int CNEGROMIN = 0;
int CNEGROMAX = 10;
int CROJOMIN = 0;
int CROJOMAX = 10;
int CAZULMIN = 0;
int CAZULMAX = 10;
int VROJO = 0;
int VAZUL = 0;
String inicio = "";
String datain;
String vector[2];
String evaesp;
String inicio_color;
String inicio_color2 = "0";

String amarillo = "";
String azul = "";

String carga;
String carga2;
String retorno;
String retorno1;


void setup() {

  BT.begin(9600);
  Serial.begin(9600);
  Servo1.attach(pinServo, pulsomin, pulsomax);


  pinMode(ENI, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(END, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(VCCL, OUTPUT); // VCC pin
  pinMode(GNDL, OUTPUT); // GND ping
  pinMode(ACNY, INPUT);
  digitalWrite(VCCL, HIGH); // VCC +5V mode
  digitalWrite(GNDL, LOW);  // GND mode
  /*
    pinMode(OUTSERVO, OUTPUT);
    pinMode(INPESO4, INPUT);
    pinMode(INPESO5, INPUT);
    pinMode(INPESO6, INPUT);

    pinMode(INAPESO8, INPUT);
    pinMode(INAPESO9, INPUT);
    pinMode(INAPESO10, INPUT);
  */


}

void loop() {
  Servo1.write(0);
  int sensorValue = analogRead(ACNY); //Lectura de entrada analogica 0


  /*
    if((sensorValue >= CROJOMIN && sensorValue <= CROJOMAX) && (VROJO == 0))
    {
    Adelante();
    //envia
    VROJO ++;
    }

    if((sensorValue >= CAZULMIN && sensorValue <= CAZULMAX) && (VAZUL == 0))
    {
    Detener();
    //envia
    //SUELTA
    VROJO ++;
    }

  */


  if (BT.available() > 0)
  { datain = BT.readStringUntil('\n');
    if (datain.charAt(0) == '#' && datain.charAt(datain.length() - 1) == '#') {
      if (datain.indexOf("INICIO") >= 0) {

        inicio = "INICIO";
        azul = "";
        amarillo = "";

      } else if (datain.indexOf("FIN") >= 0) {

        inicio = "FIN";
        retorno = "FIN";

      }
      else if (datain.indexOf("RET") >= 0) {

        retorno = "RET";
        Izquierda();

      }
      else if (datain.indexOf("LIB") >= 0) {

        carga2 = "1";

      }

      if (datain.indexOf("ESP") >= 0) {



        val2 = getValue(datain, '|', 1);
        val3 = getValue(datain, '|', 2);

        SETDISTANCIA = val2.toInt();
        esp = val3.toInt();
        SETTESPERA = esp * 1000;
        espaut="ESP";
        

        
      }

      if (datain.indexOf("AUT") >= 0) {

        val2 = getValue(datain, '|', 1);
        val3 = getValue(datain, '|', 2);

        SETDISTANCIA = val2.toInt();
        esp = val3.toInt();
        SETTESPERA = esp * 1000;

        espaut="AUT";


      }




      Serial.println(datain);
    }
    delay(1000);
  }



  //METODOS LIBERAR CARGA

  if (carga.equals("1") && carga2.equals("1")) {

    AbrirPuerta();
    carga = "";
    carga2 = "";


  } else {

    CerrarPuerta();
  }




  //METODOS SENSORES

  if ((sensorValue >= CNEGROMIN && sensorValue <= CNEGROMAX))
  {
    Adelante();
    carga = "";
  }

  else if ((sensorValue >= 30 && sensorValue <= 70))
  {
    azul = "azul";
    amarillo = "";
    carga = "";


  }

  else if ((sensorValue >= 80 && sensorValue <= 110))
  {
    azul = "";
    amarillo = "amarillo";
    inicio = "";
    carga = "1";

  }
  else
  {
    Detener();
  }


  //METODOS MOVIMIENTO


  if ((inicio.equals("INICIO") && azul.equals("azul")) || (retorno.equals("RET") && amarillo.equals("amarillo"))) {

        

    if ((ultrasonic.Ranging(CM) >= SETDISTANCIA))
    {
      Adelante();
        
    }
    else
    {
        
      if (espaut.equals("ESP")) {
        Detener();
        delay(SETTESPERA);
        
      } 
      if (espaut.equals("AUT")) {

        Detener();
        delay(3000);
        Derecha();
        delay(3000);
        Adelante();
        delay(3000);
        Izquierda();
        delay(3000);
        Adelante();
        delay(3000);
        Izquierda();
        delay(3000);
        Adelante();
        delay(3000);
        Derecha();
        delay(3000);
        Adelante();
        
      }



      // delay(SETTESPERA);






    }



    Serial.println(ultrasonic.Ranging(CM));
    Serial.println(" cm" );
    
    
    delay(100);

  } else if ((retorno.equals("RET") && amarillo.equals(""))) {

    Detener();

  }
  else if ((inicio.equals("") && azul.equals(""))) {

    Detener();

  }
  else {

    Detener();

  }


  Serial.println(sensorValue);

String v="#"+val2;
String v1=v+"+CADENA";
String v2=v1+val3;
 
        
        

  BT.println(v2);
  

}

void Detener()
{
  digitalWrite(ENI, LOW);
  digitalWrite(END, LOW);
}
void Adelante()
{
  digitalWrite(ENI, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(END, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void Atras()
{
  digitalWrite(ENI, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(END, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void Izquierda()
{
  digitalWrite(ENI, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(END, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void Derecha()
{
  digitalWrite(ENI, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(END, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
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
