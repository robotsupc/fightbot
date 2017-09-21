#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

#include <Hash.h>
#include <robot3.h>


#define MOTOR_MIN_SPEED 768
#define MOTOR_MOVE_THRESHOLD 128

#define AIA D2
#define AIB D1
#define BIA D3
#define BIB D4

#define MOTOR_A 0
#define MOTOR_B 1




#define LED_PIN LED_BUILTIN

#define WEBAPP "<head><meta name='viewport'content='width=device-width,initial-scale=1'></head><style>body {width:100%;height:100%;margin:0px;}</style><canvas id='c'/><script>var b=document.getElementById('c'),c=b.getContext('2d'),d=0,e=0,g=0,h=0,k=0,l=0,m=0,n=0,p=0,q=!1,r=0,t,u=!1;b.addEventListener('mousedown',v,!1);b.addEventListener('touchstart',v,!1);b.addEventListener('mousemove',w,!1);b.addEventListener('touchmove',w,!1);b.addEventListener('mouseup',x,!1);b.addEventListener('touchend',x,!1);y();window.addEventListener('resize',z);z();t=new WebSocket('ws://192.168.4.1:81/');t.onopen=A;t.onerror=C;t.onmessage=D;y(); function A(){console.log('Connected to','ws://192.168.4.1:81/');u=!0;t.send('Hello from Client')}function D(a){console.log('MSG: ',a.data)}function C(a){u=!1;console.log('ERR',a)}function z(){b.width=window.innerWidth;b.height=window.innerHeight;g=b.width;h=b.height;m=b.width/2;n=b.height/2}function v(a){q=!0;void 0!=a.changedTouches&&a.preventDefault();w(a)} function w(a){q&&(void 0!=a.changedTouches&&a.preventDefault(),void 0!=a.changedTouches?(d=a.changedTouches[0].pageX,e=a.changedTouches[0].pageY):(d=a.x,e=a.y))}function x(a){void 0!=a.changedTouches&&a.preventDefault();q=!1} function y(){p=Math.min(g,h)/2*.7;var a=m-d;var f=n-e;a=Math.sqrt(a*a+f*f);a>p?(k=p/a*(d-g/2)+g/2,l=p/a*(e-h/2)+h/2):(k=d,l=e);q||(k=m,l=n);a=(new Date).getTime();100>a-r||(r=a,a=k-g/2,f=l-h/2,a=Math.floor(100*a/p),f=Math.floor(100*f/p),u&&t.send(a+' '+f));c.fillStyle='#E3F2FD';c.fillRect(0,0,g,h);c.strokeStyle='#7986CB';c.beginPath();c.moveTo(g/2,0);c.lineTo(g/2,h-1);c.stroke();c.beginPath();c.moveTo(0,h/2);c.lineTo(g-1,h/2);c.stroke();a=u?'#3F51B5':'#AAAAAA';E(.7,m,n,'#7986CB',!1);E(.3,k,l,a,!0); window.requestAnimationFrame(y)}function E(a,f,F,B,G){c.beginPath();c.arc(f,F,Math.min(g,h)/2*a,0,2*Math.PI);c.lineWidth=3;c.strokeStyle=B;c.fillStyle=B;G?c.fill():c.stroke()};</script>"

ESP8266WebServer server = ESP8266WebServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);

IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;

void moveMotor(int motor, int speed);




void blink(int t, int reps) {
  for (int i = 0; i < reps; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(t);
    digitalWrite(LED_PIN, LOW);
    delay(t);
  }
}

void processJoystick (int x, int y);

// https://stackoverflow.com/questions/29671455/how-to-split-a-string-using-a-specific-delimiter-in-arduino
String substr(String data, char separator, int index){
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
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
        webSocket.sendTXT(num, "Hello from Server");
        break;
    }
    case WStype_TEXT:
        //Serial.printf("MSG: %s\n", payload);
        String s = (char*) payload;
        int x = substr(s, ' ', 0).toInt();
        int y = substr(s, ' ', 1).toInt();
        processJoystick(x, y);
        break;
    }

}

void handleRoot() {
    server.send(200, "text/html", WEBAPP);
}

void setupWebServer() {
  server.on("/", handleRoot);
  server.on("/generate_204", handleRoot);
  server.on("/fwlink", handleRoot);
  server.onNotFound(handleRoot);
  server.begin();
}

