/*
 *  Структура Mqtt Zone9
 *  
 *  home/online - передается имя устройства, вышедшего в сеть
 *  home/info   - информация от устройств не относящаяся к статусу
 *  home/комната/ - локация на объекте, где установлено устройство 
 *  
 *  home/комната/sensor - тип устройства датчик
 *  Sensor имеет свойства:
 *    state - измеряемое значение
 *    measure - ед. измеряемого значения
 *    battary - заряд батареи
 *    set - команда для устройства
 *  
 *  home/комната/actor - тип устройства исполнительный механизм
 *  Actor имеет свойства
 *    state - состояние устройства
 *    battary - заряд батарейки
 *    set - команда устройству
 */




void mqtt_init(){ 
  client.setServer(MQTT_SERVER, MQTT_PORT);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");  
    if (client.connect(CLIENT_ID, MQTT_USER, MQTT_PASSWORD )) {
       Serial.println("connected"); 
    } else { 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000); 
    }
  }
  client.setCallback(callback);
  client.subscribe(CONTROL_TOPIC);
  client.publish("home/online", CLIENT_ID);
  client.publish("home/info", "meteo can control (arguments): temperature, humidity, pressure, CO2");
   
}



void callback(char* topic, byte *payload, unsigned int length) {


    if (String(topic)== METEO_SETTINGS){
      String s="";
        for (int i = 0; i < length; i++) {
            s=s+(char)payload[i];
        }
      // сброс на 400ppm
        if (s=="zero"){
          ZERO_POINT_CALIBRATION();
          client.publish("home/info", "meteo_hall: Zero 400ppm set"); 
        }
      //выключение автокалибровки
        if (s=="abc_off"){
          abc_off();
          client.publish("home/info", "meteo_hall: ABC off"); 
        }   
        //включение автокалибровки
        if (s=="abc_on"){
          abc_on();
          client.publish("home/info", "meteo_hall: ABC on"); 
        }      
    }
}
