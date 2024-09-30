#include <WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>
#include <WebServer.h>

// Configurações de Wi-Fi e MQTT
const char* SSID = "Wokwi-GUEST";   // Nome da rede Wi-Fi (SSID)
const char* PASSWORD = "";          // Senha do Wi-Fi (vazio para redes abertas)
const char* BROKER_MQTT = "20.9.139.149";  // Endereço do broker MQTT
const int BROKER_PORT = 1883;              // Porta do broker MQTT
const char* TOPICO_PUBLISH = "/corrida/voltas";  // Tópico MQTT para publicar os tempos de volta

// Configurações do sensor PIR (simulado)
const int PINO_PIR = 4;        // Pino do sensor PIR
bool movimentoDetectado = false;  // Indica se o sensor PIR detectou movimento
bool movimentoAnterior = false;   // Guarda o estado anterior do sensor PIR

// Configurações do Buzzer
const int PINO_BUZZER = 5;      // Pino do Buzzer

// Limite de média de tempo
const unsigned long LIMITE_MEDIA = 3000;  // Limite de tempo para calcular a média das voltas

// Instancia os objetos necessários
WiFiClient espClient;           // Cliente Wi-Fi
PubSubClient MQTT(espClient);   // Cliente MQTT usando o Wi-Fi
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Display LCD I2C no endereço 0x27, com 16 colunas e 2 linhas
WebServer server(80);           // Servidor web rodando na porta 80

unsigned long temposVoltas[10]; // Armazena os tempos de até 10 voltas
unsigned long tempoTotalCorrida = 0; // Tempo total da corrida
unsigned long tempoMedia = 0;    // Média dos tempos das voltas
int numeroVoltas = 0;            // Contador de voltas
bool corridaEmAndamento = false; // Indica se a corrida está em andamento

// Função que retorna os dados da corrida em formato JSON via HTTP
void handleCorridaStatus() {
  String response = "{";
  response += "\"numeroVoltas\":" + String(numeroVoltas) + ",";   // Número de voltas
  response += "\"temposVoltas\":[";

  // Adiciona os tempos de cada volta ao JSON
  for (int i = 0; i < numeroVoltas; i++) {
    response += String(temposVoltas[i] / 1000.0, 2);
    if (i < numeroVoltas - 1) {
      response += ",";
    }
  }

  response += "],";
  response += "\"tempoTotalCorrida\":" + String(tempoTotalCorrida / 1000.0, 2) + ","; // Tempo total da corrida
  response += "\"tempoMedia\":" + String(tempoMedia / 1000.0, 2); // Média dos tempos
  response += "}";

  server.send(200, "application/json", response); // Resposta HTTP em JSON
}

void setup() {
  Serial.begin(115200);    // Inicializa o monitor serial para depuração
  lcd.init();              // Inicializa o display LCD
  lcd.backlight();         // Liga a luz de fundo do LCD
  lcd.print("Esperando..."); // Exibe mensagem inicial

  // Configura os pinos do sensor PIR e do Buzzer
  pinMode(PINO_PIR, INPUT);
  pinMode(PINO_BUZZER, OUTPUT);

  initWiFi();   // Inicializa a conexão Wi-Fi
  initMQTT();   // Inicializa a conexão MQTT

  // Configura rota HTTP para retornar o status da corrida
  server.on("/status_corrida", handleCorridaStatus);
  server.begin();  // Inicia o servidor web
}

