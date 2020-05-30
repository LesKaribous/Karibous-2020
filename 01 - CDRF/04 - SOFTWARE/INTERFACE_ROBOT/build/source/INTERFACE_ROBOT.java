import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import controlP5.*; 
import processing.serial.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class INTERFACE_ROBOT extends PApplet {


                  // Import the CP5 library
          // Import the serial library

Serial myPort;                       // Create the serial port object
ControlP5 cp5;                       // Create the CP5 object

PImage img;

// Creation of the variable for the GUI
// Console
Textarea consoleArea;
Println console;
// Creation of the lists
ScrollableList serialList;
// Creation of the buttons
Button serialBegin;
Button buttonEnvoyer;
Button serialStop;
Button serialRefresh;
// State of the buttons
boolean clicSerialBegin = false ;
boolean clicSerialStop = false ;
boolean clicSerialSend = false ;
boolean clicSerialRefresh = false ;
// Creation of the sliders
Slider heightSliderLeft;
Slider heightSliderRight;
// Creation of the toggles
Toggle togglePumpLeft;
Toggle togglePumpRight;
Toggle toggleValveLeft;
Toggle toggleValveRight;

Toggle toggleDirectSend;

Button resetTurn;
Button resetGo;

Button flecheAvant;
Button flecheArriere;
Button flecheGauche;
Button flecheDroite;

Textlabel labelDirectSend;

Textfield textKnobServoLeft;
Textfield textKnobServoRight;
Textfield textKnobPoignetLeft;
Textfield textKnobPoignetRight;

Textfield textSliderLeft;
Textfield textSliderRight;

Textfield textTurnParameter;
Textfield textGoParameter;

// Creation of the knobs
Knob knobServoRight;
Knob knobServoLeft;
Knob knobMiniServoRight;
Knob knobMiniServoLeft;

Tab tabBras;
Tab tabExtra;
Tab tabDeplacement;

int blackFont = color(20,20,20);
int blue = color(0,0,100);
int baseGreen = color(0,80,0);
int clearGreen = color(0,200,0);
int selectGreen = color(0,250,0);
// State of the toggles
boolean toggleValue = false;
// GUI Visibility
boolean GUIVisible = true;
// State new message
boolean newPosition = false;

int guiDecalageBrasX = 120;
int guiPaddingBrasX= 40;
int guiPaddingBrasY= 40;


// Communication variables
byte[] receivedBuf = new byte[14];
int[] transformBuf = new int[4];
byte[] sendBuf = new byte[16];

int distance, angle;


public void setup()
{
  
  frameRate(24);
  noStroke();
  fill(0);

  createGUI();
}

public void draw()
{
  background(255);
  tint(255,255,255,80);
  image(img, 25, 250, 200, 200);
}

public void serialEvent(Serial p)
{
  p.readBytesUntil(10,receivedBuf);
  p.clear();
  if(receivedBuf[0]=='b')
  {
    print("Raw datas : ");
    for(int i=0;i<=5;i++)
    {
      print(receivedBuf[i] + " ");
    }
    println();

    for(int i =0;i<=3;i++)
       {
         transformBuf[i] = unsignedByte(receivedBuf[i+1]);
       }
    distance = transformBuf[0] << 8 | transformBuf[1];
    angle = transformBuf[2] << 8 | transformBuf[3];
    print(distance);
    print(" ; ");
    println(angle);
    newPosition = true;
  }
  else
  {
    println("Serial corruption - informations ignored");
    newPosition = false;
  }
}

// Convert a signed Byte ( 127 to - 128 ) to an Int between 0 and 255
public int unsignedByte(byte value)
{
  if(value < 0) return value + 256 ;
  else return value;
}


public void createGUI()
{
  img = loadImage("K.png");
  
  

  //----------------------------------------------------
  // Create the GUI ControlP5 instance
  cp5 = new ControlP5(this);
  
  //---------------------TAB---------------------
  cp5.getTab("default")
     .activateEvent(true)
     .setLabel("Liaison Serie")
     .setId(1)
     ;
  tabBras = cp5.addTab("bras")
     .setLabel("Bras")
     .setColorBackground(color(0, 160, 100))
     .setColorLabel(color(255))
     .setColorActive(color(255,128,0))
     .setId(2)
     ;
  tabExtra = cp5.addTab("extra")
     .setLabel("Extra")
     .setColorBackground(color(0, 160, 100))
     .setColorLabel(color(255))
     .setColorActive(color(255,128,0))
     .setId(3)
     ;
  tabDeplacement = cp5.addTab("deplacement")
     .setLabel("Deplacement")
     .setColorBackground(color(0, 160, 100))
     .setColorLabel(color(255))
     .setColorActive(color(255,128,0))
     .setId(4)
     ;
  //---------------------------------------------
  //--------------TAB GLOBAL --------------------------
  // Creation of the console
  consoleArea = cp5.addTextarea("Console")
    .setPosition(5, height-165)
    .setSize(width-10,160)
    .setFont(createFont("", 10))
    .setLineHeight(14)
    .setColor(color(255))
    .setColorBackground(color(0,44,91,240))
    .moveTo("global")
    ;
  //--------------TAB 1 (default)--------------------------
  // List of the serial port
  serialList = cp5.addScrollableList("dropdown")
     .setCaptionLabel("Liste des liaisons serie")
     .setPosition(40, 60)
     .setSize(170, 800)
     .setBarHeight(20)
     .setItemHeight(20)
     .addItems(Serial.list())
     .setOpen(false)
     .moveTo("default")
     ;
  // create a new button with name 'serialBegin'
  serialRefresh = cp5.addButton("serialRefresh")
     .setCaptionLabel("R")
     .setPosition(40,90)
     .setSize(20,20)
     .moveTo("default")
     ;
  // create a new button with name 'serialBegin'
  serialBegin = cp5.addButton("serialBegin")
     .setCaptionLabel("Connection")
     .setPosition(80,90)
     .setSize(70,20)
     .moveTo("default")
     ;
  // create a new button with name 'serialStop'
  serialStop = cp5.addButton("serialStop")
    .setCaptionLabel("Stop")
    .setPosition(170,90)
    .setSize(40,20)
    .moveTo("default")
    ;
  //--------------TAB 2 (bras)---------------------
  // Creation of the toggle
  togglePumpLeft = cp5.addToggle("togglePumpLeft")
    .setCaptionLabel("Pompe Gauche")
    .setColorCaptionLabel(blue)
    .setColorForeground(blue)
    .setPosition(guiPaddingBrasX,guiPaddingBrasY)
    .setSize(50,20)
    .setValue(false)
    .setMode(ControlP5.SWITCH)
    .moveTo("bras")
    ; 
  toggleValveLeft = cp5.addToggle("toggleValveLeft")
    .setCaptionLabel("Vanne Gauche")
    .setColorCaptionLabel(blackFont)
    .setPosition(guiPaddingBrasX,guiPaddingBrasY+40)
    .setSize(50,20)
    .setValue(false)
    .setMode(ControlP5.SWITCH)
    .moveTo("bras")
    ;  
  togglePumpRight = cp5.addToggle("togglePumpRight")
    .setCaptionLabel("Pompe Droite")
    .setColorCaptionLabel(blackFont)
    .setColorBackground(baseGreen)
    .setColorForeground(clearGreen)
    .setColorActive(selectGreen)
    .setPosition(guiPaddingBrasX+guiDecalageBrasX,guiPaddingBrasY)
    .setSize(50,20)
    .setValue(false)
    .setMode(ControlP5.SWITCH)
    .moveTo("bras")
    ;
  toggleValveRight = cp5.addToggle("toggleValveRight")
    .setCaptionLabel("Vanne Droite")
    .setColorCaptionLabel(blackFont)
    .setColorBackground(baseGreen)
    .setColorForeground(clearGreen)
    .setColorActive(selectGreen)
    .setPosition(guiPaddingBrasX+guiDecalageBrasX,guiPaddingBrasY+40)
    .setSize(50,20)
    .setValue(false)
    .setMode(ControlP5.SWITCH)
    .moveTo("bras")
    ;
  knobServoLeft = cp5.addKnob("knobServoLeft")
    .setCaptionLabel("Servo Gauche")
    .setColorCaptionLabel(blackFont)
    .setPosition(guiPaddingBrasX,guiPaddingBrasY+85)
    .setSize(50,50)
    .setValue(0)
    .setNumberOfTickMarks(10) 
    .showTickMarks()
    .setViewStyle(ControlP5.ARC)
    .setRange(0,100)
    .setAngleRange(HALF_PI)
    .setStartAngle(-HALF_PI)
    .setDecimalPrecision(0)
    .moveTo("bras")
    ;
  textKnobServoLeft = cp5.addTextfield("textKnobServoLeft")
    .setPosition(20,guiPaddingBrasY+120)
    .setSize(20,15)
    .setAutoClear(false)
    .moveTo("bras")
    .setCaptionLabel("")
    ;
  
  knobServoRight = cp5.addKnob("knobServoRight")
    .setCaptionLabel("Servo Droit")
    .setColorCaptionLabel(blackFont)
    .setColorBackground(baseGreen)
    .setColorForeground(clearGreen)
    .setColorActive(selectGreen)
    .setPosition(guiPaddingBrasX+guiDecalageBrasX,guiPaddingBrasY+85)
    .setSize(50,50)
    .setValue(0)
    .setNumberOfTickMarks(10) 
    .showTickMarks()
    .setViewStyle(ControlP5.ARC)
    .setRange(0,100)
    .setAngleRange(HALF_PI)
    .setStartAngle(-HALF_PI)
    .setDecimalPrecision(0)
    .moveTo("bras")
    ;
  textKnobServoRight = cp5.addTextfield("textKnobServoRight")
    .setColorBackground(baseGreen)
    .setColorForeground(clearGreen)
    .setPosition(20+guiDecalageBrasX,guiPaddingBrasY+120)
    .setSize(20,15)
    .setAutoClear(false)
    .moveTo("bras")
    .setCaptionLabel("")
    ;
    
  knobMiniServoLeft = cp5.addKnob("knobMiniServoLeft")
    .setCaptionLabel("Poignet Gauche")
    .setColorCaptionLabel(blackFont)
    .setPosition(guiPaddingBrasX,guiPaddingBrasY+160)
    .setSize(50,50)
    .setValue(0)
    .setNumberOfTickMarks(8) 
    .showTickMarks()
    .setViewStyle(ControlP5.ARC)
    .setRange(0,100)
    .setAngleRange(HALF_PI)
    .setStartAngle(-HALF_PI)
    .setDecimalPrecision(0)
    .moveTo("bras")
    ;
  textKnobPoignetLeft = cp5.addTextfield("textKnobPoignetLeft")
    .setPosition(20,guiPaddingBrasY+195)
    .setSize(20,15)
    .setAutoClear(false)
    .moveTo("bras")
    .setCaptionLabel("")
    ;
  knobMiniServoRight = cp5.addKnob("knobMiniServoRight")
    .setCaptionLabel("Poignet Droit")
    .setColorCaptionLabel(blackFont)
    .setColorBackground(baseGreen)
    .setColorForeground(clearGreen)
    .setColorActive(selectGreen)
    .setPosition(guiPaddingBrasX+guiDecalageBrasX,guiPaddingBrasY+160)
    .setSize(50,50)
    .setValue(0)
    .setNumberOfTickMarks(8) 
    .showTickMarks()
    .setViewStyle(ControlP5.ARC)
    .setRange(0,100)
    .setAngleRange(HALF_PI)
    .setStartAngle(-HALF_PI)
    .setDecimalPrecision(0)
    .moveTo("bras")
    ;
  textKnobPoignetRight = cp5.addTextfield("textKnobPoignetRight")
    .setColorBackground(baseGreen)
    .setColorForeground(clearGreen)
    .setPosition(20+guiDecalageBrasX,guiPaddingBrasY+195)
    .setSize(20,15)
    .setAutoClear(false)
    .moveTo("bras")
    .setCaptionLabel("")
    ;
  //Add a slider
  heightSliderLeft = cp5.addSlider("hLeft")
    .setCaptionLabel("Hauteur\nGauche")
    .setColorCaptionLabel(blackFont)
    .setColorValueLabel(blackFont)
    .setPosition(guiPaddingBrasX+5, guiPaddingBrasY+230)
    .setSize(40, 200)
    .setRange(0, 100)
    .setValue(0)
    .setSliderMode(Slider.FLEXIBLE)
    .setDecimalPrecision(0)
    .moveTo("bras")
    ;
  textSliderLeft = cp5.addTextfield("textSliderLeft")
    .setPosition(20,guiPaddingBrasY+230+185)
    .setSize(20,15)
    .setAutoClear(false)
    .moveTo("bras")
    .setCaptionLabel("")
    ;
  heightSliderRight = cp5.addSlider("hRight")
    .setCaptionLabel("Hauteur\nDroite")
    .setColorCaptionLabel(blackFont)
    .setColorBackground(baseGreen)
    .setColorForeground(clearGreen)
    .setColorActive(selectGreen)
    .setColorValueLabel(blackFont)
    .setPosition(guiPaddingBrasX+guiDecalageBrasX+5, guiPaddingBrasY+230)
    .setSize(40, 200)
    .setRange(0, 100)
    .setValue(0)
    .setSliderMode(Slider.FLEXIBLE)
    .setDecimalPrecision(0)
    .moveTo("bras")
    ;
  textSliderRight = cp5.addTextfield("textSliderRight")
    .setColorBackground(baseGreen)
    .setColorForeground(clearGreen)
    .setPosition(20+guiDecalageBrasX,guiPaddingBrasY+230+185)
    .setSize(20,15)
    .setAutoClear(false)
    .moveTo("bras")
    .setCaptionLabel("")
    ;
  // create a new button with name 'serialSend'
  buttonEnvoyer = cp5.addButton("buttonEnvoyer")
     .setCaptionLabel("Envoyer")
     .setPosition(40,guiPaddingBrasY+220+265)
     .setSize(80,20)
     .moveTo("global")
     ;

  toggleDirectSend = cp5.addToggle("toggleDirectSend")
    .setCaptionLabel("")
    .setColorCaptionLabel(blue)
    .setColorForeground(blue)
    .setPosition(width-guiPaddingBrasX-50,guiPaddingBrasY+220+265)
    .setSize(20,20)
    .setValue(false)
    .setMode(ControlP5.DEFAULT)
    .moveTo("global")
    ;
  labelDirectSend = cp5.addTextlabel("labelDirectSend")
                    .setText("DIRECT")
                    .setPosition(width-guiPaddingBrasX-30,guiPaddingBrasY+220+265+5)
                    .setColorValue(blackFont)
                    .moveTo("global")
                    ;
//----------------------------------------------------
  resetTurn = cp5.addButton("resetTurn")
    .setCaptionLabel("R")
    .setPosition(20,guiPaddingBrasY)
    .setSize(20,20)
    .moveTo("deplacement")
    ;
  textTurnParameter = cp5.addTextfield("textTurnParameter")
    .setColorCaptionLabel(blackFont)
    .setColorCaptionLabel(blue)
    .setColorForeground(blue)
    .setPosition(45,guiPaddingBrasY)
    .setSize(60,20)
    .setAutoClear(false)
    .moveTo("deplacement")
    .setCaptionLabel("Rotation")
    ;
  resetGo = cp5.addButton("resetGo")
    .setCaptionLabel("R")
    .setPosition(guiDecalageBrasX,guiPaddingBrasY)
    .setSize(20,20)
    .moveTo("deplacement")
    ;
  textGoParameter = cp5.addTextfield("textGoParameter")
    .setColorCaptionLabel(blackFont)
    .setColorCaptionLabel(blue)
    .setColorForeground(blue)
    .setPosition(25+guiDecalageBrasX,guiPaddingBrasY)
    .setSize(60,20)
    .setAutoClear(false)
    .moveTo("deplacement")
    .setCaptionLabel("Deplacement")
    ;

  flecheAvant = cp5.addButton("flecheAvant")
    .setCaptionLabel("^")
    .setPosition(width/2-10,guiPaddingBrasY+60)
    .setSize(20,20)
    .moveTo("deplacement")
    ;
  flecheArriere = cp5.addButton("flecheArriere")
    .setCaptionLabel("v")
    .setPosition(width/2-10,guiPaddingBrasY+60+25)
    .setSize(20,20)
    .moveTo("deplacement")
    ;
 flecheGauche = cp5.addButton("flecheGauche")
    .setCaptionLabel("<")
    .setPosition(width/2-10-25,guiPaddingBrasY+60+25)
    .setSize(20,20)
    .moveTo("deplacement")
    ;
  flecheDroite = cp5.addButton("flecheDroite")
    .setCaptionLabel(">")
    .setPosition(width/2-10+25,guiPaddingBrasY+60+25)
    .setSize(20,20)
    .moveTo("deplacement")
    ;

   console = cp5.addConsole(consoleArea);

   println("Bievenue dans l'interface de robotique");
   println("Appuyez sur 'h' pour faire apparaitre l'aide");
   printArray(Serial.list());

   //----------------------------------------------------
   // Bring the dropdown list for serial to the front
   serialList.bringToFront();
   //----------------------------------------------------
}

public void getParameterGauche()
{
  sendBuf[1] = PApplet.parseByte(PApplet.parseInt(knobServoLeft.getValue()));
  sendBuf[2] = PApplet.parseByte(PApplet.parseInt(knobMiniServoLeft.getValue()));
  sendBuf[3] = PApplet.parseByte(togglePumpLeft.getBooleanValue());
  sendBuf[4] = PApplet.parseByte(toggleValveLeft.getBooleanValue());
  sendBuf[5] = PApplet.parseByte(PApplet.parseInt(heightSliderLeft.getValue()));
}

public void getParameterDroit()
{
  sendBuf[6] = PApplet.parseByte(PApplet.parseInt(knobServoRight.getValue()));
  sendBuf[7] = PApplet.parseByte(PApplet.parseInt(knobMiniServoRight.getValue()));
  sendBuf[8] = PApplet.parseByte(togglePumpRight.getBooleanValue());
  sendBuf[9] = PApplet.parseByte(toggleValveRight.getBooleanValue());
  sendBuf[10] = PApplet.parseByte(PApplet.parseInt(heightSliderRight.getValue()));
}

public void getParameterDeplacement()
{
  sendBuf[11] = PApplet.parseByte(PApplet.parseInt(textTurnParameter.getText()) >> 8);
  sendBuf[12] = PApplet.parseByte(PApplet.parseInt(textTurnParameter.getText()) & 255);
  sendBuf[13] = PApplet.parseByte(PApplet.parseInt(textGoParameter.getText()) >> 8);
  sendBuf[14] = PApplet.parseByte(PApplet.parseInt(textGoParameter.getText()) & 255);
}

public void setParameterDeplacement(int turnOrder, int goOrder)
{
  sendBuf[11] = PApplet.parseByte(turnOrder >> 8);
  sendBuf[12] = PApplet.parseByte(turnOrder & 255);
  sendBuf[13] = PApplet.parseByte(goOrder >> 8);
  sendBuf[14] = PApplet.parseByte(goOrder & 255);
}

public void buttonEnvoyer()
{
  sendBuf[0] = 'b';
  
  getParameterGauche();
  getParameterDroit();
  getParameterDeplacement();
  
  sendBuf[15] = '\n';
  
  sendBuffer();
}

public void flecheAvant()
{
  int facteur = PApplet.parseInt(textGoParameter.getText());
  if (facteur<0) facteur = - facteur;
  
  sendBuf[0] = 'b';
  
  getParameterGauche();
  getParameterDroit();
  setParameterDeplacement(0, facteur);
  
  sendBuf[15] = '\n';
  
  sendBuffer();
}

public void flecheArriere()
{
  int facteur = PApplet.parseInt(textGoParameter.getText());
  if (facteur<0) facteur = - facteur;
  
  sendBuf[0] = 'b';
  
  getParameterGauche();
  getParameterDroit();
  setParameterDeplacement(0, -facteur);
  
  sendBuf[15] = '\n';
  
  sendBuffer();
}

public void flecheGauche()
{
  int facteur = PApplet.parseInt(textTurnParameter.getText());
  if (facteur<0) facteur = - facteur;
  
  sendBuf[0] = 'b';
  
  getParameterGauche();
  getParameterDroit();
  setParameterDeplacement(facteur, 0);
  
  sendBuf[15] = '\n';
  
  sendBuffer();
}

public void flecheDroite()
{
  int facteur = PApplet.parseInt(textTurnParameter.getText());
  if (facteur<0) facteur = - facteur;
  
  sendBuf[0] = 'b';
  
  getParameterGauche();
  getParameterDroit();
  setParameterDeplacement(-facteur, 0);
  
  sendBuf[15] = '\n';
  
  sendBuffer();
}

public void serialBegin()
{
 if (clicSerialBegin == false)
 {
   clicSerialBegin = true ;
   serialBegin.setCaptionLabel("re-connect");
 }
 myPort = new Serial(this, Serial.list()[PApplet.parseInt(serialList.getValue())], 115200);
 myPort.buffer(14);
 myPort.bufferUntil('\n');
 println("Serial connection has begin on port " + PApplet.parseInt(serialList.getValue()));
}

public void serialRefresh()
{
  clicSerialRefresh = true ;
  serialList.clear();
  serialList.addItems(Serial.list());
  println("Refreshing the serial list");
}

public void resetTurn()
{
  textTurnParameter.setText("0");
}

public void resetGo()
{
  textGoParameter.setText("0");
}

public void serialStop()
{
  clicSerialStop = true ;
  myPort.stop();
  newPosition = false;
  println("Serial connection has been stopped");
}

public void sendBuffer()
{
  for(int i=0;i<=15;i++)
  {
    myPort.write(sendBuf[i]);
  }

  print("-> (");
  for(int i=1;i<=14;i++)
  {
    print(sendBuf[i]);
    print(" ; ");
  }
  println(")");
}

public void keyPressed()
{
  if ( key == 'v' ) GUIVisible = !GUIVisible;
  if ( key == 'h' )
  {
    println("Help for the beacon interface");
    println("'h' - Help");
    println("'v' - Visibility of the GUI");
  }
  if ( key == '\n' )
  {
    // Si la touche 'entrée' est préssée 
    // On test le focus, et on met à jour la valeur du controlleur correspondant
    if (textKnobServoLeft.isFocus())
    {
      int value = PApplet.parseInt(textKnobServoLeft.getText());
      knobServoLeft.setValue(value);
      print("Nouvelle valeur servo gauche : ");
      println(value);
    }
    if (textKnobServoRight.isFocus())
    {
      int value = PApplet.parseInt(textKnobServoRight.getText());
      knobServoRight.setValue(value);
      print("Nouvelle valeur servo droit : ");
      println(value);
    }
    if (textKnobPoignetLeft.isFocus())
    {
      int value = PApplet.parseInt(textKnobPoignetLeft.getText());
      knobMiniServoLeft.setValue(value);
      print("Nouvelle valeur poignet gauche : ");
      println(value);
    }
    if (textKnobPoignetRight.isFocus())
    {
      int value = PApplet.parseInt(textKnobPoignetRight.getText());
      knobMiniServoRight.setValue(value);
      print("Nouvelle valeur poignet droit : ");
      println(value);
    }
    if (textSliderLeft.isFocus())
    {
      int value = PApplet.parseInt(textSliderLeft.getText());
      heightSliderLeft.setValue(value);
      print("Nouvelle valeur ascenseur gauche : ");
      println(value);
    }
    if (textSliderRight.isFocus())
    {
      int value = PApplet.parseInt(textSliderRight.getText());
      heightSliderRight.setValue(value);
      print("Nouvelle valeur ascenseur droit : ");
      println(value);
    }
  }
  if (key == 'z') flecheAvant();
  else if (key == 's') flecheArriere();
  else if (key == 'q') flecheGauche();
  else if (key == 'd') flecheDroite();

  cp5.get(Textarea.class, "Console").setVisible(GUIVisible);
  //cp5.get(Button.class, "serialBegin").setVisible(GUIVisible);
  //cp5.get(Button.class, "serialSend").setVisible(GUIVisible);
  //cp5.get(Button.class, "serialStop").setVisible(GUIVisible);
  //cp5.get(Button.class, "serialRefresh").setVisible(GUIVisible);
  //cp5.get(ScrollableList.class, "dropdown").setVisible(GUIVisible);
  //cp5.get(Slider.class,"speed").setVisible(GUIVisible);
  //cp5.get(Toggle.class,"toggleCoordinate").setVisible(GUIVisible);
}
  public void settings() {  size(250,750); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "INTERFACE_ROBOT" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
