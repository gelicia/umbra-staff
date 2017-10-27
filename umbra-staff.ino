#include <FastLED.h>

#define SENSOR_PIN 6

#define LED_PIN     3
#define NUM_LEDS    80
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100

#define NUM_SEC 16
#define SEC_LEN 5

int mode = 0;
int storeSensorVal = 1;

uint8_t startIndex = 0;

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 mySunsetPalette;
extern const TProgmemPalette16 mySunsetPalette_p PROGMEM;

int numModes = 7;
CRGBPalette16 modes[7] = { RainbowColors_p, PartyColors_p, CloudColors_p, OceanColors_p, ForestColors_p, LavaColors_p, mySunsetPalette_p };


void setup() {
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT);

  delay( 3000 ); // power-up safety delay
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );

}

void loop() {
  int sensorVal = digitalRead(SENSOR_PIN);

  if (sensorVal == 0 && storeSensorVal == 1) {
    mode++;

    if (mode == numModes) {
      mode = 0;
    }
  }

  storeSensorVal = sensorVal;

  currentPalette = modes[mode];

  static uint8_t startIndex = 0;
  
  startIndex = startIndex + 1; /* motion speed */ 

  FillLEDsFromPaletteColors(startIndex);
  
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);

}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;
  
  for ( int i = 0; i < NUM_LEDS; i++) {
    int addr = (SEC_LEN * (i % NUM_SEC)) + (SEC_LEN - 1 - (floor(i/NUM_SEC)));
    uint8_t idx = map(addr, 0, NUM_LEDS, 0, 255) + colorIndex;
      
    leds[addr] = ColorFromPalette( currentPalette, idx, brightness, currentBlending);
    colorIndex += 3;
  }
}

void lightSingleLED(int addr) {
  //int newAddr = ((addr % SEC_LEN) * NUM_SEC) + (floor(addr/SEC_LEN));

  int newAddr = (SEC_LEN * (addr % NUM_SEC)) + (SEC_LEN - 1 - (floor(addr/NUM_SEC))); //(((SEC_LEN - 1) - (addr % SEC_LEN)) * NUM_SEC) + floor(addr/SEC_LEN);
  Serial.println(String(addr) + " " + String(newAddr));
  
  for ( int i = 0; i < NUM_LEDS; i++) {
    if (i == newAddr) {
      leds[newAddr] = CRGB::Red;
    }
    else {
      leds[i] = CRGB::Black;
    }
  }
}

const TProgmemPalette16 mySunsetPalette_p PROGMEM =
{
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Gray, 
    CRGB::Yellow,
    CRGB::Gold,
    CRGB::Goldenrod,
    CRGB::Orange,
    CRGB::Red, 
    CRGB::DarkRed,
    CRGB::Maroon, 
    CRGB::Indigo,
    CRGB::Purple,
    CRGB::BlueViolet,
    CRGB::MidnightBlue,
    CRGB::RoyalBlue,
    CRGB::Blue 
};
