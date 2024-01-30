/*
  ECOSISTEMAS POÉTICO SONOROS
  Honorino Garcia Enero 2024
*/

#include <Arduino.h>
#include <TaskScheduler.h>
#include "repro.h"
#include "grabacion.h"
#include "led.h"

//Funciones Tareas
void TareaInicio();
void TareaRecord();
void TareaPausa();
void TareaPlay();

void TempIni();
void TempPausa();
void TempRec();

//Creación Tareas
Task tarea_inicio(10, TASK_FOREVER, &TareaInicio);
Task tarea_record(5, TASK_FOREVER, &TareaRecord);
Task tarea_pausa(10, TASK_FOREVER, &TareaPausa);
Task tarea_play(10, TASK_FOREVER, &TareaPlay);

//Temnporizadores
Task tarea_temp_ini(1000, 7, &TempIni);
Task tarea_temp_pau(1000, 7, &TempPausa);
Task tarea_temp_rec(1000, 20, &TempRec);

Scheduler organizador;


void setup()
{
  Serial.begin(115200);
  while (!Serial);

  // inicio LEDS
  inicioLEDS();

  // Inicio SD
  SDinit();

  delay(1000);

  randomSeed(analogRead(34));

  t_espera_rand = random(5, 35);

  //Añade las tareas
  organizador.addTask(tarea_inicio);
  organizador.addTask(tarea_record);
  organizador.addTask(tarea_pausa);
  organizador.addTask(tarea_play);

  //Temporizadores
  //organizador.addTask(tarea_temp_ini);
  //organizador.addTask(tarea_temp_pau);
  //organizador.addTask(tarea_temp_rec);

  tarea_inicio.enable();
}

void loop()
{
  organizador.execute();
}

/////////////////////////////////////////
//Tarea Inicio
void TareaInicio(){
  switch (currState)
  {
  case EstadoSistema:: INICIO:
    if(StateTempPausa == 0){
      Serial.println("Sistema en Inicio");
      StateTempPausa = 1;
      organizador.addTask(tarea_temp_ini);
      tarea_temp_ini.enable();
      tarea_temp_ini.setIterations(t_espera_rand);
    }
    Led_Espera();
    break;
  }
}
/////////////////

//Tarea Record
void TareaRecord(){
  switch (currState)
  {
  case EstadoSistema::GRABACION:
    if(StateTempRec == 0){
      Serial.println("Estado Grabacion");
      StateTempRec = 1;
      organizador.addTask(tarea_temp_rec);
      tarea_temp_rec.restartDelayed();
      tarea_temp_rec.enable();
    }
    i2s_record();
    break;
  }
}
////////////////

//Tarea Pausa
void TareaPausa(){
  switch (currState)
  {
  case EstadoSistema::PAUSA:
    if(StateTempPausa == 0){
      Serial.println("Tarea Pausa");
      StateTempPausa = 1;
      organizador.addTask(tarea_temp_pau);
      tarea_temp_pau.setIterations(t_espera_rand);
      tarea_temp_pau.enable(); //Habilita temporizador
    }
    Led_Espera();
    break;
  }
}
/////////////

//Tarea Play
void TareaPlay(){
  switch (currState)
  {
  case EstadoSistema::PLAY:
    if(StatePlay == 0){
      Serial.println("Tarea PLAY");
      StatePlay = 1;
    }
    if(FinRepro == 0){  
        Reproducir();
    }
    else if(FinRepro == 1){
        StateTempPausa = 0;
        tarea_play.disable();
        tarea_inicio.enable();
        
        currState = EstadoSistema::INICIO;

        FinRepro = 0;
    }
    break;
  }
}


//Temporizadores///////////////////////
//Temp Inicio
void TempIni(){
  if(tarea_temp_ini.isLastIteration()){
    Serial.println("inicio disable");
    currState = EstadoSistema::GRABACION;
    leds.clear();
    leds.show();
    tarea_inicio.disable();
    tarea_temp_ini.disable();

    StateTempPausa = 0;

    tarea_record.enable();
  }
}
/////////////////

//Temp Grabación
void TempRec(){
  if(tarea_temp_rec.isLastIteration()){
    EndRecord();
    tarea_temp_rec.disable();
    tarea_record.disable();
    
    Serial.println("Grabacion disable");
    currState = EstadoSistema::PAUSA;
    leds.clear();
    leds.show();

    StateTempRec = 0;
  
    tarea_pausa.enable();
  }
}
//////////////////////////////////

//Temp Pausa
void TempPausa(){
  if(tarea_temp_pau.isLastIteration()){
    Serial.println("inicio disable");
    currState = EstadoSistema::PLAY;
    leds.clear();
    leds.show();
    tarea_inicio.disable();
    tarea_temp_pau.disable();

    tarea_play.enable();
  }
}
////////// FIN TEMPORIZADORES ///////