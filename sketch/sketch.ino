/**
 *@authores Maria Camila Hoyos Gomez - Santiago Dorado Gomez
 */

// Archivos y Bibliotecas
#include "pinout.h"
#include "StateMachineLib.h"
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Wire.h>
#include "AsyncTaskLib.h"

const String password = "1805";

/********************************************/ /**
*  Define global variables
***********************************************/
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
    NOTE_AS6, 4, NOTE_A6, 4, 0, 4, NOTE_G6, 2, 0, 8,
    NOTE_E7, 4, NOTE_E7, 4, 0, 4, NOTE_E7, 4, 0, 4, NOTE_C7, 4,
    NOTE_E7, 4, 0, 4, NOTE_G7, 4, 0, 4, 0, 4, NOTE_G6, 4, 0, 4,
    0, 4, NOTE_C7, 4, 0, 4, NOTE_G6, 4, 0, 4, 0, 4,
    NOTE_E6, 4, 0, 4, 0, 4, NOTE_A6, 4, 0, 4, NOTE_B6, 4, 0, 4,
    NOTE_AS6, 4, NOTE_A6, 4, 0, 4, NOTE_G6, 2, 0, 8};
int notes = sizeof(melodia) / sizeof(melodia[0]) / 2;

/**
 * Keypad definicion
 */
char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String Escribir();

// Configuración de la pantalla LCD
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

/**
 *  Definicion de los estados State Machine
 */
enum State
{
    INIT = 0,
    BLOQUEO = 1,
    MONITOR_LUZ = 2,
    ALERTA = 3,
    MONITOR_TEMP = 4,
    ALARMA = 5
};

/**
 *  Definicion de las entradas State Machine
 */
enum Input
{
    CORRECTO = 0,
    BLOQUEADO = 1,
    TIME_OUT = 2,
    LUZ = 3,
    TEMP = 4,
    Unknown = 5,
};


/*! Crea nuevos StateMachine 4 estados y 6 transiciones */
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

/**
 * @brief Base for all architecture implementation classes.
 */
void loop()
{
    // Actualizar tareas asincrónicas
    asyncTaskSeguridad.Update();
    asyncTask2MonitorLu.Update();
    asyncTask3Alerta.Update();
    asyncTask4Temperatura.Update();
    asyncTask4Alarma.Update();

    asyncTaskTime3.Update();
    asyncTaskTime4.Update();
    asyncTaskTime5.Update();
    asyncTaskTime6.Update();

    // Actualizar máquina de estado
    stateMachine.Update();
}

/*F**************************************************************************
 * NAME: timeoutT4
 *----------------------------------------------------------------------------
 * PARAMS:
 * return:   none
 *----------------------------------------------------------------------------
 * PURPOSE:
 * Define la funcion timeoutT4 para salir del estado alerta
 *----------------------------------------------------------------------------
 * NOTE:
 *
 *****************************************************************************/
void timeoutT4()
{
    currentInput = Input::TIME_OUT;
}

/*F**************************************************************************
 * NAME: timeoutT3
 *----------------------------------------------------------------------------
 * PARAMS:
 * return:   none
 *----------------------------------------------------------------------------
 * PURPOSE:
 * Define la funcion timeoutT3 para salir del estado monitorLuz
 *----------------------------------------------------------------------------
 * NOTE:
 *
 *****************************************************************************/
void timeoutT3()
{
    currentInput = Input::TIME_OUT;
}

/*F**************************************************************************
 * NAME: timeoutT6
 *----------------------------------------------------------------------------
 * PARAMS:
 * return:   none
 *----------------------------------------------------------------------------
 * PURPOSE:
 * Define la funcion timeoutT6 para salir del estado temperatura
 *----------------------------------------------------------------------------
 * NOTE:
 *
 *****************************************************************************/
void timeoutT6()
{
    currentInput = Input::TIME_OUT;
}

/*F**************************************************************************
 * NAME: timeoutT5
 *----------------------------------------------------------------------------
 * PARAMS:
 * return:   none
 *----------------------------------------------------------------------------
 * PURPOSE:
 * Define la funcion timeoutT5 para salir del estado alarma
 *----------------------------------------------------------------------------
 * NOTE:
 *
 *****************************************************************************/
void timeoutT5()
{
    currentInput = Input::TIME_OUT;
}

/*F**************************************************************************
 * NAME: Escribir
 *----------------------------------------------------------------------------
 * PARAMS:
 * return:   none
 *----------------------------------------------------------------------------
 * PURPOSE:
 * Define la funcion Escribir para manejar los datos ingresados para la clave
 *----------------------------------------------------------------------------
 * NOTE:
 *
 *****************************************************************************/
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

/*F**************************************************************************
 * NAME: Seguridad
 *----------------------------------------------------------------------------
 * PARAMS:
 * return:   none
 *----------------------------------------------------------------------------
 * PURPOSE:
 * Define la funcion Seguridad para validar la clave
 *----------------------------------------------------------------------------
 * NOTE:
 *
 *****************************************************************************/
void Seguridad()
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
    lcd.print(currentInput);
    delay(1000);
    lcd.clear();
}

/*F**************************************************************************
 * NAME: MonitorLu
 *----------------------------------------------------------------------------
 * PARAMS:
 * return:   none
 *----------------------------------------------------------------------------
 * PURPOSE:
 * Define la funcion MonitorLu para leer el valor del sensor de luz 
 *----------------------------------------------------------------------------
 * NOTE:
 *
 *****************************************************************************/
void MonitorLu()
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

/*F**************************************************************************
 * NAME: alerta
 *----------------------------------------------------------------------------
 * PARAMS:
 * return:   none
 *----------------------------------------------------------------------------
 * PURPOSE:
 * Define la funcion alerta para mostrar un mensaje de alerta en el LCD
 *----------------------------------------------------------------------------
 * NOTE:
 *
 *****************************************************************************/
void alerta()
{
    lcd.print("Alerta   ");
    digitalWrite(pinred, HIGH);
    delay(500);
    digitalWrite(pinred, LOW);
    delay(200);
}

/*F**************************************************************************
 * NAME: alerta
 *----------------------------------------------------------------------------
 * PARAMS:
 * return:   none
 *----------------------------------------------------------------------------
 * PURPOSE:
 * Define la funcion alerta para leer el sensor de temperatura
 *----------------------------------------------------------------------------
 * NOTE:
 *
 *****************************************************************************/
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

    if (celsius > 30)
    {
        currentInput = Input::TEMP;
    }
    Serial.println();
}

/*F**************************************************************************
 * NAME: alarma
 *----------------------------------------------------------------------------
 * PARAMS:
 * return:   none
 *----------------------------------------------------------------------------
 * PURPOSE:
 * Define la funcion alarma para iniciar la alarma
 *----------------------------------------------------------------------------
 * NOTE:
 *
 *****************************************************************************/
void alarma(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Alarma");
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calcula la duración de cada nota
    divider = melodia[thisNote + 1];

    if (divider > 0) {
      noteDuration = (wholenote) / divider;

    } else if (divider < 0) {
      //aumenta la duración a la mitad para las notas punteadas
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;
    }

    tone(buzzer, melodia[thisNote], noteDuration * 0.9);
    delay(noteDuration);
    noTone(buzzer);
  }
}