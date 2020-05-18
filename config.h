
#define CLIENT_ID     "meteo_hall" // Must be unique on the MQTT network

// wifi settings сохраняются при первой настройке. Доступ к настройкам по адресу http://192.168.4.1 
//#define WIFI_SSID "wifi_ssid"
//#define WIFI_PASSWORD  "password"

// mqtt server settings
#define MQTT_SERVER    "mqtt_server"
#define MQTT_PORT      1883
#define MQTT_USER      "mqtt_user"
#define MQTT_PASSWORD  "mqtt_pass"

// mqtt client settings
// Note PubSubClient.h has a MQTT_MAX_PACKET_SIZE of 128 defined, so either raise it to 256 or use short topics
#define CONTROL_TOPIC        "home/hall/#"
#define ONLINE_TOPIC         "home/hall/meteo_hall/online"
#define CURR_TEMP            "home/hall/meteo_hall/temperature/state"
#define HUMIDITY             "home/hall/meteo_hall/humidity/state"
#define PRESSURE             "home/hall/meteo_hall/pressure/state"
#define CURR_CO2             "home/hall/meteo_hall/co2/state"
#define METEO_SETTINGS           "home/hall/sensor/meteo/set"
/*  Датчик CO2
 *   
 *   Команды:
 *   home/sensors/co2_test/set    zero       Калибровка 400ppm по текущей концентрации CO2 в воздухе
 *   home/sensors/co2_test/set    abc_off    Выключение автокалибровки
 *   home/sensors/co2_test/set    abc_on     Включение автокалибровки
 *   
 */


// sketch settings
const unsigned int READ_SENSORS_INTERVAL_MS = 60000;
const unsigned int BLINK_INTERVAL_MS = 5000;