void setupWSServer() {
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void setupDNS() {
  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(53, "*", apIP);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(AIA, OUTPUT);
  pinMode(AIB, OUTPUT);
  pinMode(BIA, OUTPUT);
  pinMode(BIB, OUTPUT);
  //digitalWrite(AIA, HIGH);
  //digitalWrite(AIB, LOW);


  setupDNS();

  blink(30, 10);

  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println();

  Serial.print("Setting up WiFi soft-AP ... ");
  bool result = WiFi.softAP(SSID, PASS, CHAN, 0);
  if(result == true) Serial.println("Ready");
  else Serial.println("Failed!");

  setupWSServer();
  setupWebServer();

//  moveMotor(MOTOR_A, 100);
//  moveMotor(MOTOR_B, -100);
}

//formula distancia en plano cartesiano, siempre comparamos con el origen(0,0)
int distancefromCenter(int x, int y) {
  return sqrt((x*x)+(y*y));
}

void processJoystick(int x, int y) {
  //http://www.impulseadventure.com/elec/robot-differential-steering.html
  // Differential Steering Joystick Algorithm
  // Converts a single dual-axis joystick into a differential
  // drive motor control, with support for both drive, turn
  // and pivot operations.
  //

  // INPUTS
  int     nJoyX = x;              // Joystick X input                     (-128..+127)
  int     nJoyY = y;              // Joystick Y input                     (-128..+127)

  // OUTPUTS
  int     nMotMixL;           // Motor (left)  mixed output           (-128..+127)
  int     nMotMixR;           // Motor (right) mixed output           (-128..+127)

  // CONFIG
  // - fPivYLimt  : The threshold at which the pivot action starts
  //                This threshold is measured in units on the Y-axis
  //                away from the X-axis (Y=0). A greater value will assign
  //                more of the joystick's range to pivot actions.
  //                Allowable range: (0..+127)
  float fPivYLimit = 32.0;

  // TEMP VARIABLES
  float   nMotPremixL;    // Motor (left)  premixed output        (-128..+127)
  float   nMotPremixR;    // Motor (right) premixed output        (-128..+127)
  int     nPivSpeed;      // Pivot Speed                          (-128..+127)
  float   fPivScale;      // Balance scale b/w drive and pivot    (   0..1   )


  // Calculate Drive Turn output due to Joystick X input
  if (nJoyY >= 0) {
    // Forward
    nMotPremixL = (nJoyX>=0)? 99.0 : (99.0 + nJoyX);
    nMotPremixR = (nJoyX>=0)? (99.0 - nJoyX) : 99.0;
  } else {
    // Reverse
    nMotPremixL = (nJoyX>=0)? (99.0 - nJoyX) : 99.0;
    nMotPremixR = (nJoyX>=0)? 99.0 : (99.0 + nJoyX);
  }

  //  A PARTIR DE AQUI SE PUEDEN ELIMINAR COSAS, NO SON TAMPOCO LAS ENTIENDO
  //  AL 100%
  // Scale Drive output due to Joystick Y input (throttle)
  nMotPremixL = nMotPremixL * nJoyY/100.0;
  nMotPremixR = nMotPremixR * nJoyY/100.0;

  // Now calculate pivot amount
  // - Strength of pivot (nPivSpeed) based on Joystick X input
  // - Blending of pivot vs drive (fPivScale) based on Joystick Y input
  nPivSpeed = nJoyX;
  fPivScale = (abs(nJoyY)>fPivYLimit)? 0.0 : (1.0 - abs(nJoyY)/fPivYLimit);

  // Calculate final mix of Drive and Pivot
  nMotMixL = (1.0-fPivScale)*nMotPremixL + fPivScale*( nPivSpeed);
  nMotMixR = (1.0-fPivScale)*nMotPremixR + fPivScale*(-nPivSpeed);

  // Convert to Motor PWM range
  if (nMotMixL > MOTOR_MOVE_THRESHOLD)
    nMotMixL = map(nMotMixL, 0, 1023, MOTOR_MIN_SPEED, 1023);
  if (nMotMixR > MOTOR_MOVE_THRESHOLD)
    nMotMixR = map(nMotMixR, 0, 1023, MOTOR_MIN_SPEED, 1023);

  moveMotor(MOTOR_A, nMotMixL);
  moveMotor(MOTOR_B, nMotMixR);
}

void moveMotor(int motor, int speed) {
  //EL IA y el IB estan asignados de esta manera para que concuerde la
  //direccion con  rueda de atras y
  int low, pwm;
  if (motor) {
    low = AIB;
    pwm = AIA;
  }
  else {
    low = BIB;
    pwm = BIA;
  }
  if (speed < 0) {
    int tmp = low;
    low = pwm;
    pwm = tmp;
    speed = -speed;
  }


  int vel = map(speed, 0, 100, 0, 1023);

  Serial.print("VEL: ");
  Serial.println(vel);

  //digitalWrite(low, LOW);
  //digitalWrite(pwm, HIGH);
  analogWrite(low, 0);
  analogWrite(pwm, vel);
}

void loop() {
  //Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());
  dnsServer.processNextRequest();
  webSocket.loop();
  server.handleClient();
}
