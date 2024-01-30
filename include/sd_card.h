/*
Maneja la escritura y lectura de la tarjeta SD
*/

#ifndef SDCARD_
#define SDCARD_

#include <Arduino.h>
#include "SD.h"
#include "SPI.h"
#include "FS.h"
#include "global_var.h"

// Micro SD conexiones
#define SD_CS 13
#define SPI_MOSI 17
#define SPI_MISO 16
#define SPI_SCK 4
//////////////////////////////

File file;

// Delete File
void deleteFile(fs::FS &fs, const char *path)
{
    Serial.printf("Delete file %s\n", path);
    if (fs.remove(path))
    {
        Serial.println("File deleted");
    }
    else
    {
        Serial.println("Delete Failed");
    }
}
/////////////////////////////////

// Inicio SD
void SDinit()
{
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SD_CS);

    if (!SD.begin(SD_CS))
    {
        Serial.println("Error Accesing micro SD!");
        while (true);
    }
    if (SD.exists("/record.wav"))
    {
        deleteFile(SD, "/record.wav");
        Serial.println("Borrado sd inicial");
        delay(100);
    }
}
/////////////////////////////////////////////////////////

#endif