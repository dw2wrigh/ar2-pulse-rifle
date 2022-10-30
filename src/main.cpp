#include <Arduino.h>
#include <SD.h>     // need to include the SD library
#include <TMRpcm.h> // also need to include this library...
#include <SPI.h>
#include <ButtonDebounce.h>

#define SD_CARD_CS 4
#define TRIGGER_IN 8 // Note that trigger and buttons are active low (1 = not pressed, 0 = pressed)
#define BUTTON1_IN 7
#define BUTTON2_IN 5
#define BUTTON3_IN 6
#define SPEAKER_OUT 9
#define BARREL_LED 10
#define DEBOUNCE_TIME 50
#define COMBINE_SOLDIER_DIR "/cs/"
#define METRO_POLICE_DIR "/mp/"
#define OVERWATCH_CV_DIR "/ow/city/"
#define OVERWATCH_RV_DIR "/ow/radio/"
#define AR2_DIR "/ar2/"
#define CS_MAX 233
#define MP_MAX 324
#define OWC_MAX 46
#define OWR_MAX 180

typedef enum StateChanges
{
  NONE,            // Nothing has changed
  TRIGGER_PRESS,   // Ready to fire
  TRIGGER_RELEASE, // Fire sequence
  BUTTON1_PRESS,   // Button 1 pressed
  BUTTON1_RELEASE, // Button 1 released
  BUTTON2_PRESS,   // Button 2 pressed
  BUTTON2_RELEASE, // Button 2 released
  BUTTON3_PRESS,   // Button 3 pressed
  BUTTON3_RELEASE  // Button 3 released
} States;

// File combine_soldier_dir;
TMRpcm tmrpcm; // Create an object for use in this sketch
ButtonDebounce trigger(TRIGGER_IN, DEBOUNCE_TIME);
ButtonDebounce button1(BUTTON1_IN, DEBOUNCE_TIME);
ButtonDebounce button2(BUTTON2_IN, DEBOUNCE_TIME);
ButtonDebounce button3(BUTTON3_IN, DEBOUNCE_TIME);
// bool toggle = false;
bool stateChange = false; // Indicate a state change to the main code
States currentState = NONE;
String tempString;
uint8_t shots = 30;
unsigned long timestamp;

String getFileName(String directory, String filePrefix, int fileNumber)
{
  String tempString(directory);
  tempString.concat(filePrefix);
  tempString.concat("_");
  char fileNumberStr[5];
  snprintf(fileNumberStr, 5, "%04d", fileNumber);
  tempString.concat(fileNumberStr);
  tempString.concat(".wav");
  return tempString;
}

String getFileName(String directory, String filePrefix)
{
  String tempString(directory);
  tempString.concat(filePrefix);
  tempString.concat(".wav");
  return tempString;
}

void triggerCallback(int state)
{
  stateChange = true;
  // Serial.print("Trigger ");
  // Serial.println(state);
  if (state == LOW) // Trigger changed state and is pressed
  {
    currentState = TRIGGER_PRESS;
  }
  else // Trigger changed state and is not pressed
  {
    currentState = TRIGGER_RELEASE;
  }
}

void button1Callback(int state)
{
  stateChange = true;
  // Serial.print("Button1 ");
  // Serial.println(state);
  if (state == LOW) // Button changed state and is pressed
  {
    currentState = BUTTON1_PRESS;
  }
  else // Button changed state and is not pressed
  {
    currentState = BUTTON1_RELEASE;
  }
}

void button2Callback(int state)
{
  stateChange = true;
  // Serial.print("Button2 ");
  // Serial.println(state);
  if (state == LOW) // Button changed state and is pressed
  {
    currentState = BUTTON2_PRESS;
  }
  else // Button changed state and is not pressed
  {
    currentState = BUTTON2_RELEASE;
  }
}

void button3Callback(int state)
{
  stateChange = true;
  // Serial.print("Button3 ");
  // Serial.println(state);
  if (state == LOW) // Button changed state and is pressed
  {
    currentState = BUTTON3_PRESS;
  }
  else // Button changed state and is not pressed
  {
    currentState = BUTTON3_RELEASE;
  }
}

