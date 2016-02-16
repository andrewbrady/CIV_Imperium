/*


*/
#include <SoftwareSerial.h>

SoftwareSerial mySerial(7, 8); // RX, TX

String message;

byte byte_msgheader [] = {0xFE,0xFE,0x5E,0x0E};
byte byte_msgfooter [] = {0xFD};

char s_in;
byte cmd_bytes[11];
byte CLEAR[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
byte byte_off = 0x00;
byte byte_on = 0x01;
byte read_bytes[8];
/****************************/

// Set Operating Frequancy
//RX -->:FE FE E 5E 3 0 0 0 10 0 FD  <--
byte SET_OPERATING_FRQ[] = {0xFE,0xFE,0x5E,0x0E,0x05,0x00,0x00,0x00,0x10,0x00,0xFD};

//////////
// NR Level Mapping
// 1-15
// 0,16,32,48,64,80,96,01 12,01 28,01 44,01 60,01 76,01 92,02 08,02 24,02 40
byte NR_LEVEL[] = {0xFE,0xFE,0x5E,0x0E,0x14,0x06,0xFD};

byte NR_LEVELS[][9] = {
  {0xFE,0xFE,0x5E,0x0E,0x14,0x06,0x00,0xFD},
  {0xFE,0xFE,0x5E,0x0E,0x14,0x06,0x16,0xFD},
  {0xFE,0xFE,0x5E,0x0E,0x14,0x06,0x32,0xFD},
  {0xFE,0xFE,0x5E,0x0E,0x14,0x06,0x48,0xFD},
  {0xFE,0xFE,0x5E,0x0E,0x14,0x06,0x64,0xFD}, 
  {0xFE,0xFE,0x5E,0x0E,0x14,0x06,0x80,0xFD}, 
  {0xFE,0xFE,0x5E,0x0E,0x14,0x06,0x96,0xFD}, 
  {0xFE,0xFE,0x5E,0x0E,0x14,0x06,0x01,0x12,0xFD},
  {0xFE,0xFE,0x5E,0x0E,0x14,0x06,0x01,0x28,0xFD},
  {0xFE,0xFE,0x5E,0x0E,0x14,0x06,0x01,0x44,0xFD},
  {0xFE,0xFE,0x5E,0x0E,0x14,0x06,0x01,0x60,0xFD},
  {0xFE,0xFE,0x5E,0x0E,0x14,0x06,0x01,0x76,0xFD},
  {0xFE,0xFE,0x5E,0x0E,0x14,0x06,0x01,0x92,0xFD}, 
  {0xFE,0xFE,0x5E,0x0E,0x14,0x06,0x02,0x08,0xFD},
  {0xFE,0xFE,0x5E,0x0E,0x14,0x06,0x02,0x24,0xFD},
  {0xFE,0xFE,0x5E,0x0E,0x14,0x06,0x02,0x40,0xFD}
};


byte MIC_GAIN[] = {0xFE,0xFE,0x5E,0x0E,0x14,0x0B,0xFD};

byte AF_GAIN[] = {0xFE,0xFE,0x5E,0x0E,0x14,0x01,0xFD};
//Not working
//byte POWER_SW[] = {0xFE,0xFE,0x5E,0x0E,0x18,0xFD};

// L-99-H
byte RF_LEVEL[] = {0xFE,0xFE,0x5E,0x0E,0x14,0x0A,0xFD};
byte RF_LEVEL_LOW[] = {0xFE,0xFE,0x5E,0x0E,0x14,0x0A,0x00,0xFD};
byte RF_LEVEL_HI[] = {0xFE,0xFE,0x5E,0x0E,0x14,0x0A,0x02,0x55,0xFD};


// Mode 0x00 LSB, 
byte SET_MODE_LSB[] = {0xFE,0xFE,0x5E,0x0E,0x06,0x00,0xFD};
byte SET_MODE_USB[] = {0xFE,0xFE,0x5E,0x0E,0x06,0x01,0xFD};
byte SET_MODE_AM[] = {0xFE,0xFE,0x5E,0x0E,0x06,0x02,0xFD};
byte SET_MODE_RTTY[] = {0xFE,0xFE,0x5E,0x0E,0x06,0x04,0xFD};
byte SET_MODE_RTTY_REV[] = {0xFE,0xFE,0x5E,0x0E,0x06,0x08,0xFD};
byte SET_MODE_CW[] = {0xFE,0xFE,0x5E,0x0E,0x06,0x03,0xFD};
byte SET_MODE_CW_REV[] = {0xFE,0xFE,0x5E,0x0E,0x06,0x07,0xFD};


//Read Upper/Lower Frequencies
byte READ_UPPER_LOWER_FREQUENCIES[] = {0xFE,0xFE,0x5E,0x0E,0x02,0xFD};

//Read operating mode
byte READ_OPERATING_MODE[] = {0xFE,0xFE,0x5E,0x0E,0x04,0xFD};

//Read frequencys
byte READ_FREQUENCYS[] =     {0xFE,0xFE,0x5E,0x0E,0x03,0xFD};


byte SET_VFO_A[] = {0xFE,0xFE,0x5E,0x0E,0x07,0x00,0xFD};
byte SET_VFO_B[] = {0xFE,0xFE,0x5E,0x0E,0x07,0x01,0xFD};
byte VFO_A_eq_B[] = {0xFE,0xFE,0x5E,0x0E,0x07,0xA0,0xFD};
byte VFO_A_Sw_B[] = {0xFE,0xFE,0x5E,0x0E,0x07,0xB0,0xFD};

byte SPLIT_ON[] = {0xFE,0xFE,0x5E,0x0E,0x0F,0x01,0xFD};
byte SPLIT_OFF[] = {0xFE,0xFE,0x5E,0x0E,0x0F,0x00,0xFD};

byte SET_TS_ON[] = {0xFE,0xFE,0x5E,0x0E,0x10,0x01,0xFD};
byte SET_TS_OFF[] = {0xFE,0xFE,0x5E,0x0E,0x10,0x00,0xFD};

byte ATT_ON[] =  {0xFE,0xFE,0x5E,0x0E,0x11,0x20,0xFD};
byte ATT_OFF[] = {0xFE,0xFE,0x5E,0x0E,0x11,0x00,0xFD};

byte READ_SMETER[] = {0xFE,0xFE,0x5E,0x0E,0x15,0x02,0xFD};

byte PRE_AMP_ON[] = {0xFE,0xFE,0x5E,0x0E,0x16,0x02,0x01,0xFD};
byte PRE_AMP_OFF[] = {0xFE,0xFE,0x5E,0x0E,0x16,0x02,0x00,0xFD};

byte NB_OFF[] = {0xFE,0xFE,0x5E,0x0E,0x16,0x22,0x00,0xFD};
byte NB_ON[] =  {0xFE,0xFE,0x5E,0x0E,0x16,0x22,0x01,0xFD};

byte NR_OFF[] = {0xFE,0xFE,0x5E,0x0E,0x16,0x40,0x00,0xFD};
byte NR_ON[] =  {0xFE,0xFE,0x5E,0x0E,0x16,0x40,0x01,0xFD};

byte AUTO_NOTCH_OFF[] = {0xFE,0xFE,0x5E,0x0E,0x16,0x41,0x00,0xFD};
byte AUTO_NOTCH_ON[] =  {0xFE,0xFE,0x5E,0x0E,0x16,0x41,0x01,0xFD};

byte COMP_OFF[] = {0xFE,0xFE,0x5E,0x0E,0x16,0x44,0x00,0xFD};
byte COMP_ON[] =  {0xFE,0xFE,0x5E,0x0E,0x16,0x44,0x01,0xFD};

byte VOX_OFF[] = {0xFE,0xFE,0x5E,0x0E,0x16,0x46,0x00,0xFD};
byte VOX_ON[] =  {0xFE,0xFE,0x5E,0x0E,0x16,0x46,0x01,0xFD};

byte BK_IN_OFF[] = {0xFE,0xFE,0x5E,0x0E,0x16,0x47,0x00,0xFD};
byte BK_IN_ON[] =  {0xFE,0xFE,0x5E,0x0E,0x16,0x47,0x01,0xFD};
/***************************/


void setup() 
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.println("KD0PBZ - CV-I");
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
}

