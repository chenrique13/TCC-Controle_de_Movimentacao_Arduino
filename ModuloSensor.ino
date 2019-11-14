#include <VirtualWire.h>

//Define pinos Led e Sensor
const int Sensor = 9;
const int LedAzul = 11;
const int LedVerde = 10;

// Transmissor
char Valor_CharMsg[4]; 

//Receptor
int valor_recebido_RF;
char recebido_RF_char[4];

//Variaveis
int valor_sensor;
int estado = 0;

//Metodos
void LsensorQ();
void DsensorQ();

void setup() {
  Serial.begin(9600);   
  pinMode(Sensor,INPUT);
  pinMode(LedAzul,OUTPUT);
  pinMode(LedVerde, OUTPUT);
  //Pino ligado ao pino DATA do transmissor RF
  vw_set_tx_pin(8);
  //Pino ligado ao pino DATA do receptor RF
  vw_set_rx_pin(7); 
  //Velocidade de comunicacao (bits por segundo)
  vw_setup(5000);
  //Inicia a recepcao  
  vw_rx_start();
  Serial.println("Inicio");
}

void loop() {
    Receptor();
       //Quando o numero recebido for 2 liga sensor quarto
       if (valor_recebido_RF == 2){
          LsensorQ();
    }
       if (valor_recebido_RF == 3){
         DsensorQ();
    }
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
    }
}
       
void LsensorQ(){
  //Verifica o estado do Sensor
  valor_sensor = digitalRead(Sensor);
  Serial.println("Sensor do Quarto Ligado");
  digitalWrite(LedVerde, HIGH);
  delay(1000);
  Serial.println("Sem Movimeto...");
    if (valor_sensor == 1){
    Serial.println("Movimeto Detectado...");
    estado = 0;
    //Converte valor para envio
    itoa(estado,Valor_CharMsg,10);
    //Envio dos dados
    vw_send((uint8_t *)Valor_CharMsg, strlen(Valor_CharMsg));
    //Aguarda envio dos dados
    vw_wait_tx();
    digitalWrite(LedVerde, HIGH);
    digitalWrite(LedAzul, HIGH);
    delay(5000);
    Serial.print("Valor enviado: ");
    Serial.println(Valor_CharMsg);
    delay(500);
    digitalWrite(LedAzul, LOW);
  }
}

void DsensorQ(){
  Serial.println("Sensor do Quarto Desligado");  
  delay(1000);
  digitalWrite(LedAzul, LOW);
  digitalWrite(LedVerde, LOW);
}