void setup()
{
  randomSeed(analogRead(0)); // Seeds the pseudorandom number generator with a random voltage from the uncommected A0 analog pin
  pinMode(BARREL_LED, OUTPUT);
  tmrpcm.speakerPin = SPEAKER_OUT; // This tells the tmrpcm object which pin the amplifier is connected to (Pin 9)
  tmrpcm.quality(true);
  trigger.setCallback(triggerCallback); // This sets the triggerCallback function to be called whenver the trigger changes state
  button1.setCallback(button1Callback); // This sets the button1Callback function to be called whenver the trigger changes state
  button2.setCallback(button2Callback); // This sets the button2Callback function to be called whenver the trigger changes state
  button3.setCallback(button3Callback); // This sets the button3Callback function to be called whenver the trigger changes state
  // tmrpcm.setVolume(7); // Sets the volume (0 to 7)

  Serial.begin(9600); // Opens the serial port which is how you can type to the Arduino and it can respond
  delay(1000);
  Serial.println("Welcome to AR2 Pulse Rifle!");

  if (!SD.begin(SD_CARD_CS)) // SD.begin tries to initialize the SD card for access. It returns true when it is properly initialized, and false when it fails, so !SD.begin is true when it fails
  {
    Serial.println("SD card initialization failed.");
  }

  tempString = getFileName(COMBINE_SOLDIER_DIR, String("cs"), random(CS_MAX) + 1);
  tmrpcm.play(tempString.c_str()); // This sound file will play each time the arduino powers up or is reset
  Serial.println(tempString);
}

void loop()
{
  // Required by the debounce procedures
  trigger.update();
  button1.update();
  button2.update();
  button3.update();

  // This code block lets you do something when there's a trigger/button state change
  if (stateChange)
  {
    stateChange = false;
    switch (currentState)
    {
    case TRIGGER_PRESS:
      tmrpcm.stopPlayback(); // Stop whatever it's playing to begin firing and reloading
      Serial.println("trigger press");
      while (currentState == TRIGGER_PRESS)
      {
        if(shots == 0)
        {
          tempString = getFileName(COMBINE_SOLDIER_DIR, String("reload2"));
          tmrpcm.play(tempString.c_str());
          while(tmrpcm.isPlaying()) {}
          tempString = getFileName(COMBINE_SOLDIER_DIR, String("reload1"));
          tmrpcm.play(tempString.c_str());
          while(tmrpcm.isPlaying()) {}
          shots = 30;
        }
        digitalWrite(BARREL_LED, HIGH);
        tempString = getFileName(COMBINE_SOLDIER_DIR, String("fire1"));
        tmrpcm.play(tempString.c_str());
        timestamp = millis();
        while(millis() - timestamp < 100) {}
        shots -= 1;
      }

      Serial.println(tempString);
      break;

    case TRIGGER_RELEASE:
      // Do something
      digitalWrite(BARREL_LED, LOW);
      Serial.println("trigger release");
      break;

    case BUTTON1_PRESS:
      // Do something
      // Play altfire charging sound
      Serial.println("Button 1 press");
      break;

    case BUTTON1_RELEASE:
      // Do something
      // Play altfire firing sound
      Serial.println("Button 1 release");
      break;

    case BUTTON2_PRESS:
      // Do something
      // Play random combine soldier or metro police sound
      tempString = getFileName(COMBINE_SOLDIER_DIR, String("cs"), random(CS_MAX) + 1);
      tmrpcm.play(tempString.c_str());
      Serial.println("Button 2 press");
      break;

    case BUTTON2_RELEASE:
      // Do something
      Serial.println("Button 2 release");
      break;

    case BUTTON3_PRESS:
      // Do something
      // Play random overwatch city or radio sound

      Serial.println("Button 3 press");
      break;

    case BUTTON3_RELEASE:
      // Do something
      Serial.println("Button 3 release");
      break;

    default:
      // Do something
      break;
    }
  }

  // These code blocks will execute based on the trigger/button state all the time
  if (trigger.state() == LOW) // Trigger pressed
  {
    if (!tmrpcm.isPlaying()) // Done playing last sound
    {
      // Update counter and play AR2 firing or reload sound
    }
  }
}
