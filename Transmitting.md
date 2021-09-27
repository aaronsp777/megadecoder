# How to transmit with only a Rapsberry PI and a piece of Wire
## Download & Install rpitx
* Follow directions at https://github.com/F5OEO/rpitx
## Install Antenna
* Plug a wire on GPIO 4, means Pin 7 of the GPIO header
## Compile codetool.go

  go build codetool.go

## Calculate and Transmit the bits

  sudo sendook -f 318000000 -1 1000 -0 1000 -r 10 -p 11000 $(codetool -output bits -facility 1 -id 48881)
