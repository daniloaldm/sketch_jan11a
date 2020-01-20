#include "CTBot.h"

CTBot myBot;

//Definicoes da rede wifi e conexao
String ssid  = "Dan"; //Nome da sua rede wifi
String pass  = "semsenha"; //Senha da sua rede wifi
String token = "937555706:AAE_N4SomQHEjN6bXsyNw5QDIE_twOY5LoM"; //Token bot Telegram

//Pinos dos leds
uint8_t led1 = D4;
uint8_t led2 = D3;
int LDRPin = A0; 
int sensorValue=0;
int varAux=0;

void setup()
{
  Serial.begin(115200);
  Serial.println("Inicializando bot Telegram...");

  //Conexao na rede wifi
  myBot.wifiConnect(ssid, pass);

  //Define o token
  myBot.setTelegramToken(token);

  //Verifica a conexao
  if (myBot.testConnection())
    Serial.println("nConexao Ok!");
  else
    Serial.println("nFalha na conexao!");

  //Define os pinos dos leds como saida e apaga os leds
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
}

void loop()
{

  if (sensorValue < 20 and varAux == 0)
  {
    digitalWrite(led2, LOW);
    Serial.println("Led vermelho ligado, luminosidade do ambiente estava baixa!");
    varAux = 1;
  }
  else if(sensorValue > 20 and varAux == 1)
  {
    digitalWrite(led2, HIGH);
    Serial.println("Led vermelho desligado, luminosidade do ambiente estava alta!");
    varAux = 0;
  }
  
  sensorValue=analogRead(LDRPin);   //read  the value of the photoresistor.
  Serial.println(sensorValue);  // value of the photoresistor to the serial monitor.
  
  //Variavel que armazena a mensagem recebida
  TBMessage msg;
  //Verifica se chegou alguma mensagem
  if (myBot.getNewMessage(msg))
  {
    //Verifica se foi recebia a mensagem "Liga 1"
    if (msg.text.equalsIgnoreCase("LIGAR LED VERDE"))
    {
      //Liga o led 1 (vermelho)
      digitalWrite(led1, LOW);
      //Retorna uma pensagem informando que o led foi aceso
      myBot.sendMessage(msg.sender.id, "Led verde aceso!");     
    }
    else if (msg.text.equalsIgnoreCase("DESLIGAR LED VERDE"))
    {
      digitalWrite(led1, HIGH);
      myBot.sendMessage(msg.sender.id, "Led verde Apagado!");
    }
//    else if (msg.text.equalsIgnoreCase("LIGA 2"))
//    {
//      digitalWrite(led2, LOW);
//      myBot.sendMessage(msg.sender.id, "LED 2 Aceso!");
//    }
//    else if (msg.text.equalsIgnoreCase("DESLIGA 2"))
//    {
//      digitalWrite(led2, HIGH);
//      myBot.sendMessage(msg.sender.id, "LED 2 Apagado!");
//    }
    else if (sensorValue < 20 and msg.text.equalsIgnoreCase("Luminosidade"))
    {
      digitalWrite(led2, LOW);
      myBot.sendMessage(msg.sender.id, (String)"Led vermelho ligado, luminosidade do ambiente esta em "+sensorValue+(String)", considerada baixa!");
    }
    else if(sensorValue > 20 and msg.text.equalsIgnoreCase("Luminosidade"))
    {
      digitalWrite(led2, HIGH);
      myBot.sendMessage(msg.sender.id, (String)"Led vermelho desligado, luminosidade do ambiente está em "+sensorValue+(String)", considerada alta!");
    }
    else
    {
      //Caso receba qualquer outro comando, envia uma
      //mensagem generica/informativa
      String reply;
      reply = "Desculpe, não entendi. \n\nSegue a lista de comandos que consigo entender: \n\nLigar led verde - Para ligar led verde \n\nDesligar led verde - Para desligar led verde \n\nLuminosidade - Para checar luminosidade do ambiente e acender ou não o led vermelho.";
      myBot.sendMessage(msg.sender.id, reply);
    }
  }
  delay(1000);
}
