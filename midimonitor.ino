#include <MIDI.h>

#include <FastLED.h>

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#include <ClickEncoder.h>
#include <TimerOne.h>

// Create and bind the MIDI interface to the default hardware Serial port
MIDI_CREATE_DEFAULT_INSTANCE();
#define LED 13           // LED pin on Arduino Uno

#define LED_STRIP_PIN 2
#define NUM_LEDS 8

CRGB leds[NUM_LEDS];


// OLED display TWI address
#define OLED_ADDR   0x3C

Adafruit_SSD1306 display(-1);


ClickEncoder *encoder;
int16_t value;
int16_t last, midiChannelSelect;
byte lastNote, note;


void timerIsr() {
  encoder->service();
}

int16_t midiChannels;

void displayMidiChannelMenu() {

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Midi Channel ");
  bool xxx = midiChannels & midiChannelSelect;
  //  display.print(midiChannels & value);
  //  display.print(xxx);

  int channelX;
  int channelY;
  int yyy;

  for (int i = 1; i <= 16; i++) {
    channelX = ((i - 1) % 8) * 16;
    channelY = 8 + ((i - 1) / 8) * 15;
    display.setCursor(channelX, channelY);
    display.print(i);
    yyy = midiChannels & (1 << (i - 1));
    xxx = yyy;
    if (i == midiChannelSelect) {
      display.setCursor(110, 0);
      display.print(yyy);
      display.setCursor(120, 0);
      display.print(xxx);
    }
    if (yyy) {
      display.drawPixel(channelX + 2, channelY + 8, WHITE);
    }
  }

  display.setCursor((midiChannelSelect * 16) % 128, 16 + 1);
  if (midiChannelSelect < 8) {
    display.print('^');
  } else {
    display.print('v');
  }

  //  display.print(encoder->getAccelerationEnabled() ? "on " : "off");
  display.display();
}

void displayAccelerationStatus() {
  display.clearDisplay();
  display.setCursor(19, 1);
  display.print("Acceleration ");
  display.print(encoder->getAccelerationEnabled() ? "on " : "off");
  display.display();
}
void MyHandleNoteOff(byte channel, byte pitch, byte velocity) {
  digitalWrite(LED, LOW); //Turn LED off
  note = pitch;
  leds[note % 8] = 0;
  FastLED.show();
}

void MyHandleNoteOn(byte channel, byte pitch, byte velocity) {
  note = pitch;

  digitalWrite(LED, HIGH); //Turn LED on
  
  const String colour[4] = { "Bnhjkh", "R", "O", "Y"};
  String new_note = colour[note];

  if (note != lastNote) {

    leds[note % 8] = CRGB(255, 100, 100);
    FastLED.show();
    
    display.clearDisplay();
//    last = value;
    display.setCursor(10, 1);
    display.print("Note Value: ");
    //    display.println(new_note);

    //  displayNoteValue(note, velocity);
  }

}

void displayNoteValue(byte note, byte velocity) {
  if (note % 11 == 0) {
    display.println("A ");
  } else if (note % 11 == 1) {
    display.println("Bb");
  } else if (note % 11 == 2) {
    display.println("B ");
  } else if (note % 11 == 3) {
    display.println("C ");
  } else if (note % 11 == 4) {
    display.println("Db");
  } else if (note % 11 == 5) {
    display.println("D ");
  } else if (note % 11 == 6) {
    display.println("Eb");
  } else if (note % 11 == 7) {
    display.println("E ");
  } else if (note % 11 == 8) {
    display.println("F ");
  } else if (note % 11 == 9) {
    display.println("G ");
  } else if (note % 11 == 10) {
    display.println("Ab");
  }
  display.setCursor(10, 20);
  display.print("Velocity Value: ");
  display.println(velocity);
  display.display();
}


void testRGB() {
  leds[0] = CRGB(255, 0, 0);
  leds[1] = CRGB(0, 255, 0);
  leds[2] = CRGB(0, 0, 255);
  FastLED.setBrightness(12);
  FastLED.show();
  //  for (int i=0; i<NUM_LEDS; i++){
  //    leds[i] = CRGB(0, 255 - 4*i, 4*i );
  //    FastLED.setBrightness(2*i);
  //    FastLED.show();
  //    delay(150);
  //  }
  //  // Turn lights from blue to magenta from right to left
  //  for (int i=NUM_LEDS; i>=0; i--){
  //    leds[i] = CRGB(4*i,0 , 255-4*i);
  //    FastLED.setBrightness(16-2*i);
  //    FastLED.show();
  //    delay(150);
  //  }
}

byte menuItem, menuDepth;
// byte lastMenuItem, lastMenuDepth;
// bool anyMenuChanged, menuItemChanged, menuDepthChanged;
const int menuLength = 4;
byte menuItemIndex;

const String menuHeadings[menuLength] = {
  "Menu item 1",
  "Menu item 2",
  "Midi Channel Select",
  "Note Display"
  };

int test1() {
  display.print("prntfn1");
  return 0;
}

int test2() {
  display.print("prntfn1");
  return 1;
}
int n;
void thing(int x){
  // This apparently needs to be inside a function 
  // Doesn't work without 'void thing('
  int (*(functions[2]))() = {test1, test2};
  n = functions[0]();
}
//printf("%d\n", n);

