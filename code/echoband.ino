#include <Adafruit_NeoPixel.h>
#include <arduinoFFT.h>

#define MIC_PIN        34
#define LED_PIN        13
#define NUM_LEDS       12
#define SAMPLES        64
#define SAMPLING_FREQ  4000

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
arduinoFFT FFT;

double vReal[SAMPLES];
double vImag[SAMPLES];

unsigned int samplingPeriodUs;
unsigned long timer;

int fadeLevels[NUM_LEDS];
int breathLevel = 0;
int breathDirection = 4;

void setup() {
  Serial.begin(115200);
  strip.begin();
  strip.show();
  samplingPeriodUs = round(1000000 * (1.0 / SAMPLING_FREQ));
  for (int i = 0; i < NUM_LEDS; i++) fadeLevels[i] = 0;
}

void loop() {
  // === collect mic data ===
  for (int i = 0; i < SAMPLES; i++) {
    timer = micros();
    vReal[i] = analogRead(MIC_PIN);
    vImag[i] = 0;
    while (micros() - timer < samplingPeriodUs);
  }

  // === fft ===
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

  double maxAmp = 0;
  for (int i = 2; i < SAMPLES / 2; i++) {
    if (vReal[i] > maxAmp) maxAmp = vReal[i];
  }

  // === decide mode based on loudness ===
  if (maxAmp < 100) {
    // breathe mode
    breathLevel += breathDirection;
    if (breathLevel >= 120 || breathLevel <= 10) breathDirection *= -1;
    uint32_t color = strip.Color(0, breathLevel, breathLevel + 60);
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, color);
    }

  } else if (maxAmp < 600) {
    // solid color mode
    int brightness = constrain(map(maxAmp, 100, 600, 30, 200), 0, 255);
    uint32_t color = strip.Color(brightness, 0, 255 - brightness);
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, color);
    }

  } else {
    // rainbow bar + fade
    int activeLeds = constrain(map(maxAmp, 600, 1000, 0, NUM_LEDS), 0, NUM_LEDS);
    for (int i = 0; i < NUM_LEDS; i++) {
      if (i < activeLeds) {
        fadeLevels[i] = 255;
      } else {
        fadeLevels[i] = max(0, fadeLevels[i] - 20);
      }
      uint32_t color = strip.gamma32(strip.ColorHSV((i * 65536L / NUM_LEDS), 255, fadeLevels[i]));
      strip.setPixelColor(i, color);
    }
  }

  strip.show();

  // debug
  Serial.print("amp: ");
  Serial.println(maxAmp);
}

