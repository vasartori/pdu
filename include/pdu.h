
#ifndef PDU_H
#define PDU_H

#include <PCF8574.h>


#define RELES_POR_PCF 8
#define TOTAL_RELES (TOTAL_TOMADAS * 2)
#define PCF_PIN_SDA 22
#define PCF_PIN_SDL 23
#define TEMPO_PULSO 50
#define TOTAL_PCF 3

// extern uint8_t pcf_enderecos[] = {0x20, 0x21, 0x22};
extern uint8_t pcf_enderecos[];
extern PCF8574* pcf[TOTAL_PCF];
// extern bool tomadasEstado[TOTAL_TOMADAS] = {false};
extern bool tomadasEstado[];
extern bool acpiEstado[];

bool validaTomada(int tomada);
void ligarTomada(int idx);
void desligarTomada(int idx);
void pulsoTomada(int idx);

#endif
