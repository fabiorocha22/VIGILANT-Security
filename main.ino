#include <Wire.h>

#include <LiquidCrystal_I2C.h>

//bibliotecas do Ethernet
#include <Ethernet.h>
#include <SPI.h>
#include "RestClient.h"

//sensor de temperatura
#include <DHT.h>

//biliotecas do LCD



//macros do sensor de tempeartura
#define DHTPIN 2
#define DHTTYPE DHT11

//macros do sensor de presença
#define pir 8

//variáveis do Ethernet 
RestClient client = RestClient("192.168.0.101",3000);
String response;

//variável da temperatura
DHT dht(DHTPIN,DHTTYPE);

//variável do LCD
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);

//variáveis do sensor de presença
int estado = 0;

//vetor dos leds
int leds[4] = {46, 48, 42, 44};

void setup() {
    Serial.begin(9600);
    Serial.println("Connecting to Network");
    client.dhcp();  // Connect via DHCP
    Serial.println("Setup!");
    dht.begin();
    lcd.begin(16,2);
    pinMode(pir,INPUT);
    zeraLEDS();
}

void loop(){
    response = "";
    int statusCode = client.get("/devices/check/1", &response);
    Serial.print("Response body from server: ");
    Serial.println(response);
    tratamentoDados();
    //setupLCD();    
    estado = digitalRead(pir);
    if (estado == 1)
    {
        alarme();
    }
    delay(1000);
}

void alarme()
{   
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("ALERTA DE");
    lcd.setCursor(4,1);
    lcd.print("INVASÃO");
}

void setupLCD(){
    int temperatura = dht.readTemperature();
    int umidade = dht.readHumidity();
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("VIGILANT");
    lcd.setCursor(4,1);
    lcd.print("SECURITY");
    delay(2000);    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temperatura: ");
    lcd.print(temperatura);
    lcd.print("C");
    lcd.setCursor(2,1);
    lcd.print("Umidade: ");
    lcd.print(umidade);
    lcd.print("%");
}

void tratamentoDados(){
    int i;
    String c = "00";
    for(int i=0; i<4; i++){
        if(response.charAt(i) != '0'){
            digitalWrite(leds[i], HIGH);
        }
        else
            digitalWrite(leds[i], LOW);
    }
}

void zeraLEDS(){
    for(int i=0; i<4; i++){
        digitalWrite(leds[i], LOW);
    }
}
