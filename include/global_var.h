/*
Variables globales usadas por el sistema
*/
#ifndef GLOBAL_
#define GLOBAL_

#define I2S_PORT I2S_NUM_0


unsigned int t_espera_rand = 0;

const char filename[] = "/record.wav";

bool recording = false;
static bool playing = false;

bool StateTempPausa = 0;
bool StateTempRec = 0;
bool StatePlay = 0;
bool FinRepro = 0;

//Estados del sistema
enum class EstadoSistema : uint8_t{
  INICIO,
  GRABACION,
  PAUSA,
  PLAY
};

static EstadoSistema currState = EstadoSistema::INICIO;
///////////////////////////////

#endif