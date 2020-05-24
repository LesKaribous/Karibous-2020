

void createGUI()
{

  //----------------------------------------------------
  // Create the GUI ControlP5 instance
  cp5 = new ControlP5(this);
  // List of the serial port
  serialList = cp5.addScrollableList("dropdown")
     .setCaptionLabel("Liste des liaisons serie")
     .setPosition(40, 20)
     .setSize(170, 800)
     .setBarHeight(20)
     .setItemHeight(20)
     .addItems(Serial.list())
     .setOpen(false)
     ;
  // create a new button with name 'serialBegin'
  serialRefresh = cp5.addButton("serialRefresh")
     .setCaptionLabel("R")
     .setPosition(40,50)
     .setSize(20,20)
     ;
  // create a new button with name 'serialBegin'
  serialBegin = cp5.addButton("serialBegin")
     .setCaptionLabel("Connection")
     .setPosition(80,50)
     .setSize(70,20)
     ;
  // create a new button with name 'serialStop'
  serialStop = cp5.addButton("serialStop")
    .setCaptionLabel("Stop")
    .setPosition(170,50)
    .setSize(40,20)
    ;
  // Creation of the console
  consoleArea = cp5.addTextarea("Console")
    .setPosition(5, height-200)
    .setSize(width-10,195)
    .setFont(createFont("", 10))
    .setLineHeight(14)
    .setColor(color(255))
    .setColorBackground(color(0,44,91,240))
    ;
  // Creation of the toggle
  togglePumpLeft = cp5.addToggle("togglePumpLeft")
    .setCaptionLabel("Pompe Gauche")
    .setColorCaptionLabel(blue)
    .setColorForeground(blue)
    .setPosition(guiPaddingBrasX,guiPaddingBrasY)
    .setSize(50,20)
    .setValue(false)
    .setMode(ControlP5.SWITCH)
    ; 
  toggleValveLeft = cp5.addToggle("toggleValveLeft")
    .setCaptionLabel("Vanne Gauche")
    .setColorCaptionLabel(blackFont)
    .setPosition(guiPaddingBrasX,guiPaddingBrasY+50)
    .setSize(50,20)
    .setValue(false)
    .setMode(ControlP5.SWITCH)
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
    ;
  toggleValveRight = cp5.addToggle("toggleValveRight")
    .setCaptionLabel("Vanne Droite")
    .setColorCaptionLabel(blackFont)
    .setColorBackground(baseGreen)
    .setColorForeground(clearGreen)
    .setColorActive(selectGreen)
    .setPosition(guiPaddingBrasX+guiDecalageBrasX,guiPaddingBrasY+50)
    .setSize(50,20)
    .setValue(false)
    .setMode(ControlP5.SWITCH)
    ;
  knobServoLeft = cp5.addKnob("knobServoLeft")
    .setCaptionLabel("Servo Gauche")
    .setColorCaptionLabel(blackFont)
    .setPosition(guiPaddingBrasX,guiPaddingBrasY+90)
    .setSize(50,50)
    .setValue(0)
    .setNumberOfTickMarks(10) 
    .showTickMarks()
    .setViewStyle(ControlP5.ARC)
    .setRange(0,100)
    .setAngleRange(HALF_PI)
    .setStartAngle(-HALF_PI)
    .setDecimalPrecision(0);
    ;
  knobServoRight = cp5.addKnob("knobServoRight")
    .setCaptionLabel("Servo Droit")
    .setColorCaptionLabel(blackFont)
    .setColorBackground(baseGreen)
    .setColorForeground(clearGreen)
    .setColorActive(selectGreen)
    .setPosition(guiPaddingBrasX+guiDecalageBrasX,guiPaddingBrasY+90)
    .setSize(50,50)
    .setValue(0)
    .setNumberOfTickMarks(10) 
    .showTickMarks()
    .setViewStyle(ControlP5.ARC)
    .setRange(0,100)
    .setAngleRange(HALF_PI)
    .setStartAngle(-HALF_PI)
    .setDecimalPrecision(0);
    ;
  knobMiniServoLeft = cp5.addKnob("knobMiniServoLeft")
    .setCaptionLabel("Poignet Gauche")
    .setColorCaptionLabel(blackFont)
    .setPosition(guiPaddingBrasX+5,guiPaddingBrasY+160)
    .setSize(40,40)
    .setValue(0)
    .setNumberOfTickMarks(8) 
    .showTickMarks()
    .setViewStyle(ControlP5.ARC)
    .setRange(0,100)
    .setAngleRange(HALF_PI)
    .setStartAngle(-HALF_PI)
    .setDecimalPrecision(0);
    ;
  knobMiniServoRight = cp5.addKnob("knobMiniServoRight")
    .setCaptionLabel("Poignet Droit")
    .setColorCaptionLabel(blackFont)
    .setColorBackground(baseGreen)
    .setColorForeground(clearGreen)
    .setColorActive(selectGreen)
    .setPosition(guiPaddingBrasX+guiDecalageBrasX+5,guiPaddingBrasY+160)
    .setSize(40,40)
    .setValue(0)
    .setNumberOfTickMarks(8) 
    .showTickMarks()
    .setViewStyle(ControlP5.ARC)
    .setRange(0,100)
    .setAngleRange(HALF_PI)
    .setStartAngle(-HALF_PI)
    .setDecimalPrecision(0);
    ;
  //Add a slider
  heightSliderLeft = cp5.addSlider("hLeft")
    .setCaptionLabel("Hauteur \n Gauche")
    .setColorCaptionLabel(blackFont)
    .setColorValueLabel(blackFont)
    .setPosition(guiPaddingBrasX+5, guiPaddingBrasY+220)
    .setSize(40, 200)
    .setRange(0, 100)
    .setValue(0)
    .setSliderMode(Slider.FLEXIBLE)
    .setDecimalPrecision(0);
    ;
  heightSliderRight = cp5.addSlider("hRight")
    .setCaptionLabel("Hauteur \n Droite")
    .setColorCaptionLabel(blackFont)
    .setColorBackground(baseGreen)
    .setColorForeground(clearGreen)
    .setColorActive(selectGreen)
    .setColorValueLabel(blackFont)
    .setPosition(guiPaddingBrasX+guiDecalageBrasX+5, guiPaddingBrasY+220)
    .setSize(40, 200)
    .setRange(0, 100)
    .setValue(0)
    .setSliderMode(Slider.FLEXIBLE)
    .setDecimalPrecision(0);
    ;
  // create a new button with name 'serialSend'
  serialSend = cp5.addButton("serialSend")
     .setCaptionLabel("Envoyer")
     .setPosition(40,guiPaddingBrasY+220+240)
     .setSize(170,20)
     ;

   console = cp5.addConsole(consoleArea);

   println("Welcome to the Beacon interface");
   println("press'h' for more help");
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

  cp5.get(Textarea.class, "Console").setVisible(GUIVisible);
  //cp5.get(Button.class, "serialBegin").setVisible(GUIVisible);
  //cp5.get(Button.class, "serialSend").setVisible(GUIVisible);
  //cp5.get(Button.class, "serialStop").setVisible(GUIVisible);
  //cp5.get(Button.class, "serialRefresh").setVisible(GUIVisible);
  //cp5.get(ScrollableList.class, "dropdown").setVisible(GUIVisible);
  //cp5.get(Slider.class,"speed").setVisible(GUIVisible);
  //cp5.get(Toggle.class,"toggleCoordinate").setVisible(GUIVisible);
}
