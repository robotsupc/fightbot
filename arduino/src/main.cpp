#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <Hash.h>



#define LED_PIN LED_BUILTIN
#define SSID "Test 01"
#define PASS "759e319018"

#define WEBAPP "<head><meta name='viewport'content='width=device-width,initial-scale=1'></head><style>body {width:100%;height:100%;margin:0px;}</style><canvas id='c'/><script>var BACKGROUND='#E3F2FD',OUTER_CIRCLE='#7986CB',INNER_CIRCLE='#3F51B5',PRESSED_CIRCLE='#303F9F',CIRCLE_SIZE=.7,SEND_INTERVAL=300,canvas=document.getElementById('c'),ctx=canvas.getContext('2d'),mouseX=0,mouseY=0,width=0,height=0,joyX=0,joyY=0,cx=0,cy=0,mouseDown=!1,lastSendTime=0;init(); function init(){canvas.addEventListener('mousedown',mousedown,!1);canvas.addEventListener('touchstart',mousedown,!1);canvas.addEventListener('mousemove',mousemove,!1);canvas.addEventListener('touchmove',mousemove,!1);canvas.addEventListener('mouseup',mouseup,!1);canvas.addEventListener('touchend',mouseup,!1);process();window.addEventListener('resize',onresize);onresize();process()} function onresize(){canvas.width=window.innerWidth;canvas.height=window.innerHeight;width=canvas.width;height=canvas.height;cx=canvas.width/2;cy=canvas.height/2}function mousedown(a){mouseDown=!0;void 0!=a.changedTouches&&a.preventDefault();mousemove(a)}function mousemove(a){mouseDown&&(void 0!=a.changedTouches&&a.preventDefault(),void 0!=a.changedTouches?(mouseX=a.changedTouches[0].pageX,mouseY=a.changedTouches[0].pageY):(mouseX=a.x,mouseY=a.y))} function mouseup(a){void 0!=a.changedTouches&&a.preventDefault();mouseDown=!1}function process(){processJoystick();sendToServer();draw();window.requestAnimationFrame(process)}function sendToServer(){var a=(new Date).getTime();a-lastSendTime<SEND_INTERVAL||(lastSendTime=a)} function processJoystick(){var a=Math.min(width,height)/2*CIRCLE_SIZE,b=dst(mouseX,mouseY,cx,cy);b>a?(joyX=a/b*(mouseX-width/2)+width/2,joyY=a/b*(mouseY-height/2)+height/2):(joyX=mouseX,joyY=mouseY);mouseDown||(joyX=cx,joyY=cy)} function draw(){ctx.fillStyle=BACKGROUND;ctx.fillRect(0,0,width,height);ctx.strokeStyle=OUTER_CIRCLE;ctx.beginPath();ctx.moveTo(width/2,0);ctx.lineTo(width/2,height-1);ctx.stroke();ctx.beginPath();ctx.moveTo(0,height/2);ctx.lineTo(width-1,height/2);ctx.stroke();drawCircle(CIRCLE_SIZE,cx,cy,OUTER_CIRCLE,!1);drawCircle(.3,joyX,joyY,INNER_CIRCLE,!0)} function drawCircle(a,b,d,c,e){ctx.beginPath();ctx.arc(b,d,Math.min(width,height)/2*a,0,2*Math.PI);ctx.lineWidth=3;ctx.strokeStyle=c;ctx.fillStyle=c;e?ctx.fill():ctx.stroke()}function dst(a,b,d,c){a=d-a;b=c-b;return Math.sqrt(a*a+b*b)};</script>"


ESP8266WebServer server = ESP8266WebServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void blink(int t, int reps) {
  for (int i = 0; i < reps; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(t);
    digitalWrite(LED_PIN, LOW);
    delay(t);
  }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
    case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", num);
        break;
    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocket.sendTXT(num, "Connected");
        break;
    }
    case WStype_TEXT:
        Serial.printf("[%u] get Text: %s\n", num, payload);
        break;
    }

}

void setupWebServer() {
  server.on("/", []() {
        // send index.html
        server.send(200, "text/html", WEBAPP);
  });
  server.begin();
}

void setupWSServer() {
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  blink(30, 10);

  Serial.begin(115200);
  Serial.println();

  Serial.print("Setting up WiFi soft-AP ... ");
  boolean result = WiFi.softAP(SSID, PASS);
  if(result == true) Serial.println("Ready");
  else Serial.println("Failed!");

  setupWSServer();
  setupWebServer();
}

void loop() {
  Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
  webSocket.loop();
  server.handleClient();
  delay(1000);
}
