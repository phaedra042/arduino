/**************************************************************************/
/*! 
    This example will wait for any ISO14443A card or tag, and
    depending on the size of the UID will attempt to read from it.
   
    If the card has a 4-byte UID it is probably a Mifare
    Classic card, and the following steps are taken:
   
    - Authenticate block 4 (the first block of Sector 1) using
      the default KEYA of 0XFF 0XFF 0XFF 0XFF 0XFF 0XFF
    - If authentication succeeds, we can then read any of the
      4 blocks in that sector (though only block 4 is read here)
	 
    If the card has a 7-byte UID it is probably a Mifare
    Ultralight card, and the 4 byte pages can be read directly.
    Page 4 is read by default since this is the first 'general-
    purpose' page on the tags.

*/
/**************************************************************************/

// choose to SPI or I2C or HSU
  #include <Wire.h>
  #include <PN532_I2C.h>
  #include <PN532.h>

  PN532_I2C pn532i2c(Wire);
  PN532 nfc(pn532i2c);
// console messages
int debug=true;
int authenticated = false;
// led status globals
int statusled = 0;
//analogue output pins
int redledpin = 9;
int greenledpin = 10;
int blueledpin = 11;
//door switches
boolean door_open = false;
boolean door_closed = false;
boolean door_state_unknown = false;


// status led constants
const int status_off[3] = {0,0,0};
const int status_power_on[3] = {255,0,0};
const int status_reader_fail[3] = {255,0,0};
const int status_reader_ready[3] = {255,0,0};
const int status_reader_authenticated[3] = {255,0,0};
const int status_reader_unauthenticated[3] = {255,0,0};

//door time
const int door_operate_time = 30;

void setup(){
//intiitilise the status leds before anything else (they will be needed first)
setup_ledstatus(); 
//setup input switches

//clear the led so we know the state
display_status(status_off);
//set to fail before initilising rfid, if the initiilisation fails then it will lock up
//otherwise this should be a barely percievable blip
display_status(status_reader_fail);

//initilise the rfid reader, 
setup_rfid();

//now that teh rfid is iniilised we can set the status mode to be on and start the program propper
display_status(status_power_on);
  
  
//read input switches

//are we booting up on a running bike?
  
}

void setup_ledstatus(){
pinMode (redledpin,OUTPUT);
pinMode(greenledpin,OUTPUT);
pinMode(blueledpin,OUTPUT);       

  
}


void display_status (const int status[3]){
//set the status light to the status    
}



void loop (){

  //read door status
  //if status unknown set door_state_unknown true
      //status light set unknown
      //wait timer (door_operate_time)
      //read door status
      //if still unknown state
        //alarm on
  //
  //if door is open
      //set status light open
      // if close attemopts < 3
          //alarm off
      //wait 1 time period
      //decrement timer
      //read door state 
      //if open and closed
          //increment time period by 2
          //beep
          //wait 1 time period
      //if close attempts = 3
          //alarm on
      //if time period = 0 
          //push door close button
          //increment close attempts 
      
  //if door closed
    //set status light off
    //alarm off
    //set timer to inital door open time
    //set close attempts = 0
    //wait time period
    //attempt to read rfid
    //if rfid success
       //push open door button 
  

  
  
}

void setup_rfid (void) {
  Serial.begin(9600);
  Serial.println("Hello!");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // configure board to read RFID tags
  nfc.SAMConfig();
  
  Serial.println("Waiting for an ISO14443A Card ...");
}

void read_rfid(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (success) {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");
    
    if (uidLength == 4)
    {
      // We probably have a Mifare Classic card ... 
      Serial.println("Seems to be a Mifare Classic card (4 byte UID)");
	  
      // Now we need to try to authenticate it for read/write access
      // Try with the factory default KeyA: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
      Serial.println("Trying to authenticate block 4 with default KEYA value");
      uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	  
	  // Start with block 4 (the first block of sector 1) since sector 0
	  // contains the manufacturer data and it's probably better just
	  // to leave it alone unless you know what you're doing
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);
	  
      if (success)
      {
        Serial.println("Sector 1 (Blocks 4..7) has been authenticated");
        uint8_t data[16];
		
        // If you want to write something to block 4 to test with, uncomment
		// the following line and this text should be read back in a minute
        // data = { 'a', 'd', 'a', 'f', 'r', 'u', 'i', 't', '.', 'c', 'o', 'm', 0, 0, 0, 0};
        // success = nfc.mifareclassic_WriteDataBlock (4, data);

        // Try to read the contents of block 4
        success = nfc.mifareclassic_ReadDataBlock(4, data);
		
        if (success)
        {
          // Data seems to have been read ... spit it out
          Serial.println("Reading Block 4:");
          nfc.PrintHexChar(data, 16);
          Serial.println("");
		  
          // Wait a bit before reading the card again
          delay(1000);
        }
        else
        {
          Serial.println("Ooops ... unable to read the requested block.  Try another key?");
        }
      }
      else
      {
        Serial.println("Ooops ... authentication failed: Try another key?");
      }
    }
    
    if (uidLength == 7)
    {
      // We probably have a Mifare Ultralight card ...
      Serial.println("Seems to be a Mifare Ultralight tag (7 byte UID)");
	  
      // Try to read the first general-purpose user page (#4)
      Serial.println("Reading page 4");
      uint8_t data[32];
      success = nfc.mifareultralight_ReadPage (4, data);
      if (success)
      {
        // Data seems to have been read ... spit it out
        nfc.PrintHexChar(data, 4);
        Serial.println("");
		
        // Wait a bit before reading the card again
        delay(1000);
      }
      else
      {
        Serial.println("Ooops ... unable to read the requested page!?");
      }
    }
  }
}
