# Linear Megacode Protocol.

## Carrier and Timing

The remotes are supposed to transmit at 318.0 MHz OOK, though I've seen the MCT-11 stray significantly off frequency to the point a Linear Receiver cannot even receive the transmission.  The MCT-11 has a potentiometer you can change on the board but I've found it falls out of calibration within a month.  Remotes such as the ACT-31B and ACT-34B seem to be much more frequency stable.

A Linear Megacode transmission consists of 24 bit frames starting with the most significant bit and ending with the least.  Each of the 24 bit frames is 6 milliseconds wide and always contains a single 1 millisecond pulse.  A frame with more than 1 pulse or a frame with no pulse is invalid and a receiver should reset and begin watching for another start bit.

The position of the pulse within the bit frame determines if it represents a binary 0 or binary 1.  If the pulse is within the first half of the frame, it represents binary 0.  The second half of the frame represents a binary 1.

[image]

Since each bit contains a pulse, the protocol is self clocking since the time can be measured from when the most recent bit frame was decoded.  This is particularly important since most of the remotes I have seen  have varied in their clock rates by up to 15%.  My first decoding approach failed because I decoded the time delta since the very first bit and my decoder would often drift 2-3 bit frames before decoding the last bit frame.

The 24 bits are always the same very time the remote is pressed.  There is no "rolling code" support in this protocol.  In theory cloning & replay attacks would be possible.

[link to cookoo vudo]  His work typically records the 24 bits as 6 hex digits.  So my code also shows this for completeness.

## Decoding the bits

The first bit (the start bit) is always a binary 1, followed by a 4-bit "Facility" code, a unique 16-bit "Remote" code, and a 3-bit "Button" code.

## Facility Codes, Remote Codes, and Button Codes

### Facility Codes

Remotes are programmed with a 4-bit facility code that ranges from 1 to 
15.  I've never seen 0 used on a transmitter, though often 0 is used on 
receivers to mean match any facility code number.

### Unique Remote Code
Remotes are typically sold either individually and have a unique 16-bit 
"Remote" code printed in decimal on a sticky label.  The label is 
important in that many receivers require the number for programming from 
a touch-tone keypad.

With 16 bits, in theory his yields values in theory from 1 to 65535.  
Values less than 10000 will have a leading 0.  I've never seen a value 
less than 1000.

Bulk packages of transmitters are often sold in blocks of 100 and have a 
start range and end range of codes.  Each remote has a number higher 
than the previous in the pack.  They typically also share the same 
Facility code.  This allows convenient "block programming" on the 
receiver.

I've heard of resellers breaking these packs up and selling them 
indivually though typically they don't have the sticker on the back of 
each remote with the code necessary for programming most systems that 
require the number.  However they do work with Linear receivers that 
have a "learn button".

I mainly made did this project so I could figure out what the code is 
when there is no label.


## Button Codes

With 3 bits in theory 8 buttons are possible.  Single button remotes 
always send a value of 2.  Here are common button numbers I've seen:

1 - Top left button
2 - Default or Large button on most remotes
5 - round red lower right button
6 - round grey lower left button

Some receivers can be programmed to have different buttons operate 
different relays by system policy.