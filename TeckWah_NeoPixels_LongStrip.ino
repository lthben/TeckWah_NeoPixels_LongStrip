/* Author: Benjamin Low
 * 
 * Description: Teck Wah gallery NeoPixel Long Strip
 * 
 * Last updated: 21 Jan 2016
 */

#include <Adafruit_NeoPixel.h>

//USER DEFINED SETTINGS
const int SNAKE_LENGTH = 60; //the length in pixels of the lit trail
const float STRIP_LENGTH = 2.5; //the length in metres of the neopixel strip
const int PORT_NUM = 2; //the starting Arduino pin for the data line

// Pattern types supported:
enum  pattern { NONE, SCANNER };
// Pattern directions supported:
enum  direction { FORWARD, REVERSE };

// NeoPattern Class - derived from the Adafruit_NeoPixel class
class NeoPatterns : public Adafruit_NeoPixel
{
  public:

    // Member Variables:
    pattern  ActivePattern;  // which pattern is running
    direction Direction;     // direction to run the pattern

    unsigned long Interval;   // milliseconds between updates
    unsigned long lastUpdate; // last update of position

    uint32_t Color1, Color2;  // What colors are in use
    uint16_t TotalSteps;  // total number of steps in the pattern
    int Index;  // current step within the pattern

    uint32_t pinkColor = Color(200, 40, 60);
    uint32_t greenColor = Color(10, 220, 20 );
    uint32_t blueColor = Color(20, 20, 220);
    uint32_t yellowColor = Color( 160, 160, 10);
    uint32_t noColor = Color(0, 0, 0);

    void (*OnComplete)();  // Callback on completion of pattern
//    void (*TriggerNext)(); //Callback on hitting the next strip

    // Constructor - calls base-class constructor to initialize strip
    NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback1)() )
      : Adafruit_NeoPixel(pixels, pin, type)
    {
      OnComplete = callback1;
//      TriggerNext = callback2;
    }

    // Update the pattern
    void Update()
    {
      if ((millis() - lastUpdate) > Interval) // time to update
      {
        lastUpdate = millis();

        switch (ActivePattern)
        {
          case SCANNER:
            ScannerUpdate();
            break;
          default:
            break;
        }
      }
    }

    // Increment the Index and reset at the end
    void Increment()
    {
      if (Direction == FORWARD)
      {
        Index++;

        if (Index >= TotalSteps)
        {
          Index = 0;

          if (OnComplete != NULL)
          {
            OnComplete();
          }
        }
      }
      else // Direction == REVERSE
      {
        --Index;

        if (Index < 0)
        {
          Index = TotalSteps - 1;

          if (OnComplete != NULL)
          {
            OnComplete();
          }
        }
      }
    }

    void Scanner(uint32_t color1, uint8_t interval, int my_dir)
    {
      ActivePattern = SCANNER;
      Interval = interval;
      TotalSteps = numPixels();
      Color1 = color1;

      if (my_dir == 1)
      {
        Direction = FORWARD;
        Index = 0;
      }
      else if (my_dir == -1)
      {
        Direction = REVERSE;
        Index = numPixels() - 1;
      }
    }

    // Update the Scanner Pattern
    void ScannerUpdate()
    {
      for (int i = 0; i < numPixels(); i++)
      {
        if ( (i <= Index) && (Index - i < SNAKE_LENGTH) )
          //            if (i==Index)
        {
          //                byte my_index = ( (i + Index) * 256 / numPixels() ) & 255; //1st method for color
//          byte my_index =  ( (i * 256 / numPixels() ) + Index ) & 255; //2nd method 
//          setPixelColor(i, Wheel(my_index) );
          
                             setPixelColor(i, Color1);
        }
        else // Fading tail
        {
          setPixelColor(i, DimColor(getPixelColor(i)));
        }
      }
      show();
      Increment();
    }


    // Calculate 50% dimmed version of a color (used by ScannerUpdate)
    uint32_t DimColor(uint32_t color)
    {
      // Shift R, G and B components one bit to the right
      uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
      return dimColor;
    }

    // Set all pixels to a color (synchronously)
    void ColorSet(uint32_t color)
    {
      ActivePattern = NONE;

      for (int i = 0; i < numPixels(); i++)
      {
        setPixelColor(i, color);
      }
      show();
    }

    // Returns the Red component of a 32-bit color
    uint8_t Red(uint32_t color)
    {
      return (color >> 16) & 0xFF;
    }

    // Returns the Green component of a 32-bit color
    uint8_t Green(uint32_t color)
    {
      return (color >> 8) & 0xFF;
    }

    // Returns the Blue component of a 32-bit color
    uint8_t Blue(uint32_t color)
    {
      return color & 0xFF;
    }

    // Input a value 0 to 255 to get a color value.

    // The colours are a transition r - g - b - back to r.
    uint32_t Wheel(byte WheelPos)
    {
      WheelPos = 255 - WheelPos;

      if (WheelPos < 85)
      {
        return Color( 255 - WheelPos * 3, 0, WheelPos * 3);
      }
      else if (WheelPos < 170)
      {
        WheelPos -= 85;
        return Color(0, WheelPos * 3, 255 - WheelPos * 3);
      }
      else
      {
        WheelPos -= 170;
        return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
      }
    }

}; //end of class definition

