/**
    MyTpFinalThaninaAdda
    un projet de système de porte verrouillable
    Expérimentations sur l'utilisation d'un Keypad et un ESP32 et d'un écran Oled et un NéoPixel 8
    @file main.cpp
    @author Thanina Adda 
    @version 1.0 20/04/21  
    
    Historique des vérsions   
                            Versions  Date      Auteur      Déscription
                            1.0      20/04/21   Ta          Première version qui démontre l'utilisation des 4 éléments
                            
    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps = 
            adafruit/Adafruit GFX Library @ ^1.10.1    pour le Oled
            adafruit/Adafruit SSD1306 @ ^2.4.0         pour le Oled
            adafruit/Adafruit NeoPixel @ ^1.7.0        pour le Néo pixel
            chris--a/Keypad@^3.1.1                     pour le Keypad
            
    Classes du système

        Main      V1.0    qui contient le projet au complet
          Le Neo Pixel:
          in 25  le fil vert du Néo pixel
          NUMPIXELS      8 le nombre de Led que contient le Néo Pixel
          int delayval = 500; // délai d'une demi-seconde pour les leds de l'anneau.


          Le Keypad:
          numRows= 4;          //nombre de lignes sur le keypad
          numCols= 4;          //nombre de Colonnes sur le keypad

          Oled screen:
          #define SCREEN_WIDTH 128 // Largeur de l'écran OLED, en pixels
          #define SCREEN_HEIGHT 64 // Hauteur de l'écran OLED, en pixels


**/
// les librairies nécessaires.
#include <Arduino.h>
#include <Keypad.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#define PIN            25  // la pin 25

// le nombre de NeoPixels qui  sont attachés à l'Arduino
#define NUMPIXELS      8

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // délai d'une demi-seconde pour les leds de l'anneau

#define SCREEN_WIDTH 128 // Largeur de l'écran OLED, en pixels
#define SCREEN_HEIGHT 64 // Hauteur de l'écran OLED, en pixels

// Déclaration pour un écran SSD1306 connecté à I2C (broches SDA, SCL)
// Les broches pour I2C sont définies par la bibliothèque Wire. 
// Sur un arduino UNO: A4 (SDA), A5 (SCL)
// Sur un arduino MEGA 2560: 20 (SDA), 21 (SCL)
// Sur un arduino LEONARDO: 2 (SDA), 3 (SCL)
#define OLED_RESET     4 // Réinitialiser la broche # (ou -1 si vous partagez la broche de réinitialisation Arduino)

#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define O_Button 8             //Push Button
int Button = 8;           //Push Button

const byte numRows= 4;          //nombre de lignes sur le keypad
const byte numCols= 4;          //nombre de colonnes sur le keypad

char keymap[numRows][numCols]= 
{
{'1', '2', '3', 'A'}, 
{'4', '5', '6', 'B'}, 
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};

char keypressed;                 //l'emplacement des keys.
char code[] = {'C', '0', '8', '9'}; //le code par défaut.
int currentposition=0;
char check1[sizeof(code)];  // Où la nouvelle clé est stockée
char check2[sizeof(code)];  // Où la nouvelle clé est à nouveau stockée afin qu'elle soit comparée à la précédente

short a=0,i=0,s=0,j=0;          //Variables utilisées plus tard

byte rowPins[numRows] = {19,18,17,16}; // Rangées 19 à 16 // si vous modifiez vos broches, vous devez également le modifier
byte colPins[numCols]= {15,14,13,12}; //Colonnes de 15 à 12


Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

// Filename: space4
#define imageWidth 128 // la largeur de l'image
#define imageHeight 64 // la longueur de l'image

