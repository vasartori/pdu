# ESP32 PDU Controller

English version available [here](README.en.md).

Este projeto implementa uma **PDU (Power Distribution Unit)** com ESP32, permitindo o controle remoto de 12 tomadas por meio de HTTP. Cada tomada possui:

- Um relé de **energia contínua** (mantém o equipamento ligado/desligado);
- Um relé de **pulso ACPI** (simula o botão "power" por 50 ms).

O controle é feito com módulos **PCF8574 via I2C**, usando uma interface simples via rotas HTTP e respostas em **JSON**.

## Por que isso existe?

Tenho um home lab relativamente grande e uma das dores ao utilizar o Canonical MaaS é ligar e desligar os PCs.

Como não encontrei nenhuma solução pronta e barata, resolvi construir o meu próprio PDU.

---

## Funcionalidades

- Controle HTTP individual de 12 tomadas
- Comandos para ligar, desligar e acionar o pulso ACPI (ligar ou desligar via pulso)
- API REST com respostas em JSON
- Consulta de status de energia e estado ACPI por tomada

---

## API HTTP

Todas as rotas seguem o padrão:

http://<ip_do_esp>/outlet/<n>/<ação>


| Método | Endpoint                 | Descrição                                      |
|--------|--------------------------|------------------------------------------------|
| POST   | `/outlet/<n>/poweron`    | Liga a tomada `n`                              |
| POST   | `/outlet/<n>/poweroff`   | Desliga a tomada `n`                           |
| POST   | `/outlet/<n>/acpi-on`    | Envia pulso ACPI de "ligar" para a tomada `n`  |
| POST   | `/outlet/<n>/acpi-off`   | Envia pulso ACPI de "desligar" para a tomada `n` |
| GET    | `/outlet/<n>/status`     | Retorna o status da tomada `n` em JSON         |

---

## Exemplos de respostas

**Sucesso (201):**

```json
{ "status": "Outlet powered on" }
```

**Erro (400)**
```json
{ "error": "Invalid outlet number" }
```

**Status (200)**
```json
{
  "outlet": 3,
  "power": true,
  "acpi": false
}
```

## Hardware
- ESP32 DevKit
- 3x PCF8574 (expansores I2C)
- Módulos de relé
- Fonte 5V com corrente suficiente (ex: fonte tipo colmeia 5V 5A - usada neste projeto)

## Mapeamento de pinos
- I2C:
    - SDA = GPIO 22
    - SCL = GPIO 23

Endereços dos PCF8574:

- 0x20, 0x21, 0x22

Cada tomada e seu respectivo botão (relé de pulso) são mapeados de forma explícita no arquivo tomadas_map.cpp.

## Desenvolvendo
O projeto utiliza o [PlatformIO](https://platformio.org/), integrado como extensão do VSCode.

As dependências estão listadas no arquivo [platformio.ini](platformio.ini),  que inclui:

- ESPAsyncWebServer
- AsyncTCP
- ArduinoJson
- PCF8574

---

Sinta-se a vontade para enviar PR's
