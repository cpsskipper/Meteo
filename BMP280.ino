void bmp_init(){
  if (!bme.begin(BME280_ADDRESS)) {  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
}