// l'image bitmap du nom de space4
static const uint8_t space4 [] = {
	0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x40 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x01 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x20 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x02 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x20 , 0x00 , 0x00 , 0x00 , 0x04 , 0x00 , 0x00 , 
0x10 , 0x00 , 0x00 , 0x00 , 0x00 , 0x04 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x40 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x01 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x64 , 0xE6 , 0x4C , 0x36 , 0x63 , 0x8D , 0x8E , 0x1C , 0x38 , 0xF1 , 0xC0 , 0x00 , 0x08 , 
0x00 , 0x00 , 0x00 , 0x74 , 0xC6 , 0x4C , 0x36 , 0x66 , 0xCD , 0x9B , 0x36 , 0x6C , 0xD9 , 0x80 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x74 , 0xC6 , 0xEC , 0x36 , 0x66 , 0xCD , 0x9B , 0x36 , 0x6C , 0xD9 , 0x80 , 0x20 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x74 , 0xC6 , 0xEC , 0x36 , 0x66 , 0xCD , 0x98 , 0x36 , 0x6C , 0xD9 , 0x80 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x7C , 0xE2 , 0xA8 , 0x36 , 0x66 , 0x0D , 0x9E , 0x30 , 0x6C , 0xF1 , 0xC0 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x6C , 0xC2 , 0xA8 , 0x3E , 0x66 , 0xCF , 0x87 , 0x36 , 0x6C , 0xD9 , 0x80 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x10 , 0x6C , 0xC2 , 0xA8 , 0x36 , 0x66 , 0xCD , 0x83 , 0x36 , 0x6C , 0xD9 , 0x80 , 0x00 , 0x00 ,
0x00 , 0x00 , 0x00 , 0x6C , 0xC2 , 0xA8 , 0x36 , 0x66 , 0xCD , 0x9B , 0x36 , 0x6C , 0xD9 , 0x80 , 0x00 , 0x00 ,
0x00 , 0x00 , 0x00 , 0x64 , 0xC3 , 0xB8 , 0x36 , 0x66 , 0xCD , 0x9B , 0x36 , 0x6C , 0xD9 , 0x80 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x64 , 0xF3 , 0xB8 , 0x36 , 0x63 , 0x4D , 0x8E , 0x1C , 0x38 , 0xD9 , 0xE0 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x02 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x04 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x01 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x03 , 0x87 , 0x1F , 0x1C , 0x38 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x06 , 0xCD , 0x98 , 0x36 , 0x6C , 0x00 , 0x00 , 0x00 , 0x00 , 0x80 , 0x00 ,
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x06 , 0xCD , 0x98 , 0x30 , 0x6C , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0xC1 , 0x9F , 0x3C , 0x6C , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x01 , 0xC3 , 0x1B , 0x36 , 0x6C , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x08 , 0x00 , 0x00 , 0x00 , 0x01 , 0x81 , 0x83 , 0x36 , 0x6C , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x03 , 0x8D , 0x9B , 0x36 , 0x3C , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x03 , 0x0D , 0x9B , 0x36 , 0x0C , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x07 , 0x0D , 0x9B , 0x36 , 0x6C , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x07 , 0xC7 , 0x0E , 0x1C , 0x38 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x08 , 
0x08 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0xC0 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x06 , 0x00 , 0x00 , 0x00 , 0x01 , 0x00 , 
0x00 , 0x00 , 0x01 , 0xE0 , 0x02 , 0x08 , 0x00 , 0x00 , 0x08 , 0x20 , 0x0F , 0x00 , 0x00 , 0x10 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x03 , 0xF0 , 0x01 , 0x10 , 0x00 , 0xE0 , 0x04 , 0x40 , 0x1F , 0x80 , 0x38 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x80 , 0x06 , 0xD8 , 0x03 , 0xF8 , 0x01 , 0xF0 , 0x0F , 0xE0 , 0x36 , 0xC0 , 0x7C , 0x00 , 0x00 , 0x00 ,
0x00 , 0x00 , 0x07 , 0xF8 , 0x07 , 0xFC , 0x02 , 0x48 , 0x1F , 0xF0 , 0x3F , 0xC0 , 0x92 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x07 , 0xF8 , 0x0E , 0xEE , 0x03 , 0xF8 , 0x3B , 0xB8 , 0x3F , 0xC0 , 0xFE , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x01 , 0x20 , 0x0B , 0xFA , 0x01 , 0x50 , 0x2F , 0xE8 , 0x09 , 0x00 , 0x54 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x02 , 0xD0 , 0x09 , 0x12 , 0x02 , 0xA8 , 0x24 , 0x48 , 0x16 , 0x80 , 0xAA , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x05 , 0x28 , 0x03 , 0x18 , 0x00 , 0x00 , 0x0C , 0x60 , 0x29 , 0x40 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x04 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x80 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x40 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 
0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 
};