void loop() // run over and over
{
   
  message = readSerial();
  int length = message.length();
  if(length > 0){
    Serial.println("CMD>\"" + message + "\"");   
   
//    AF_GAIN

    if(message == "AF_GAIN"){
      memcpy(cmd_bytes, AF_GAIN,7);
    }
   
    if(message == "SET_MODE_LSB"){
      memcpy(cmd_bytes, SET_MODE_LSB,7);
    }   

    if(message == "SET_MODE_USB"){
      memcpy(cmd_bytes, SET_MODE_USB,7);
    }   

    if(message == "SET_MODE_AM"){
      memcpy(cmd_bytes, SET_MODE_AM,7);
    }

    if(message == "SET_MODE_RTTY"){
      memcpy(cmd_bytes, SET_MODE_RTTY,7);
    }
   
    if(message == "SET_MODE_RTTY_REV"){
      memcpy(cmd_bytes, SET_MODE_RTTY_REV,7);
    }
   
    if(message == "SET_MODE_CW"){
      memcpy(cmd_bytes, SET_MODE_CW,7);
    }
   
    if(message == "SET_MODE_CW_REV"){
      memcpy(cmd_bytes, SET_MODE_CW_REV,7);
    }
   
    if(message == "NR_LEVEL"){
      memcpy(cmd_bytes, NR_LEVEL,7);
    }
  
     
    if(message.substring(0,8) == "AF_GAIN "){
      byte one = 0x00; // First Byte
      byte two = 0x00; // Second Byte
      level_convert(message.substring(8,13),one,two);
      byte AF_GAIN_SET[] = {0xFE,0xFE,0x5E,0x0E,0x14,0x01,one,two,0xFD};
      memcpy(cmd_bytes, AF_GAIN_SET, sizeof(AF_GAIN_SET));
    }

    if(message == "RF_GAIN"){
      byte RF_GAIN[] = {0xFE,0xFE,0x5E,0x0E,0x14,0x02,0xFD};
      memcpy(cmd_bytes, RF_GAIN, sizeof(RF_GAIN));
    }
   
    if(message.substring(0,8) == "RF_GAIN "){
      byte one = 0x00; // First Byte
      byte two = 0x00; // Second Byte
      level_convert(message.substring(8,13),one,two);
      byte RF_GAIN_SET[] = {0xFE,0xFE,0x5E,0x0E,0x14,0x02,one,two,0xFD};
      memcpy(cmd_bytes, RF_GAIN_SET, sizeof(RF_GAIN_SET));
    }
   
    if(message.substring(0,9) == "RF_POWER "){
      byte one = 0x00; // First Byte
      byte two = 0x00; // Second Byte
      level_convert(message.substring(9,13),one,two);     
      byte RF_POWER_SET[] = {0xFE,0xFE,0x5E,0x0E,0x14,0x0A,one,two,0xFD};
      memcpy(cmd_bytes, RF_POWER_SET, sizeof(RF_POWER_SET));
    }
   
    if(message.substring(0,9) == "MIC_GAIN "){
      byte one = 0x00; // First Byte
      byte two = 0x00; // Second Byte
      level_convert(message.substring(9,13),one,two);           
      byte MIC_GAIN_SET[] = {0xFE,0xFE,0x5E,0x0E,0x14,0x0B,one,two,0xFD};
      memcpy(cmd_bytes, MIC_GAIN_SET, sizeof(MIC_GAIN_SET));
    }
   
    if(message.substring(0,9) == "NR_LEVEL "){
     
      String level_str = message.substring(9,11);
      level_str.trim();
      int level_int;
      level_int = level_str.toInt();

      if(level_int > 0 and level_int < 16){
        memcpy(cmd_bytes, NR_LEVELS[level_int], sizeof(NR_LEVELS[level_int]));
      }
    }
   
   
    if(message == "MIC_GAIN"){
      memcpy(cmd_bytes, MIC_GAIN,7);
    }  

    if(message == "RF_LEVEL"){
      memcpy(cmd_bytes, RF_LEVEL,7);
    }
   
    if(message == "RF_LEVEL"){
      memcpy(cmd_bytes, RF_LEVEL,7);
    }

    if(message == "RF_LEVEL_HI"){
      memcpy(cmd_bytes, RF_LEVEL_HI,sizeof(RF_LEVEL_HI));
    }

    if(message == "RF_LEVEL_LOW"){
      memcpy(cmd_bytes, RF_LEVEL_LOW,sizeof(RF_LEVEL_LOW));
    }

   
    if(message == "SET_VFO_A"){
      memcpy(cmd_bytes, SET_VFO_A,7);
    }   
   
    if(message == "SET_VFO_B"){
      memcpy(cmd_bytes, SET_VFO_B,7);
    }   

    if(message == "VFO_A_eq_B"){
      memcpy(cmd_bytes, VFO_A_eq_B,7);
    }   
   
    if(message == "VFO_A_Sw_B"){
      memcpy(cmd_bytes, VFO_A_Sw_B,7);
    }   
   
    if(message == "SPLIT_ON"){
      memcpy(cmd_bytes, SPLIT_ON,7);
    }   
    if(message == "SPLIT_OFF"){
      memcpy(cmd_bytes, SPLIT_OFF,7);
    }

    if(message == "SET_TS_ON"){
      memcpy(cmd_bytes, SET_TS_ON,7);
    }   
    if(message == "SET_TS_OFF"){
      memcpy(cmd_bytes, SET_TS_OFF,7);
    }   
   
    if(message == "ATT_ON"){
      memcpy(cmd_bytes, ATT_ON,7);
    }   
    if(message == "ATT_OFF"){
      memcpy(cmd_bytes, ATT_OFF,7);
    }       
   
    if(message == "NB_OFF" ){   
      memcpy(cmd_bytes,NB_OFF,8);
    }
    if(message == "NB_ON"){
      memcpy(cmd_bytes,NB_ON,8);
    }
   
    if(message == "PRE_AMP_ON"){
      memcpy(cmd_bytes, PRE_AMP_ON,8);
    }   
    if(message == "PRE_AMP_OFF"){
      memcpy(cmd_bytes, PRE_AMP_OFF,8);
    }
   
    if(message == "NR_ON"){
      memcpy(cmd_bytes, NR_ON,8);
    }   
    if(message == "NR_OFF"){
      memcpy(cmd_bytes, NR_OFF,8);
    }   
   
    if(message == "COMP_ON"){
      memcpy(cmd_bytes, COMP_ON,8);
    }   
    if(message == "COMP_OFF"){
      memcpy(cmd_bytes, COMP_OFF,8);
    }       

    if(message == "VOX_ON"){
      memcpy(cmd_bytes, VOX_ON,8);
    }   
    if(message == "VOX_OFF"){
      memcpy(cmd_bytes, VOX_OFF,8);
    }

    if(message == "BK_IN_ON"){
      memcpy(cmd_bytes, BK_IN_ON,8);
    }   
    if(message == "BK_IN_OFF"){
      memcpy(cmd_bytes, BK_IN_OFF,8);
    }
   
    if(message == "READ_OPERATING_MODE"){
      memcpy(cmd_bytes, READ_OPERATING_MODE,6);
    }
   
    if(message == "READ_FREQUENCYS"){
      memcpy(cmd_bytes, READ_FREQUENCYS,6);
    }

    if(message == "READ_UPPER_LOWER_FREQUENCIES"){
      memcpy(cmd_bytes, READ_UPPER_LOWER_FREQUENCIES,6);
    }   
   
    if(message == "READ_SMETER"){
      memcpy(cmd_bytes, READ_SMETER,7);
    }
   
    if(message == "SET_OPERATING_FRQ"){
      memcpy(cmd_bytes, SET_OPERATING_FRQ,sizeof(SET_OPERATING_FRQ));
    }   
 
   
   
/*
    if(message.substring(0,4) == "CMD "){
      memcpy(cmd_bytes,genericCommand(message.substring(4,message.length())),8);
    }
*/   
    //Issue Command
    for(int i = 0; i < sizeof(cmd_bytes); i = i + 1){
      Serial.print(cmd_bytes[i],HEX);
      Serial.print(" ");
    }
    Serial.println();
   
    mySerial.write(cmd_bytes,sizeof(cmd_bytes));
    //Clear the memory
    memcpy(cmd_bytes,CLEAR,8);
  }
 
  byte read_byte = mySerial.read();   
  byte read_bytes[11];
  int read_byte_index = 0;
  if(read_byte == 0xFE){
    Serial.print("RX -->:");
    Serial.print(read_byte,HEX);
    Serial.print(" ");
   
    read_bytes[read_byte_index] = read_byte;
    read_byte_index++;

      if(read_byte != 0xFF){     
        while(1 == 1){
          read_byte = mySerial.read();
          Serial.print(read_byte,HEX);
          Serial.print(" ");   
     
          read_bytes[read_byte_index] = read_byte;
          read_byte_index++;
         
          if(read_byte == 0xFD){
            Serial.println(" <--");
            break;
          }        
        }

        Serial.print("RX:");       
        for(int i = 0; i < sizeof(read_bytes); i = i + 1){
          Serial.print(i);
          Serial.print("=>");
          Serial.print(read_bytes[i],HEX);         
          Serial.print(" ");
        }       
        Serial.println();
       
        if(read_bytes[4] == 0x00 or read_bytes[4] == 0x03){
          Serial.println("ParseFreq:" + parseFreq(read_bytes));
        }
       
        if(read_bytes[4] == 0x01){
          Serial.println("ParseMode:" + parseMode(read_bytes));
        }
       
        if(read_bytes[4] == 0x04){
          Serial.println("ReadOperatingMode:" + parseMode(read_bytes));
        }
       
      }
    
  }
}

