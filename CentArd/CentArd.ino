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
File dataFile;


void setup() 
{
  Serial.begin(115200);
  delay(100);
  
  Serial.println("Init SD");
  if (!SD.begin(4)) 
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
  pinMode(51, INPUT_PULLUP);  // D6
  pinMode(53, INPUT_PULLUP);  // D7

  // Update timeout
  last_update = millis();
  
  Serial.println("Init Complete");
}


void loop() 
{
  if (data_ready)
  {
    // Ack byte, reset busy
    digitalWrite(30, false);  // ACK
    delayMicroseconds(7);
    digitalWrite(28, false);  // BUSY
    delayMicroseconds(5);
    digitalWrite(30, true);   // ACK

    Serial.print("Got byte: ");
    Serial.println(data);
    buff[buff_index] = data;
    buff_index++;

    // Reset data ready flag
    data_ready = false;

    // Reset timeout
    last_update = millis();

    // Actively printing
    print_in_progress = true;
  }

  // Check buffer size
  if(buff_index >= 512)
  {
    // Flush buffer to file
    Serial.println("Flushing buffer to file");
    WriteToFile(buff, sizeof(buff));    
    buff_index = 0;
  }
  
  if ( print_in_progress && (millis() > (last_update + 1000)) )
  {
    // Timeout. Flush the buffer to file
    Serial.println("Timeout. Closing the file");
    if (buff_index > 0)
    {
      WriteToFile(buff, buff_index - 1);
      buff_index = 0;
    }
    dataFile.close();
    print_in_progress = false;
  }

  // TODO: Create new file each time
/*
  while (true) 
  {
    File entry =  dir.openNextFile();
    if (!entry) 
    {
      // no more files
      break;
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
*/
  
}


void WriteToFile(byte* b, int b_size)
{
  // Flush buffer to file
  if (!dataFile)
  {
    // Create file
    dataFile = SD.open("out.prn", FILE_WRITE);
  }
  
  if (dataFile) 
  {
    dataFile.write(b, b_size);
  }
  else 
  {
    Serial.println("Can't open file");
  }
}


// Strobe pin on falling edge interrupt
void StrobeFallingEdge()
{
    // Set busy signal
    digitalWrite(28, true);

    // Read data from port
    byte data = (digitalRead(39) << 0) | 
                (digitalRead(41) << 1) | 
                (digitalRead(43) << 2) | 
                (digitalRead(45) << 3) |
                (digitalRead(47) << 4) |
                (digitalRead(49) << 5) |
                (digitalRead(51) << 6) |
                (digitalRead(53) << 7);

    // Set ready bit
    data_ready = true;    
}

