

void createGUI()
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
  cp5.addTab("bras")
     .setLabel("Bras")
     .setColorBackground(color(0, 160, 100))
     .setColorLabel(color(255))
     .setColorActive(color(255,128,0))
     .setId(2)
     ;
  cp5.addTab("extra")
     .setLabel("Extra")
     .setColorBackground(color(0, 160, 100))
     .setColorLabel(color(255))
     .setColorActive(color(255,128,0))
     .setId(3)
     ;
  cp5.addTab("deplacement")
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
  serialSend = cp5.addButton("serialSend")
     .setCaptionLabel("Tout envoyer")
     .setPosition(40,guiPaddingBrasY+220+265)
     .setSize(80,20)
     .moveTo("bras")
     ;

  toggleDirectSend = cp5.addToggle("toggleDirectSend")
    .setCaptionLabel("")
    .setColorCaptionLabel(blue)
    .setColorForeground(blue)
    .setPosition(width-guiPaddingBrasX-50,guiPaddingBrasY+220+265)
    .setSize(20,20)
    .setValue(false)
    .setMode(ControlP5.DEFAULT)
    .moveTo("bras")
    ; 
  labelDirectSend = cp5.addTextlabel("labelDirectSend")
                    .setText("DIRECT")
                    .setPosition(width-guiPaddingBrasX-30,guiPaddingBrasY+220+265+5)
                    .setColorValue(blackFont)
                    .moveTo("bras")
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

public void serialSend()
{
 int servoLeftValue = int(knobServoLeft.getValue());
 int servoRightValue = int(knobServoRight.getValue());
 
 int miniServoRightValue = int(knobMiniServoRight.getValue());
 int miniServoLeftValue = int(knobMiniServoLeft.getValue());
 
 boolean pumpLeftValue = togglePumpLeft.getBooleanValue();
 boolean pumpRightValue = togglePumpRight.getBooleanValue();
 
 boolean valveLeftValue = toggleValveLeft.getBooleanValue();
 boolean valveRightValue = toggleValveRight.getBooleanValue();
 
 myPort.write(byte(servoLeftValue));
 myPort.write(byte(miniServoLeftValue));
 myPort.write(byte(pumpLeftValue));
 myPort.write(byte(valveLeftValue));
 
 myPort.write(byte(servoRightValue));
 myPort.write(byte(miniServoRightValue));
 myPort.write(byte(pumpRightValue));
 myPort.write(byte(valveRightValue));
 
 //myPort.write('\n');
 println("Parameters sent : speed set to " + servoLeftValue);
}

public void serialBegin()
{
 if (clicSerialBegin == false)
 {
   clicSerialBegin = true ;
   serialBegin.setCaptionLabel("re-connect");
 }
 myPort = new Serial(this, Serial.list()[int(serialList.getValue())], 9600);
 myPort.buffer(6);
 myPort.bufferUntil('\n');
 println("Serial connection has begin on port " + int(serialList.getValue()));
}

public void serialRefresh()
{
  clicSerialRefresh = true ;
  serialList.clear();
  serialList.addItems(Serial.list());
  println("Refreshing the serial list");
}

public void serialStop()
{
  clicSerialStop = true ;
  myPort.stop();
  newPosition = false;
  println("Serial connection has been stopped");
}

void keyPressed()
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
      int value = int(textKnobServoLeft.getText());
      knobServoLeft.setValue(value);
      print("Nouvelle valeur servo gauche : ");
      println(value);
    }
    if (textKnobServoRight.isFocus())
    {
      int value = int(textKnobServoRight.getText());
      knobServoRight.setValue(value);
      print("Nouvelle valeur servo droit : ");
      println(value);
    }
    if (textKnobPoignetLeft.isFocus())
    {
      int value = int(textKnobPoignetLeft.getText());
      knobMiniServoLeft.setValue(value);
      print("Nouvelle valeur poignet gauche : ");
      println(value);
    }
    if (textKnobPoignetRight.isFocus())
    {
      int value = int(textKnobPoignetRight.getText());
      knobMiniServoRight.setValue(value);
      print("Nouvelle valeur poignet droit : ");
      println(value);
    }
    if (textSliderLeft.isFocus())
    {
      int value = int(textSliderLeft.getText());
      heightSliderLeft.setValue(value);
      print("Nouvelle valeur ascenseur gauche : ");
      println(value);
    }
    if (textSliderRight.isFocus())
    {
      int value = int(textSliderRight.getText());
      heightSliderRight.setValue(value);
      print("Nouvelle valeur ascenseur droit : ");
      println(value);
    }
    
  }

  cp5.get(Textarea.class, "Console").setVisible(GUIVisible);
  //cp5.get(Button.class, "serialBegin").setVisible(GUIVisible);
  //cp5.get(Button.class, "serialSend").setVisible(GUIVisible);
  //cp5.get(Button.class, "serialStop").setVisible(GUIVisible);
  //cp5.get(Button.class, "serialRefresh").setVisible(GUIVisible);
  //cp5.get(ScrollableList.class, "dropdown").setVisible(GUIVisible);
  //cp5.get(Slider.class,"speed").setVisible(GUIVisible);
  //cp5.get(Toggle.class,"toggleCoordinate").setVisible(GUIVisible);
}