void level_convert(String message_sub,byte &one, byte &two){
  int message_input;
  int message_bin;
  String message_str;
  String message_str_bin;
  char buff[3];
  long val;
 
  message_str = message_sub.substring(0,4);
  message_str.trim();
  message_input = message_str.toInt();
  message_bin = map(message_input,0,101,0,257) + 1;
 
  if(message_bin < 100){
    message_str_bin = String(message_bin);
    message_str_bin.toCharArray(buff,3);
    char *stuff = buff;
    val = strtoul(stuff, NULL, 16);
    two = val;
  }
 
  if(message_bin > 99 and message_bin < 200){
    one = 1;
    message_bin = message_bin - 100;
    message_str_bin = String(message_bin);
    message_str_bin.toCharArray(buff,3);
   
    char *stuff = buff;
    val = strtoul(stuff, NULL, 16);
    two = val;   
   
  }
 
  if(message_bin > 199 and message_bin < 256){
    one = 2;
    message_bin = message_bin - 200;
    message_str_bin = String(message_bin);
    message_str_bin.toCharArray(buff,3);
   
    char *stuff = buff;
    val = strtoul(stuff, NULL, 16);
    two = val;   

  }
 
  Serial.println(val,HEX);
 
}

int rf_power_level(int power_int){
  int value;
  value = map(power_int,0,101,0,257) + 1;
  return value;
}

