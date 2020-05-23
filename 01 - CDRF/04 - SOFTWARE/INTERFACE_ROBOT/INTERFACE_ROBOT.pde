
import controlP5.*;                  // Import the CP5 library
import processing.serial.*;          // Import the serial library

Serial myPort;                       // Create the serial port object
ControlP5 cp5;                       // Create the CP5 object

// Creation of the object effectCircle
robotCircle myRobotCircle = new robotCircle(500, 4000.0);
robotCircle robot1Circle = new robotCircle(200, 3400.0);
robotCircle robot2Circle = new robotCircle(200, 1000.0);

// Creation of the variable for the GUI
// Console
Textarea consoleArea;
Println console;
// Creation of the lists
ScrollableList serialList;
// Creation of the buttons
Button serialBegin;
Button serialSend;
Button serialStop;
Button serialRefresh;
// State of the buttons
boolean clicSerialBegin = false ;
boolean clicSerialStop = false ;
boolean clicSerialSend = false ;
boolean clicSerialRefresh = false ;
// Creation of the sliders
Slider speed;
// Creation of the toggles
Toggle toggleCoordinate;
// State of the toggles
boolean toggleValue = false;
// GUI Visibility
boolean GUIVisible = true;
// State new message
boolean newPosition = false;


// Communication variables
byte[] receivedBuf = new byte[6];
int[] transformBuf = new int[4];

int distance, angle;


void setup()
{
  size(displayWidth,displayHeight);
  frameRate(24);
  noStroke();
  fill(0);

  createGUI();
}

void draw()
{
  background(255);
  robotDisplay();
  mouseDisplay();

  //myRobotCircle.updateXY(width/2,height/2,300);
  if(newPosition)robot2Circle.updateDA(distance,angle);
}

void serialEvent(Serial p)
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
int unsignedByte(byte value)
{
  if(value < 0) return value + 256 ;
  else return value;
}
