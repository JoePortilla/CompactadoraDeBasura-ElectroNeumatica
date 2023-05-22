/**
 * \file CompactadoraElectroneumatica main.cpp
 * \brief Codigo convertido de Ladder a Arduino para el funcionamiento de una compactadora
 * de basura automatizada mediante electroneumática.
 * \author Joseph Santiago Portilla. Ing. Electrónico - @JoePortilla
 */

// 1. BIBLIOTECAS
#include <Arduino.h>

// 2. DEFINICIÓN DE VARIABLES Y CONSTANTES
// 2.1. GPIO DE ENTRADAS
// Botones
const uint8_t START = 33; // Boton de Inicio
const uint8_t STOP = 25;  // Boton de Paro
// Sensores
const uint8_t SA0 = 26; // Sensor de contracción. Cilindro A
const uint8_t SA1 = 27; // Sensor de expansión. Cilindro A
const uint8_t SB0 = 14; // Sensor de contracción. Cilindro B
const uint8_t SB1 = 12; // Sensor de expansión. Cilindro B
const uint8_t SC0 = 13; // Sensor de contracción. Cilindro C
const uint8_t SC1 = 15; // Sensor de expansión. Cilindro C
const uint8_t SL = 4;   // Sensor de llenado del tanque

// 2.2. GPIO DE SALIDAS
// Bobinas
const uint8_t KM = 16; // Bobina de control del Motor
// Electrovalvulas
const uint8_t EVA = 17; // Electroválvula de control del cilindro A
const uint8_t EVB = 5;  // Electroválvula de control del cilindro B
const uint8_t EVC = 18; // Electroválvula de control del cilindro C

// 2.3. MEMORIAS DE CONTROL LADDER
bool M1 = 0; // Memoria 1
bool M2 = 0; // Memoria 2
bool M3 = 0; // Memoria 3
bool M4 = 0; // Memoria 4
bool M5 = 0; // Memoria 5
bool M6 = 0; // Memoria 6
bool M7 = 0; // Memoria 7
bool M8 = 0; // Memoria 8

// 2.4. BANDERAS DE ESTADO
bool KMstatus = 0;
bool EVAstatus = 0;
bool EVBstatus = 0;
bool EVCstatus = 0;

// 2.5. TIEMPOS
const uint16_t intervalo = 1000; // Intervalo de tiempo para la muestra de estados en el monitor serial [ms].
uint32_t tActual = 0;            // Almacena el tiempo actual de ejecución.
uint32_t tPrevio = 0;            // Almacena el ultimo tiempo en que el contador fue incrementado.

void setup()
{
  // Inicializar la comunicación serial a 115200 baudios.
  Serial.begin(115200);
  // Definición de pines como entradas, y activar la resistencia de pull-down interna.
  pinMode(START, INPUT_PULLDOWN);
  pinMode(STOP, INPUT_PULLDOWN);
  pinMode(SL, INPUT_PULLDOWN);
  pinMode(SA0, INPUT_PULLDOWN);
  pinMode(SA1, INPUT_PULLDOWN);
  pinMode(SB0, INPUT_PULLDOWN);
  pinMode(SB1, INPUT_PULLDOWN);
  pinMode(SC0, INPUT_PULLDOWN);
  pinMode(SC1, INPUT_PULLDOWN);
  // Definición de pines como salidas.
  pinMode(KM, OUTPUT);
  pinMode(EVA, OUTPUT);
  pinMode(EVB, OUTPUT);
  pinMode(EVC, OUTPUT);
  // Inicializar salidas apagadas
  digitalWrite(KM, 0);
  digitalWrite(EVA, 0);
  digitalWrite(EVB, 0);
  digitalWrite(EVC, 0);
}

void loop()
{
  // ESCALONES DE LOGICA
  // Escalon 1
  if ((digitalRead(START) || M1) && !(digitalRead(STOP)))
  {
    M1 = 1;
  }
  else
  {
    M1 = 0;
  }

  // Escalon 2
  if (!M8 && M1 && (digitalRead(SL) || M2))
  {
    M2 = 1;
  }
  else
  {
    M2 = 0;
  }

  // Escalon 3
  if (M2 && (digitalRead(SA1) || M3))
  {
    M3 = 1;
  }
  else
  {
    M3 = 0;
  }

  // Escalon 4
  if (M3 && (digitalRead(SA0) || M4))
  {
    M4 = 1;
  }
  else
  {
    M4 = 0;
  }

  // Escalon 5
  if (M4 && (digitalRead(SB1) || M5))
  {
    M5 = 1;
  }
  else
  {
    M5 = 0;
  }

  // Escalon 6
  if (M5 && (digitalRead(SC1) || M6))
  {
    M6 = 1;
  }
  else
  {
    M6 = 0;
  }

  // Escalon 7
  if (M6 && (digitalRead(SC0) || M7))
  {
    M7 = 1;
  }
  else
  {
    M7 = 0;
  }

  // Escalon 8
  if (M7 && (digitalRead(SB0) || M8))
  {
    M8 = 1;
  }
  else
  {
    M8 = 0;
  }

  // ESCALONES DE CONTROL DE SALIDAS
  // En la función digitalWrite se niega el estado ya que los modulos relé utilizados para
  // activar las cargas de potencias estan configurados de esa manera.
  // Salida Bobina Motor
  if (M1 && !M2)
  {
    KMstatus = 1;
    digitalWrite(KM, 0);
  }
  else
  {
    KMstatus = 0;
    digitalWrite(KM, 1);
  }

  // Salida Electrovalvula A
  if (M2 && !M3)
  {
    EVAstatus = 1;
    digitalWrite(EVA, 0);
  }
  else
  {
    EVAstatus = 0;
    digitalWrite(EVA, 1);
  }

  // Salida Electrovalvula B
  if (M4 && !M7)
  {
    EVBstatus = 1;
    digitalWrite(EVB, 0);
  }
  else
  {
    EVBstatus = 0;
    digitalWrite(EVB, 1);
  }

  // Salida Electrovalvula C
  if (M5 && !M6)
  {
    EVCstatus = 1;
    digitalWrite(EVC, 0);
  }
  else
  {
    EVCstatus = 0;
    digitalWrite(EVC, 1);
  }

  // DEBUGGING
  // Se muestra en el monitor serial los estados de las entradas y salidas del sistema.
  tActual = millis();
  if (tActual - tPrevio > intervalo)
  {
    Serial.printf("START:%d-STOP:%d-SL:%d\na0:%d-a1:%d-b0:%d-b1:%d-c0:%d-c1:%d\nM1:%d-EVA:%d-EVB:%d-EVC:%d\n\n", digitalRead(START), digitalRead(STOP), digitalRead(SL), digitalRead(SA0), digitalRead(SA1), digitalRead(SB0), digitalRead(SB1), digitalRead(SC0), digitalRead(SC1), KMstatus, EVAstatus, EVBstatus, EVCstatus);
    tPrevio = tActual;
  }
}