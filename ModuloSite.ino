#include <VirtualWire.h>
#include <SPI.h>
#include <Ethernet.h>
String readString;

char Valor_CharMsg[4]; 
int quarto = 0;

boolean ligado = true;

//Endereco IP, gateway, mascara de rede
byte mac[] = { 0xA4, 0x28, 0x72, 0xCA, 0x55, 0x2F };
byte ip[] = { 192, 168, 1, 120 };
byte gateway[] = { 192, 168, 1, 1 };
byte subnet[] = { 255, 255, 255, 0 };
 
EthernetServer server(80);
 
void setup(){
  Serial.begin(9600);
    //Pino ligado ao pino DATA do transmissor RF
   vw_set_tx_pin(8);
  //Velocidade de comunicacao (bits por segundo)
  vw_setup(5000); 
  //Inicializa Ethernet Shield
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  Serial.println("Sistema de Monitoramento!!! ~~C&C~~"); 
}
 
void loop(){
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()){
      if (client.available()){
        char c = client.read();
        if (readString.length() < 100) {
          readString += c;
        }
        if (c == '\n'){
          //Controle do Sensor
          Serial.println(readString);
          //Liga o Sensor
          if (readString.indexOf("?QuartoLigado") > 0){
            //  (quarto = 2) LIGA SENSOR
             quarto = 2;
            //Converte valor para envio
            itoa(quarto,Valor_CharMsg,10);
            //Envio dos dados
            vw_send((uint8_t *)Valor_CharMsg, strlen(Valor_CharMsg));
            //Aguarda envio dos dados
            vw_wait_tx();
            Serial.print("Valor enviado: ");
            Serial.println(Valor_CharMsg);
            delay(500);
            ligado = false;
          }
          else{
            //Desliga o Sensor
            if (readString.indexOf("?QuartoDesligado") > 0){
            //  (quarto = 3) DESLIGA SENSOR
             quarto = 3;
            //Converte valor para envio
            itoa(quarto,Valor_CharMsg,10);
            //Envio dos dados
            vw_send((uint8_t *)Valor_CharMsg, strlen(Valor_CharMsg));
            //Aguarda envio dos dados
            vw_wait_tx();
            Serial.print("Valor enviado: ");
            Serial.println(Valor_CharMsg);
            delay(500);
            ligado = true;
            }
          }
 
          readString = "";
 
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<HTML>");
          client.println("<style>body{background-color: rgba(184,184,184,1.00)}</style>");
          client.println("<BODY>");
          client.println("<center><div id='wrapper'><img alt='LogoC&C' src='http://c1.staticflickr.com/5/4203/34066633463_284045921d_b.jpg'/><br/></center>");
          client.println("<br />");
          client.println("");
          client.println("<div><center><a href=\'/?QuartoLigado\'><img src='https://c1.staticflickr.com/5/4270/34745511561_4a0741290d_q.jpg' alt='QLigado'/></a><a href='/?QuartoDesligado\'><img src='https://c1.staticflickr.com/5/4221/34745511841_d4a13676eb_q.jpg' alt='QDesligado'/></a><br /></center></div>");          
          client.println("");
         // client.println("<div><center><a href=\'/?SalaLigado\'><img src='https://' alt='SLigado'/></a><a href='/?SalaDesligado\'><img src='https:' alt='SDesligado1'/></a><br /></center></div>");
         // client.print(ligado);    
          client.println("</BODY>");
          client.println("</HTML>");
 
          delay(1);
          client.stop();
        }
      }
    }
  }
}
