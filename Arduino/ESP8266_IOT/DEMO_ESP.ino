#include <ESP8266WiFi.h>
#include <SocketIOClient.h>
#include <ArduinoJson.h>
#include "DHTesp.h"
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

SocketIOClient client;

const char* ssid = "TMT_2.4Ghz";        //Tên mạng Wifi mà Socket server của bạn đang kết nối
const char* password = "63689098";      //Pass mạng wifi ahihi, anh em rãnh thì share pass cho mình với.
 
char host[] = "afternoon-earth-41665.herokuapp.com";          //Địa chỉ IP dịch vụ, hãy thay đổi nó theo địa chỉ IP Socket server của bạn.
int port = 80;                        //Cổng dịch vụ socket server do chúng ta tạo!
char namespace_esp8266[] = "esp8266";   //Thêm Arduino!
 
//từ khóa extern: dùng để #include các biến toàn cục ở một số thư viện khác. Trong thư viện SocketIOClient có hai biến toàn cục
// mà chúng ta cần quan tâm đó là
// RID: Tên hàm (tên sự kiện)
// Rfull: Danh sách biến (được đóng gói lại là chuỗi JSON)

extern String RID;
extern String Rfull;

const byte SERVO = D5;
// D1, D2 : LCD
const byte DHT_SENSOR = D6;
const byte RAIN_SENSOR = D7;

//const byte RELAY_1 = D2;
//const byte RELAY_2 = D5;
//const byte RELAY_3 = D6;
//const byte RELAY_4 = D7;

bool relayStatus_A_std = false;
bool relayStatus_B_std = false;
bool relayStatus_C_std = false;
bool relayStatus_D_std = false;

Servo servo;
LiquidCrystal_I2C lcd(0x27,16,2); //Khai báo LCD
DHTesp dht;

void setup()
{
//    pinMode(RELAY_1, OUTPUT);
//    pinMode(RELAY_2, OUTPUT);
//    pinMode(RELAY_3, OUTPUT);
//    pinMode(RELAY_4, OUTPUT);
    pinMode(RAIN_SENSOR, INPUT);
    
//
//    digitalWrite(RELAY_1,HIGH);
//    digitalWrite(RELAY_2,HIGH);
//    digitalWrite(RELAY_3,HIGH);
//    digitalWrite(RELAY_4,HIGH);
    
    servo.attach(SERVO);
    dht.setup(DHT_SENSOR, DHTesp::DHT11);
    
    // Khởi tạo thông số màn hình
    lcd.init();
    //Bật đèn màn hình LCD
    lcd.backlight();
    lcd.print("Connectting");
  
    //Bật baudrate ở mức 57600 để giao tiếp với máy tính qua Serial
    Serial.begin(57600);
    delay(10);
 
    // Kết nối vào mạng Wifi
    Serial.print("Đang kết nối vào mạng: ");
    Serial.println(ssid);
 
    //Kết nối vào mạng Wifi
    WiFi.begin(ssid, password);
 
    //Chờ đến khi đã được kết nối
    while (WiFi.status() != WL_CONNECTED) 
    { //Thoát ra khỏi vòng 
        delay(500);
        lcd.setCursor(0, 1);
        Serial.print('.'); 
        lcd.print('.');    
    }
 
    Serial.println();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wifi Connected");
    Serial.println("Kết nối thành công");
    Serial.print("Địa chỉ IP của ESP8266: ");
    Serial.println(WiFi.localIP());
 
    if (!client.connect(host, port, namespace_esp8266)) 
    {
        Serial.println("Kết nối đến Socket Server thất bại ... Restart");
        ESP.restart();
    }
    
    Serial.println("Đã sẵn sàng nhận lệnh !");
    lcd.setCursor(0, 1);
    lcd.println("Ready !");
}
 unsigned long chuky1 = 0;
 const unsigned long CHU_KY_1_LA_BAO_NHIEU = 5000UL;
