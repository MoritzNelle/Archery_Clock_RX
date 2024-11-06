#include <Adafruit_NeoPixel.h>
#include <esp_now.h>
#include <WiFi.h>

// Define the pin and number of LEDs
#define LED_PIN    14
#define BUZZER_PIN 13
#define NUM_LEDS   75

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800); // Create an instance of the Adafruit_NeoPixel class

// Define the data structure
typedef struct {
  uint8_t numBuzzerBeeps;
  uint8_t buzzerDuration;
  uint8_t buzzerBreak;
  uint8_t buzzerPitch;
  uint8_t ledColors[NUM_LEDS][3]; // RGB values for each LED
} DataPackage;

DataPackage receivedData;

// Callback function executed when data is received
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));

  // Set LED colors
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t r = receivedData.ledColors[i][0];
    uint8_t g = receivedData.ledColors[i][1];
    uint8_t b = receivedData.ledColors[i][2];
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();

  // Play buzzer beeps
  for (int i = 0; i < receivedData.numBuzzerBeeps; i++) {
    tone(BUZZER_PIN, receivedData.buzzerPitch * 10, receivedData.buzzerDuration * 10);
    delay(receivedData.buzzerDuration * 10 + receivedData.buzzerBreak * 10);
  }
}

void setup() {
  // Initialize the LED strip
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // Initialize Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register for a callback function when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // TODO: programm a time out to fade out the LEDs, if no new data is received
    //TODO: Make sure to stop the buzzer as well.
    //TODO: Turn back on, when new data is received agin.
}