void loop() {
  if (!MQTT.connected()) reconnectMQTT();  // Verifica a conexão MQTT e reconecta se necessário
  MQTT.loop();  // Mantém a conexão MQTT ativa
  server.handleClient();  // Lida com requisições HTTP do servidor

  // Simulação do sensor PIR (gera um movimento aleatório)
  if (random(0, 2)) {
    movimentoDetectado = true;
    Serial.println("Movimento detectado!");
  } else {
    movimentoDetectado = false;
    Serial.println("Nenhum movimento detectado.");
  }

  delay(2000);  // Atraso para simular tempo de processamento

  // Inicia a corrida se ainda não tiver começado
  if (!corridaEmAndamento) {
    iniciarCorrida();
  }

  // Registra uma volta quando movimento é detectado e a corrida está em andamento
  if (corridaEmAndamento && movimentoDetectado && !movimentoAnterior) {
    registrarVolta();
    movimentoAnterior = true; // Atualiza o estado anterior do movimento
  }

  // Reseta o estado do movimento se não há detecção
  if (!movimentoDetectado) {
    movimentoAnterior = false;
  }

  // Calcula e exibe a média de tempo das voltas no display LCD
  if (numeroVoltas > 0) {
    tempoMedia = tempoTotalCorrida / numeroVoltas;
    lcd.setCursor(0, 0);
    lcd.print("Media: ");
    lcd.print(tempoMedia / 1000.0, 2);
    lcd.print("s    ");
  }

  // Finaliza a corrida após 10 voltas
  if (numeroVoltas >= 10) {
    finalizarCorrida();
  }
}

// Inicia a corrida
void iniciarCorrida() {
  numeroVoltas = 0;
  tempoTotalCorrida = 0;
  corridaEmAndamento = true;
  lcd.clear();
  lcd.print("Corrida iniciada");
  Serial.println("Corrida iniciada");
}

// Registra o tempo de uma volta
void registrarVolta() {
  unsigned long tempoVolta = random(1000, 5000);  // Gera tempo de volta aleatório entre 1 e 5 segundos
  temposVoltas[numeroVoltas] = tempoVolta;
  tempoTotalCorrida += tempoVolta;

  // Exibe o tempo da volta no monitor serial
  Serial.print("Volta ");
  Serial.print(numeroVoltas + 1);
  Serial.print(": ");
  Serial.print(tempoVolta / 1000.0, 2);
  Serial.println(" segundos");

  numeroVoltas++;  // Incrementa o número de voltas
  tocarBuzzer(500);  // Toca o Buzzer por 500ms
  MQTT.publish(TOPICO_PUBLISH, String(tempoVolta / 1000.0, 2).c_str());  // Publica o tempo da volta no MQTT
}

// Finaliza a corrida
void finalizarCorrida() {
  // Exibe mensagem de fim de corrida
  Serial.println("Fim da corrida!");
  delay(5000);
  numeroVoltas = 0;  // Reseta o número de voltas
  corridaEmAndamento = false;
  lcd.clear();
  lcd.print("Esperando...");
}

// Toca o Buzzer por um tempo determinado
void tocarBuzzer(int duracao) {
  digitalWrite(PINO_BUZZER, HIGH);
  delay(duracao);
  digitalWrite(PINO_BUZZER, LOW);
}

// Inicializa a conexão Wi-Fi
void initWiFi() {
  Serial.print("Conectando ao Wi-Fi...");
  WiFi.begin(SSID, PASSWORD);  // Conecta ao Wi-Fi

  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 10) {  // Tenta conectar 10 vezes
    delay(500);
    Serial.print(".");
    tentativas++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(" Conectado ao Wi-Fi!");
  } else {
    Serial.println(" Falha ao conectar ao Wi-Fi!");
    lcd.clear();
    lcd.print("Erro de Wi-Fi");
    while (1);  // Para o programa em caso de falha
  }
}

// Inicializa o MQTT
void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);  // Configura o servidor MQTT
}

// Reconecta ao MQTT se a conexão cair
void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("Tentando conectar ao MQTT...");
    if (MQTT.connect("ESP32Corrida")) {
      Serial.println("Conectado ao MQTT!");
    } else {
      Serial.print("Falha: ");
      Serial.println(MQTT.state());
      delay(1000);  // Tenta reconectar após 1 segundo
    }
  }
}
