// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"


#include "Adafruit_Si7021.h"
#include <Wire.h>
#include "Adafruit_TCS34725.h"
/* Initialise with default values (int time = 2.4ms, gain = 1x) RGB Light sensor */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();
/* Initialise with specific int time and gain values RGB Light sensor */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);  
// Initialise temp sensor 
Adafruit_Si7021 sensor = Adafruit_Si7021();

// Dust Sensor code 
#define measurePin 34 //Connect dust sensor to Arduino A34 pin
#define ledPower 13  //Connect 3 led driver pins of dust sensor to Arduino D2 at pin 13
int samplingTime = 280; // time required to sample signal coming out of the sensor
int deltaTime = 40; //
int sleepTime = 9680;
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;



int sort_desc(const void *cmp1, const void *cmp2);
int getTemprature();
int getHumidity();
int getLight();
int getSound();

// Replace with your network credentials
const char* ssid = "IEEE";
const char* password = "Ilovesolder";

// Web Server HTTP Authentication credentials
const char* http_username = "admin";
const char* http_password = "admin";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncEventSource events("/events");

const char* PARAM_INPUT_1 = "state";


// Main HTML web page in root url /
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>SLEEP BETTER DASHBOARD</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h3 {font-size: 1.8rem; color: white;}
    h4 { font-size: 1.2rem;}
    p { font-size: 1.4rem;}
    body {  margin: 0;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px; margin-bottom: 20px;}
    .switch input {display: none;}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 68px;   opacity: 0.8;   cursor: pointer;}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #1b78e2}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
    .topnav { overflow: hidden; background-color: #1b78e2;}
    .content { padding: 20px;}
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);}
    .cards { max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));}
    .slider2 { -webkit-appearance: none; margin: 14px;  height: 20px; background: #ccc; outline: none; opacity: 0.8; -webkit-transition: .2s; transition: opacity .2s; margin-bottom: 40px; }
    .slider:hover, .slider2:hover { opacity: 1; }
    .slider2::-webkit-slider-thumb { -webkit-appearance: none; appearance: none; width: 40px; height: 40px; background: #008B74; cursor: pointer; }
    .slider2::-moz-range-thumb { width: 40px; height: 40px; background: #008B74; cursor: pointer;}
    .reading { font-size: 2.6rem;}
    .card-switch {color: #50a2ff; }
    .card-light{ color: #008B74;}
    .card-bme{ color: #572dfb;}
    .card-sound{ color: #008B74;}
    .icon-pointer{ cursor: pointer;}
  </style>
</head>
<body>
  <div class="topnav">
    <h3>SLEEPWELL DASHBOARD <span style="text-align:right;">&nbsp;&nbsp; <i class="fas fa-user-slash icon-pointer" onclick="logoutButton()"></i></span></h3>
  </div>
  <div class="content">
    <div class="content">
      <div class="card card-res">
        <h2> RESULT OF SENSORS</h2><p class="res"><span id="res"></span></p></div>
      </div>
    <div class="cards">
      <div class="card card-score">
        <h4><i class="fas fa-book"></i> SCORE</h4><div><p class="reading"><span id="score"></span></p></div>
      </div>
      <div class="card card-leader">
        <h4><i class="fas fa-table"></i> LEADERBOARD</h4><div><p class="reading"><span id="lead"></span></p></div>
      </div>
      <div class="card card-bme">
        <h4><i class="fas fa-chart-bar"></i> TEMPERATURE</h4><div><p class="reading"><span id="temp"></span>&deg;F</p></div>
      </div>
      <div class="card card-bme">
        <h4><i class="fas fa-chart-bar"></i> HUMIDITY</h4><div><p class="reading"><span id="humi"></span>&percnt;</p></div>
      </div>
      <div class="card card-light">
        <h4><i class="fas fa-sun"></i> LIGHT</h4><div><p class="reading"><span id="light"></span> Lux</p></div>
      </div>
      <div class="card card-sound">
        <h4><i class="fas fa-volume-up"></i> SOUND</h4><div><p class="reading"><span id="sound"></span>&percnt;</p></div>
      </div>
  </div>
<script>
function logoutButton() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/logout", true);
  xhr.send();
  setTimeout(function(){ window.open("/logged-out","_self"); }, 1000);
}
function controlOutput(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/output?state=1", true); }
  else { xhr.open("GET", "/output?state=0", true); }
  xhr.send();
}
function toggleLed(element) {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/toggle", true);
  xhr.send();
}
if (!!window.EventSource) {
 var source = new EventSource('/events');
 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);
 source.addEventListener('message', function(e) {
  console.log("message", e.data);
 }, false);
 source.addEventListener('temperature', function(e) {
  console.log("temperature", e.data);
  document.getElementById("temp").innerHTML = e.data;
 }, false);
 source.addEventListener('humidity', function(e) {
  console.log("humidity", e.data);
  document.getElementById("humi").innerHTML = e.data;
 }, false);
 source.addEventListener('light', function(e) {
  console.log("light", e.data);
  document.getElementById("light").innerHTML = e.data;
 }, false);
 source.addEventListener('sound', function(e) {
  console.log("sound", e.data);
  document.getElementById("sound").innerHTML = e.data;
 }, false);
 source.addEventListener('result', function(e) {
  console.log("result", e.data);
  document.getElementById("res").innerHTML = e.data;
 }, false);
 source.addEventListener('score', function(e) {
 console.log("score", e.data);
 document.getElementById("score").innerHTML = e.data;
 }, false);
 source.addEventListener('leaderboard', function(e) {
 console.log("leaderboard", e.data);
 document.getElementById("lead").innerHTML = e.data;
 }, false);


}</script>
</body>
</html>)rawliteral";

String outputState(int gpio){
  if(digitalRead(gpio)){
    return "checked";
  }
  else {
    return "";
  }
}

String processor(const String& var){
  //Serial.println(var);
  return String("");
}

// Logged out web page
const char logout_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <p>Logged out or <a href="/">return to homepage</a>.</p>
  <p><strong>Note:</strong> close all web browser tabs to complete the logout process.</p>
</body>
</html>
)rawliteral";

void setup(){
     pinMode(2, INPUT); // Touch Sensor 
     pinMode(23,OUTPUT); // Pin for RBG Light sensor
     pinMode(ledPower,OUTPUT); // Pin for Dust Sensor
  
     if (tcs.begin()) 
     {
        Serial.println("Found RGB Light sensor");
     }
     else
     {
        Serial.println("No TCS34725 found ... check your connections");
        //while (1);
     }
     digitalWrite(23, LOW); // Pin for RBG Light sensor
  
  
  
  // Serial port for debugging purposes
  Serial.begin(115200);

  randomSeed(42);
  
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
   if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/logged-out", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", logout_html, processor);
  });
  server.on("/logout", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(401);
  });
  // Send a GET request to control output socket <ESP_IP>/output?state=<inputMessage>
  server.on("/output", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    String inputMessage;
    // GET gpio and state value
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
//      digitalWrite(output, inputMessage.toInt());
      request->send(200, "text/plain", "OK");
    }
    request->send(200, "text/plain", "Failed");
  });
  
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis and set reconnect delay to 1 second
    client->send("hello!",NULL,millis(),1000);
  });
  server.addHandler(&events);
  
  // Start server
  server.begin();
}
 
