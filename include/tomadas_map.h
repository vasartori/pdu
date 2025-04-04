#ifndef TOMADAS_MAP_H
#define TOMADAS_MAP_H

#define TOTAL_TOMADAS 12

struct TomadaMap {
  int pcfEnergia;
  int pinoEnergia;
  int pcfPulso;
  int pinoPulso;
};

extern TomadaMap mapaTomadas[TOTAL_TOMADAS];

#endif
