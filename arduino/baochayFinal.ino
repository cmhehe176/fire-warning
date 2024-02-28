#include <DHT.h>
#include <Wire.h> 
 #include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <FirebaseESP32.h>

#define WIFI_SSID "Pangliang"
#define WIFI_PASSWORD "11111111"

#define FIREBASE_HOST "https://iotapp-fb613-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "LRu9RGhS0s3S0KHls18lExTyBmUzXefS9CeiwnR7"

#define flameSensor 4
#define buzzer 19
#define MQ2_ADC 32
#define MQ2 14


#define DHT11PIN 16

DHT dht(DHT11PIN, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);
FirebaseData firebaseData;

void setup_wifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("connecting...");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }
  Serial.print("IP WiFi: ");
  Serial.println(WiFi.localIP()); 
}

void setup()
{
  /* Start the DHT11 Sensor */
  Serial.begin(115200);
  setup_wifi();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  dht.begin();
  lcd.init();
  lcd.backlight();
  delay(1000);
  lcd.clear();

  pinMode(flameSensor,INPUT);
  pinMode(buzzer,OUTPUT);
  
  pinMode(MQ2, INPUT);
  pinMode(MQ2_ADC, INPUT);
  
}

void loop()
{
  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Nhiet do = ");
  // lcd.write(0);
  lcd.setCursor(10,0);
  lcd.print(temp);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Do am = ");
  lcd.setCursor(10,1);
  lcd.print(humi);
  lcd.print("%");
  delay(1000);
  Serial.print("Do am: ");
  Serial.println(humi);
  Serial.print("Nhiet do: ");
  Serial.println(temp);
  float sensorMq2 = analogRead(MQ2_ADC);
  
  int sensorVal = digitalRead(flameSensor);
  int setdataSensorVal= !sensorVal;
  float setDataGas;
  Serial.println(sensorVal);
  if (sensorVal == LOW) {
      Serial.println("co chay");
      digitalWrite(buzzer,HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Warning!!!");
      lcd.setCursor(0,1);
      lcd.print("CHAY");
      delay(1000);
    } else {
      Serial.println("khong chay");
      digitalWrite(buzzer,LOW);

      
    }
  Serial.println(sensorMq2);
  if (sensorMq2 > 2200) {
      Serial.println("co khi gas");
      setDataGas = 1;
      digitalWrite(buzzer,HIGH);

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Warning!!!");
      lcd.setCursor(0,1);
      lcd.print("KHI GAS");
      delay(1000);

      
    } else {
      setDataGas = 0;
      digitalWrite(buzzer,LOW);
    }
  

  Firebase.setFloat(firebaseData,"baochay/humi", humi );
  
  Firebase.setFloat(firebaseData,"baochay/temp", temp );
  
  Firebase.setFloat(firebaseData,"baochay/flame", setdataSensorVal );

  Firebase.setFloat(firebaseData,"baochay/gas", setDataGas );

}