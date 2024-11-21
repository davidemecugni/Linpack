#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define BUILDIN_LED 2
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// Number of floating-point operations to perform
const unsigned long numOperations = 1000000;
volatile float a = 1.0, b = 2.0, c = 3.0, d = 4.0;

void blinkBuiltInLed(int interval) {
  digitalWrite(BUILDIN_LED, HIGH);
  delay(interval);
  digitalWrite(BUILDIN_LED, LOW);
  delay(interval);
}

void serialSetup() {
  Serial.begin(115200);
  while (!Serial) {
    blinkBuiltInLed(50);
  }
}

void displaySetup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;){
      blinkBuiltInLed(50);
    }
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();
}


void setup() {
  pinMode(BUILDIN_LED, OUTPUT);
  serialSetup();
  displaySetup();
}

void loop() {
  Serial.println("Starting FLOP/s calculation...");
  unsigned long startTime = micros();

  // Perform the floating-point operations
  for (unsigned long i = 0; i < (numOperations / 10); i++) {
    a = a * b + c - d;
    b = b * c + d - a;
    c = c * d + a - b;
    d = d * a + b - c;
    a = a / b + c * d;
    b = b / c + d * a;
    c = c / d + a * b;
    d = d / a + b * c;
    a = a + b / c * d;
    b = b + c / d * a;
  }

  unsigned long endTime = micros();

  unsigned long duration = endTime - startTime;
  float durationInSeconds = duration / 1000000.0;

  float flops = (numOperations) / durationInSeconds;

  float floapsToDisplay = 0.0;
  char flopsUnit[10] = "FLOP/s";
  if ( flops > 1000000.0){
    floapsToDisplay = flops / 1000000.0;
    strcpy(flopsUnit, "MFLOP/s");
  } else if (flops > 1000.0){
    floapsToDisplay = flops / 1000.0;
    strcpy(flopsUnit, "kFLOP/s");
  } else {
    floapsToDisplay = flops;
  }

  Serial.print("");
  Serial.print(durationInSeconds);
  Serial.println(" s");
  Serial.print("FLOP/s: ");
  Serial.print(floapsToDisplay);
  Serial.println(flopsUnit);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Duration (s): ");
  display.println(durationInSeconds);
  display.print("FLOP/s: ");
  display.print(floapsToDisplay);
  display.println(flopsUnit);
  display.display();

  delay(1000);
}