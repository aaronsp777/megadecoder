package main

import (
	"flag"
	"fmt"
	"log"
	"strconv"
	"strings"
)

var (
	facility = flag.Int("facility", -1, "Facility code (0-15)")
	id       = flag.Int("id", -1, "Transmitter ID (10001-65535)")
	button   = flag.Int("button", 2, "Button Code (0-7)")
	raw_s    = flag.String("raw", "", "Raw transmitter (Hexidecimal)")
	bits_s   = flag.String("bits", "", "130-140 bits")
	output   = flag.String("output", "all", "...")
)

// TODO - add invoke flag.Usage

func decodeFlags() (int, error) {
	if *raw_s != "" {
		raw, err := strconv.ParseInt(*raw_s, 16, 25)
		return int(raw), err
	} else if *bits_s != "" {
		return bitsToRaw(*bits_s)
	} else if *facility != -1 || *id != -1 {
		if *facility == -1 || *id == -1 {
			return 0, fmt.Errorf("Specify -facility and -id together")
		}
		if *facility < 0 || *facility > 15 {
			return 0, fmt.Errorf("-facility outside of 0..15")
		}
		if *id < 0 || *id > 65535 {
			return 0, fmt.Errorf("-id outside of 0..65535")
		}
		if *button < 0 || *button > 7 {
			return 0, fmt.Errorf("-button outside of 0..7")
		}
		return codesToRaw(*facility, *id, *button)
	}
	// TODO - show usage with no args
	return 0, nil
}

func bitsToRaw(bits string) (int, error) {
	// TODO: write
	return 0, nil
}

func rawToBits(raw int) (out string) {
	v := raw
	for i := 0; i < 24; i++ {
		if v%2 == 0 {
			out = "001000" + out
		} else {
			out = "000001" + out
		}
		v /= 2
	}
	return strings.Trim(out, "0")
}

func codesToRaw(facility, id, button int) (int, error) {
	return 1<<23 | facility<<19 | id<<3 | button, nil
}

func RawToCodes(raw int) (int, int, int) {
	return (raw >> 19) & 15, (raw >> 3) & 65535, raw & 7
}

func main() {
	var raw int
	flag.Parse()
	raw, err := decodeFlags()
	*facility, *id, *button = RawToCodes(raw)
	if err != nil {
		log.Fatal(err)
	}
	switch *output {
	case "all":
		fmt.Printf("raw: %06X\n", raw)
		fmt.Printf("facility: %d id: %d button: %d\n", *facility, *id, *button)
		fmt.Println("bits: " + rawToBits(raw))
	case "raw":
		fmt.Printf("%06X\n", raw)
	case "facility":
		fmt.Println(*facility)
	case "id":
		fmt.Println(*id)
	case "button":
		fmt.Println(*button)
	case "bits":
		fmt.Println(rawToBits(raw))
	default:
		log.Fatal(fmt.Errorf("Unknown output: '%s', ", *output))
	}
}