// fonction qui contient l'image bitmap space4 et aussi un texte
void showSplash() {
  
  String splashString="TP integration "; // Message à afficher
  display.clearDisplay();     // efface l'écran
  display.setTextSize(1);      // la taille de l'écriture
  display.setTextColor(WHITE);   // la couleur de l'écriture
  display.setCursor(0,0);        // le curseur
  display.setCursor(64-(splashString.length()*3),0);
  display.print(splashString);    // affiche le message
  display.setTextSize(2);         // la taille de l'écriture
  splashString="De";              // Message à afficher
  display.setCursor(64-(splashString.length()*6),16);  // la longueur du texte
  display.print(splashString);    // affiche le message
  display.setTextSize(2);         // la taille de l'écriture
  splashString="Thanina ";        // Message à afficher
  display.setCursor(64-(splashString.length()*6),40);  // la longueur du texte
  display.print(splashString);    // affiche le message
  display.display();              // actualise l'écran
  delay(2500);                    // le délai est de 2 secondes et demi.

  display.clearDisplay();  // Efface le buffer.
  display.drawBitmap(0, 0, space4  , 128, 64, 1);  // la longueur et largeur de l'image Bitmap
  display.display();      // actualise l'écran
  delay(2500);            // le délai est de 2 secondes et demi.
   
}

void setup()               //fonction void setup
{
  Serial.begin(9600);    //initilise l'écran
 
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed")); // Message à afficher
    for(;;); // Don't proceed, loop forever
  }
  

  display.display();    // actualise l'écran
  showSplash();         // démarre la fonction showSplash
  display.setTextSize(2);     //met l'écriture dn taille 2
  display.clearDisplay();    //Efface le buffer.
  display.display();
  pixels.begin(); // Ceci initialise la bibliothèque NeoPixel.
}

// fonction pour allumer l'anneau Led
void allumerLed()
{
   for(int i=0;i<NUMPIXELS;i++)
  {
    
    // pixels.Color prend des valeurs RVB, de 0,0,0 à 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,255,0)); // Couleur verte moyennement brillante.
    pixels.show(); // Ceci envoie la couleur de pixel mise à jour au matériel.
    delay(delayval);
     // Délai pour une période de temps (en millisecondes).

  }
  for(int i=0;i<NUMPIXELS;i++)
  {
    // pixels.Color prend des valeurs RVB, de 0,0,0 à 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,0,0)); // éteint toutes les lumières
    pixels.show();
  } 

}


void GetNewCode1() {   // dans la fonction GetNewCode1 le code est stocké dans un tableau
     
               
  i = 0;              // la variable i 
  j = 1;               // la variable j
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Entez");      // dit à l'utilisateur d'entrer le nouveau code 
  display.setCursor(0, 20);
  display.print("le nouveau"); 
  display.setCursor(0, 40);
  display.print("code"); 
  delay(3000);            // le délai est de 3 secondes.
  display.display();
  display.clearDisplay();
  while (keypressed != '#') {          //# pour confirmer le nouveau code
    keypressed = myKeypad.getKey();
    
    if (keypressed != NO_KEY && keypressed != '#' ) {
      display.setCursor(j,0);
      display.print("*");     //le code est affiché en * pour plus de sécurité.
      display.display();              
      check1[i] = keypressed;   // Stocke les caractères dans le tableau
      i++;
      j=j+12;
    }
  }
  keypressed = NO_KEY;
}

void GetNewCode2() {      // C'est exactement comme la fonction GetNewCode1 mais cette fois le code est stocké dans un autre tableau
  i = 0;                  // la variable i
  j = 1;                  // la variable j

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Confirmez le code");    // dit à l'utilisateur de confirmer le nouveau code
  delay(3000);                     // le délai est de 3 secondes.

  display.display();
  display.clearDisplay();
  while (keypressed != '#') {     //# pour confirmer le nouveau code
    keypressed = myKeypad.getKey();
    
    if (keypressed != NO_KEY && keypressed != '#' ) {
      display.setCursor(j, 0);
      display.print("*");    //le code est affiché en * pour plus de sécurité.
      display.display();
      check2[i] = keypressed;   // Stocke les caractères dans le tableau
      i++;
      j=j+12;
    }
  }
  keypressed = NO_KEY;
}