// typedef void (* PrintFuncs)(int i);
// 
// void functionOne(int value){display.print("prntfn1");}
// void functionTwo(int value){display.print("prntfn2");}
// void functionThr(int value){display.print("prntfn3");}
// void functionFou(int value){display.print("prntfn4");}
// static PrintFuncs voidFuncs[] =
// {
//     functionOne,
//     functionTwo,
//     functionThr,
//     functionFou,
// };

void displayMainMenu(){

  // menuItemChanged = menuItem != lastMenuItem;
  // menuDepthChanged = menuDepth != lastMenuDepth;
  // anyMenuChanged = menuItemChanged | menuDepthChanged;
  // if (anyMenuChanged) {
  //   return;
  // }


  String menuString = menuHeadings[menuItem];

  if (menuDepth > 0) {
    display.clearDisplay();
    display.setCursor(0, 0);
    // display.print(menuHeadings[menuItem]);
    display.print("Note Display");
  } else {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Main Menu");

    display.setCursor(2, 8*2);
    display.print(">");

    for (int i = 0; i <= 2; i++) {
      display.setCursor(10, (i+1)*8);
      menuItemIndex = (menuItem+i-1+menuLength) % menuLength;
      // display.print(menuHeadings[menuItemIndex]);
      // PrintFuncs[](2);
      // n = functions[0]();
      thing(2);
      // display.print(((i-1) % menuLength));
    }
  }

  //  display.setCursor(110, 0);
  //  display.print(yyy);
  //  display.setCursor(120, 0);
  //  display.print(xxx);

  display.display();

}
void initializeOled() {
  // initialize and clear display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();

  // display a pixel in each corner of the screen
  display.drawPixel(0, 0, WHITE);
  display.drawPixel(127, 0, WHITE);
  display.drawPixel(0, 31, WHITE);
  display.drawPixel(127, 31, WHITE);

  // display a line of text
  display.setTextSize(1);
  display.setTextColor(WHITE);
  // display.setCursor(10,1);
  // display.print("hello, world!");

  display.display();
}

void handleMidiChannelClick(){
  midiChannels = midiChannels ^ 1 << midiChannelSelect;
  displayMidiChannelMenu();
}
void handleAccelerationChange(){

  display.println("ClickEncoder::DoubleClicked");
  encoder->setAccelerationEnabled(!encoder->getAccelerationEnabled());
  display.print("  Acceleration is ");
  display.println((encoder->getAccelerationEnabled()) ? "enabled" : "disabled");
  displayAccelerationStatus();
}


void handelEncoderInput() {
  value = encoder->getValue();
  if (value != 0){
    if (menuDepth == 0){
      menuItem += value;
      displayMainMenu();
    } else {
      // put this in a function array
      midiChannelSelect += value;
      displayMidiChannelMenu();
    }

  }


  ClickEncoder::Button b = encoder->getButton();
  if (b != ClickEncoder::Open) {
    display.print("Button: ");
    #define VERBOSECASE(label) case label: display.println(#label); break;
    switch (b) {
        VERBOSECASE(ClickEncoder::Pressed);
        VERBOSECASE(ClickEncoder::Held)
        VERBOSECASE(ClickEncoder::Released)
      case ClickEncoder::Clicked:
        if (menuDepth == 0){
          menuDepth = 1;
        } else if(true) {
          handleMidiChannelClick(); // TODO: put this in function array?
        }
        break;
      case ClickEncoder::DoubleClicked:
        handleAccelerationChange();
        break;
    }
  }
}

void setup() {
  initializeOled();

  // Initialize Menu
  menuItem = 0;
  menuDepth = 0;
  displayMainMenu();

  FastLED.addLeds<WS2812, LED_STRIP_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 10); // 500ma is probably the max?
  FastLED.clear();
  FastLED.setBrightness(12); // TODO: make this a menu item
  FastLED.show();


  pinMode(LED, OUTPUT);
  MIDI.begin(MIDI_CHANNEL_OMNI);          // Launch MIDI and listen to channel 4

  MIDI.setHandleNoteOn(MyHandleNoteOn); // This is important!! This command
  // tells the Midi Library which function I want called when a Note ON command
  // is received. in this case it's "MyHandleNoteOn".
  MIDI.setHandleNoteOff(MyHandleNoteOff);
  midiChannels = 1 << (3 - 1);


  // Encoder init
  //  Serial.begin(9600);
  encoder = new ClickEncoder(A0, A2, A1);
  Timer1.initialize(100);
  Timer1.attachInterrupt(timerIsr);
  last = -1;

}

void loop() {
  // put your main code here, to run repeatedly:

  // test_rgb()

  MIDI.read();


  handelEncoderInput();

  // value += encoder->getValue();
  midiChannelSelect %= 16;

  //  value += 1;
  //  if (value != last) {
  //    display.clearDisplay();
  //    last = value;
  //    display.setCursor(10,1);
  //    display.print("Encoder Value: ");
  //    display.println(value);
  //    display.display();
  //    displayMidiChannelMenu();
  //  }

  //  if (note != lastNote) {
  //    display.clearDisplay();
  //    last = value;
  //    display.setCursor(10,1);
  //    display.print("Note Value: ");
  //    display.println(note);
  //    display.display();
  //  }

}