#include "tomadas_map.h"

/*
PCF[N] - Pinos de 0 a 7
|--------------------------------------------------------|
|               0 1  2 3  4 5  6 7                       |
|               o o  o o  o o  o o                       |
|--------------------------------------------------------|
Tomada 1 será acionada via pino 0
Pulso tomada 1 será acionada via pino 1

Tomada 2 será acionada via pino 2
Pulso tomada 2 será acionada via pino 3

Assim sucessivamente pelos PCF
*/

TomadaMap mapaTomadas[TOTAL_TOMADAS] = {
    {0,0,0,1}, // PCF0 (0x20), tomada 1
    {0,2,0,3}, // PCF0 (0x20), tomada 2
    {0,4,0,5}, // PCF0 (0x20), tomada 3
    {0,6,0,7}, // PCF0 (0x20), tomada 4

    {1,0,1,1}, // PCF1 (0x21), tomada 1
    {1,2,1,3}, // PCF1 (0x21), tomada 2
    {1,4,1,5}, // PCF1 (0x21), tomada 3
    {1,6,1,7}, // PCF1 (0x21), tomada 4

    {2,0,2,1}, // PCF2 (0x22), tomada 1
    {2,2,2,3}, // PCF2 (0x22), tomada 2
    {2,4,2,5}, // PCF2 (0x22), tomada 3
    {2,6,2,7}, // PCF2 (0x22), tomada 4
};
