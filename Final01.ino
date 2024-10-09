#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TimeLib.h>
#include <DHT.h>
#include <TM1637Display.h>
#include <WebSocketsServer.h>

// Defina seu SSID e senha do WiFi
const char* ssid = "XXXXX";
const char* password = "XXXXXXXX";

// Configurações do TM1637
#define CLK 2  // Pino de clock
#define DIO 4  // Pino de dados

TM1637Display display(CLK, DIO); // Cria uma instância do display

// Configurações do DHT
#define DHTPIN 14         // Pino onde o DHT está conectado
#define DHTTYPE DHT11    // Definindo o tipo do sensor DHT

DHT dht(DHTPIN, DHTTYPE);

// Configurações do NTP
WiFiUDP udp;
NTPClient ntp(udp, "pool.ntp.br", -3 * 3600, 60000); // UTC offset em segundos e atualização a cada 60 segundos
WebSocketsServer webSocket = WebSocketsServer(8080); // Cria uma instância do servidor WebSocket na porta 8080

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Inicializa o display
  display.setBrightness(0x0f);  // Brilho máximo

  // Conecta ao WiFi
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Conectado ao WiFi");
  Serial.println(WiFi.localIP());

  // Inicia o NTP
  ntp.begin();
  Serial.println("NTP iniciado");

  // Inicia o servidor WebSocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent); // Define a função de callback para eventos do WebSocket

}

void loop() {
  webSocket.loop(); // Mantém o servidor WebSocket em execução, verificando por novas conexões e mensagens
}

void webSocketEvent(uint8_t numero_msg, WStype_t type, uint8_t * conteudo_recebido, size_t tamanho_conteudo) {
  String temp, umid, date, hora;
  switch (type) { // Verifica o tipo de evento
    case WStype_DISCONNECTED: // Caso a conexão seja desconectada
      Serial.printf("[%u] Desconectou!\n", numero_msg); // Imprime no serial que o cliente numero_msg foi desconectado
      break;
    case WStype_CONNECTED: // Caso uma nova conexão seja estabelecida
      Serial.printf("[%u] Conectou!\n", numero_msg); // Imprime no serial que o cliente numero_msg foi conectado
      webSocket.sendTXT(numero_msg, "Conectou"); // Envia mensagem de confirmação de conexão ao cliente
      break;
    case WStype_TEXT: // Caso uma mensagem de texto seja recebida
      Serial.printf("[%u] Recebeu pelo Socket o texto: %s\n", numero_msg, conteudo_recebido); // Imprime no serial a mensagem recebida do cliente numero_msg
      if (strcmp((char *)conteudo_recebido, "on") == 0) { // Se a mensagem for "on"
        // Atualiza a hora
        ntp.update();

        // Lê a temperatura e umidade
        float h = dht.readHumidity();
        float t = dht.readTemperature();

        // Verifica se as leituras falharam
        if (isnan(h) || isnan(t)) {
          Serial.println("Falha ao ler do DHT!");
          return;
        }

        // Obtém o timestamp
        unsigned long epochTime = ntp.getEpochTime();

        // Define a hora usando TimeLib
        setTime(epochTime); // Define a hora atual
        int ano = year();
        int mes = month();
        int dia = day();

        // Exibe a data no formato dd/mm/aaaa
        // Serial.print("Data: ");
        /*        Serial.print((dia < 10 ? "0" : "") + String(dia));
                Serial.print("/");
                Serial.print((mes < 10 ? "0" : "") + String(mes));
                Serial.print("/");
                Serial.print(ano);
        */
        String date = "data ";
        date += (dia < 10 ? "0" : "") + String(dia);
        date += "/";
        date += (mes < 10 ? "0" : "") + String(mes);
        date += "/";
        date += String(ano);
        Serial.print(date);
        webSocket.sendTXT(numero_msg, date); // Envia mensagem de confirmação de conexão ao cliente

        // Exibe a hora, temperatura e umidade no Serial Monitor
        Serial.print(" | Hora: ");
        Serial.print(ntp.getFormattedTime());
        hora = "hora " + ntp.getFormattedTime();
        webSocket.sendTXT(numero_msg, hora); // Envia mensagem de confirmação de conexão ao cliente
        Serial.print(" | Temperatura: ");
        Serial.print(t);
        Serial.print(" °C");
        temp = "temp " + String(t, 1);
        webSocket.sendTXT(numero_msg, temp); // Envia mensagem de confirmação de conexão ao cliente
        Serial.print(" | Umidade: ");
        Serial.print(h);
        Serial.println(" %");
        umid = "umid " + String(h, 1);
        webSocket.sendTXT(numero_msg, umid); // Envia mensagem de confirmação de conexão ao cliente

        // Exibir temperatura e umidade no display TM1637
        display.showNumberDec(t * 10, false, 3, 0); // Exibe temperatura (multiplicada por 10 para mostrar um decimal)
        delay(2000);
        display.showNumberDec(h * 10, false, 3, 0); // Exibe umidade

        // Aguarda 10 segundos antes da próxima leitura
        // delay(10000);

        //webSocket.sendTXT(numero_msg, ""); // Limpa a mensagem do cliente
        webSocket.sendTXT(numero_msg, "Dados atualizados"); // Envia mensagem "Dados atualizados" de volta ao cliente
      }
      break;
  }
}
