#include <Wire.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <PCF8574.h>
#include "pdu.h"
#include "tomadas_map.h"
#include <ArduinoJson.h>

const char* ssid = "xxx";
const char* password = "xxx";

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);


  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  WiFi.onEvent([](WiFiEvent_t event) {
    Serial.printf("[WiFi event] %d\n", event);
  });

  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado! IP: ");
  Serial.println(WiFi.localIP());

  Wire.begin(PCF_PIN_SDA, PCF_PIN_SDL);
  for (int i = 0; i < 3; i++) {
    pcf[i] = new PCF8574(pcf_enderecos[i]);
    pcf[i]->begin();
    for (int j = 0; j < RELES_POR_PCF; j++) {
      pcf[i]->write(j, HIGH); // quando liga, desativa todos os reles
    }
  }

  // POST /outlet/N/poweron
  server.on("^/outlet/(\\d+)/poweron$", HTTP_POST, [](AsyncWebServerRequest *request){
    int tomada = request->pathArg(0).toInt();
    if (!validaTomada(tomada)) return request->send(400, "application/json", "{\"error\":\"Invalid outlet number\"}");

    ligarTomada(tomada - 1);
    Serial.printf("turn on outlet number: %s", request->pathArg(0));

    request->send(201, "application/json", "{\"status\":\"Outlet powered on\"}");
  });

  // POST /outlet/N/poweroff
  server.on("^/outlet/(\\d+)/desligar$", HTTP_POST, [](AsyncWebServerRequest *request){
    int tomada = request->pathArg(0).toInt();
    if (!validaTomada(tomada)) return request->send(400, "application/json", "{\"error\":\"Invalid outlet number\"}");
    desligarTomada(tomada - 1);
    Serial.printf("shutting down outlet number: %s", request->pathArg(0));
    request->send(201, "application/json", "{\"status\":\"Outlet powered off\"}");
  });

  // POST /outlet/N/acpi-off
  server.on("^/outlet/(\\d+)/acpi-off$", HTTP_POST, [](AsyncWebServerRequest *request){
    int tomada = request->pathArg(0).toInt();
    if (!validaTomada(tomada)) return request->send(400, "application/json", "{\"error\":\"Invalid outlet number\"}");
    if (!tomadasEstado[tomada - 1]) return request->send(400, "application/json", "{\"error\":\"Outlet must be powered on to send ACPI signal\"}");
    if (!acpiEstado[tomada -1]) return request->send(201, "application/json", "{\"status\":\"ACPI power off was sent\"}");

    pulsoTomada(tomada - 1);
    acpiEstado[tomada -1] = false;
    Serial.printf("pulse sent to outlet nunmber: %s", request->pathArg(0));
    request->send(201, "application/json", "{\"status\":\"ACPI power off was sent\"}");
  });

  // POST /outlet/N/acpi-on
  server.on("^/outlet/(\\d+)/acpi-poweron$", HTTP_POST, [](AsyncWebServerRequest *request){
    int tomada = request->pathArg(0).toInt();
    if (!validaTomada(tomada)) return request->send(400, "application/json", "{\"error\":\"Invalid outlet number\"}");
    if (!tomadasEstado[tomada - 1]) return request->send(400, "application/json", "{\"error\":\"Outlet must be powered on to send ACPI signal\"}");
    if (acpiEstado[tomada - 1]) return request->send(201, "application/json", "{\"status\":\"ACPI power on was sent\"}");

    pulsoTomada(tomada - 1);
    acpiEstado[tomada -1] = true;
    Serial.printf("pulse sent to outlet number: %s", request->pathArg(0));
    request->send(201, "application/json", "{\"status\":\"ACPI power on was sent\"}");
  });

  // GET /outlet/N/status
  server.on("^/outlet/(\\d+)/status$", HTTP_GET, [](AsyncWebServerRequest *request){
    int tomada = request->pathArg(0).toInt();
    if (!validaTomada(tomada)) return request->send(400, "application/json", "{\"erro\":\"Invalid outlet number\"}");
    bool ligada = tomadasEstado[tomada - 1];
    bool acpi = acpiEstado[tomada -1];

    Serial.printf("outlet: %s is powered on? %d acpi status: %d\n",  request->pathArg(0), ligada, acpi);

    JsonDocument json;
    json["outlet"] = tomada;
    json["power"] = ligada;
    json["acpi"] = acpi;

    String response;
    serializeJson(json, response);
    request->send(200, "application/json", response);

  });

  server.begin();
}


void loop() {

}
