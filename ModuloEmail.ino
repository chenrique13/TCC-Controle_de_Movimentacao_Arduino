#include <SPI.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <VirtualWire.h>
#define time 1000

//Pinos Digitais
int LedVermelho = 2;
int LedAmarelo = 3;
int LedAzul = 4;
int Sala = 8;
int Quarto = 9;

//Variaveis
int valorsala = 0;
int valorquarto = 0;

// Receptor
int valor_recebido_RF;
char recebido_RF_char[4];

// Protocolo Eternet
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 150);
byte server[] = {200,147,99,132}; // Colocar o ip do servidor SMTP (no Caso Bol)
EthernetClient client;

void setup(){
 
  //INICIO CONEXAO COM A WEB
  if(Ethernet.begin(mac) == 0) {
    Serial.println("Falha ao Configurar Ethernet Usando DHCP");
    Ethernet.begin(mac, ip);
  }
  pinMode(Sala, INPUT);
  pinMode(Quarto, INPUT);
  pinMode(LedVermelho, OUTPUT);
  pinMode(LedAmarelo, OUTPUT);
  pinMode(LedAzul,OUTPUT);
  Serial.begin(9600);
  //Pino ligado ao pino DATA do receptor RF
  vw_set_rx_pin(7);
  //Velocidade de comunicacao (bits por segundo)
  vw_setup(5000); 
  //Inicia a recepcao  
  vw_rx_start();
  delay(time);
  Serial.println("Iniciando Sistema...");
  Serial.println("Sem Movimentção na Casa");
  digitalWrite(LedVermelho, HIGH);
}
 
void loop(){
  valorsala = digitalRead(Sala);
  valorquarto = digitalRead(Quarto);
  byte EntradaSerial;
  EntradaSerial = Serial.read();
  /* if(valorsala == 1){
    EnviarSala();
   }*/
Receptor();
    digitalWrite(LedAmarelo, LOW);
    digitalWrite(LedAzul,LOW);
   }

// Metodo Sala
void EnviarSala(){
  Serial.println("Atenção!!! Movimentação na Sala");
  digitalWrite(LedAmarelo, HIGH);
  delay(time);
  delay(time);
  Serial.println("Pronto para enviar o e-mail.");
  delay(time);
  Serial.println("Conectando...");
  if (client.connect(server, 587)){
    Serial.println("Conectado!");
    Serial.println("Enviando email...");
    Serial.println();
    client.println("EHLO localhost");
    recebe();
    delay(time);
    client.println("AUTH LOGIN");
    recebe();
    delay(time);
    client.println("dW5vLmFyZHVpbm9AYm9sLmNvbS5icg=="); // Email de login em base de 64:
    recebe();
    delay(time);
    client.println("SDNuciFxdWU="); // Senha do email em base de 64:
    recebe();
    delay(time);
    client.println("mail from: <uno.arduino@bol.com.br>"); //Email remetente
    recebe();
    delay(time);
    client.println("rcpt to: <c.henrique1309@gmail.com>"); // Email destinatário
    Serial.println("Destinatário: c.henrique1309@gmail.com");
    recebe();
    delay(time);
    client.println("data");
    recebe();
    delay(time);
    client.println("Subject: Alerta de Movimentação na sua Casa!"); // Assunto
    recebe();
    delay(time);
    client.println("Atenção!!! Movimentação na Sala");// Corpo
    client.println();
    client.println("~~~~ Carlos & Claudio (CC) ~~~~");
    recebe();
    delay(time);
    client.println("."); // Indica fim do email.
    recebe();
    delay(time);
    client.println();
    recebe();
    delay(time);
    Serial.println("Email Enviado!");
    delay(time);
    if (client.connected()){
      Serial.println();
      Serial.println("Desconectando...");
      client.stop();
      Serial.println();
      delay(30000);
      digitalWrite(LedAmarelo, LOW);
    }
  }else{
    Serial.println("Falha na Conexão");
    digitalWrite(LedAmarelo, LOW);
    digitalWrite(LedAzul,LOW);
    delay(10000);
  }
  Serial.println("Sem Movimentção na Casa");
}

 // Metodo Quarto
void EnviarQuarto(){
  Serial.println("Atenção!!! Movimentação no Quarto");
  digitalWrite(LedAzul, HIGH);
  delay(time);
  delay(time);
  Serial.println("Pronto para enviar o e-mail.");
  delay(time);
  Serial.println("Conectando...");
  if (client.connect(server, 587)){
    Serial.println("Conectado!");
    Serial.println("Enviando Email...");
    Serial.println();
    client.println("EHLO localhost");
    recebe();
    delay(time);
    client.println("AUTH LOGIN");
    recebe();
    delay(time);
    client.println("dW5vLmFyZHVpbm9AYm9sLmNvbS5icg=="); // Email de login em base de 64:
    recebe();
    delay(time);
    client.println("SDNuciFxdWU="); // Senha do email em base de 64:
    recebe();
    delay(time);
    client.println("mail from: <uno.arduino@bol.com.br>"); //Email remetente
    recebe();
    delay(time);
    client.println("rcpt to: <c.henrique1309@gmail.com>"); // Email destinatário
    Serial.println("Destinatário: c.henrique1309@gmail.com");
    recebe();
    delay(time);
    client.println("data");
    recebe();
    delay(time);
    client.println("Subject: Alerta de Movimentação na sua Casa!"); // Assunto
    recebe();
    delay(time);
    client.println("Atenção!!! Movimentação no Quarto"); // Corpo
    client.println();
    client.println("~~~~ Carlos & Claudio (CC) ~~~~");
    recebe();
    delay(time);
    client.println("."); // Indica fim do email.
    recebe();
    delay(time);
    client.println();
    recebe();
    delay(time);
    Serial.println("Email Enviado!");
    delay(time);
    if (client.connected()){
      Serial.println();
      Serial.println("Desconectando...");
      client.stop();
      Serial.println();
      delay(10000);
      digitalWrite(LedAzul, LOW);
    }
  }else{
    Serial.println("Falha na Conexão");
    digitalWrite(LedAmarelo, LOW);
    digitalWrite(LedAzul,LOW);
    delay(10000);
  }
  Serial.println("Sem Movimentção na Casa");  
}

void Receptor(){
   uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    
    if (vw_get_message(buf, &buflen)) {
    int i;
        for (i = 0; i < buflen; i++){            
          //Armazena os caracteres recebidos  
          recebido_RF_char[i] = char(buf[i]);
       }
       recebido_RF_char[buflen] = '\0';
       
       //Converte o valor recebido para integer
       valor_recebido_RF = atoi(recebido_RF_char);
         
       //Mostra no serial monitor o valor recebido
       Serial.print("Recebido: ");
       Serial.println(valor_recebido_RF);
       
       //Altera o estado do led conforme o numero recebido
       if (valor_recebido_RF == 0){
         EnviarQuarto();
       }
    }
}

void recebe(){
  while (client.available()){
    char conteudo = client.read();
    Serial.print(conteudo);
  }
}
