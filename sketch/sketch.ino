/**
 * \file sketch.ino
 * \date 2023-12-11
 * \author Maria Camila Hoyos Gomez
            Santiago Alexander Dorado Gomez
 * \brief ejecucion completa.
 *
 * \par Copyright
 * Information contained herein is proprietary to and constitutes valuable
 * confidential trade secrets of Unicauca, and
 * is subject to restrictions on use and disclosure.
 *
 * \par
 * Copyright (c) Unicauca 2023. All rights reserved.
 *
 * \par
 * The copyright notices above do not evidence any actual or
 * intended publication of this material.
 ******************************************************************************
 */

 /** \addtogroup sketch
 * @{
 */

#include "pinout.h"
#include "StateMachineLib.h"
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Wire.h>
#include "AsyncTaskLib.h"

const String password = "1805";

int attempts = 0;
int buzzer = 7;
int tempo = 140;
int wholenote = (60000 * 4) / tempo;
int divider = 0, noteDuration = 0;
int melodia[] = {
    NOTE_E7, 4, NOTE_E7, 4, 0, 4, NOTE_E7, 4, 0, 4, NOTE_C7, 4,
    NOTE_E7, 4, 0, 4, NOTE_G7, 4, 0, 4, 0, 4, NOTE_G6, 4, 0, 4,
    0, 4, NOTE_C7, 4, 0, 4, NOTE_G6, 4, 0, 4, 0, 4,
    NOTE_E6, 4, 0, 4, 0, 4, NOTE_A6, 4, 0, 4, NOTE_B6, 4, 0, 4,
    NOTE_AS6, 4, NOTE_A6, 4, 0, 4, NOTE_G6, 2, 0, 8};
int notes = sizeof(melodia) / sizeof(melodia[0]) / 2;

char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String Escribir();

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void seguridad();
AsyncTask asyncTaskSeguridad(0, seguridad);

void monitorLu();
AsyncTask asyncTask2MonitorLu(0, true, monitorLu);

void alerta();
AsyncTask asyncTask3Alerta(0, true, alerta);

void temperatura();
AsyncTask asyncTask4Temperatura(500, true, temperatura);

void alarma();
AsyncTask asyncTask4Alarma(0, true, alarma);

void timeoutT3();
AsyncTask asyncTaskTime3(3000, timeoutT3);

void timeoutT4();
AsyncTask asyncTaskTime4(4000, timeoutT4);

void timeoutT5();
AsyncTask asyncTaskTime5(5000, timeoutT5);

void timeoutT6();
AsyncTask asyncTaskTime6(6000, timeoutT6);

enum State
{
    INIT = 0,
    BLOQUEO = 1,
    MONITOR_LUZ = 2,
    ALERTA = 3,
    MONITOR_TEMP = 4,
    ALARMA = 5
};


enum Input
{
    CORRECTO = 0,
    BLOQUEADO = 1,
    TIME_OUT = 2,
    LUZ = 3,
    TEMP = 4,
    Unknown = 5,
};

StateMachine stateMachine(6, 9);
Input currentInput;

void setup()
{
    Serial.begin(9600);
    pinMode(pingreen, OUTPUT);
    pinMode(pinblue, OUTPUT);
    pinMode(pinred, OUTPUT);

    Serial.println("Starting State Machine...");
    setup_State_Machine();

    Serial.println("Start Machine Started");
    stateMachine.SetState(State::INIT, false, true);
    lcd.begin(16, 2);
}

void loop()
{
    asyncTaskSeguridad.Update();
    asyncTask2MonitorLu.Update();
    asyncTask3Alerta.Update();
    asyncTask4Temperatura.Update();
    asyncTask4Alarma.Update();

    asyncTaskTime3.Update();
    asyncTaskTime4.Update();
    asyncTaskTime5.Update();
    asyncTaskTime6.Update();

    stateMachine.Update();
}

/**
 * brief Funcion timeoutT4 para salir del estado alerta
 */
void timeoutT4()
{
    currentInput = Input::TIME_OUT;
}

/**
 * brief Funcion timeoutT3 para salir del estado monitorLuz
 */
void timeoutT3()
{
    currentInput = Input::TIME_OUT;
}

/**
 * brief Funcion timeoutT6 para salir del estado temperatura
 */
void timeoutT6()
{
    currentInput = Input::TIME_OUT;
}

/**
 * brief Funcion timeoutT5 para salir del estado alarma
 */
void timeoutT5()
{
    currentInput = Input::TIME_OUT;
}

/**
 * brief Funcion Escribir para manejar los datos ingresados para la clave
 */
String Escribir()
{
    lcd.print("Ingrese la clave:");
    lcd.setCursor(0, 1);
    String result = "";
    char key;
    while (true)
    {
        key = keypad.getKey();
        if (key >= '0' && key <= '9')
        {
            lcd.print("*");
            result += key;
        }
        else if (key == '#')
        {
            break;
        }
    }
    lcd.clear();
    return result;
}

/**
 * brief Funcion Seguridad para validar la clave
 */
void seguridad()
{
    const int maxAttempts = 3;

    for (int intentos = 0; intentos < maxAttempts; ++intentos)
    {

        String enteredPassword = Escribir();

        if (enteredPassword.equals(password))
        {
            lcd.print("Clave");
            lcd.setCursor(0, 1);
            lcd.print("correcta");
            digitalWrite(pingreen, HIGH);
            delay(1000);
            digitalWrite(pingreen, LOW);
            currentInput = Input::CORRECTO;
            return;
        }
        else
        {
            lcd.print("Clave");
            lcd.setCursor(0, 1);
            lcd.print("incorrecta");
            digitalWrite(pinblue, HIGH);
            delay(1000);
            digitalWrite(pinblue, LOW);
            delay(1000);
            lcd.clear();
        }
    }
    lcd.print("Bloqueado");
    digitalWrite(pinred, HIGH);
    delay(1000);
    digitalWrite(pinred, LOW);
    currentInput = Input::BLOQUEADO;
    delay(1000);
    lcd.clear();
}

/**
 * brief Funcion MonitorLu para leer el valor del sensor de luz
 */
void monitorLu()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    int outputValue = analogRead(photocellPin);
    lcd.print("Luz: ");
    lcd.setCursor(0, 1);
    lcd.print(outputValue);
    Serial.println(outputValue);
    delay(1000);

    if (outputValue < 40)
    {
        currentInput = Input::LUZ;
    }
}

/**
 * brief Funcion alerta para mostrar un mensaje de alerta en el LCD
 */
void alerta()
{
    lcd.print("Alerta");
    digitalWrite(pinred, HIGH);
    delay(500);
    digitalWrite(pinred, LOW);
    delay(200);
}

/**
 * brief Funcion alerta para leer el sensor de temperatura
 */
void temperatura()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    int analogValue = analogRead(DHTPIN);
    float celsius = 1 / (log(1 / (1023. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;
    Serial.print(" Temperatura:");
    Serial.print(celsius);
    Serial.print("°C");
    lcd.print(" Temperatura:");
    lcd.setCursor(0, 1);
    lcd.print(celsius);
    delay(1000);

    if (celsius > 100)
    {
        currentInput = Input::TEMP;
    }
    Serial.println();
}

/**
 * brief Funcion alarma para iniciar la alarma
 */
void alarma(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Alarma");
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    divider = melodia[thisNote + 1];

    if (divider > 0) {
      noteDuration = (wholenote) / divider;

    } else if (divider < 0) {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;
    }

    tone(buzzer, melodia[thisNote], noteDuration * 0.9);
    delay(noteDuration);
    noTone(buzzer);
  }
}