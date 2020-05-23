void mouseDisplay()
{
  if (toggleValue)showCoordinates(mouseX, mouseY);
}

void showCoordinates(int pX, int pY)
{
  //create two vector to calculate the angle between them
  PVector v1 = new PVector(1, 0);    // Vector for the origin
  PVector v2 = new PVector(pX-(displayWidth/2), pY-(displayHeight/2));
  fill(0, 102, 153);
  textSize(11);
  text(int(dist(displayWidth/2, displayHeight/2, pX, pY))+" cm", pX+7, pY-5);       // the distance
  text(int(degrees(PVector.angleBetween(v1, v2)))+" °", pX+7, pY-16);       // the angle
  circle(pX, pY, 5);
}

void robotDisplay()
{
  // Print the help text at the top right of the interface
  fill(157, 157, 157);
  textSize(11);
  text("Press 'V' to show/hide the GUI",displayWidth-180, 20);
  //----------------------------------------------------
   // Create the robot at the center of the interface
   fill(222, 158, 29);
   circle(displayWidth/2, displayHeight/2, 30);
}

void createGUI()
{
  //----------------------------------------------------
  // Create the GUI ControlP5 instance
  cp5 = new ControlP5(this);
  // List of the serial port
  serialList = cp5.addScrollableList("dropdown")
     .setCaptionLabel("Liste des liaisons serie")
     .setPosition(20, 20)
     .setSize(180, 160)
     .setBarHeight(20)
     .setItemHeight(20)
     .addItems(Serial.list())
     .setOpen(false)
     ;
  // create a new button with name 'serialBegin'
  serialRefresh = cp5.addButton("serialRefresh")
     .setCaptionLabel("R")
     .setPosition(210,20)
     .setSize(20,20)
     ;
  // create a new button with name 'serialBegin'
  serialBegin = cp5.addButton("serialBegin")
     .setCaptionLabel("Connection")
     .setPosition(240,20)
     .setSize(80,20)
     ;
  // create a new button with name 'serialSend'
  serialSend = cp5.addButton("serialSend")
     .setCaptionLabel("Send parameters")
     .setPosition(20,100)
     .setSize(180,20)
     ;
  // create a new button with name 'serialStop'
  serialStop = cp5.addButton("serialStop")
    .setCaptionLabel("Stop")
    .setPosition(330,20)
    .setSize(40,20)
    ;
  //Add a slider
  speed = cp5.addSlider("speed")
    .setCaptionLabel("Speed")
    .setColorCaptionLabel(color(20,20,20))
    .setPosition(20, 50)
    .setSize(180, 20)
    .setRange(0, 255)
    .setValue(0)
    ;
  // Creation of the console
  consoleArea = cp5.addTextarea("Console")
    .setPosition(20, height-200)
    .setSize(width-40,140 )
    .setFont(createFont("", 10))
    .setLineHeight(14)
    .setColor(color(255))
    .setColorBackground(color(0,44,91,240))
    ;
  // Creation of a toggle for the Visibility of the coordinates
  toggleCoordinate = cp5.addToggle("toggleCoordinate")
    .setCaptionLabel("Show coordinates")
    .setColorCaptionLabel(color(20,20,20))
    .setPosition(20,150)
    .setSize(50,20)
    .setValue(false)
    .setMode(ControlP5.SWITCH)
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

public void toggleCoordinate(boolean flag)
{
  toggleValue = flag ;
}

public void serialSend()
{
 int speedValue = int(speed.getValue());
 myPort.write(byte(speedValue));
 //myPort.write('\n');
 println("Parameters sent : speed set to " + speedValue);
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
  cp5.get(Button.class, "serialBegin").setVisible(GUIVisible);
  cp5.get(Button.class, "serialSend").setVisible(GUIVisible);
  cp5.get(Button.class, "serialStop").setVisible(GUIVisible);
  cp5.get(Button.class, "serialRefresh").setVisible(GUIVisible);
  cp5.get(ScrollableList.class, "dropdown").setVisible(GUIVisible);
  cp5.get(Slider.class,"speed").setVisible(GUIVisible);
  cp5.get(Toggle.class,"toggleCoordinate").setVisible(GUIVisible);
}

class robotCircle
{
  final int radiusMax ;     // Max radius of the "searching circle"
  int radius ;              // Radius of the "searching circle"
  int searchTime = 0;
  int searchTimeLast = 0 ;
  float boucle ;            // Duration of a loop in milliseconds by default

  robotCircle(int setRadiusMax, float setBoucle)
  {
    radiusMax = setRadiusMax ;
    radius = radiusMax ;
    boucle = setBoucle ;
    searchTimeLast = millis();
  }

  void updateXY(int xCircle, int yCircle ,int distance)
  {
    searchTime = millis()- searchTimeLast; // Take the elapsed time
    float nbr = map(searchTime,boucle,0,radiusMax,0);

    radius=radius-int(nbr);
    if(radius<=0) radius = radiusMax;
    searchTimeLast = millis();

    float transparency = map(radius,radiusMax,radiusMax/2,40,0);
    float red = map(distance,0,300,255,0);
    float green = 44 + map(distance,0,300,0,255);

    pushMatrix();
    // Transparency evolution
    fill(red,green,91,transparency);
    noStroke();
    ellipse(xCircle, yCircle, radius, radius);

    fill(255,0);
    strokeWeight(abs(map(radius,radiusMax,radiusMax/2,8,0)));
    // Transparency evolution
    stroke(red,green,91,transparency);
    float radiusExtern = map(radius,radiusMax,0,radiusMax,radiusMax+100);
    ellipse(xCircle, yCircle, radiusExtern, radiusExtern);

    // Create the robot circle
    fill(red, green, 0, 125);
    circle(xCircle, yCircle, map(distance,0,300,70,20));

    // Create and show the coordinates if visibility checked
    if (toggleValue) showCoordinates(xCircle, yCircle);

    popMatrix();
  }

  void updateDA(int distanceCircle, int angleCircle)
  {
    float px = width/2 + cos(radians(angleCircle))*distanceCircle;
    float py = height/2 + sin(radians(angleCircle))*distanceCircle;
    updateXY((int)px, (int)py, distanceCircle);
  }
}
