#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <Hash.h>

#define BIA D1
#define BIB D2
#define AIA D3
#define AIB D4

#define MOTOR_A 0
#define MOTOR_B 1




#define LED_PIN LED_BUILTIN
#define SSID "Test 01"
#define PASS "759e319018"

#define WEBAPP "<head><meta name='viewport'content='width=device-width,initial-scale=1'></head><style>body {width:100%;height:100%;margin:0px;}</style><canvas id='c'/><script>var b=document.getElementById('c'),c=b.getContext('2d'),d=0,e=0,g=0,h=0,k=0,l=0,m=0,n=0,p=0,q=!1,r=0,t,u=!1;b.addEventListener('mousedown',v,!1);b.addEventListener('touchstart',v,!1);b.addEventListener('mousemove',w,!1);b.addEventListener('touchmove',w,!1);b.addEventListener('mouseup',x,!1);b.addEventListener('touchend',x,!1);y();window.addEventListener('resize',z);z();t=new WebSocket('ws://192.168.4.1:81/');t.onopen=A;t.onerror=C;t.onmessage=D;y(); function A(){console.log('Connected to','ws://192.168.4.1:81/');u=!0;t.send('Hello from Client')}function D(a){console.log('MSG: ',a.data)}function C(a){u=!1;console.log('ERR',a)}function z(){b.width=window.innerWidth;b.height=window.innerHeight;g=b.width;h=b.height;m=b.width/2;n=b.height/2}function v(a){q=!0;void 0!=a.changedTouches&&a.preventDefault();w(a)} function w(a){q&&(void 0!=a.changedTouches&&a.preventDefault(),void 0!=a.changedTouches?(d=a.changedTouches[0].pageX,e=a.changedTouches[0].pageY):(d=a.x,e=a.y))}function x(a){void 0!=a.changedTouches&&a.preventDefault();q=!1} function y(){p=Math.min(g,h)/2*.7;var a=m-d;var f=n-e;a=Math.sqrt(a*a+f*f);a>p?(k=p/a*(d-g/2)+g/2,l=p/a*(e-h/2)+h/2):(k=d,l=e);q||(k=m,l=n);a=(new Date).getTime();100>a-r||(r=a,a=k-g/2,f=l-h/2,a=Math.floor(100*a/p),f=Math.floor(100*f/p),u&&t.send(a+' '+f));c.fillStyle='#E3F2FD';c.fillRect(0,0,g,h);c.strokeStyle='#7986CB';c.beginPath();c.moveTo(g/2,0);c.lineTo(g/2,h-1);c.stroke();c.beginPath();c.moveTo(0,h/2);c.lineTo(g-1,h/2);c.stroke();a=u?'#3F51B5':'#AAAAAA';E(.7,m,n,'#7986CB',!1);E(.3,k,l,a,!0); window.requestAnimationFrame(y)}function E(a,f,F,B,G){c.beginPath();c.arc(f,F,Math.min(g,h)/2*a,0,2*Math.PI);c.lineWidth=3;c.strokeStyle=B;c.fillStyle=B;G?c.fill():c.stroke()};</script>"

ESP8266WebServer server = ESP8266WebServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void moveMotor(int motor, int speed);




void blink(int t, int reps) {
  for (int i = 0; i < reps; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(t);
    digitalWrite(LED_PIN, LOW);
    delay(t);
  }
}

void pene (int x, int y);

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
        pene(x,y);
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
  pinMode(AIA, OUTPUT);
  pinMode(AIB, OUTPUT);
  pinMode(BIA, OUTPUT);
  pinMode(BIB, OUTPUT);

  blink(30, 10);

  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println();

  Serial.print("Setting up WiFi soft-AP ... ");
  boolean result = WiFi.softAP(SSID, PASS);
  if(result == true) Serial.println("Ready");
  else Serial.println("Failed!");

  setupWSServer();
  setupWebServer();
}

//formula distancia en plano cartesiano, siempre comparamos con el origen(0,0)
int distancefromCenter(int x, int y) {
  return sqrt((x*x)+(y*y));
}

//el robot nunca va marcha atras. velocidad siempre positva
void pene(int x, int y)
{
  //check distancia al centro, se puede tambien haer con la funcion
  //distancefromCenter(x,y) pero not worth
  if (x < 50 and x > -50 and y > -50 and y < 50 ) {
    moveMotor(MOTOR_A , 0);
    moveMotor(MOTOR_B , 0);
    //Serial.printf("0");
    return;
  }
  int vel_a = 0;
  int vel_b = 0;
  if (y >= 0) {
    vel_a = (x >= 0)? 100 : (100 + x);
    vel_b = (x >= 0)? (100- x) : 100;
  }
  else {
    vel_a = (x >= 0) ? (100 - x) : 100;
    vel_b = (x >= 0) ? 100 : (100 + x);
  }

  vel_a = map (vel_a, 0, 100, 50, 100);
  vel_b = map (vel_b, 0, 100, 50, 100);

  moveMotor(MOTOR_B, vel_b);
  moveMotor(MOTOR_A, vel_a);

  //Serial.printf("%d %d\n",vel_a,vel_b);
//http://www.impulseadventure.com/elec/robot-differential-steering.html

}

void moveMotor(int motor, int speed) {
  //EL IA y el IB estan asignados de esta manera para que concuerde la
  //direccion con  rueda de atras y puede variar depende como este montado
  //es mas facil cambiar los 4 cables del driver que empezar a trastear el
  //codigo
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
  webSocket.loop();
  server.handleClient();
}
