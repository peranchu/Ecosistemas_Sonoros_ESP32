/*
Control y funciones led ws2812b Libreria
Adafruit Neopixel
*/

#ifndef LED_
#define LED_

 #include <Adafruit_NeoPixel.h>

 #define NUM_LEDS 4
 #define PIN_LED 5

 int brillo = 0;
 int ValorPausa = 0;

 Adafruit_NeoPixel leds (NUM_LEDS, PIN_LED, NEO_GRB + NEO_KHZ800);


//Inicio secuencia LED
void inicioLEDS(){
    Serial.println("inicio LEDS");

    leds.begin();
    leds.setBrightness(150);

    leds.clear();

    for(int i = 0; i < NUM_LEDS; i++){
        leds.setPixelColor(i, leds.Color(0, 0, 255));  //Azul
        leds.show();
        delay(300);
    }

    for(int i = NUM_LEDS; i >= 0; i--){
        leds.setPixelColor(i, leds.Color(0, 0, 0));
        leds.show();
        delay(300);
    }

    delay(1000);
} 
////////// FIN INICIO SECUENCIA LEDS /////////////////////////

//Estado LED Grabación
void Led_Record(){
    leds.setBrightness(180);
    leds.fill(leds.Color(255, 0, 0), 0, 4);  //Rojo
    leds.show();
}
///////// FIN ESTADO GRABACIÓN //////////////

//Estado LED PLAY
void Led_Play(){
    leds.setBrightness(180);
    leds.fill(leds.Color(0, 255, 0), 0, 4);  //verde
    leds.show();
}
//////////// FIN ESTADO PLAY ////////////////

//Estado LED Espera
void Led_Espera(){
    leds.fill(leds.Color(255, 0, 255), 0, 4); //magenta
    brillo = exp((sin(millis() / 2000.0 * PI)) - 0.36787944) * 108.0; //funcion latido
    leds.setBrightness(brillo);
    leds.show();
}
/////////////////////////////////////

#endif