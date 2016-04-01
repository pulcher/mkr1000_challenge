// MKR1000 challenge entry: Say thanks to courteous drivers.
// Please refer to the following examples to make sure that everything works
// and is setup correctly:
// - Adafruit_DotStarMatrix > Matrix Test


#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_DotStarMatrix.h>
#include <Adafruit_DotStar.h>
#include <ArduinoJson.h>
#include <VirtualShield.h>
#include <Text.h>
#include <Graphics.h>
#include <Recognition.h>
#include <Colors.h>

#define DATAPIN  11
#define CLOCKPIN 12

Adafruit_DotStarMatrix matrix = Adafruit_DotStarMatrix(
  32, 8, DATAPIN, CLOCKPIN,
  DS_MATRIX_TOP     + DS_MATRIX_LEFT +
  DS_MATRIX_COLUMNS + DS_MATRIX_ZIGZAG,
  DOTSTAR_GBR);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255),
  matrix.Color(255, 0, 255), matrix.Color(255, 0, 106), matrix.Color( 255, 255, 255) };

VirtualShield shield;
Graphics screen = Graphics(shield);
Recognition speech = Recognition(shield);

int x    = matrix.width();
int pass = 0;
int thanksId, welcomeId, backoffId, stoppingId, startingId, turnLeftId, turnRightId, calling911Id;

void setup() {
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.setTextColor(colors[0]);

  DisplayText("Initializing...", 4, 10);

    // set up virtual shield events: 
    shield.setOnRefresh(refresh);
    speech.setOnEvent(onSpeech);
    
    screen.setOnEvent(screenEvent);   

    // begin the shield communication (also calls refresh()).
    shield.begin(); //assumes 115200 Bluetooth baudrate


      DisplayText("Initializing...finished", 3, 10);
}



void loop() {
  // checkSensors() checks for return events and handles them (calling callbacks). This is VirtualShield's single loop() method.
  shield.checkSensors();
}

void refresh(ShieldEvent* shieldEvent)
{
  // put your refresh code here
  // this runs whenever Bluetooth connects, whenever the shield.begin() executes, or the 'refresh' button is pressed in the app:
  screen.clear(ARGB(123,86,204));
  
  screen.drawAt(0,0, "");
  //screen.drawImage(0,50, "https://blog.arduino.cc/wp-content/uploads/2015/04/windowsarduino.jpg", "", 300, 100);
  thanksId      = screen.addButton(10, 50,   ". Thank you .");
  welcomeId     = screen.addButton(10, 100,  ".  Welcome  .");
  turnLeftId    = screen.addButton(10, 150,  ".      Left       .");
  stoppingId    = screen.addButton(175, 50,  ". Stopping  .");
  startingId    = screen.addButton(175, 100, ".   Starting  .");
  turnRightId   = screen.addButton(175, 150, ".    Right     .");
  backoffId     = screen.addButton(10, 225,  ".              Back Off                 .");
  calling911Id  = screen.addButton(10, 290,  ".           Calling 911                .");
  //rectangleId = screen.fillRectangle(0, 265, 100, 50, GREEN);
  //screen.line(105,265,300,305, YELLOW);
}

void screenEvent(ShieldEvent* shieldEvent) 
{
  if(screen.isPressed(thanksId))
  {
    DisplayText("Thank You", 5, 100);
  }

  if(screen.isPressed(welcomeId))
  {
    DisplayText("You're welcome", 2, 100);
  }
  
  if(screen.isPressed(backoffId))
  {
    DisplayText("Please back off", 4, 100);
  }
  
  if(screen.isPressed(stoppingId))
  {
    DisplayText("...stopping...", 0, 100);
  }
  
  if(screen.isPressed(startingId))
  {
    DisplayText("...starting...", 1, 100);
  }
  
  if(screen.isPressed(turnLeftId))
  {
    DisplayText("Turning LEFT", 3, 100);
  }
  
  if(screen.isPressed(turnRightId))
  {
    DisplayText("Turning RIGHT", 3, 100);
  }
    
  if(screen.isPressed(calling911Id))
  {
    DisplayText("Calling 911", 0, 100);
  }
  
//  if (screen.isReleased(thanksId)) 
//  {
//    screen.change(buttonId, ARGB(0,255,0));
//    screen.change(rectangleId, ARGB(0,255,0));
//  }
}

void onSpeech(ShieldEvent* shieldEvent) {
  screen.printAt(5, shieldEvent->resultId);
  screen.printAt(6, shieldEvent->result);
  screen.printAt(7, shieldEvent->action);
}

void DisplayText(String message, int colorIndex, int currDelay)
{
  int x = matrix.width(); 
  int maxX = -1 * (message.length() * 5 + message.length());
  matrix.setTextColor(colors[colorIndex]);
  
  while( x > maxX) {
    matrix.fillScreen(0);
    matrix.setCursor(x, 0);
    matrix.print(message);

    --x;

    matrix.show();
    delay(currDelay);
  }
}

