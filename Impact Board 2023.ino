//  Written for MakeShift 4039's Impact Presentation Board 2023
//  Code devloped by Vincent Lombardi, Blake McFadden, & Angela Tollis

//Import libraries
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

//4039 Button LED Pins
#define fourButton 50
#define zeroButton 51
#define threeButton 52
#define nineButton 53

//Presenter Button Pins
#define advanceButton 3
#define advanceButtonLED 2

//Neopixel elements pins
#define batt1PIN 22
#define batt2PIN 23
#define matrixPIN 24
#define node4PIN 25
#define node0PIN 26
#define node3PIN 27
#define node9PIN 28

//Neopixel elements LED Counts
#define BATT_LED_COUNT 60
#define NODE_4_LED_COUNT 6
#define NODE_0_LED_COUNT 5
#define NODE_3_LED_COUNT 4
#define NODE_9_LED_COUNT 5

//Define 4-0-3-9 neopixel elements
Adafruit_NeoPixel node4(NODE_4_LED_COUNT, node4PIN, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel node0(NODE_0_LED_COUNT, node0PIN, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel node3(NODE_3_LED_COUNT, node3PIN, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel node9(NODE_9_LED_COUNT, node9PIN, NEO_RGBW + NEO_KHZ800);

//define battery led strip elements
Adafruit_NeoPixel batt1(BATT_LED_COUNT, batt1PIN, NEO_GRBW + NEO_KHZ800), batt2(BATT_LED_COUNT, batt2PIN, NEO_GRBW + NEO_KHZ800);

//define matrix
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(64, 8, matrixPIN, //Initalizing Matrix
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_RIGHT + //Starting position
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG, //Matrix configuration
  NEO_GRB            + NEO_KHZ800); //Data setup

//Standard Colours
uint32_t red = node4.Color(255, 0, 0, 0);
uint32_t orange = node4.Color(255, 128, 0, 0);
uint32_t yellow = node4.Color(255, 255, 0, 0);
uint32_t green = node4.Color(0, 255, 0, 0);
uint32_t blue = node4.Color(0, 0, 255, 0);
uint32_t indigo = node4.Color(75, 0, 130, 0);
uint32_t violet = node4.Color(148, 0, 211, 0);
uint32_t white = node4.Color(0, 0, 0, 255);
uint32_t blank = node4.Color(0, 0, 0, 0);

/* Button Stuff*/
volatile bool should_advance = false;
volatile uint32_t time_pressed = 0;

// Wait if the advance button hasn't been pressed
void delay_if_not_advance(int time_millis) {
  while(time_millis > 100 && !should_advance) { delay(100); time_millis -= 100;}
  if(!should_advance) { delay(time_millis); }
}

// Wait for the button to be pressed. Calls the given function
// while waiting.
void wait_for_advance(void (*function)() = NULL) {
  while(!should_advance) {
    if(function == NULL) {      
      delay(100);      
    } else {
      function();       
    }
  }
  should_advance = false;
}

// Called when the button state is changed, regardless of whatever else is happening
void advance_change() {
    if(millis() - time_pressed > 200 && !digitalRead(advanceButton)) {
      should_advance = true;
    }  
    time_pressed = millis();    
}


//setup for rainbow effect color changing
uint32_t Wheel(byte WheelPos) {       
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return batt1.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return batt1.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return batt1.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


void setup() {
  
  //initialize Neopixel elements
  //batt1
  batt1.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  batt1.show();            // Turn OFF all pixels ASAP
  batt1.setBrightness(255); // Set BRIGHTNESS to about 1/5 (max = 255)
  //batt2
  batt2.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  batt2.show();            // Turn OFF all pixels ASAP
  batt2.setBrightness(255); // Set BRIGHTNESS to about 1/5 (max = 255)
  //node 4
  node4.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  node4.show();            // Turn OFF all pixels ASAP
  node4.setBrightness(20); // Set BRIGHTNESS to about 1/5 (max = 255)
  //node 0
  node0.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  node0.show();            // Turn OFF all pixels ASAP
  node0.setBrightness(20); // Set BRIGHTNESS to about 1/5 (max = 255)
  //node 3
  node3.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  node3.show();            // Turn OFF all pixels ASAP
  node3.setBrightness(20); // Set BRIGHTNESS to about 1/5 (max = 255)
  //node 9
  node9.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  node9.show();            // Turn OFF all pixels ASAP
  node9.setBrightness(20); // Set BRIGHTNESS to about 1/5 (max = 255)
  //matrix
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(20);
  matrix.show();

  //init 4039 Buttons pin mode
  pinMode(fourButton, OUTPUT);
  pinMode(zeroButton, OUTPUT);
  pinMode(threeButton, OUTPUT);
  pinMode(nineButton, OUTPUT);

  //init presenter Buttons pin mode
  pinMode(advanceButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(advanceButton), advance_change, FALLING); //check for button
  pinMode(advanceButtonLED, OUTPUT);

}

void loop() {
  //clear the display
  matrix.fillScreen(0);
  node4.show();
  node0.show();  
  node3.show();  
  node9.show();     

  //start the sequence
    wait_for_advance();

  //Red nodes  
  setNode4(red, 1);
  battColorWipe(red, 0, 4, 250);
  wait_for_advance();
  setNode4(red, 3);
  battColorWipe(red, 5, 10, 250);
  wait_for_advance();
  setNode4(red, 5);
  battColorWipe(red, 11, 16, 250);  

  //display first number on matrix
  matrix.setTextColor(matrix.Color(255,0,0));
  drawCentreString("398,522", 32, 1);
  matrix.show();

  //clear matrix, start first half of orange nodes
  wait_for_advance();
  matrix.fillScreen(0);
  matrix.show();
  setNode4(orange, 0);
  battColorWipe(orange, 17, 19, 250);
  wait_for_advance();
  setNode4(orange, 2);
  battColorWipe(orange, 20, 22, 250);
  wait_for_advance();
  setNode4(orange, 4);
  battColorWipe(orange, 23, 25, 250);
  wait_for_advance();
  setNode0(orange, 3);
  battColorWipe(orange, 26, 28, 250);

  //display second number on matrix
  matrix.setTextColor(matrix.Color(255,128,0));
  drawCentreString("1,566", 32, 1);
  matrix.show();

  //clear matrix, start second half of orange nodes
  wait_for_advance();
  matrix.fillScreen(0);
  matrix.show();
  setNode0(orange, 0);
  battColorWipe(orange, 29, 31, 250);
  wait_for_advance();
  setNode0(orange, 1);
  battColorWipe(orange, 32, 34, 250);
  wait_for_advance();
  setNode0(orange, 2);
  battColorWipe(orange, 35, 38, 250);  

  //green nodes
  wait_for_advance();
  setNode3(green, 0);
  battColorWipe(green, 39, 41, 250);
  wait_for_advance();
  setNode3(green, 1);
  battColorWipe(green, 42, 44, 250);
  wait_for_advance();
  setNode3(green, 2);
  battColorWipe(green, 45, 47, 250);
  wait_for_advance();
  setNode3(green, 3);
  battColorWipe(green, 48, 49, 250);

  //display third number on matrix
  matrix.setTextColor(matrix.Color(0,255,0));
  drawCentreString("5,628", 32, 1);
  matrix.show();

  //clear matrix, start blue nodes
  wait_for_advance();
  matrix.fillScreen(0);
  matrix.show();
  setNode9(blue, 0);
  battColorWipe(blue, 50, 50, 250);
  wait_for_advance();
  setNode9(blue, 1);
  battColorWipe(blue, 51, 52, 250);
  wait_for_advance();
  setNode9(blue, 2);
  battColorWipe(blue, 53, 54, 250);
  wait_for_advance();
  setNode9(blue, 3);
  battColorWipe(blue, 55, 56, 250);
  wait_for_advance();
  setNode9(blue, 4);
  battColorWipe(blue, 57, 58, 250);
  wait_for_advance();
  setNode3(blue, 3);
  battColorWipe(blue, 59, 60, 250);

  //display fourth number on matrix
  matrix.setTextColor(matrix.Color(0,0,255));
  drawCentreString("10,219,636", 32, 1);
  matrix.show();


  //clear matrix
  wait_for_advance();
  matrix.fillScreen(0);
  matrix.show();

  //illumante red, "4", button, display "WE" on matrix
  digitalWrite(fourButton, HIGH);
  matrix.setTextColor(matrix.Color(255,0,0));
  drawCentreString("WE", 7, 1);
  matrix.show();

  //illumante orange, "0", button, add "C" on matrix
  wait_for_advance();
  digitalWrite(zeroButton, HIGH);
  matrix.setTextColor(matrix.Color(255,128,0));
  drawCentreString("C", 22, 1);
  matrix.show();

  //illumante green, "3", button, add "A" on matrix
  wait_for_advance();
  digitalWrite(threeButton, HIGH);
  matrix.setTextColor(matrix.Color(0,255,0));
  drawCentreString("A",39, 1);
  matrix.show();

  //illumante blue, "9", button, add "N!" on matrix
  wait_for_advance();
  digitalWrite(nineButton, HIGH);
  matrix.setTextColor(matrix.Color(0,0,255));
  drawCentreString("N!", 55, 1);
  matrix.show();


  //Display "MakeShift" on matrix in multi-colour
  wait_for_advance();
  matrix.fillScreen(0);  
  matrix.setTextColor(matrix.Color(255, 0, 0));
  drawCentreString("Ma", 11, 1);
  matrix.setTextColor(matrix.Color(255, 128, 0));
  drawCentreString("ke", 22, 1);
  matrix.setTextColor(matrix.Color(0, 255, 0));
  drawCentreString("Sh", 35, 1);
  matrix.setTextColor(matrix.Color(0, 0, 255));
  drawCentreString("if", 47, 1);
  matrix.setTextColor(matrix.Color(148, 0, 211));
  drawCentreString("t", 56, 1);
  matrix.show();

  //set battery LED strips to rainbow - removed this in favour as rainbow effect
  //below as this line "locks out" the code and prevents stopping/restarting the presentation

  //battrainbow(10);


  //Rainbow effect for battery strips
  //setup variables
  uint16_t i, j;
  //loop through strip length
  for(i=0; i< batt1.numPixels(); i++) { 
    //set color using wheel function                                         
    batt1.setPixelColor(i, Wheel(((i * 256 / batt1.numPixels()) + j) & 255));
    batt2.setPixelColor(i, Wheel(((i * 256 / batt2.numPixels()) + j) & 255));
  }
  //show effect
  batt1.show();
  batt2.show();
  delay(20);

  //Turn everything off, this way the presentation can be looped
  //clear the display
  wait_for_advance();
  matrix.fillScreen(0);
  //clear the 4039 buttons
  digitalWrite(fourButton, LOW);
  digitalWrite(zeroButton, LOW);
  digitalWrite(threeButton, LOW);
  digitalWrite(nineButton, LOW); 
  //clear the nodes 
  node4.fill(blank);
  node0.fill(blank);  
  node3.fill(blank);  
  node9.fill(blank);
  //clear the strips    
  batt1.fill(blank);
  batt2.fill(blank);
  //apply all the effects
  matrix.show();
  node4.show();
  node0.show();
  node3.show();
  node9.show();
  batt1.show();
  batt2.show();

}

//This function writes text to the matrix and allows for the center pos of the text to be set
//Accepts the string to be written as well as the X and Y pos for center of the text
void drawCentreString(const String &buf, int x, int y)
{
    int16_t x1, y1;
    uint16_t w, h;
    matrix.getTextBounds(buf, x, y, &x1, &y1, &w, &h); //calc width of new string
    matrix.setCursor(x - w / 2, y);
    matrix.print(buf); //print matrix text
}

//  NODE FUNCTION //
//  One function per "node strip" ie, one for 4,0,3,9
//  accepts the pixel color as well as position in the strip as inputs

// Set pixel function for four node
void setNode4(uint32_t color, int pos) {
  node4.setPixelColor(pos, color);
  node4.show();
}

// Set pixel function for zero node
void setNode0(uint32_t color, int pos) {
  node0.setPixelColor(pos, color);
  node0.show();
}

// Set pixel function for three node
void setNode3(uint32_t color, int pos) {
  node3.setPixelColor(pos, color);
  node3.show();
}

// Set pixel function for nine node
void setNode9(uint32_t color, int pos) {
  node9.setPixelColor(pos, color);
  node9.show();
}

//This function "fills up" or "counts up" pixels on the battery LED strip
//It controls and writes to both battery strip 1 & 2 at the same time
//The color, as well as starting and ending pixels to count up can be set
//The delay between each step or count can also be set in ms
void battColorWipe( uint32_t color, int startPixel, int endPixel, int wait) {
  for(int i=startPixel; i<=endPixel; i++) { // For each pixel in strip...
    batt1.setPixelColor(i, color);
    batt2.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    delay(wait);
    batt1.show();
    batt2.show();                          //  Update strip to match
    }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void battrainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    // strip.rainbow() can take a single argument (first pixel hue) or
    // optionally a few extras: number of rainbow repetitions (default 1),
    // saturation and value (brightness) (both 0-255, similar to the
    // ColorHSV() function, default 255), and a true/false flag for whether
    // to apply gamma correction to provide 'truer' colors (default true).
    batt1.rainbow(firstPixelHue);
    batt2.rainbow(firstPixelHue);
    // Above line is equivalent to:
    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
    batt1.show(); // Update strip with new contents
    batt2.show();
    delay(wait);  // Pause for a moment
  }
}
