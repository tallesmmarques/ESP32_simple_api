#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

const int ts = 100;

float targetAngle;
float pitch;
float PWM;
float Loop_time;

const char* SSID = "UaiFai";
const char* PWD  = "MinhaFamiliaETudo";

WebServer server(80);

void myLoop(void *);
void threadHtml(void *);

void mainPage()
{
  Serial.println("GET /");
  server.send(200, "text/html", "<h1>Teste</h1>");
}

DynamicJsonDocument doc(250);
char buffer[250];
void getLeitura()
{
  Serial.println("GET /leitura");

  doc.clear();
  doc["Loop_time"] = Loop_time;
  doc["PWM"] = PWM;
  doc["pitch"] = pitch;
  doc["targetAngle"] = targetAngle;

  serializeJson(doc, buffer);
  server.send(200, "application/json", buffer);
}

// void getEscrita()
// {
//   Serial.println("GET /escrita");
//   if (server.hasArg("ws"))
//   {
//     ws = server.arg("ws").toFloat();
//   }
//   if (server.hasArg("amplitude"))
//   {
//     amplitude = server.arg("amplitude").toFloat();
//   }

//   String response = "ws:" + String(ws) + "," + "amplitude:" + String(amplitude);
//   server.send(200, "text/plain", response);
// }

void setup() 
{
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println("Connecting to " + String(SSID));
  WiFi.begin(SSID, PWD);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();

  Serial.print("Connected IP: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN, HIGH);

  server.on("/", mainPage);
  server.on("/leitura", getLeitura);
  // server.on("/escrita", getEscrita);
  server.enableCORS();
  server.begin();

  randomSeed(analogRead(0));  

  xTaskCreatePinnedToCore(
    myLoop,           // função
    "myLoop",         // nome da thread
    10000,            // espaço de memória reservado para a thread (stack)
    NULL,             // parâmetros
    1,                // prioridade da thread
    NULL,             // handle para a thread
    0                 // qual núcleo a thread será executada
  );
  xTaskCreatePinnedToCore(
    threadHtml,       // função
    "threadHtml",     // nome da thread
    10000,            // espaço de memória reservado para a thread (stack)
    NULL,             // parâmetros
    2,                // prioridade da thread
    NULL,             // handle para a thread
    1                 // qual núcleo a thread será executada
  );

  Serial.println("Threads created successfully");
}

void myLoop(void *parameter)
{
  Serial.printf("My Loop: %d\n", xPortGetCoreID());

  int last_time = millis();
  while (true)
  {
    if (millis() - last_time <= ts)
    {
      targetAngle = random(-90, 90);
      pitch = random(-180, 180);
      PWM = random(0, 100);
      Loop_time = millis()/1000.0;
      last_time = millis();
    }
    delay(1);
  }
  vTaskDelete(NULL);
}

void threadHtml(void * parameter)
{
  Serial.printf("Thread Html: %d\n", xPortGetCoreID());

  while (true)
  {
    server.handleClient();
    delay(1);
  }
  vTaskDelete(NULL);
}

void loop() {
  delay(1);
}