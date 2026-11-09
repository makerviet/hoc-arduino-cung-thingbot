/*
 * Chuong 18 - Dieu khien robot qua Wi-Fi bang trinh duyet dien thoai
 * ThingBot tu phat Wi-Fi (Access Point). Dien thoai ket noi vao mang:
 *     Ten : ThingBot-xxxx   (xxxx lay tu dia chi MAC, xem Serial Monitor)
 *     Mat khau: thingbot123
 * Roi mo trinh duyet vao dia chi http://192.168.4.1
 * Trang web co can dieu khien ao (joystick) va nut coi.
 * An toan: neu 500 ms khong nhan lenh -> robot tu dung.
 */
#include <ThingBot.h>
#include <WiFi.h>
#include <WebServer.h>

const char *AP_PASS = "thingbot123";  // toi thieu 8 ky tu
WebServer server(80);
unsigned long lastCmd = 0;

const char PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no">
<title>ThingBot</title>
<style>
 body{margin:0;font-family:sans-serif;background:#10202c;color:#fff;text-align:center;touch-action:none;user-select:none}
 h1{font-size:22px;margin:16px 0 4px}
 #pad{width:260px;height:260px;margin:20px auto;border-radius:50%;background:#1d3a4f;position:relative;border:3px solid #42ccff}
 #knob{width:90px;height:90px;border-radius:50%;background:#42ccff;position:absolute;left:85px;top:85px}
 button{font-size:20px;padding:14px 36px;border:0;border-radius:12px;background:#ff5a5a;color:#fff}
 #info{opacity:.7;margin:8px}
</style></head><body>
<h1>ThingBot Wi-Fi</h1><div id="info">L 0 | R 0</div>
<div id="pad"><div id="knob"></div></div>
<button id="horn">COI</button>
<script>
const pad=document.getElementById('pad'),knob=document.getElementById('knob'),info=document.getElementById('info');
let x=0,y=0,active=false;
function setKnob(){knob.style.left=(85+x*85)+'px';knob.style.top=(85-y*85)+'px';}
function move(e){const r=pad.getBoundingClientRect();
 let dx=(e.clientX-r.left-r.width/2)/(r.width/2),dy=-(e.clientY-r.top-r.height/2)/(r.height/2);
 const m=Math.hypot(dx,dy);if(m>1){dx/=m;dy/=m;}x=dx;y=dy;setKnob();}
pad.addEventListener('pointerdown',e=>{active=true;pad.setPointerCapture(e.pointerId);move(e);});
pad.addEventListener('pointermove',e=>{if(active)move(e);});
pad.addEventListener('pointerup',()=>{active=false;x=0;y=0;setKnob();});
setInterval(()=>{
 const t=Math.round(y*100),s=Math.round(x*100);
 const l=Math.max(-100,Math.min(100,t+s)),r=Math.max(-100,Math.min(100,t-s));
 info.textContent='L '+l+' | R '+r;
 fetch('/drive?l='+l+'&r='+r).catch(()=>{});
},100);
const horn=document.getElementById('horn');
horn.addEventListener('pointerdown',()=>fetch('/horn?on=1'));
horn.addEventListener('pointerup',()=>fetch('/horn?on=0'));
</script></body></html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", PAGE);
}

void handleDrive() {
  int l = server.arg("l").toInt();
  int r = server.arg("r").toInt();
  ThingBot.drive(l, r);
  lastCmd = millis();
  server.send(200, "text/plain", "OK");
}

void handleHorn() {
  ThingBot.pwm.setPin(TB_BUZZER, server.arg("on") == "1" ? 2048 : 0);
  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);
  ThingBot.begin();

  uint8_t mac[6];
  WiFi.mode(WIFI_AP);
  WiFi.softAPmacAddress(mac);
  char ssid[20];
  snprintf(ssid, sizeof(ssid), "ThingBot-%02X%02X", mac[4], mac[5]);
  WiFi.softAP(ssid, AP_PASS);

  Serial.printf("Wi-Fi: %s  /  mat khau: %s\n", ssid, AP_PASS);
  Serial.print("Mo trinh duyet: http://");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/drive", handleDrive);
  server.on("/horn", handleHorn);
  server.begin();
  ThingBot.beep();
}

void loop() {
  server.handleClient();

  bool connected = millis() - lastCmd < 500;
  if (!connected) ThingBot.stop();  // mat ket noi -> dung
  ThingBot.led(1, connected ? 100 : 0);
  ThingBot.led(2, WiFi.softAPgetStationNum() > 0 ? 100 : 0);
}
