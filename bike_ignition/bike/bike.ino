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
// display console messages
int debug=true;
// do we have an authenticated card
int authenticated = false;
// what state do we think the bike is in
int running = true;

// led status globals
//analogue output pins
int redledpin = 9;
int greenledpin = 10;
int blueledpin = 11;
// status led constants
const int status_off[3] = {0,0,0};
const int status_power_on[3] = {255,0,0};
const int status_reader_fail[3] = {255,0,0};
const int status_reader_ready[3] = {255,0,0};
const int status_reader_authenticated[3] = {255,0,0};
const int status_reader_unauthenticated[3] = {255,0,0};

void display_status (const int status[3]){
//set status led output values here, overwrite all 3 values and don't allow for mixing
  
}
    
void setup_ledstatus(){
//set the output modes and intial state of the output leds
pinMode (redledpin,OUTPUT);
pinMode(greenledpin,OUTPUT);
pinMode(blueledpin,OUTPUT);       
display_status(status_off);
  
}
    


void setup_inputsw(){
//setup all input switches to allow reading of the state of the bike  
//examine state of emergancy program stop switch.
//halt if required (safety valve to lock ignition off or allow easily programming)
  
}

void read_inputsw(){
//read the input switches and other bike state information
//interact with global variables to set state information
  
}
int is_running_bike(){
//read bike state information and decide whether the bike is already running
//possible suggestions of state.. tachometer, wheels moving, speedo above 5km/hr

  read_inputsw();
//decision goes here  
  return true;
}  


void arduinopower(boolean state){
  if (state == true){
    //activate arduino power relay
  }
//  else power off
  
}

void setup_rfid (void) {
//currently magic and voodoo here.. must learn
if (debug == true) {Serial.begin(9600);
  Serial.println("Hello!");}

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    if (debug == true) { Serial.print("Didn't find PN53x board");}
    while (1); // halt
  }
  // Got ok data, print it out!
  if (debug == true) {Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);}
  
  // configure board to read RFID tags
  nfc.SAMConfig();
  
  if (debug == true) {Serial.println("Waiting for an ISO14443A Card ...");}
}




void read_rfid(void) {
 //currently magic and voodoo here.. must learn
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (success) {
    // Display some basic information about the card
    if (debug == true) {Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");}
    
    if (uidLength == 4)
    {
      // We probably have a Mifare Classic card ... 
      if (debug == true) {Serial.println("Seems to be a Mifare Classic card (4 byte UID)");}
	  
      // Now we need to try to authenticate it for read/write access
      // Try with the factory default KeyA: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
      if (debug == true) {Serial.println("Trying to authenticate block 4 with default KEYA value");}
      uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	  
	  // Start with block 4 (the first block of sector 1) since sector 0
	  // contains the manufacturer data and it's probably better just
	  // to leave it alone unless you know what you're doing
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);
	  
      if (success)
      {
        if (debug == true) {Serial.println("Sector 1 (Blocks 4..7) has been authenticated");}
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
          if (debug == true) {Serial.println("Reading Block 4:");
          nfc.PrintHexChar(data, 16);
          Serial.println("");}
		  
          // Wait a bit before reading the card again
          delay(1000);
        }
        else
        {
          if (debug == true) {Serial.println("Ooops ... unable to read the requested block.  Try another key?");}
        }
      }
      else
      {
        if (debug == true) {Serial.println("Ooops ... authentication failed: Try another key?");}
      }
    }
    
    if (uidLength == 7)
    {
      // We probably have a Mifare Ultralight card ...
      if (debug == true) {Serial.println("Seems to be a Mifare Ultralight tag (7 byte UID)");}
	  
      // Try to read the first general-purpose user page (#4)
      if (debug == true) {Serial.println("Reading page 4");}
      uint8_t data[32];
      success = nfc.mifareultralight_ReadPage (4, data);
      if (success)
      {
        // Data seems to have been read ... spit it out
        if (debug == true) {nfc.PrintHexChar(data, 4);
        Serial.println("");}
		
        // Wait a bit before reading the card again
        delay(1000);
      }
      else
      {
        if (debug == true) {Serial.println("Ooops ... unable to read the requested page!?");}
      }
    }
  }
}

boolean authenticate_card(){
  display_status(status_reader_unauthenticated);
  //loop to read the card
 //compare to list of known good cards (in eeprom) 
 //seperate program to load
 //if card is good, return true. else false
  
  return true;
}


void setup(){
//intiitilise the status leds before anything else (they will be needed first)
//initilises as off
setup_ledstatus(); 
//setup input switches
setup_inputsw();

arduinopower(true);


//set to fail before initilising rfid, if the initiilisation fails then it will lock up
//otherwise this should be a barely percievable blip
display_status(status_reader_fail);

//initilise the rfid reader, 
setup_rfid();

//now that the rfid is iniilised we can set the status mode to be on and start the program propper
display_status(status_power_on);
  
  
//read input switches
read_inputsw();
//are we booting up on a running bike?
running = is_running_bike();

  
}


void loop (){

if (running == false){
//try to read a card and authenticate
authenticated = authenticate_card();

  if (authenticated){
   display_status(status_reader_authenticated);
   //toggle bike ignition relay on
  }
}

if ((running == false) && (authenticated == false)){
  //shutdown bike and arduino
}

read_inputsw();
//check bike state (primary switch.)
//if primary switch is off toggle bike relay off
//power down arduino


  
  
}

