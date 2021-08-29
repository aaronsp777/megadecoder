# megadecoder
Decoder for Linear Megacode Remotes

# Required Parts
* An Arduino that works with 5V logic.
  Some only do 3.3V.
* A 318Mhz RF Digital Receiver.  I used this 315Mhz receiver instead:
  https://www.amazon.com/gp/product/B00LNADJS6

# Instructions
* Hookup the receiver's output to Pin 8 on your arduino.
* Attach 5V & Ground.
* Attaching a short coiled wire antenna will help reception.
* Program your adruino and view the console at 115200 Baud.

You can find the code to program in [megadecoder.ino](megadecoder.ino).

For those curious one can find more information about the [Transmission Protocol](Protocol.md).
