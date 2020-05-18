
 


#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiManager.h> 
#include <Adafruit_BME280.h>
#include <PubSubClient.h>
#include "Adafruit_NeoPixel.h"
#include <ArduinoOTA.h>

#include "config.h"


//-------------------------------------------------------LED

// Указываем, какое количество пикселей у нашей ленты.
#define LED_COUNT 2
 
// Указываем, к какому порту подключен вход ленты DIN.
#define LED_PIN 14
// Создаем переменную strip для управления нашей лентой.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

float blink_CO2; 
 

//-------------------------------------------------------MH-Z19
//SoftwareSerial myserial(12,13); //RX,TX
unsigned char response[9];
byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
  byte zerocalib[9] = {0xFF, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78};
  byte spancalib[9] = {0xFF, 0x01, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00,0x77};
  byte autocalib_on[9] = {0xFF, 0x01, 0x79, 0xA0, 0x00, 0x00, 0x00, 0x00, 0xE6};
  byte autocalib_off[9] = {0xFF, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86};

int rst = 0;


//----------------------------------------------------------MQTT
WiFiClient espClient;
PubSubClient client(espClient);



//-------------------------------------------------------BME280 Давление и температура
#define BME280_ADDRESS 0x76
Adafruit_BME280 bme; // I2C
//поправочный коэффициент температуры
float koeff = 0.78;

// -------------------------------------------------------Время работы для прерывания loop
unsigned long times_1min =0;
unsigned long times_led =0;

void setup() {
    Serial.begin(9600);
  //  myserial.begin(9600);
    
    WiFiManager wifiManager;
    // доступ к настройкам по адресу http://192.168.4.1 
    wifiManager.autoConnect(CLIENT_ID);

ota_init();
Serial.println("OTA Ready");

mqtt_init();
Serial.println("MQTT Ready");


bmp_init();
Serial.println("BMP Ready");  

  // Инициализируем ленту.
strip.begin();

setLedColor(255,255,255);
setLedColor(0,0,0);
Serial.println("LED Ready");  
  
//delay(20000);

}

void loop() {

 ArduinoOTA.handle();

    if (!client.connected()) {
    Serial.println("Attempting to reconnect MQTT...");
    mqtt_init();
  }

 //Обработка MQTT
 client.loop();
 
  // Считывание сенсоров один раз в минуту
if ((millis()- times_1min >READ_SENSORS_INTERVAL_MS)|| (times_1min>millis())){
    times_1min = millis();
    //Отправка показаний
    send_sensors();
}

  // Мигание светодиодом
if ((millis()- times_led >BLINK_INTERVAL_MS)|| (times_led>millis())){
    times_led = millis();
    //Отправка показаний
    blink_led(blink_CO2);
}

delay(1000);

}



void send_sensors(){
  
char result[8]; // Buffer big enough for 7-character float
float f = 0;

//----------------------------------------------------------KeepAlive

client.publish(ONLINE_TOPIC, "ON"); 

//-----------------------------------------------------------Температура

      f = bme.readTemperature()*koeff;
      dtostrf(f, 6, 2, result); // Leave room for too large numbers!
      client.publish(CURR_TEMP, result); 
      delay(2000);

//------------------------------------------------------------Влажность
      dtostrf(bme.readHumidity(), 6, 2, result); // Leave room for too large numbers!
      client.publish(HUMIDITY, result); 
      delay(2000);
          
//-------------------------------------------------------------Давление
      dtostrf(bme.readPressure()/133.32239F, 6, 2, result); // Leave room for too large numbers!
      client.publish(PRESSURE, result); 
      delay(2000);

//--------------------------------------------------------------CO2
f =-1;
f = read_sensor_mh_z19();
if (f== -1){
  Serial.println("CO2 error");

  for (int i=0; i<30; i++){
   f = read_sensor_mh_z19();
   if (f != -1){
    break; 
   }
   delay(2000);
  }
}
if (f!= -1){
      dtostrf(f, 6, 2, result); // Leave room for too large numbers!
      client.publish(CURR_CO2, result);

}
    blink_CO2 = f;

}
