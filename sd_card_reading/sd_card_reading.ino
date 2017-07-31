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
#include <SD.h>
#include <LinkedList.h>
#include <string.h>


#define CARDCS 4     // Card chip select pin

// Let's define a new class
class Track {
  public:
    char *name;
};


int index = 0;
LinkedList<Track*> trackList = LinkedList<Track*>();

// File listing helper
void read_SD_Card(File dir, int numTabs) {

/*
char* tracks[] = {"track001.mp3","track002.mp3","track003.mp3","track004.mp3","track005.mp3","track006.mp3"};

Track *newTrack;

for(int i=0; i<6; i++){
         Serial.print("------- i = ");
         Serial.print(i);
         Serial.println(" -------");
         
         Track *currentReadTrack = trackList.get(i-1);
         Serial.print("vor copy: trackList.get(");
         Serial.print(i-1);
         Serial.print(")=");
         Serial.println(currentReadTrack->name);
         
         char *copy = new char[14](); 
         strncpy(copy, tracks[i], 13); 
         copy[14] = '\0';


         currentReadTrack = trackList.get(i-1);
         Serial.print("vor assignment: trackList.get(");
         Serial.print(i-1);
         Serial.print(")=");
         Serial.println(currentReadTrack->name);
         
         newTrack = new Track();
         newTrack->name = copy;  
         
         Serial.print("copy = ");
         Serial.println(newTrack->name);
      
         currentReadTrack = trackList.get(i-1);
         Serial.print("nach assignment: trackList.get(");
         Serial.print(i-1);
         Serial.print(")=");
         Serial.println(currentReadTrack->name);
         
         trackList.add(newTrack);
         
         currentReadTrack = trackList.get(i-1);
         Serial.print("nach add Operation: trackList.get(");
         Serial.print(i-1);
         Serial.print(")=");
         Serial.println(currentReadTrack->name);
         
         currentReadTrack = trackList.get(i);
         Serial.print("nach add Operation: trackList.get(");
         Serial.print(i);
         Serial.print(")=");
         Serial.println(currentReadTrack->name);
         
         int size = trackList.size();
         Serial.print("trackList.size()=");
         Serial.println(size);
         
  }
 */  
   while(true) {
     
     File entry =  dir.openNextFile();
   
     if (! entry) {
       // no more files
       Serial.println("**nomorefiles**");
       break;
     }
     
     if (entry.isDirectory()) {
     
       read_SD_Card(entry, numTabs+1);
     
     } else {

        
       if((entry.size() > 0) && (index > -1)){

         char *copy = new char[14](); 
         strncpy(copy, entry.name(), 13); 
         copy[14] = '\0';


         Track *newTrack = new Track();
         newTrack->name = copy;
         
         Serial.print("copy = ");
         Serial.println(newTrack->name);
      
         trackList.add(newTrack);
         
         Track *currentReadTrack = trackList.get(index-1);
         Serial.print("trackList.get(");
         Serial.print(index-1);
         Serial.print(")=");
         Serial.println(currentReadTrack->name);
         

         if(index > 1){
            currentReadTrack = trackList.get(index-2);
            Serial.print("trackList.get(");
            Serial.print(index-2);
            Serial.print(")=");
            Serial.println(currentReadTrack->name);
         }
         if(index > 2){
            currentReadTrack = trackList.get(index-3);
            Serial.print("trackList.get(");
            Serial.print(index-3);
            Serial.print(")=");
            Serial.println(currentReadTrack->name);
         }
         
         int size = trackList.size();
         Serial.print("trackList.size()=");
         Serial.println(size);
         
         
        
       } 
       index = index + 1;
       
     }
     entry.close();
   }
   
}



void printTrackList(){

  int size = trackList.size();

  Serial.print("trackList.size()=");
  Serial.println(size);
    
  for(int i=0; i < size; i++){
    
    Serial.print("trackList.get(");
    Serial.print(i);
    Serial.print(") = ");

    Track *track = trackList.get(i);
    
    Serial.println(track->name);
  
  } 
  
}

void setup() {
  Serial.begin(9600);

   if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  
  File root = SD.open("/");
  read_SD_Card(root,0);

  //fillTrackList();
  printTrackList();
}

void loop() {         
}



