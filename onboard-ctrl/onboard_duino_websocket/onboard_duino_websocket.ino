

/*
 * Sketch: ESP8266_Part9_01_Websocket_LED
 * Intended to be run on an ESP8266
 */
 
String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
 
String html_1 = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta name='viewport' content='width=device-width, initial-scale=1.0'/>
  <meta charset='utf-8'>
  <style>
    body    { font-size:120%;} 
    #main   { display: table; width: 300px; margin: auto;  padding: 10px 10px 10px 10px; border: 3px solid blue; border-radius: 10px; text-align:center;} 
    .button { width:200px; height:40px; font-size: 110%;  }
  </style>
  <title>Websockets</title>
</head>
<body>
  <div id='main'>
    <h3>LED CONTROL</h3>
    <div id='content'>
      <p id='LED_status'>LED is off</p>
      <button id='BTN_LED'class="button">Turn on the LED</button>
    </div>
    <br />
   </div>
</body>
 
<script>
  var Socket;
  function init() 
  {
    Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
  }
 
  document.getElementById('BTN_LED').addEventListener('click', buttonClicked);
  function buttonClicked()
  {   
    var btn = document.getElementById('BTN_LED')
    var btnText = btn.textContent || btn.innerText;
    if (btnText ==='Turn on the LED') { btn.innerHTML = "Turn off the LED"; document.getElementById('LED_status').innerHTML = 'LED is on';  sendText('1'); }  
    else                              { btn.innerHTML = "Turn on the LED";  document.getElementById('LED_status').innerHTML = 'LED is off'; sendText('0'); }
  }
 
  function sendText(data)
  {
    Socket.send(data);
  }
 
  window.onload = function(e)
  { 
    init();
  }
</script>
</html>
)=====";
 
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
 
WiFiServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
 
int pin_led = 0;
 
char ssid[] = "ssid";  // use your own network ssid and password
char pass[] = "pass";
 
void setup()
{
  pinMode(pin_led, OUTPUT);
  digitalWrite(pin_led,LOW);
 
  Serial.begin(115200);
  Serial.println();
  Serial.println("Serial started at 115200");
  Serial.println();
 
  // Connect to a WiFi network
  Serial.print(F("Connecting to "));  Serial.println(ssid);
  WiFi.begin(ssid,pass);
 
  // connection with timeout
  int count = 0; 
  while ( (WiFi.status() != WL_CONNECTED) && count < 17) 
  {
      Serial.print(".");  delay(500);  count++;
  }
 
  if (WiFi.status() != WL_CONNECTED)
  { 
     Serial.println("");  Serial.print("Failed to connect to ");  Serial.println(ssid);
     while(1);
  }
 
  Serial.println("");
  Serial.println(F("[CONNECTED]"));   Serial.print("[IP ");  Serial.print(WiFi.localIP()); 
  Serial.println("]");
 
  // start a server
  server.begin();
  Serial.println("Server started");
 
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}
 
void loop()
{
    webSocket.loop();
 
    WiFiClient client = server.available();     // Check if a client has connected
    if (!client)  {  return;  }
 
    client.flush();
    client.print( header );
    client.print( html_1 ); 
    Serial.println("New page served");
 
    delay(5);
}
 
void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length)
{
  if(type == WStype_TEXT)
  {
      if (payload[0] == '0')
      {
          digitalWrite(pin_led, LOW);
          Serial.println("LED=off");        
      }
      else if (payload[0] == '1')
      {
          digitalWrite(pin_led, HIGH);
          Serial.println("LED=on");        
      }
  }
 
  else 
  {
    Serial.print("WStype = ");   Serial.println(type);  
    Serial.print("WS payload = ");
    for(int i = 0; i < length; i++) { Serial.print((char) payload[i]); }
    Serial.println();
 
  }
}
