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

| Name      | Centronics pin. DB25 connector | Arduino pin | Arduino pin direction | Notes                                                                     |
|-----------|--------------------------------|-------------|-----------------------|---------------------------------------------------------------------------|
| Strobe    | 1                              | pin 18      | Input                 | Pullup enabled. Attached to falling edge interrupt.                       |
| Error     | 15                             | pin 22      | Output                | Not used. Forced high.                                                    |
| Select    | 13                             | pin 24      | Output                | Not used. Forced high.                                                    |
| Paper Out | 12                             | pin 26      | Output                | Not used. Forced high.                                                    |
| Busy      | 11                             | pin 28      | Output                | Set high on the falling edge of Strobe. Set low after acknowledging data. |
| Ack       | 10                             | pin 30      | Output                | Generate falling edge to acknowledge data.                                |
| D0        | 2                              | pin 39      | Input                 | Parallel data.                                                            |
| D1        | 3                              | pin 41      | Input                 | Parallel data.                                                            |
| D2        | 4                              | pin 43      | Input                 | Parallel data.                                                            |
| D3        | 5                              | pin 45      | Input                 | Parallel data.                                                            |
| D4        | 6                              | pin 47      | Input                 | Parallel data.                                                            |
| D5        | 7                              | pin 49      | Input                 | Parallel data.                                                            |
| D6        | 8                              | pin 46      | Input                 | Parallel data.                                                            |
| D7        | 9                              | pin 48      | Input                 | Parallel data.                                                            |

## How to use it
Insert SD card and press reset button on the Arduino. It should dispaly "Ready" message on the LCD.
Configure your instrument to use Centronics (parallel port) for printing. 
Configure printer type. The device saves whatever it sees on the parallel port to the file on the SD card, so we should probably select a printer with standard protocol. 
* HP 54522A oscilloscope. Select "HP 7470A" plotter. The output would be a standard HP-GL format.
* Tektronix TDS2024 scope. Configure "RLE" format. You can open these files with MS Paint
* HP 8594E spectrum analyzer. Select Plotter ("PLT") option. The output would be a standard HP-GL format.
Press "Print" (or "Copy") button, wait for "Done" message on the device LCD.

To view HP_GL files I am using free and open-source [HP-GL Viewer](http://service-hpglview.web.cern.ch/service-hpglview/download_index.html) from CERN. But you should be able to use any other HP-GL viewers, including [HP 7470A Emulator](http://www.ke5fx.com/gpib/7470.htm) I mentioned above.
RLE is a standard bitmap, you should be able to open it with Microsoft Paint or any other graphic editor.

Here is a quick demonstration video: https://youtu.be/vRhbX8HyUxA