void ReadCode() {                 // Obtention de la séquence de code
  i = 0;                         // Toutes les variables définies sur 0
  a = 0;
  j = 1;
  
  while (keypressed != '#') {                                  // L'utilisateur appuie sur # pour confirmer le code sinon il peut continuer à taper
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != '#' ) {     // Si le caractère tapé n'est ni # ni "rien"
      display.setCursor(j, 45);                                 // Ceci pour écrire "*" sur l'écran Oled chaque fois qu'une touche est enfoncée, sa position est contrôlée par j
      display.print("*");
       display.display();
      
      j=j+12;
      if (keypressed == code[i] && i < sizeof(code)) {       // si le caractère tapé est correct a et i incrémente pour vérifier le caractère suivant
        a++;
        i++;
      }
      else
        a--;                                               // si le caractère tapé est erroné a décrémente et ne peut pas être égal à la taille du code []
    }
  }
  keypressed = NO_KEY;

}

void ChangeCode() {                     //fonction pour changer le code
  
  display.setCursor(0, 0);
  display.print("Entez ");    // affiche le message entrez l'ancien code
  display.setCursor(0, 20);
  display.print("l'ancien");
  display.setCursor(0, 40);
  display.print("code");
  display.display();
  display.clearDisplay();
  ReadCode();              // appel la fonction ReadCode
  
  if (a == sizeof(code)) {  //vérifie à nouveau la valeur a
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Changement du Code");   // affiche le message changement du code
    display.display();
    GetNewCode1();            //prend le nouveau code
    GetNewCode2();            //prend le nouveau code encore une fois poru valider
    
    s = 0;
    for (i = 0 ; i < sizeof(code) ; i++) { // Compare les codes du tableau 1 et du tableau 2 de deux fonctions précédentes
      if (check1[i] == check2[i])
        s++;                                // encore une fois, c'est ainsi que nous vérifions, incrémenter chaque fois que les codes correspondent
    }
    if (s == sizeof(code)) {        // Correct est toujours la taille du tableau

      for (i = 0 ; i < sizeof(code) ; i++) {
        code[i] = check2[i];       // le tableau de code reçoit maintenant le nouveau code
        EEPROM.put(i, code[i]);       // Et le stocke dans l'EEPROM

      }
      
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Changement ");    //affiche le message changement du code réussi 
      display.setCursor(0, 20);
      display.print("du code");
      display.setCursor(0, 40);
      display.print("reussi");
      display.display();
      delay(4000);             // le délai est de 4 secondes.
    }
    else {                        //le message affiche codes non identiques
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Codes");
      display.setCursor(0, 20);
      display.print("non");
      display.setCursor(0, 40);
      display.print("identiques");
      display.display();
      delay(4000);          // le délai est de 4 secondes.
    }

  }

  else {                    //Si l'ancien code est erroné, vous ne pouvez pas le changer
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("le code ");
    display.setCursor(0, 20);
    display.print("est ");
    display.setCursor(0, 40);
    display.print("incorrect");
    display.display();
    delay(4000);       // le délai est de 4 secondes.
 
  }
}


void OpenDoor() {           // Fonction d'ouverture des portes pendant 4s
  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Ouverture"); // Message à afficher
  display.setCursor(0, 20);
  display.print("Reussite");  // Message à afficher
  display.setCursor(0, 40);
  display.print("WELCOME!!");  // Message à afficher
  display.display();
  allumerLed();        // appel la fonction allumerLed
  delay(4000);      // le délai est de 4 secondes.
  
}

void loop()                    // fonction void loop
{
  display.clearDisplay();
      display.display();
   marqueur: 
  keypressed = myKeypad.getKey();    // Attend constamment qu'une touche soit enfoncée
  if (keypressed == '*') {           // * pour ouvrir la serrure
                                     //marqueur
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Entrez le code");   // Message à afficher
   
    display.display();
    ReadCode();
                              // Récupération de la fonction de code
    if (a == sizeof(code))  // La fonction ReadCode attribue une valeur à a (c'est correct quand elle a la taille du tableau de codes)
    {       
      OpenDoor();              // Fonction de verrouillage ouverte si le code est correct
      display.clearDisplay();
      goto marqueur;           //goto marqueur
    }
    else {
      
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Le code"); // Message à afficher quand le code est incorrect
      display.setCursor(0, 20);
      display.print("est");
      display.setCursor(0, 40);
      display.print("incorrect");
      display.display();
      delay(4000);      // le délai est de 4 secondes.
      goto marqueur;
      display.clearDisplay();
      display.display();
    } 
  }
  if (keypressed == 'C') { // L'utilisateur appuie sur C pour changer le code.
      ChangeCode();  //Pour changer le code, il appelle la fonction de changement de code
      goto marqueur;  //goto marqueur.
              
  }
}