void loop()
{
    if (millis() - chuky1 > CHU_KY_1_LA_BAO_NHIEU) 
    {
      chuky1 = millis();
      rain_detect();
      dht_detect();
    }
  
    if (client.monitor()) 
    {  
        if(RID == "LCD_PRINT") lcd_print();
        if(RID == "RELAY") relay();
        if(RID == "SERVO") servo_enjoy();
        if(RID == "RAIN") rain_detect();
        if(RID == "DHT") dht_detect();
    }
    
    // Nếu kết nối thất bại -> Kết nối lại
    if (!client.connected()) 
    {
      client.reconnect(host, port, namespace_esp8266);
      if (!client.connect(host, port, namespace_esp8266)) 
        ESP.restart();
    }
}

// Hàm xử lí lệnh RELAY
void relay()
{
  StaticJsonDocument<200> Doc; //tạo Buffer json có khả năng chứa tối đa 200 ký tự
  char json[Rfull.length()+1];
  Rfull.toCharArray(json, Rfull.length()+1);
  deserializeJson(Doc, json);
  
  bool relayStatus_A = Doc["relay"][0];
  bool relayStatus_B = Doc["relay"][1];
  bool relayStatus_C = Doc["relay"][2];
  bool relayStatus_D = Doc["relay"][3];

  if(relayStatus_A != relayStatus_A_std) 
  {
    if(relayStatus_A)Serial.println("RELAY_A 0");
    else Serial.println("RELAY_A 1");
    relayStatus_A_std = relayStatus_A;
  }
  if(relayStatus_B != relayStatus_B_std) 
  {
    if(relayStatus_B)Serial.println("RELAY_B 0");
    else Serial.println("RELAY_B 1");
    relayStatus_B_std = relayStatus_B;
  }
  if(relayStatus_C != relayStatus_C_std) 
  {
    if(relayStatus_C)Serial.println("RELAY_C 0");
    else Serial.println("RELAY_C 1");
    relayStatus_C_std = relayStatus_C;
  }
  if(relayStatus_D != relayStatus_D_std) 
  {
    if(relayStatus_D)Serial.println("RELAY_D 0");
    else Serial.println("RELAY_D 1");
    relayStatus_D_std = relayStatus_D;
  }
  
//  digitalWrite(RELAY_1,!relayStatus_1);
//  digitalWrite(RELAY_2,!relayStatus_2);
//  digitalWrite(RELAY_3,!relayStatus_3);
//  digitalWrite(RELAY_4,!relayStatus_4);
  
  done();
}
// Hàm xử lí LCD
void lcd_print() 
{
  StaticJsonDocument<200> Doc;
  char json[Rfull.length()+1];
  Rfull.toCharArray(json, Rfull.length()+1);
  deserializeJson(Doc, json);
  
  String line1 = Doc["line"][0];
  String line2 = Doc["line"][1];
 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
 
  done();
}
// Hàm xử lí Servo
void servo_enjoy() 
{
  StaticJsonDocument<50> Doc; //tạo Buffer json có khả năng chứa tối đa 100 ký tự
  char json[Rfull.length()+1];
  Rfull.toCharArray(json, Rfull.length()+1);
  deserializeJson(Doc, json);
  
  int degree = Doc["degree"];
  
  servo.write(degree);

  done();
}
// Hàm xử lí cảm biến mưa
void rain_detect()
{
  RID = "RAIN";
  
  StaticJsonDocument<200> Doc;
  
  Doc["digital"] = digitalRead(RAIN_SENSOR);
  
  String json;
  serializeJson(Doc, json);
  client.send(RID, json);
  
  done();
}
// Hàm xử lí cảm biến DHT
void dht_detect()
{
  RID = "DHT";

  StaticJsonDocument<200> Doc;
  
  Doc["doam"] = dht.getHumidity();
  Doc["nhietdo"] = dht.getTemperature();

  String json;
  serializeJson(Doc, json);
  client.send(RID, json);

  done();
}
//Hàm kết thúc xử lí, reset RID và Rfull để nhận lệnh tiếp theo
void done()
{
  RID = "";
  Rfull = "";
}
