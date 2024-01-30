/*
Configuración protocolo i2s para grabación de audio a través
del micrófono INMP441, utilizando la libreria AudioTools
*/

#ifndef GRABACION_
#define GRABACION_

#include "Arduino.h"
#include "AudioTools.h"
#include "sd_card.h"
#include "global_var.h"

I2SStream i2s_rec;
EncodedAudioStream encoder(&file, new WAVEncoder());   // Salvar como WAV
VolumeStream gain(encoder);                           //Ganancia Micrófono
FormatConverterStream conv(gain);

StreamCopy copier_rec(conv, i2s_rec);

AudioInfo from(44100, 1, 32);
AudioInfo to(44100, 1, 16);
/////////////////////////////////////////

//Configuración grabación
void config_i2s_Rec()
{
  auto cfg_rec = i2s_rec.defaultConfig(RX_MODE);
  cfg_rec.i2s_format = I2S_STD_FORMAT;
  cfg_rec.bits_per_sample = 32;
  cfg_rec.channels = 1;
  cfg_rec.sample_rate = 44100;
  cfg_rec.pin_ws = 22;
  cfg_rec.pin_bck = 26;
  cfg_rec.pin_data = 21;

  i2s_rec.begin(cfg_rec);

  //Configuracion encoder
  auto cfg_out = encoder.defaultConfig();
  cfg_out.bits_per_sample = 16;
  cfg_out.channels = 1;
  cfg_out.sample_rate = 44100;

  encoder.begin(cfg_out);

  //Convierte de 32 a 16 bits para la lectura
  conv.begin(from, to);

  //Ganancia Micrófono
  auto gain_cfg = gain.defaultConfig();
  gain_cfg.sample_rate = 44100;
  gain_cfg.channels = 1;
  gain_cfg.bits_per_sample = 32;
  gain_cfg.allow_boost = true; // permite valores más altos de 1.0
  gain.begin(gain_cfg);
  gain.setVolume(7.0);
}
///////////// FIN CONFIGURACIÓN GRABACIÓN /////////////////////////////

//Función Grabación
void i2s_record()
{
  if (!recording)
  {
    leds.clear();
    leds.show();

    file.close();

    file = SD.open("/record.wav", FILE_WRITE);

    config_i2s_Rec();  //Llama a la configuración del i2s

    copier_rec.begin(conv, i2s_rec);

    Serial.println("GRABANDO");

    recording = true;

    Led_Record();
  }
  copier_rec.copy();
}
///////////////// FIN FUNCIÓN GRABACIÓN ////////////////

//Final grabación, se ejecuta cuando termina la grababción
void EndRecord()
{
  file.close();

  //Cierra estancias i2s
  i2s_rec.end();
  encoder.end();
  gain.end();
  conv.end();
  copier_rec.end();

  recording = false;

  Serial.println("STOP RECORDING");

  leds.clear();
  leds.show();
}
////////////////////////////////


#endif