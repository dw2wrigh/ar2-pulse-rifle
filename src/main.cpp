#include <Arduino.h>
#include <SD.h>            // need to include the SD library
#include <TMRpcm.h>        // also need to include this library...
#include <SPI.h>
#include <ButtonDebounce.h>

#define SD_CARD_CS 4
#define SPEAKER_OUT 9
#define TRIGGER1_IN 1
#define TRIGGER2_IN 2
#define TRIGGER3_IN 3

TMRpcm tmrpcm; // Create an object for use in this sketch
boolean toggle = false;
// ButtonDebounce button(10, 250);

void setup()
{

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_BUILTIN, INPUT_PULLUP); // Active Low
  pinMode(LED_BUILTIN, INPUT_PULLUP); // Active Low
  pinMode(LED_BUILTIN, INPUT_PULLUP); // Active Low
  tmrpcm.speakerPin = SPEAKER_OUT;
  //tmrpcm.setVolume(4);

  Serial.begin(9600);
  if (!SD.begin(SD_CARD_CS))
  { // see if the card is present and can be initialized:
    Serial.println("SD fail");
    return; // don't do anything more if not
  }
  tmrpcm.play("test2.wav"); // the sound file "music" will play each time the arduino powers up, or is reset
  digitalWrite(LED_BUILTIN, toggle);
}

void loop()
{
  // delay(1000);
  // if (Serial.available())
  // {
  //   if (Serial.read() == 'p')
  //   { // send the letter p over the serial monitor to start playback
  //     tmrpcm.play("music");
  //   }
  // }
  // toggle = !toggle;
  // digitalWrite(LED_BUILTIN, toggle);
  // tone(9, 440, 500);
}

// void loop() {
//   button.update();
//   if(button.state() == HIGH){
//     Serial.println("Clicked");
//   }
// }