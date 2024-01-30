#ifndef REPRO_
#define REPRO_

#include <Arduino.h>
#include <AudioTools.h>
#include "led.h"
#include "global_var.h"
#include "sd_card.h"
#include "AudioLibs/AudioSourceSD.h"
#include "AudioCodecs/CodecWAV.h"

#define SD_CARD_CS 13

const char *startFilePath = "/";
const char *ext = "wav";

AudioSourceSD source(startFilePath, ext, SD_CARD_CS);
I2SStream i2s_play;
VolumeStream Vol_play (i2s_play);
WAVDecoder decoder;
AudioPlayer player(source, Vol_play, decoder);


void config_i2s_play()
{
    auto cfg_play = i2s_play.defaultConfig(TX_MODE);
    cfg_play.i2s_format = I2S_STD_FORMAT;
    cfg_play.bits_per_sample = 16;
    cfg_play.sample_rate = 44100;
    cfg_play.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT;
    cfg_play.channels = 1;
    cfg_play.pin_ws = 27;   // LRC
    cfg_play.pin_bck = 19;  // BCLK
    cfg_play.pin_data = 18; // DIN
    i2s_play.begin(cfg_play);

    //ganancia de salida del audio
    auto cfg_vol = Vol_play.defaultConfig();
    cfg_vol.sample_rate = 44100;
    cfg_vol.bits_per_sample = 16;
    cfg_vol.channels = 1;
    cfg_vol.allow_boost = true;
    Vol_play.begin(cfg_vol);
    Vol_play.setVolume(2.0);
}

void Reproducir()
{
    if (!playing)
    {
        Serial.println("Config Play");

        config_i2s_play();

        player.begin();
        player.setVolume(1.0);
        player.isAutoFade();

        playing = true;

        Led_Play();
    }
    if (!player)
    {
        FinRepro = 1;

        i2s_play.end();
        decoder.end();
        player.end();

        playing = false;
        Serial.println("Stop PLAY");
        

        deleteFile(SD, "/record.wav"); // Borra el archivo guardado anteriormente

        leds.clear();
        leds.show();
    }
    else
    {
        player.copy();
    }
}
////////////////////////////////////


#endif