void loop(){
  int temprature_sensor = getTemprature();//random(50,80);
  int humidity_sensor = getHumidity();//(30,90);
  int light_sensor = getLight();//(0,20000);
  int sound_sensor = getSound();//random(1000,2000);

  int score = 100;
  String res = "";
  
  events.send("ping",NULL,millis());
  events.send(String(temprature_sensor).c_str(),"temperature",millis());
  events.send(String(humidity_sensor).c_str(),"humidity",millis());
  events.send(String(light_sensor).c_str(),"light",millis());
  events.send(String(sound_sensor).c_str(),"sound",millis());
  
  if(!(temprature_sensor>=62 && temprature_sensor<=68))
  {
    res+="Not Ideal Temprature for Sleep";
    // 40 to 90, mean(65)
    score -= min(abs(temprature_sensor-65), 25);
    Serial.print(score+" a ");
  }

  if(!(humidity_sensor>=30 && humidity_sensor<=50))
  {
    res+="<br />Not Ideal Humidity for Sleep";
    // 0 to 80, mean (40)
    score -= min(((abs(humidity_sensor-40)*25)/40), 25);
    Serial.print(score+" b ");
  }

  if(!(light_sensor<=200))
  {
    res+="<br />Not Ideal Light for Sleep";
    // max 20,000
    // log(201)=2.3, log(20,000) = 4.3
    score -= min(int((abs(log(light_sensor)-log(20000))*25)/2), 25);
    Serial.print(score+" c ");
  }

  if(!(sound_sensor<=47))
  {
    res+="<br />Not Ideal Sound for Sleep";
    // 47 to 100
    score -= min(((abs(sound_sensor-100)*25)/53), 25);
    Serial.println(score+" d ");
  }

  events.send(String(score).c_str(),"score",millis());

  String lead = "";
  int lt[] = {score, random(40, 100), random(40, 100), random(40, 100)};

  int lt_length = sizeof(lt) / sizeof(lt[0]);
  // qsort - last parameter is a function pointer to the sort function
  qsort(lt, lt_length, sizeof(lt[0]), sort_desc);
  bool flag = false;
  
  for(int i=0;i<4;i++)
  {
    if(!flag && lt[i] == score)
    {
      lead+=String(i+1)+". "+String(lt[i])+"*"+"<br />";
      flag = true;
    }
    else
    {
      lead+=String(i+1)+". "+String(lt[i])+" "+"<br />";
    }
  }
  events.send(lead.c_str(), "leaderboard", millis());
  
  
  events.send(res.c_str(),"result",millis());

  
  delay(5000);
}

int getLight(){
  uint16_t r, g, b, c, colorTemp, lux;

  tcs.getRawData(&r, &g, &b, &c);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);

  return int(lux);
//  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
//  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
//  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
//  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
//  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
//  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
//  Serial.println(" ");
}

int getTemprature(){
//Serial.println(sensor.readHumidity());
  int c = sensor.readHumidity();
  int f = 32 + ((c*9)/5);
  return f;
}

int getHumidity(){
//Serial.println(sensor.readTemperature());
  return int(sensor.readTemperature());
}

int getSound(){
  int sensorValue = analogRead(15);
//  // print out the value you read:
  Serial.print("SOUND = ");
  Serial.println(sensorValue);

  // 0 to 3000
  // 0%, 100%
  
  int sound_per = min((sensorValue*100)/3000, 100);
  return sensorValue;
}

// qsort requires you to create a sort function
int sort_desc(const void *cmp1, const void *cmp2)
{
  // Need to cast the void * to int *
  int a = *((int *)cmp1);
  int b = *((int *)cmp2);
  // The comparison
  return a > b ? -1 : (a < b ? 1 : 0);
  // A simpler, probably faster way:
  //return b - a;
}
