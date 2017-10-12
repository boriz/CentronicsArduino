// Arduino Centronics printer capture tool 
// Using Ethernet shield with built-in SD card
// SD card pinout:
// MOSI: pin 11
// MISO: pin 12
// CLK:  pin 13
// CS:   pin 4


#include <SPI.h>
#include <SD.h>


// Global variables/flags
bool print_in_progress = false;
bool data_ready = false;
byte data = 0;
byte buff[512];
int buff_index = 0;
long last_update;
File current_file;


void setup() 
{
  Serial.begin(115200);
  delay(100);

  // Initialize SD card
  Serial.println("Init SD");
  pinMode(53, OUTPUT);  // HW CS pin, init it just in case
  pinMode(4, OUTPUT);
  if ( !SD.begin(4) ) 
  {
    Serial.println("SD Init Failed");
  }
  else
  {
    Serial.println("SD Init Ok");
  }
  
  // Configure pins
  pinMode(18, INPUT_PULLUP); // Strobe - normally high
  attachInterrupt(digitalPinToInterrupt(18), StrobeFallingEdge, FALLING);

  pinMode(22, OUTPUT);  // Error - normally high
  digitalWrite(22, true);

  pinMode(24, OUTPUT);  // Select - normally high
  digitalWrite(24, true);

  pinMode(26, OUTPUT);  // Paper out - normally low
  digitalWrite(26, false);

  pinMode(28, OUTPUT);  // Busy - normally low
  digitalWrite(28, false);
  
  pinMode(30, OUTPUT);  // Ack - normally high
  digitalWrite(30, true);

  pinMode(39, INPUT_PULLUP);  // D0
  pinMode(41, INPUT_PULLUP);  // D1
  pinMode(43, INPUT_PULLUP);  // D2
  pinMode(45, INPUT_PULLUP);  // D3
  pinMode(47, INPUT_PULLUP);  // D4
  pinMode(49, INPUT_PULLUP);  // D5
  pinMode(46, INPUT_PULLUP);  // D6
  pinMode(48, INPUT_PULLUP);  // D7

  // Update timeout
  last_update = millis();
  Serial.println("Init Complete");
}


void loop() 
{
  if (data_ready)
  {
    // Receive byte
    buff[buff_index] = data;
    buff_index++;

    // Reset data ready flag
    data_ready = false;
    
    // Ack byte, reset busy
    digitalWrite(30, false);  // ACK
    delayMicroseconds(7);
    digitalWrite(28, false);  // BUSY
    delayMicroseconds(5);
    digitalWrite(30, true);   // ACK

    // Reset timeout
    last_update = millis();

    // Actively printing?
    if (!print_in_progress)
    {
      // Just started printing. Create new file
      CreateNewFile();
      Serial.print("Receiving from printer.");
    }
    print_in_progress = true;
  }

  // Check buffer size
  if(buff_index >= 512)
  {
    // Flush buffer to file
    Serial.print(".");
    WriteToFile(buff, sizeof(buff));    
    buff_index = 0;
  }
  
  if ( print_in_progress && (millis() > (last_update + 1000)) )
  {
    // Timeout. Flush the buffer to file
    if (buff_index > 0)
    {
      WriteToFile(buff, buff_index - 1);
      buff_index = 0;
    }
    Serial.println(".Done");
    Serial.print("Closing file..");
    current_file.close();
    Serial.println("..Ok");
    print_in_progress = false;
  } 
}


void CreateNewFile()
{  
  // Find unique file 
  char fname[30];  
  int i = 1;  
  do
  {
    sprintf (fname, "sa%03d.plt", i);
    i++;
  } while(SD.exists(fname));

  // Found new file
  Serial.println();
  current_file = SD.open(fname, FILE_WRITE);
  Serial.print("New file created: ");
  Serial.println(fname);
}


void WriteToFile(byte* b, int b_size)
{
  // Verify that the file is open
  if (current_file) 
  {
    current_file.write(b, b_size);
  }
  else 
  {
    Serial.println();
    Serial.println("Can't write to file");
  }
}


// Strobe pin on falling edge interrupt
void StrobeFallingEdge()
{
    // Set busy signal
    digitalWrite(28, true);

    // Read data from port
    data = (digitalRead(39) << 0) | 
           (digitalRead(41) << 1) | 
           (digitalRead(43) << 2) | 
           (digitalRead(45) << 3) |
           (digitalRead(47) << 4) |
           (digitalRead(49) << 5) |
           (digitalRead(46) << 6) |
           (digitalRead(48) << 7);

    // Set ready bit
    data_ready = true;    
}

