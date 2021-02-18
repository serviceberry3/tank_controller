//This and the html code are sent to the web browser when a request is received. 
String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";

String html_1 = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'/>
    <meta charset='utf-8'>
    <style>
      body    { font-size:120%;} 
      #main   { display: table; width: 300px; margin: auto;  padding: 10px 10px 10px 10px; 
      border: 3px solid blue; border-radius: 10px; text-align:center;} 
      .button { width:200px; height:40px; font-size: 110%;  }
    </style>
    <title>Websockets</title>
  </head>
  <body>
    <div id='main'>
      <h3>TANK CONTROL</h3>
      <div id='content'>
        <p id='LED_status'>Use a direction and then 'stop' to move</p>
        <button id='BTN_FWD' class="button">Forward</button>
        <button id='BTN_BKD' class="button">Backward</button>
        <button id='BTN_LT' class="button">Left</button>
        <button id='BTN_RT' class="button">Right</button>
        <button id='BTN_STP' class="button">STOP</button>
      </div>
      <br/>
    </div>
  </body>
   
  <script>
    var Socket;
    function init() 
    {
      Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
    }
   
    document.getElementById('BTN_FWD').addEventListener('click', function() { command('0'); });
    document.getElementById('BTN_BKD').addEventListener('click', function() { command('1'); });
    document.getElementById('BTN_LT').addEventListener('click', function() { command('2'); });
    document.getElementById('BTN_RT').addEventListener('click', function() { command('3'); });
    document.getElementById('BTN_STP').addEventListener('click', function() { command('4'); });
    
    
    function forward()
    {   
      /*
      var btn = document.getElementById('BTN_LED')
      var btnText = btn.textContent || btn.innerText;
      
      if (btnText ==='Turn on the LED') 
      { 
        btn.innerHTML = "Turn off the LED"; 
        document.getElementById('LED_status').innerHTML = 'LED is on';  
        sendText('1'); 
      }  
      else { 
        btn.innerHTML = "Turn on the LED";  
        document.getElementById('LED_status').innerHTML = 'LED is off'; 
        sendText('0'); 
      }*/
      sendText('0');
    }

    function command(data) {
      sendText(data);
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

//instantiate a Wifi server on port 80 (HTTP). It responds to clients (in this case - web browsers) on port 80
WiFiServer server(80);

//run websocket server on port 81
WebSocketsServer webSocket = WebSocketsServer(81);
 
int motor1pin1 = 4;
int motor1pin2 = 5;
 
char ssid[] = "Rangers27";  // use your own network ssid and password
char pass[] = "Yankees27";
 
void setup()
{
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
 
  Serial.begin(115200);
  //Serial.println();
  //Serial.println("Serial started at 115200");
  //Serial.println();
 
  // Connect to a WiFi network
  //Serial.print(F("Connecting to "));  
  //Serial.println(ssid);
  WiFi.begin(ssid, pass);
 
  // connection with timeout
  int count = 0; 
  while ( (WiFi.status() != WL_CONNECTED) && count < 34) 
  {
      //print loading symbol
      //Serial.print(".");  
      delay(500);  
      count++;
  }

  //if connection can't be est in ~16 sec, time out and enter infinite loop
  if (WiFi.status() != WL_CONNECTED)
  { 
     //Serial.println(""); 
     //Serial.print("Failed to connect to ");  
     //Serial.println(ssid);
     while(1);
  }

  //otherwise we connected successfully
  //Serial.println("");
  //Serial.println(F("[CONNECTED]"));   
  //Serial.print("[IP ");  
  //Serial.print(WiFi.localIP()); 
  //Serial.println("]");
 
  //start a Wifi server
  server.begin();
  //Serial.println("WifiServer started");

  //start the websocket server, listening for events
  webSocket.begin();

  //onEvent() fires when anything happens w/websocket server
  //event can be: a connection, disconnection, error, data is received, etc. 
  webSocket.onEvent(webSocketEvent);
}
 
void loop()
{
    //run code for the WebSocket
    webSocket.loop();

    //check if a client has connected
    WiFiClient client = server.available();   

    //if no client, return from the program
    if (!client) 
      return;

    //otherwise, we have a new client sending some request.

    client.flush();
    client.print(header);
    client.print(html_1); 
    //Serial.println("New page served");
 
    delay(5);
}

/*onEvent calls another function called webSocketEvent
* @param num - current client/cnnxn number/ID, arduinoWebsocket lib allows max of 5 at a time
* @param type - the response type (WStype_ERROR, WStype_TEXT, etc.)
* @param payload - the received data, if applicable
* @param length - size of the data
*/
void webSocketEvent(byte num, WStype_t type, uint8_t* payload, size_t length)
{
  //text is one of the event types. If of this type, we know we received some ASCII txt data
  if (type == WStype_TEXT)
  {
      //relay the first element of the payload to the Arduino
      Serial.write(payload[0]);
  }

  //event is not text, display details in Serial monitor
  else 
  {
    //Serial.print("WStype = ");   Serial.println(type);  
    //Serial.print("WS payload = ");
    
    //since payload is not a char array but just a uint8_t pointer to the data, we need to typecast to char
    for (int i = 0; i < length; i++) 
    { 
      //Serial.print((char) payload[i]); 
    }
    //Serial.println();
  }
}
