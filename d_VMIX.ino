// Connect to vMix instance
boolean connectTovMix(bool recursive){
  Serial.println("Connecting to vMix...");
  if (client.connect(&(VMIX_IP[0]), VMIX_PORT))
  {
    connectedTovMix = true;
    Serial.println("Connected to vMix!");
    
    // Subscribe to the tally events
    client.println("SUBSCRIBE TALLY");
    showTallyScreen();
    return true;
  }
  else
  {
    if(recursive){
      return false;
    }
    char tryCount = 0;
    Serial.println("Could not connect to vMix");
    Serial.println("Retrying: 0/3");
     
    boolean retry = false;
    for (uint8_t i = 0; i < 3; i++)
    {
      Serial.print(i);
      retry = retryConnectionvMix(i);
      if (!retry) {
        return true;
      }
    }
    connectedTovMix = false;
    noConnectionTovMix();
    return false;
  }
}

boolean retryConnectionvMix(int tryCount) {
  Serial.println("Couldn't connect to vMix");
  Serial.println("Retrying: ");
  Serial.print(tryCount);
  Serial.print("/3");

  delay(2000);
  boolean conn = connectTovMix(true);
  if (conn) {
    return false;
  }
  return true;
}

void setTallyProgram()
{
  led.clear(); 
  led.setPixelColor(0, red);
  RGB_color(255,0,0);
  led.show(); 
  led.show();  
}

void setTallyPreview() {
  led.clear(); 
  led.setPixelColor(0, green);
  RGB_color(0,255,0);
  led.show();
  led.show();   
}

void setTallyOff() {
  led.clear(); 
  led.setPixelColor(0, white);
  RGB_color(0,0,0);
  led.show();
  led.show();
}

// Handle incoming data
void handleData(String data)
{
  bool changed = false;
  // Check if server data is tally data
  if (data.indexOf("TALLY") == 0)
  {
    char newState = data.charAt(TALLY_NR + 8);
    // Check if tally state has changed
    if (currentState != newState)
    {
      currentState = newState;
      changed = true;
      if(M_TALLY == ""){
        showTallyScreen();
      }
    }
    if(M_TALLY != ""){
      if(currentState != '1' && M_TALLY != ""){
        int str_len = M_TALLY.length() + 1;
       
        // Prepare the character array (the buffer) 
        char str[str_len];
         
        // Copy it over 
        M_TALLY.toCharArray(str, str_len);
        
        char * pch;
        pch = strtok(str, ",");
        while (pch != NULL)
        {
          String c(pch);
          char newMState = data.charAt(c.toInt() + 8);
          if((currentState == '0' && (newMState == '1' || newMState == '2')) || (currentState == '2' && (newMState == '1'))){
            currentState = newMState;
            changed = true;
          }
          pch = strtok(NULL, ",");
        }
      }
      if(changed){
        showTallyScreen();
      }
    }
  }
  else
  {
    Serial.print("Response from vMix: ");
    Serial.println(data);
  }
}

void showTallyScreen() {
  switch (currentState)
  {
    case '0':
      setTallyOff();
      break;
    case '1':
      setTallyProgram();
      break;
    case '2':
      setTallyPreview();
      break;
    default:
      setTallyOff();
  }
}

void singleReconnect(){
  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Reconnect WiFi");
    WiFi.reconnect();
  }
 
  Serial.println("Connecting to vMix...");
  RGB_color(0,0,0);
  led.setPixelColor(0, black);
  led.show();
  led.show();
  delay(1000);
  led.setPixelColor(0, blue);
  led.show();
  led.show();
  
  if (client.connect(&(VMIX_IP[0]), VMIX_PORT))
  {
    lastConnCheck = millis();
    connectedTovMix = true;
    Serial.println("Connected to vMix!");

    // Subscribe to the tally events
    client.println("SUBSCRIBE TALLY");
    showTallyScreen();
  } else {
    lastConnCheck = millis();
  }
}


void noConnectionTovMix(){
  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Reconnect WiFi");
    WiFi.reconnect();
  }
  RGB_color(0,0,0);
  
  led.setPixelColor(0, black);
  led.show();
  led.show();
  delay(1000);
  led.setPixelColor(0, blue);
  led.show();
  led.show();
  Serial.println("Couldn't connect to vMix");
  Serial.println();
  Serial.println("vMix is closed or check settings");
  Serial.println();
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.println("Press double BTN39 to reconnect");
  Serial.println();
}