// ------------------------------------------------------
// Function prototypes for completion callback routines
// ------------------------------------------------------
void Strip1Complete();
void Strip2Complete();
void Strip3Complete();
void Strip4Complete();
void Strip5Complete();
void Strip6Complete();
void Strip7Complete();
void Strip8Complete();
void Strip9Complete();
void Strip10Complete();

// -----------------------------
// Object declarations
// -----------------------------

NeoPatterns Strip1(int(60 * STRIP_LENGTH), PORT_NUM, NEO_GRB + NEO_KHZ800, &Strip1Complete);
NeoPatterns Strip2(int(60 * STRIP_LENGTH), PORT_NUM + 1, NEO_GRB + NEO_KHZ800, &Strip2Complete);
NeoPatterns Strip3(int(60 * STRIP_LENGTH), PORT_NUM + 2, NEO_GRB + NEO_KHZ800, &Strip3Complete);
NeoPatterns Strip4(int(60 * STRIP_LENGTH), PORT_NUM + 3, NEO_GRB + NEO_KHZ800, &Strip4Complete);
NeoPatterns Strip5(int(60 * STRIP_LENGTH), PORT_NUM + 4, NEO_GRB + NEO_KHZ800, &Strip5Complete);
NeoPatterns Strip6(int(60 * STRIP_LENGTH), PORT_NUM + 5, NEO_GRB + NEO_KHZ800, &Strip6Complete);
NeoPatterns Strip7(int(60 * STRIP_LENGTH), PORT_NUM + 6, NEO_GRB + NEO_KHZ800, &Strip7Complete);
NeoPatterns Strip8(int(60 * STRIP_LENGTH), PORT_NUM + 7, NEO_GRB + NEO_KHZ800, &Strip8Complete);
NeoPatterns Strip9(int(60 * STRIP_LENGTH), A0, NEO_GRB + NEO_KHZ800, &Strip9Complete);
NeoPatterns Strip10(int(60 * STRIP_LENGTH), A1, NEO_GRB + NEO_KHZ800, &Strip10Complete);

long startTime;

//------------------------------
// setup
//------------------------------
void setup() {
  Serial.begin(9600);

  Strip1.begin();
  Strip2.begin();
  Strip3.begin();
  Strip4.begin();
  Strip5.begin();
  Strip6.begin();
  Strip7.begin();
  Strip8.begin();
  Strip9.begin();
  Strip10.begin();
}

//-------------------------
// The main loop
//-------------------------

void loop() {

  Strip1.Update();
  Strip2.Update();
  Strip3.Update();
  Strip4.Update();
  Strip5.Update();
  Strip6.Update();
  Strip7.Update();
  Strip8.Update();
  Strip9.Update();
  Strip10.Update();

  read_from_serial();
}

//-----------------------
// supporting functions
//-----------------------

void read_from_serial() {

  unsigned char incomingbyte = 0;

  if (Serial.available() > 0) {

    incomingbyte = Serial.read();

    if (incomingbyte == '0') {

        Strip1.ColorSet(Strip1.noColor);
        Strip2.ColorSet(Strip1.noColor);
        Strip3.ColorSet(Strip1.noColor);
        Strip4.ColorSet(Strip1.noColor);
        Strip5.ColorSet(Strip1.noColor);
        Strip6.ColorSet(Strip1.noColor);
        Strip7.ColorSet(Strip1.noColor);
        Strip8.ColorSet(Strip1.noColor);
        Strip9.ColorSet(Strip1.noColor);
        Strip10.ColorSet(Strip1.noColor);

    } else if (incomingbyte == '1') {

        Strip8.ColorSet(Strip1.yellowColor);

    } else if (incomingbyte == '2') {

        Strip9.ColorSet(Strip1.yellowColor);

    } else if (incomingbyte == '3') {

        Strip10.ColorSet(Strip1.yellowColor);

    } else if (incomingbyte == '4') {

    } else if (incomingbyte == '5') {

      Strip1.Scanner( Strip1.Color(255, 255, 255), 1, 1 );

    } else if (incomingbyte == '6') {

      Strip10.Scanner( Strip1.Color(255, 255, 255), 1, -1);
    }
  }
}

