/*
Projeto: Wearable Auxiliary Mobile Mother(WAMM) - (Auxiliar de mãe móvel vestível);
Kit de desenvolvimento utilizado: KIT IoT V.3 Wearable ;
Desenvolvido por: Equipe CDC MINAS UP - Diêgo Geovani Pereira Silva, Ítalo Henrique Damasceno Bessa e Matheus Santos Caires;
Finalidade: Desenvolver um sistema de baixo custo que informa via smartphone a mãe e ou o pai informações importantes para os cuidados com o bebê como a temperatura, se esta havendo movimento, qual é a intensidade da luz e qual é o nível de umidade do ar dentre outras possibilidades.
Local: cpbr8
Data: 07/02/2015

Componente adicional: Sensor de umidade e temperatura DHT11.
Ligação do DHT11 No kit
Pino 1 => VCC - 3.3v
Pino 2 => A4
Pino 3 => Não é utilizado
Pino 4 => GND

Para a conexção Bluetooth foi feito utilizando um smartphone LG G2 que contém o Bluetooth na verção 4.0 com a senha 1234(que é a padrão)
A leitura dos valores no smartphone é feito com o app Terminal BT(pode ser utilizado qualque app que leia os valores impressos para serial) para fazer a leitura dos valores da comunicação serial 

*/

//Pino do sensor de luminosidade
const int Luz = 12; 

//Pinos dos Botões
const int Botao1 = 4; //Pino do botão 1
const int Botao2 = A1; //Pino do botão 2

//Pinos dos Led RGB
const int ledAzul = 6;     //Pino do led Azul.
const int ledVerde = 13;   //Pino do led Verde.
const int ledVermelho = 5; //Pino do led Vermelho.

//Pino do sensor e sua biblioteca Umidade e temperatura
#include <dht.h>
#define dht_dpin A4 //Pino DATA do Sensor de umidade e temperatura ligado na porta Analogica A4
dht DHT; //Inicializa o sensor

//Pino do Buzzer e configurações para melodia
const int speakerPin = 11; // Pino do Buzzer.
int length = 15; // Número de notas
char notes[] = "ccggaagffeeddc "; // espaços representam pausas
int beats[] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4 };
int tempo = 300;

//Biblioteca Acelerometro
#include <Wire.h>
#include <MMA7660.h>

void setup()
{
  //Luz
  pinMode(Luz, INPUT);
  
  //Botão
  pinMode(Botao1, INPUT_PULLUP);   //Configura o pino como entrada e ativa o butao1
  pinMode(Botao2, INPUT_PULLUP);   //Configura o pino como entrada e ativa o butao2
  
  //Definindo os pinos digitais como de saída.
  //Led RGB
  pinMode(ledAzul,OUTPUT);   
  pinMode(ledVerde,OUTPUT);   
  pinMode(ledVermelho,OUTPUT);    

  // Buzzer
  pinMode(speakerPin, OUTPUT);

  //Grava o valor inicial das variáveis.(Desligar o led RGB)
  digitalWrite(ledAzul,HIGH);
  digitalWrite(ledVerde,HIGH);
  digitalWrite(ledVermelho,HIGH);

  // Acelerometro
  Serial.begin(9600); //Inicia a comunicação serial e define a velocidade
  MMA7660.init();
  delay(2000);//Aguarda 2 seg antes de acessar as informações do sensor
}

void loop(){
  
  //Ler Valor luz
  int Luminozidade = analogRead(Luz);

  //Ler valor dos Botões
  int sensorVal = digitalRead(Botao1);  // le o valor do Botao1 e armazena em sensorVal
  int sensorVal2 = digitalRead(Botao2);  // le o valor do Botao2 e armazena em sensorVal
  
  //Ler valor do Acelerometro
  int x,y,z;
  MMA7660.getValues(&x,&y,&z);

  //Ler valor de Umidade e temperatura
  DHT.read11(dht_dpin); //Lê as informações do sensor

  //Imprimir valores na serial
  Serial.println(" "); 
  Serial.println("Nova Leitura abaixo: "); 

  Serial.print("Botao 01: "); 
  Serial.print(sensorVal); // Imprime o valor do pushbutton1
  Serial.print("   Botao 02: "); 
  Serial.println(sensorVal2); // Imprime o valor do pushbutton2

  Serial.print("x: ");
  Serial.print(x);
  Serial.print(" y: ");
  Serial.print(y);
  Serial.print(" z: ");
  Serial.println(z);

  Serial.print("Umidade = ");
  Serial.print(DHT.humidity);
  Serial.print("%  ");
  Serial.print("Temperatura = ");
  Serial.print(DHT.temperature); 
  Serial.println("C  ");

  Serial.print("Luminozidade: ");
  Serial.println(Luz);

  delay(1000);  
   
   //Ação para o botão 01
   // Tenha em mente o pullup significa o botão do Lógica é invertida. Ele vai ALTA quando é aberto, E LOW quando é pressionado.
   if (sensorVal == HIGH) { 
   } 
    else { 
    //Buzzer
      for (int i = 0; i < length; i++) {
       if (notes[i] == ' ') {
         delay(beats[i] * tempo);
       }
        else {
          playNote(notes[i], beats[i] * tempo);
        }
       // pausa entre notas
       delay(tempo / 2); 
     }
    }

   //Ação para o botão 02
   if (sensorVal2 == HIGH) {
   } 
    else {
     //Inicia sequancia de Led RGB
     //Acendendo cada cor individualmente.  Esperando 1s
     digitalWrite(ledAzul,LOW);
     delay(1000);
     digitalWrite(ledAzul,HIGH);
     digitalWrite(ledVerde,LOW);
     delay(1000);
     digitalWrite(ledVerde,HIGH);
     digitalWrite(ledVermelho,LOW);    
     delay(1000);
     digitalWrite(ledVermelho,HIGH);
    
     //Misturando as cores do led para obter cores diferentes. Esperando 2s
     digitalWrite(ledAzul,LOW);     
     digitalWrite(ledVerde,LOW);
     digitalWrite(ledVermelho,HIGH);
     delay(2000);    
     digitalWrite(ledAzul,HIGH);
     digitalWrite(ledVerde,LOW);
     digitalWrite(ledVermelho,LOW);  
     delay(2000);      
     digitalWrite(ledAzul,LOW);
     digitalWrite(ledVerde,LOW);
     digitalWrite(ledVermelho,LOW);
     delay(2000);      
     digitalWrite(ledAzul,LOW);
     digitalWrite(ledVerde,HIGH);
     digitalWrite(ledVermelho,LOW);
     delay(2000);

  //Desligar o led RGB
  digitalWrite(ledAzul,HIGH);
  digitalWrite(ledVerde,HIGH);
  digitalWrite(ledVermelho,HIGH);
    }
}

// Buzzer
void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

  // toque o tom correspondente ao nome da nota
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}
