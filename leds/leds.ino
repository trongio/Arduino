#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
#define LED_COUNT 66

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int animationMode = 0;
int lastButtonState = LOW;
unsigned long lastButtonPressTime = 0;
const int debounceDelay = 50;

unsigned long lastUpdate = 0;
unsigned long updateInterval = 50;

bool isFilled = false;


void setup() {
  pinMode(2, INPUT_PULLUP);
  strip.begin();
  strip.show();
  strip.setBrightness(10);
}

void loop() {
  unsigned long currentTime = millis();

  int buttonState = digitalRead(2);

  if (buttonState == HIGH && lastButtonState == LOW && (currentTime - lastButtonPressTime > debounceDelay)) {
    lastButtonPressTime = currentTime;
    animationMode = (animationMode + 1) % 9;  // Increased to 10 for new animations
    strip.clear();
    strip.show();
       // Reset the isFilled flag for fillColorFade
    isFilled = false;
  }

  lastButtonState = buttonState;

  if (currentTime - lastUpdate >= updateInterval) {
    lastUpdate = currentTime;
    switch (animationMode) {
      case 0:
        colorWipe(strip.Color(255, 0, 0), 50);
        break;
      case 1:
        colorWipe(strip.Color(0, 255, 0), 50);  // Green
        break;
      case 2:
        colorWipe(strip.Color(0, 0, 255), 50);  // Blue
        break;
      case 3:
        colorWipe(strip.Color(255, 255, 255), 50);  // white
        break;
      case 4:
        rainbow(10);
        break;
      case 5:
        theaterChaseRainbow(50);
        break;
      case 6:
        snake();
        break;
      case 7:
        randomPixels();
        break;
      case 8:
        fillColorFade(strip.Color(255, 255, 255), 50);  // Example color: Orange. Change the color as per your needs.
        break;
    }
  }
}

void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void theaterChase(uint32_t color, int wait) {
  strip.clear();
  for (int i = 0; i < strip.numPixels(); i += 3) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void rainbow(int wait) {
  static long firstPixelHue = 0;
  firstPixelHue += 256;
  if (firstPixelHue >= 5 * 65536) firstPixelHue = 0;
  strip.fill(strip.gamma32(strip.ColorHSV(firstPixelHue)));
  strip.show();
}

void theaterChaseRainbow(int wait) {
  static int firstPixelHue = 0;
  strip.clear();
  for (int i = 0; i < strip.numPixels(); i += 1) {
    int hue = firstPixelHue + i * 65536L / strip.numPixels();
    uint32_t color = strip.gamma32(strip.ColorHSV(hue));
    strip.setPixelColor(i, color);
  }
  strip.show();
  firstPixelHue += 65536 / 90;
}

void blinking() {
  static bool state = true;
  state = !state;
  strip.fill(state ? strip.Color(0, 255, 0) : strip.Color(0, 0, 0));
  strip.show();
}

void snake() {
  static int head = 0;
  static int tail = 0;
  static int filledPixels = 5;
  static bool animationComplete = false;  // Flag to indicate animation completion

  // If animation is complete, don't proceed
  if (animationComplete) {
    strip.show();
    return;
  }

  strip.clear();
  strip.setPixelColor(head, strip.Color(255, 255, 255));
  strip.setPixelColor(tail, strip.Color(100, 100, 100));
  head = (head + 1) % strip.numPixels();
  tail = (tail + 1) % strip.numPixels();

  // If the strip is fully filled, set the animationComplete flag
  if (filledPixels >= strip.numPixels()) {
    animationComplete = true;
  }

  for (int i = 0; i < filledPixels; i++) {
    int index = (tail + i) % strip.numPixels();
    strip.setPixelColor(index, strip.Color(255, 255, 255));
  }

  strip.show();
}

void randomPixels() {
  int pixel = random(strip.numPixels());
  strip.setPixelColor(pixel, strip.Color(random(256), random(256), random(256)));
  strip.show();
}

void alternateColors(int wait) {
  static bool state = true;
  state = !state;
  strip.fill(state ? strip.Color(255, 0, 0) : strip.Color(0, 0, 255));  // Alternating between red and blue
  strip.show();
}


void fillColorFade(uint32_t color, int wait) {
  // If already filled, just return to keep the color
  if (isFilled) {
    return;
  }

  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }

  // Mark the strip as filled
  isFilled = true;
}