// -----------------------------
// Completion callback routines
// -----------------------------

void Strip1Complete() {

  Serial.println("strip 1 complete");
  Strip1.ActivePattern = NONE;
  Strip1.ColorSet(Strip1.noColor);

  if (Strip1.Direction == FORWARD)
  {
    Strip2.Scanner( Strip1.Color(255, 255, 255), 1, 1 );
  } else
  {
  }
}

void Strip2Complete() {

  Serial.println("strip 2 complete");
  Strip2.ActivePattern = NONE;
  Strip2.ColorSet(Strip1.noColor);

  if (Strip2.Direction == FORWARD)
  {
    Strip3.Scanner( Strip1.Color(255, 255, 255), 1, 1 );
  } else
  {
    Strip1.Scanner( Strip1.Color(255, 255, 255), 1, -1 );
  }
}

void Strip3Complete() {

  Serial.println("strip 3 complete");
  Strip3.ActivePattern = NONE;
  Strip3.ColorSet(Strip1.noColor);

  if (Strip3.Direction == FORWARD)
  {
    Strip4.Scanner( Strip1.Color(255, 255, 255), 1, 1 );
  } else
  {
    Strip2.Scanner( Strip1.Color(255, 255, 255), 1, -1 );
  }
}

void Strip4Complete() {

  Serial.println("strip 4 complete");
  Strip4.ActivePattern = NONE;
  Strip4.ColorSet(Strip1.noColor);

  if (Strip4.Direction == FORWARD)
  {
    Strip5.Scanner( Strip1.Color(255, 255, 255), 1, 1 );
  } else
  {
    Strip3.Scanner( Strip1.Color(255, 255, 255), 1, -1 );
  }
}

void Strip5Complete() {

  Serial.println("strip 5 complete");
  Strip5.ActivePattern = NONE;
  Strip5.ColorSet(Strip1.noColor);

  if (Strip5.Direction == FORWARD)
  {
    Strip6.Scanner( Strip1.Color(255, 255, 255), 1, 1 );
  } else
  {
    Strip4.Scanner( Strip1.Color(255, 255, 255), 1, -1 );
  }
}

void Strip6Complete() {

  Serial.println("strip 6 complete");
  Strip6.ActivePattern = NONE;
  Strip6.ColorSet(Strip1.noColor);

  if (Strip6.Direction == FORWARD)
  {
    Strip7.Scanner( Strip1.Color(255, 255, 255), 1, 1 );
  } else
  {
    Strip5.Scanner( Strip1.Color(255, 255, 255), 1, -1 );
  }
}

void Strip7Complete() {

  Serial.println("strip 7 complete");
  Strip7.ActivePattern = NONE;
  Strip7.ColorSet(Strip1.noColor);

  if (Strip7.Direction == FORWARD)
  {
    Strip8.Scanner( Strip1.Color(255, 255, 255), 1, 1 );
  } else
  {
    Strip6.Scanner( Strip1.Color(255, 255, 255), 1, -1 );
  }
}

void Strip8Complete() {

  Serial.println("strip 8 complete");
  Strip8.ActivePattern = NONE;
  Strip8.ColorSet(Strip1.noColor);

  if (Strip8.Direction == FORWARD)
  {
    Strip9.Scanner( Strip1.Color(255, 255, 255), 1, 1 );
  } else
  {
    Strip7.Scanner( Strip1.Color(255, 255, 255), 1, -1 );
  }
}

void Strip9Complete() {

  Serial.println("strip 9 complete");
  Strip9.ActivePattern = NONE;
  Strip9.ColorSet(Strip1.noColor);

  if (Strip9.Direction == FORWARD)
  {
    Strip10.Scanner( Strip1.Color(255, 255, 255), 1, 1 );
  } else
  {
    Strip8.Scanner( Strip1.Color(255, 255, 255), 1, -1 );
  }
}

void Strip10Complete() {

  Serial.println("strip 10 complete");
  Strip10.ActivePattern = NONE;
  Strip10.ColorSet(Strip1.noColor);

  if (Strip10.Direction == FORWARD)
  {
  } else
  {
    Strip9.Scanner( Strip1.Color(255, 255, 255), 1, -1 );
  }
}






