
void setup_State_Machine()
{
    /**
     * Configuracion de transiciones
     */
    stateMachine.AddTransition(INIT, BLOQUEO, []()
                               { return currentInput == BLOQUEADO; });
    stateMachine.AddTransition(INIT, MONITOR_LUZ, []()
                               { return currentInput == CORRECTO; });
    stateMachine.AddTransition(BLOQUEO, INIT, []()
                               { return currentInput == TIME_OUT; });

    stateMachine.AddTransition(MONITOR_LUZ, ALERTA, []()
                               { return currentInput == LUZ; });
    stateMachine.AddTransition(ALERTA, MONITOR_LUZ, []()
                               { return currentInput == TIME_OUT; });
    stateMachine.AddTransition(MONITOR_LUZ, MONITOR_TEMP, []()
                               { return currentInput == TIME_OUT; });
    stateMachine.AddTransition(MONITOR_TEMP, MONITOR_LUZ, []()
                               { return currentInput == TIME_OUT; });

    stateMachine.AddTransition(MONITOR_TEMP, ALARMA, []()
                               { return currentInput == TEMP; });
    stateMachine.AddTransition(ALARMA, MONITOR_TEMP, []()
                               { return currentInput == TIME_OUT; });

    /**
     * Configuracion de eventos de estado
     */
    stateMachine.SetOnEntering(INIT, inputInit);
    stateMachine.SetOnEntering(BLOQUEO, inputBloqueado);
    stateMachine.SetOnEntering(MONITOR_LUZ, inputLuz);
    stateMachine.SetOnEntering(ALERTA, inputALerta);
    stateMachine.SetOnEntering(MONITOR_TEMP, inputTemperatura);
    stateMachine.SetOnEntering(ALARMA, inputAlarma);

    stateMachine.SetOnLeaving(INIT, outputInit);
    stateMachine.SetOnLeaving(BLOQUEO, outputBlo);
    stateMachine.SetOnLeaving(MONITOR_LUZ, outputLuz);
    stateMachine.SetOnLeaving(ALERTA, outputAlerta);
    stateMachine.SetOnLeaving(MONITOR_TEMP, outputTemperatura);
    stateMachine.SetOnLeaving(ALARMA, outputAlarma);
}

/*F**************************************************************************
 * NAME: inputInit
 *----------------------------------------------------------------------------
 * PARAMS:
 * return:   none
 *----------------------------------------------------------------------------
 * PURPOSE:
 * Define la funcion inputInit para establecer el estado inicial
 *----------------------------------------------------------------------------
 * NOTE:
 *
 *****************************************************************************/
void inputInit()
{
    currentInput = Input::Unknown;
    Serial.println("Estado INIT");
    Serial.println("A   B   C   D   E   F");
    Serial.println("    X                ");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0, 0);
    asyncTaskSeguridad.Start();
}
void outputInit()
{
    asyncTaskSeguridad.Stop();
}

/*F**************************************************************************
 * NAME: inputBloqueado
 *----------------------------------------------------------------------------
 * PARAMS:
 * return:   none
 *----------------------------------------------------------------------------
 * PURPOSE:
 * Define la funcion inputBloqueado para cuando se entra en estado BLOQUEADO
 *----------------------------------------------------------------------------
 * NOTE:
 *
 *****************************************************************************/
void inputBloqueado()
{
    currentInput = Input::Unknown;
    Serial.println("Estado BLOQUEADO");
    Serial.println("A   B   C   D   E   F");
    Serial.println("X                    ");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0, 0);
    asyncTaskTime5.Start();
}
void outputBlo()
{
    asyncTaskTime5.Stop();
}

/*F**************************************************************************
 * NAME: inputLuz
 *----------------------------------------------------------------------------
 * PARAMS:
 * return:   none
 *----------------------------------------------------------------------------
 * PURPOSE:
 * Define la funcion inputLuz para cuando se entra en estado monitoreo de luz
 *----------------------------------------------------------------------------
 * NOTE:
 *
 *****************************************************************************/
void inputLuz()
{
    Serial.println();
    currentInput = Input::Unknown;
    Serial.println(" Estado Luz");
    Serial.println("A   B   C   D   E   F");
    Serial.println("        X            ");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0, 0);
    asyncTask2MonitorLu.Start();
    asyncTaskTime4.Start();
}
void outputLuz()
{

    asyncTask2MonitorLu.Stop();
    asyncTaskTime4.Stop();
}

/*F**************************************************************************
 * NAME: inputALerta
 *----------------------------------------------------------------------------
 * PARAMS:
 * return:   none
 *----------------------------------------------------------------------------
 * PURPOSE:
 * Define la funcion inputALerta para cuando se entra en estado alerta
 *----------------------------------------------------------------------------
 * NOTE:
 *
 *****************************************************************************/
void inputALerta()
{
    currentInput = Input::Unknown;
    Serial.println("Estado Alerta Luz");
    Serial.println("A   B   C   D   E   F");
    Serial.println("            X        ");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0, 0);
    asyncTask3Alerta.Start();
    asyncTaskTime4.Start();
}
void outputAlerta()
{
    asyncTask3Alerta.Stop();
    asyncTaskTime4.Stop();
}

/*F**************************************************************************
 * NAME: inputTemperatura
 *----------------------------------------------------------------------------
 * PARAMS:
 * return:   none
 *----------------------------------------------------------------------------
 * PURPOSE:
 * Define la funcion inputTemperatura para cuando se entra en estado temperatura
 *----------------------------------------------------------------------------
 * NOTE:
 *
 *****************************************************************************/
void inputTemperatura()
{
    currentInput = Input::Unknown;
    Serial.println("Estado Temperatura");
    Serial.println("A   B   C   D   E   F");
    Serial.println("                X    ");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0, 0);
    asyncTask4Temperatura.Start();
    asyncTaskTime6.Start();
}
void outputTemperatura()
{
    asyncTask4Temperatura.Stop();
    asyncTaskTime6.Stop();
}

/*F**************************************************************************
 * NAME: inputAlarma
 *----------------------------------------------------------------------------
 * PARAMS:
 * return:   none
 *----------------------------------------------------------------------------
 * PURPOSE:
 * Define la funcion inputAlarma para cuando se entra en estado alarma
 *----------------------------------------------------------------------------
 * NOTE:
 *
 *****************************************************************************/
void inputAlarma()
{
    currentInput = Input::Unknown;
    Serial.println(" Estado Alarma");
    Serial.println("A   B   C   D   E   F");
    Serial.println("                    X");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0, 0);
    asyncTask4Alarma.Start();
    asyncTaskTime5.Start();
}
void outputAlarma()
{
    asyncTask4Alarma.Stop();
    asyncTaskTime5.Stop();
}
