#include "pdu.h"
#include "tomadas_map.h"

PCF8574* pcf[TOTAL_PCF];
bool tomadasEstado[TOTAL_TOMADAS] = {false};
bool acpiEstado[TOTAL_TOMADAS] = {false};
uint8_t pcf_enderecos[TOTAL_PCF] = {0x20, 0x21, 0x22};

bool validaTomada(int tomada) {
    return tomada >= 1 && tomada <= TOTAL_TOMADAS;
  }

  // Liga energia + dá pulso
void ligarTomada(int idx) {
    Serial.printf("ligando tomada (idx) %d\n", idx);
    TomadaMap map = mapaTomadas[idx];
    tomadasEstado[idx] = true;
    acpiEstado[idx] = true;
    pcf[map.pcfEnergia]->write(map.pinoEnergia, LOW); // Descobri que o nivel baixo ativa a porta :/
    pulsoTomada(idx);
    Serial.printf("tomada %d ligada e pulso enviado\n", idx +1);
}

// Desliga energia
void desligarTomada(int idx) {
    Serial.printf("desligando tomada (idx) %d\n", idx);
    tomadasEstado[idx] = false;
    acpiEstado[idx] = false;
    TomadaMap map = mapaTomadas[idx];
    pcf[map.pcfEnergia]->write(map.pinoEnergia, HIGH);
    Serial.printf("tomada %d desligada\n", idx +1);
}


// Envia pulso no relê
void pulsoTomada(int idx) {
    TomadaMap map = mapaTomadas[idx];
    Serial.printf("pulso tomada (idx) %d | tomada nro: %d\n",idx, idx +1);
    Serial.printf("enviando pulso no pcf %d no pino %d\n", map.pcfPulso, map.pinoPulso);
    pcf[map.pcfPulso]->write(map.pinoPulso, LOW);
    delay(TEMPO_PULSO);
    pcf[map.pcfPulso]->write(map.pinoPulso, HIGH);
    Serial.printf("finalizado envio de pulso da tomada %d no pcf %d no pino %d",idx+1, map.pcfPulso, map.pinoPulso);
  }
