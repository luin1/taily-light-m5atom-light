#include <WiFi.h>
#include <M5StickC.h>
#include <PinButton.h>
#include <WebServer.h>
#include <Preferences.h>
#include <Adafruit_NeoPixel.h>

#define PIN 27 // PIN LED
#define NUMPIXELS 1 // ilość LED

#define LED_BUILTIN 10

Preferences preferences;

Adafruit_NeoPixel led(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500

//DON'T CHANGE THESE VARIABLES, YOU CAN CHANGE THEM IN THE WEB UI
String WIFI_SSID = "";
String WIFI_PASS = "";
String VMIX_IP = "";
String M_TALLY = "";
int VMIX_PORT = 8099; //USES THE TCP API PORT, THIS IS FIXED IN VMIX
int TALLY_NR = 1;
int CONN_INT = 5;

//rgb pin
#define red_light_pin 33
#define green_light_pin 23
#define blue_light_pin 19

//Kolory
uint32_t black = led.Color(0, 0, 0);
uint32_t white = led.Color(255, 255, 255);
uint32_t red = led.Color(255, 0, 0);
uint32_t green = led.Color(0, 255, 0);
uint32_t blue = led.Color(0, 0, 255);
uint32_t yellow = led.Color(255, 255, 0);
uint32_t purple = led.Color(128, 0, 128);
uint32_t magenta = led.Color(255,0,255);
