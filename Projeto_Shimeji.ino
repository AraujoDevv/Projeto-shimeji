
#include <DHT.h> //Biblioteca DHT sensor library
//#include <DHT_U.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h> //Biblioteca LiquidCrystalI2C_2

#include <DallasTemperature.h>
#include <OneWire.h>



#define dadosumi 6 // Pino digital utilizado pelo DHT22
#define DHTTYPE DHT22 // Define o modelo do sensor (DHT22 / AM2302)

#define dadostemp 2
#define lampada 9
#define ventilador 8
#define umidificador 10
#define sensorCO2 A3

unsigned long initTime = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire oneWire(dadostemp);
DallasTemperature sensors(&oneWire);

DHT dht (dadosumi, DHTTYPE); // Passa os parametros para a função

void wait(int period);

void setup()
{
  lcd.init();  // Inicia o LCD
  lcd.backlight(); // Aciona a iluminação do LCD
  Serial.begin (9600);
  sensors.begin();
  dht.begin(); //Inicializa a função
  wait(2000);

  pinMode(lampada, OUTPUT); // Porta 9
  pinMode(ventilador, OUTPUT); //Porta 8
  pinMode(umidificador, OUTPUT); //Porta 10
}

void loop()
{
  //DHT.read11(dadosumi); //LÊ AS INFORMAÇÕES DO SENSOR
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
  Serial.println(sensors.getTempCByIndex(0));
  int humidity = dht.readHumidity();
  float temperatura = dht.readTemperature();

  lcd.home(); // Seta o cursor para o inicio caracter 0, na linha 0
  lcd.print("  Temp = ");
  lcd.print(temp);
  lcd.print(" C");
  lcd.setCursor(0, 1);
  lcd.print("  umi: "); //IMPRIME O TEXTO NA SERIAL
  lcd.print(humidity);
  lcd.print("%"); //ESCREVE O TEXTO EM SEGUIDA
  wait(5000); //INTERVALO DE 2 SEGUNDOS * NÃO DIMINUIR ESSE VALOR
  lcd.clear();
  lcd.home();
  lcd.print("  CO2 = ");
  lcd.print(analogRead(A3) * (5.0 / 1023.0));
  lcd.print(" V");
  wait(5000);
  lcd.clear();

  Serial.println(humidity);
  Serial.println(temperatura);


  if (temp >= 26.0) {
    //acionamento do rele
    digitalWrite(lampada, HIGH); //aciono o rele da lâmpada
    digitalWrite(ventilador , LOW); //aciono o rele do ventilador
  }
  else if (temp >= 29.0) {
    digitalWrite(umidificador, LOW);  //aciono o rele do umidificador
  }
  else if (temp < 25.0) {
    digitalWrite(lampada, LOW);
    digitalWrite(ventilador, HIGH);
  }

  if (humidity <= 70.0) {
    digitalWrite(umidificador, LOW);
  }
  else if (humidity > 90) {
    digitalWrite(umidificador, HIGH);
  }
  //digitalWrite(8, HIGH); // Liga/Fecha
  // delay(2000); // 2 seg
  //digitalWrite(8, LOW); // Desliga/Abre
  // delay(2000); // 2 seg
}

void wait(int period) {
  initTime = millis();
  while (millis() - initTime <= period) {
  }
} 
