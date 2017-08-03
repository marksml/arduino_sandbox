/*************************************************** 
  This is an example for the Adafruit VS1053 Codec Breakout

  Designed specifically to work with the Adafruit VS1053 Codec Breakout 
  ----> https://www.adafruit.com/products/1381

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// define the pins used
//#define CLK 13       // SPI Clock, shared with SD card
//#define MISO 12      // Input data, from VS1053/SD card
//#define MOSI 11      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins. 
// See http://arduino.cc/en/Reference/SPI "Connections"

// These are the pins used for the breakout example
//#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
//#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
//#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = 
  // create breakout-example object!
  //Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
  // create shield-example object!
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);


//define tracks
char* tracks[] = {"track001.mp3","track002.mp3","track003.mp3","track004.mp3","track005.mp3","track006.mp3","track007.mp3","track008.mp3","track009.mp3","track010.mp3"};
//char** tracks = new char*[1000];
//int index = 0;

int trackNumber = 0;
int numberOfTracks = 10;
boolean stopped = false;
boolean paused = false;

int NEXT_2 = 2;
int PLAYPAUSE_5 = 5;
int PREV_8 = 8;

long MAX_VOLUME = 60;

int nextPinNow = HIGH;
int nextPinBefore = HIGH;
boolean nextPressed = false;

int prevPinNow = HIGH;
int prevPinBefore = HIGH;
boolean prevPressed = false;

int playPinNow = HIGH;
int playPinBefore = HIGH;
boolean playPressed = false;
  
// File listing helper
void printDirectory(File dir, int numTabs) {
   
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/"); 
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.name());

       //tracks[index] = entry.name();
       /*Serial.print("added ");
       Serial.print(tracks[index]);
       Serial.print(" on tracks index =");
       Serial.print(index);
       
       index = index + 1;
       */
     }
     entry.close();
   } 
}


void setup() {
  Serial.begin(9600);

  //configure digital pins as an input and enable the internal pull-up resistor
  //next
  pinMode(NEXT_2, INPUT_PULLUP);
  //previous
  pinMode(PREV_8, INPUT_PULLUP);
  //pause/play
  pinMode(PLAYPAUSE_5, INPUT_PULLUP);

  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
   if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  
  File root = SD.open("/");
  printDirectory(root,0);
  
  
  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int

  playTrack(trackNumber);
 
}

void loop() {         

  // read the input on analog pin 0:
  long sensorValue = analogRead(A0);
  long volume = (sensorValue * 100L)/1023;
  
  //Serial.print("volume="); 
  //Serial.println(volume);
  
  if(volume < MAX_VOLUME){
    volume = MAX_VOLUME;
  }
  //Serial.print("volume="); 
  //Serial.println(volume);
  
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(volume,volume);

  //Serial.println("---------------------------------------------");
  
  //Serial.println("loop entered");
  if((!musicPlayer.playingMusic) && !stopped && !paused && (trackNumber < (numberOfTracks-1))){
    Serial.println("Play next track");
    trackNumber = trackNumber + 1;
    playTrack(trackNumber);
  }

  //repeat all 
  if((!musicPlayer.playingMusic) && !stopped && !paused && (trackNumber == (numberOfTracks-1))){
    Serial.println("Play first track");
    trackNumber = 0;
    playTrack(trackNumber);
  }

  
  // File is playing in the background
  if (musicPlayer.stopped()) {
    //Serial.println("Done playing music");
    //while (1);
  }

    if (playPressed) {
      Serial.println("-------------------- Play Paused Pressed -----------------------");
      playPressed = false;
        
      if (! musicPlayer.paused()) {
        Serial.println("Paused");
        paused = true;
        musicPlayer.pausePlaying(true);
      } else { 
        Serial.println("Resumed");
        paused = false;
        musicPlayer.pausePlaying(false);
      }
    }

    if (nextPressed) {
      nextPressed = false;
      Serial.println("-------------------- Next Pressed -----------------------");
      if(trackNumber < numberOfTracks-1){
        trackNumber = trackNumber +1;
        playTrack(trackNumber);
      }else{
        trackNumber = 0;
        playTrack(trackNumber);  
      }
    }

    if (prevPressed) {
      prevPressed = false;
      Serial.println("-------------------- Previous Pressed -----------------------");
      
      if(trackNumber > 0){
        trackNumber = trackNumber - 1;
        playTrack(trackNumber);
      }else{
         trackNumber = numberOfTracks - 1;
         playTrack(trackNumber); 
      }
      
    } 

  
  readButtons();

  delay(10);


}

void readButtons(){


  //read the pushbutton values into a variable
  nextPinBefore = nextPinNow;
  nextPinNow = digitalRead(NEXT_2);
  if((nextPinBefore == HIGH) && (nextPinNow == LOW)){
    nextPressed = true;
  }
  
//  Serial.print("nextPinBefore=");
//  Serial.println(nextPinBefore);
//  Serial.print("nextPinNow=");
//  Serial.println(nextPinNow);
//  Serial.print("nextPressed=");
//  Serial.println(nextPressed);

 
  prevPinBefore = prevPinNow;
  prevPinNow = digitalRead(PREV_8);
  if((prevPinBefore == HIGH) && (prevPinNow == LOW)){
    prevPressed = true;
  }
  
//  Serial.print("prevPinBefore=");
//  Serial.println(prevPinBefore);
//  Serial.print("prevPinNow=");
//  Serial.println(prevPinNow);
//  Serial.print("prevPressed=");
//  Serial.println(prevPressed);

  
   playPinBefore = playPinNow;
  playPinNow = digitalRead(PLAYPAUSE_5);
  if((playPinBefore == HIGH) && (playPinNow == LOW)){
    playPressed = true;
  } 
  
//  Serial.print("playPinBefore=");
//  Serial.println(playPinBefore);
//  Serial.print("playPinNow=");
//  Serial.println(playPinNow);
//  Serial.print("playPressed=");
//  Serial.println(playPressed);
  
  
  
  
  
}



void playTrack(int trackNumber){
  musicPlayer.stopPlaying();
  char* trackName = tracks[trackNumber];
  Serial.println(trackName);
  musicPlayer.startPlayingFile(trackName);
}



