#include <ESP8266WiFi.h>
#include <IFTTTWebhook.h>

#define wlan_ssid "SSID"
#define wlan_pass "WLAN-Password"
#define myKey "IFTTT-Key"
#define brief "Briefmelder"
#define host "maker.ifttt.com"
int analogValue = 0;
float voltage = 0;
WiFiClient client;


void setup()
{
  pinMode(A0, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(9600);
  WiFi.begin(wlan_ssid, wlan_pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Wlan verbunden mit der IP:");
  Serial.print(WiFi.localIP());
  //IFTTTWebhook breifmelder(myKey, brief);
  //breifmelder.trigger();
  client.connect(host, 80);
  String url = "/trigger/Briefmelder/with/key/";
  url+=myKey;
  client.print(String("GET ") + url +" HTTP/1.1\r\n" + "Host: " + host "\r\n" + "Connection: close\r\n" + "\r\n");
  while (client.connected() || client.available())
{
  if (client.available())
  {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }
}  
  


}

void loop(){
  
  analogValue = analogRead(A0);
  Serial.print(analogValue);
  voltage = analogValue/1023;
  voltage = 4.2*voltage;
  Serial.print(voltage);
  delay(500);
  if (voltage <= 3.9)
  {
    //IFTTTWebhook battery(myKey, "battery_melder");
    //battery.trigger();
    client.connect(host, 80);
    String url = "/trigger/Battery_Melder/with/key/";
    url+=myKey;
    client.print(String("GET ") + url +" HTTP/1.1\r\n" + "Host: " + host "\r\n" + "Connection: close\r\n" + "\r\n");
    
    while (client.connected() || client.available())
    {
      if (client.available())
      {
        String line = client.readStringUntil('\n');
        Serial.println(line);
      }
    }  

    
    delay(5000);
  }
  digitalWrite(BUILTIN_LED, HIGH);
  ESP.deepSleep(0);
  
  

}