int mic_gain_level(int gain_int){
  int value;
  value = map(gain_int,0,101,0,257) + 1;
  return value;
}

int mic_gain_bin(int gain_int){
 
  int value;
  value = map(gain_int,0,255,0,101);
 
  return value;
}

String parseMode(byte rx[]){
  String mode = "";
  if(rx[5] == 0x00){
    mode = "LSB";
  }
 
  if(rx[5] == 0x01){
    mode = "USB";
  }
 
  if(rx[5] == 0x02){
    mode = "AM";
  }
 
  if(rx[5] == 0x03){
    mode = "CW";
  }
 
  if(rx[5] == 0x07){
    mode = "CW-REV";
  }
 
  if(rx[5] == 0x04){
    mode = "RTTY";
  }

  if(rx[5] == 0x08){
    mode = "RTTY-REV";
  } 
   
  return mode;
}

String parseFreq(byte rx[]) {
 
  String rx_seven;
 
  if(rx[7] < 0x10){
    rx_seven = "0" + String(rx[7],HEX);
  } else {
    rx_seven = String(rx[7],HEX);
  }
 
  String freq = String(rx[8],HEX) + "." + rx_seven + String(rx[6],HEX) + String(rx[5],HEX);
 
  if(rx[8] > 9){
    return freq.substring(0,6) + "." + freq.substring(5,7);
  } else {
    return freq.substring(0,5) + "." + freq.substring(5,7);
  }
}

//Here we define our function `readSerial`
/*
  readSerial() takes no arguments and returns a String object
*/
String readSerial() {
  //We create a temporary String named `msg` to store the message as we get it
  String msg = "";
 
  //We will be getting the message one character at a time, so this stores that character
  char receivedChar = NULL;
 
  //Keep asking for more characters as long as they're available
  while(Serial.available() > 0) {
 
    //Read the next character and save it into `receivedChar`
    receivedChar = Serial.read();
   
    //Add the new character to the end of our `msg` String
    msg = msg + receivedChar;
   
    //Wait for just a millisecond so the computer has time to send the next character
    delay(1);
  }
 
  //Now that we've built our message out of all the characters, we can return it for the
  //code that called this function to use however it wants.
  return msg;
}
