WiFiClient client;
WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

//PinButton btnM5(39);
PinButton btnAction(39);

char currentState = -1;
bool connectedTovMix = false;

char deviceName[32];
int status = WL_IDLE_STATUS;
bool apEnabled = false;
boolean started = false;

// Time measure
int interval = 5000;
unsigned long lastCheck = 0;
unsigned long lastConnCheck = 0;

void setup()
{
  uint8_t c;
  Serial.begin(115200);
  M5.begin();
  led.setBrightness(20);
  led.begin();
  delay(10);
  
  loadSettings();

  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);
  digitalWrite(red_light_pin, 0);
  digitalWrite(green_light_pin, 0);
  digitalWrite(blue_light_pin, 0);
  
  Serial.print("SSID: ");
  Serial.println(&(WIFI_SSID[0]));
  Serial.print("PASS: ");
  Serial.println(&(WIFI_PASS[0]));
}

void loop()
{
  server.handleClient();
  led.clear();
 
  if(!started){
    started = true;
    start();
  }
  
  btnAction.update();
 
  if (btnAction.isDoubleClick()) {
        connectTovMix(false);
  }

  
  if(btnAction.isLongClick()){
    if(client.connected()){
       resetSettings();
       delay(3000);
       ESP.restart();
    } else {
      Serial.println("No need to reset");
      Serial.println("You're no connected");
      delay(3000);
    }
  }
  
  while (client.available())
  {
    server.handleClient();
    String data = client.readStringUntil('\r\n');
    handleData(data);
  }
  
  if (CONN_INT != 0 && !client.connected() && !apEnabled && millis() > lastConnCheck + (CONN_INT * 1000))
  {
    client.stop();
    lastCheck = millis();
    //noConnectionTovMix();
    singleReconnect();
  }

  //if(WiFi.status() != WL_CONNECTED)
  //{
  //   Serial.println("Reconnect WiFi");
  //  WiFi.reconnect();
  //}

}

void start()
{
  loadSettings();
  Serial.println(""); 
  Serial.println("vMix M5 ATOM Lite Tally");
  Serial.println("by AW");  
  Serial.println("");

  led.setPixelColor(0, led.Color(0, 255, 0));
  RGB_color(0,255,0);
  led.show(); 
  led.show();
  delay(1000);
  led.setPixelColor(0, led.Color(255, 0, 0));
  RGB_color(255,0,0);
  led.show(); 
  led.show();
  delay(1000);
  led.setPixelColor(0, led.Color(0, 0, 255));
  RGB_color(0,0,255);
  led.show(); 
  led.show();
  delay(1000);
  led.setPixelColor(0, led.Color(0, 0, 0));
  RGB_color(0,0,0);
  led.show(); 
  led.show();

  delay(2000);

  startWiFi();
}

void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  digitalWrite(red_light_pin, red_light_value);
  digitalWrite(green_light_pin, green_light_value);
  digitalWrite(blue_light_pin, blue_light_value);
}
