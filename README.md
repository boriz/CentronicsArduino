# Centronics (Parallell Interface) Printer Emulator 
It is not uncommon for electronic enthusiast to use an outdated test gears in their labs. Actually it is pretty common even for companies to rely on vintage instruments born in 80s/90s/00s. There is probably a good reason for it, these relics are usually moderately priced and indestructible. They are extremely well built, can withstand hurricanes and small nuclear explosions. These tools are obviously outdated and lacking modern features, but pretty adequate for most cases.
The problem I am typically having (and probably other users of these antique tools) is that I can't easily take a screenshot of the instrument. You can obviously take a picture of an old CRT screen, but it just doesn't look professional. Some instruments have GPIB/HP-IB interfaces and if you are a lucky owner of GPIB adapter you can use [HP 7470A Emulator](http://www.ke5fx.com/gpib/7470.htm) emulate printer over GPIB. But what if you don't have GPIB adapter handy or your instrument don't have a GPIB port? 
Fear no more - Centronics printer emulator is to the rescue! It saves whatever it sees on the parallel port to the file on the SD card. When your instrument thinks that it send screensnot to the printer it actually sends it to the SD card. After that you can use your favorite viewer to view/print this file. This emulator can be built in probably less than an hour, using readily available parts. It built around Arduino Mega board with SD card and LCD modules.

## SD card pinout
I did not have SD card breakout board, so I used Ethrenet shield, witch happened to have microSD card connector. I don't remember where exactly I got this shield from, but it seems to be pretty standard part. For example this one looks identical to mine: [Ethrenet shield](https://www.ebay.com/itm/Ethernet-Shield-Lan-W5100-For-Arduino-Board-UNO-R3-ATMega-328-MEGA-1280-2560/322267901919)
This shield uses the following pins for the SD card communication:

| SD card pin name | Arduino pin |
|------------------|-------------|
| MOSI             | pin 11      |
| MISO             | pin 12      |
| CLK              | pin 13      |
| CS               | pin 4       |
| GND              | GND         |


## LCD pinout
I used the simples LCD module I can find in the random parts pile. It is 2x16 display module. Again, I am not 100% sure, but this one looks identical to mine: [LCD](https://www.ebay.com/itm/HOBBY-COMPONENTS-UK-LCD-1602-16x2-Keypad-Shield-For-Arduino-LA/372201166520) 
Only teh following pins are connected to the LCD shield. I left other pins dicsonnected:

| LCD pin name | Arduino pin number |
|--------------|--------------------|
| RS           | pin 8              |
| EN           | pin 9              |
| D4           | pin 4              |
| D5           | pin 5              |
| D6           | pin 6              |
| D7           | pin 7              |

Here are a few pictures on the shields stackup:



## Parallel interface
Centronics parallel interface is pretty old and it was well documented in nineties, so it was easy to figure out how to connect Arduino to it.
* Strobe: pin 18, input, pullup enabled, attached to the falling edge interrupt
* Error: pin 22, output, normally high
* Select: pin 24, output, normally high
* Paper out: pin 26, output, normally high
* Busy: pin 28, output, normally low
* Ack: pin 30, output, output, normally high
* D0: pin 39, input, pullup enabled
* D1: pin 41, input, pullup enabled
* D2: pin 43, input, pullup enabled
* D3: pin 45, input, pullup enabled
* D4: pin 47, input, pullup enabled
* D5: pin 49, input, pullup enabled
* D6: pin 46, input, pullup enabled
* D7: pin 48, input, pullup enabled

## How to use it
Configure your instrument to use parallel port for printing. Configure printer type. The device saves whatever it sees on the parallel port to the file on the SD card, so we should probably select a printer with standard protocol. One of the option is usually "HP PaintJet" printer.

