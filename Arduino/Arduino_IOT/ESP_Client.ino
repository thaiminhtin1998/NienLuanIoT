#include <SerialCommand.h>

SerialCommand sCmd;

const byte RELAY_1 = D2;
const byte RELAY_2 = D5;
const byte RELAY_3 = D6;
const byte RELAY_4 = D7;

void setup() 
{
  Serial.begin(57600);

  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);

  digitalWrite(RELAY_1,HIGH);
  digitalWrite(RELAY_2,HIGH);
  digitalWrite(RELAY_3,HIGH);
  digitalWrite(RELAY_4,HIGH);

  sCmd.addCommand("RELAY_1",   relay_1);
  sCmd.addCommand("RELAY_2",   relay_2);
  sCmd.addCommand("RELAY_3",   relay_3);
  sCmd.addCommand("RELAY_4",   relay_4);
}

void loop() 
{
  sCmd.readSerial();
}

void relay_1() 
{
  //Đoạn code này dùng để đọc TỪNG tham số. Các tham số mặc định có kiểu dữ liệu là "chuỗi"
  char *arg;
  arg = sCmd.next();
  
  int value = atoi(arg); // Chuyển chuỗi thành số
  digitalWrite(RELAY_1,value);
}
void relay_2() 
{
  //Đoạn code này dùng để đọc TỪNG tham số. Các tham số mặc định có kiểu dữ liệu là "chuỗi"
  char *arg;
  arg = sCmd.next();
  
  int value = atoi(arg); // Chuyển chuỗi thành số
  digitalWrite(RELAY_2,value);
}
void relay_3() 
{
  //Đoạn code này dùng để đọc TỪNG tham số. Các tham số mặc định có kiểu dữ liệu là "chuỗi"
  char *arg;
  arg = sCmd.next();
  
  int value = atoi(arg); // Chuyển chuỗi thành số
  digitalWrite(RELAY_3,value);
}
void relay_4() 
{
  //Đoạn code này dùng để đọc TỪNG tham số. Các tham số mặc định có kiểu dữ liệu là "chuỗi"
  char *arg;
  arg = sCmd.next();
  
  int value = atoi(arg); // Chuyển chuỗi thành số
  digitalWrite(RELAY_4,value);
}
