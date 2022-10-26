#include <Arduino.h>
#include <SD.h>     // need to include the SD library
#include <TMRpcm.h> // also need to include this library...
#include <SPI.h>
#include <ButtonDebounce.h>

#define SD_CARD_CS 4
#define TRIGGER_IN 5 // Note that trigger and buttons are active low (1 = not pressed, 0 = pressed)
#define BUTTON1_IN 6
#define BUTTON2_IN 8
#define BUTTON3_IN 7
#define SPEAKER_OUT 9
#define BARREL_LED 10
#define DEBOUNCE_TIME 50
#define COMBINE_SOLDIER_DIR "/COMBIN~1/VO/"
#define METRO_POLICE_DIR "/METROP~1/VO/"
#define OVERWATCH_CV_DIR "/OVERWA~1/CITYVO~1/"
#define OVERWATCH_RV_DIR "/OVERWA~1/RADIOV~1/"
#define AR2_DIR "/WEAPONS/AR2/"

File combine_soldier_dir;
TMRpcm tmrpcm; // Create an object for use in this sketch
ButtonDebounce trigger(TRIGGER_IN, DEBOUNCE_TIME);
ButtonDebounce button1(BUTTON1_IN, DEBOUNCE_TIME);
ButtonDebounce button2(BUTTON2_IN, DEBOUNCE_TIME);
ButtonDebounce button3(BUTTON3_IN, DEBOUNCE_TIME);
bool toggle = false;
bool stateChange = false; // Indicate a state change to the main code

void triggerCallback(int state)
{
  stateChange = true;
  Serial.print("Trigger ");
  Serial.println(state);

  if (state == LOW) // Trigger changed state and is pressed
  {
    // Do something
    if (tmrpcm.isPlaying()) // Check if audio is currently playing
    {
      tmrpcm.stopPlayback();
    }
    tmrpcm.play("tp.wav");
  }
  else // Trigger changed state and is not pressed
  {
    // Do something
    if (tmrpcm.isPlaying()) // Check if audio is currently playing
    {
      tmrpcm.stopPlayback();
    }
    tmrpcm.play("tr.wav");
  }
}

void button1Callback(int state)
{
  stateChange = true;
  Serial.print("Button1 ");
  Serial.println(state);
  if (state == LOW) // Button changed state and is pressed
  {
    // Do something
    if (tmrpcm.isPlaying()) // Check if audio is currently playing
    {
      tmrpcm.stopPlayback(); // Stop whatever is playing
    }
    tmrpcm.play("b1p.wav");
  }
  else // Button changed state and is not pressed
  {
    // Do something
    if (tmrpcm.isPlaying()) // Check if audio is currently playing
    {
      tmrpcm.stopPlayback(); // Stop whatever is playing
    }
    tmrpcm.play("b1r.wav");
  }
}

void button2Callback(int state)
{
  stateChange = true;
  Serial.print("Button2 ");
  Serial.println(state);
  if (state == LOW) // Button changed state and is pressed
  {
    // Do something
    if (tmrpcm.isPlaying()) // Check if audio is currently playing
    {
      tmrpcm.stopPlayback(); // Stop whatever is playing
    }
    tmrpcm.play("b2p.wav");
  }
  else // Button changed state and is not pressed
  {
    // Do something
    if (tmrpcm.isPlaying()) // Check if audio is currently playing
    {
      tmrpcm.stopPlayback(); // Stop whatever is playing
    }
    tmrpcm.play("b2r.wav");
  }
}

void button3Callback(int state)
{
  stateChange = true;
  Serial.print("Button3 ");
  Serial.println(state);
  if (state == LOW) // Button changed state and is pressed
  {
    // Do something
    if (tmrpcm.isPlaying()) // Check if audio is currently playing
    {
      tmrpcm.stopPlayback(); // Stop whatever is playing
    }
    tmrpcm.play("b3p.wav");
  }
  else // Button changed state and is not pressed
  {
    // Do something
    if (tmrpcm.isPlaying()) // Check if audio is currently playing
    {
      tmrpcm.stopPlayback(); // Stop whatever is playing
    }
    tmrpcm.play("b3r.wav");
  }
}

void printDirectory(File dir, int numTabs)
{
  while (true)
  {
    File entry = dir.openNextFile();
    if (!entry)
    {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++)
    {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory())
    {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    }
    else
    {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

int numFiles(File dir)
{
  int count = 0;
  while (true)
  {
    File entry = dir.openNextFile();
    if (!entry) // No more files
    {
      break;
    }
    if (!entry.isDirectory()) // Don't count subdirectories
    {
      count++;
    }
    entry.close();
    return count;
  }
}

void setup()
{
  pinMode(BARREL_LED, OUTPUT);
  tmrpcm.speakerPin = SPEAKER_OUT; // This tells the tmrpcm object which pin the amplifier is connected to (Pin 9)
  tmrpcm.quality(true);
  trigger.setCallback(triggerCallback); // This sets the triggerCallback function to be called whenver the trigger changes state
  button1.setCallback(button1Callback); // This sets the button1Callback function to be called whenver the trigger changes state
  button2.setCallback(button2Callback); // This sets the button2Callback function to be called whenver the trigger changes state
  button3.setCallback(button3Callback); // This sets the button3Callback function to be called whenver the trigger changes state
  tmrpcm.setVolume(1); // Sets the volume (0 to 7)

  Serial.begin(9600); // Opens the serial port which is how you can type to the Arduino and it can respond
  delay(1000);
  Serial.println("Welcome to AR2 Pulse Rifle!");

  if (!SD.begin(SD_CARD_CS)) // SD.begin tries to initialize the SD card for access. It returns true when it is properly initialized, and false when it fails, so !SD.begin is true when it fails
  {
    Serial.println("SD card initialization failed.");
  }

  // combine_soldier_dir = SD.open(COMBINE_SOLDIER_DIR);
  // printDirectory(combine_soldier_dir, 0);
  String tempString(COMBINE_SOLDIER_DIR);
  tempString.concat("PRISON~1.wav");
  tmrpcm.play(tempString.c_str()); // This sound file will play each time the arduino powers up or is reset
}

void loop()
{
  // Required by the debounce procedures
  trigger.update();
  button1.update();
  button2.update();
  button3.update();

  if (stateChange)
  {
    digitalWrite(BARREL_LED, !trigger.state());
  }

  // Serial.print("tmrpcm.isPlaying() = ");
  // Serial.println(tmrpcm.isPlaying());



  // delay(1000);
  // toggle = !toggle;
  // digitalWrite(LED_BUILTIN, toggle);
  // digitalWrite(BARREL_LED, toggle);

  // tmrpcm.play("test.wav"); // This sound file will play each loop
  // tone(9, 440, 500);
}
