import controlP5.*;
import processing.serial.*;
ControlP5 cp5, cp;
CheckBox Box;
Serial myPort;

PImage img1;
PImage img2;
PImage img3;
PImage img;
String Monitoreo="";
String bufferin;
Textfield Txt;
Textfield Txt2;
Button Bt; 
Boolean valor;


void setup() {
  background(255);

  img = loadImage("image.png");
  img.resize(250, 200);
  image(img, 750, -30);  
  size(1000, 800);


  String[] CkItems = {"INICIO", "MONITOREO", "LIBERAR CARGA", "RETORNO", "EVACION ESPERA", "EVASION AUTOMATICA", "FIN", "CERRAR MONITOREO"};
  float[] CkValue = {11, 12, 13, 14, 15, 16, 1, 2};

  cp5 = new ControlP5(this);

  Box = cp5.addCheckBox("CheckBox")
    .setPosition(20, 20)
    .setSize(125, 125)  
    .setLabel("CheckBox")
    .setFont(createFont("arial", 20)); //texto de la etiqueta
  for (int i=0; i<CkItems.length; i++) {
    Box.addItem(CkItems[i], CkValue[i]);
    Box.getCaptionLabel().setFont(createFont("Arial", 500));
  }    

  //Box.activate(2);  
  customize(Box);

  Txt = cp5.addTextfield("inputText") 
    .setPosition(840, 750) 
    .setSize(150, 40)
    .setFont(createFont("arial", 20))
    .setColorBackground(color(50, 136, 136)); //por defecto: 0xff00365;

  Txt2 = cp5.addTextfield("inputText2") 
    .setPosition(680, 750) 
    .setSize(150, 40)
    .setFont(createFont("arial", 20))
    .setColorBackground(color(50, 136, 136)); //por defecto: 0xff00365;

  textSize(20);
  fill(0, 102, 153);
  text("Tiempo(s):", 840, 740);
  fill(0, 102, 153);
  text("Distancia(cm):", 680, 740);



  Bt=cp5.addButton("Conectar")
    .setValue(0)
    .setPosition(20, 750)
    .setSize(150, 40)
    .setFont(createFont("arial", 20));
}

void draw() {

  if (Monitoreo.equals("Monitoreo")) {

    if (myPort.available()>0)
    {
      bufferin=null;
      bufferin=myPort.readStringUntil('\n');
      if (bufferin!=null) {

        //if (bufferin.substring(0, 1).equals("1")) {
          //println(bufferin);
        //}

if (bufferin.charAt(0) == '#' && bufferin.charAt(bufferin.length() - 1) == '#') {

      println(bufferin);
    }


        myPort.clear();
      }
    }
  } else if (Monitoreo.equals("")) {
  }
}

void controlEvent(ControlEvent theEvent) {
  String concat="";
  if (theEvent.isGroup() && theEvent.name().equals("CheckBox")) {

    for (int i=0; i<theEvent.getArrayValue().length; i++) {
      int n = (int)theEvent.getArrayValue()[i];
      concat+=str(n);
    }

    if (concat.equals("00100000")) {
      println("Liberar Carga");
      myPort.write("#LIB#\n");
    } else if (concat.equals("10000000")) {
      println("Inicio");
      myPort.write("#INICIO#\n");
    } else if (concat.equals("01000000")) {
      println("Monitoreo");
      Monitoreo="Monitoreo";
    } else if (concat.equals("00000001")) {
      println("Monitoreo cerrado");
      Monitoreo="";
    } else if (concat.equals("00001000")) {


      if (Txt.getText().equals("")&&Txt2.getText().equals("")) {
        MsgBox("Ingrese Distancia y tiempo", "Mensaje de Advertencia");
      } else {
        println("Evasion por espera");
        println(Txt.getText());
        println(Txt2.getText());
        myPort.write("#ESP|"+Txt2.getText()+"|"+Txt.getText()+"|eso#\n");
      }
    } else if (concat.equals("00000100")) {


      if (Txt.getText().equals("")&&Txt2.getText().equals("")) {
        MsgBox("Ingrese Distancia y tiempo", "Mensaje de Advertencia");
      } else {
        println("Evasion AUTOMATICA");
        println(Txt.getText());
        println(Txt2.getText());
        myPort.write("#AUT|"+Txt2.getText()+"|"+Txt.getText()+"|eso#\n");
      }
    } else if (concat.equals("00010000")) {
      println("Retornar");
      myPort.write("#RET#\n");
    } else if (concat.equals("00000010")) {
      myPort.write("#FIN#\n");
      println("FIN");
    }
  } 
  println(concat);
}

void Conectar(int index) {

  valor=cp5.get(Button.class, "Conectar").getBooleanValue();

  if (valor==false) {
    //myPort.clear();
  } else if (valor==true) {
    println("CONECTANDO...");
    MsgBox("CONECTANDO...", "Mensaje de Confirmacion!");
    myPort = new Serial(this, "COM5", 9600);
    valor=false;
  }
}


void customize(CheckBox Box) {



  Box.setItemsPerRow(2); //Cantidad de elementos por linea default : 1
  Box.setSpacingColumn(100); //Default:1
  Box.setSpacingRow(20);  //Default : 1
  Box.setBackgroundColor(color(255)); //por defecto: 0x00ffffff //cambia el color del espacio que queda entre los elementos.
  Box.setColorBackground(color(50, 136, 136)); //por defecto: 0xff003652
  Box.setColorForeground(color(8, 200, 209)); // por defecto: 0xff00698c
  Box.setColorActive(color(0, 173, 255)); //por defecto: 0xff08a2cf

  Box.setColorLabel(color(0, 0, 0)); // por defecto: 0xffffffff modifica el color de la etiqueta.
}

void MsgBox( String Msg, String Titel ) {

  javax.swing.JOptionPane.showMessageDialog ( null, Msg, Titel, javax.swing.JOptionPane.INFORMATION_MESSAGE  );
}
