
const int rxPin = 2;
unsigned long data; 

void setup() {
  pinMode(rxPin, INPUT);
  Serial.begin(9600);
  Serial.println("Ready to receive...");
}

// Waits for a leading edge of the next bit.  Returns 0 on timeout.
int wait_on() {
  unsigned long start = micros();
  // Wait until we recive a low.
  while(digitalRead(rxPin)) {
    if (micros() - start > 5000) return 0;
  }
  // Wait until we recive a high.
  while(!digitalRead(rxPin)) {
    if (micros() - start > 5000) return 0;
  }
  return 1;
}

// Waits for 5ms ensuring signal stays off.  Returns 1 if we saw silence the whole time.
int wait5ms_off() {
  unsigned long start = micros();
  while(micros() - start < 5000) {
    if (digitalRead(rxPin)) {
      return 0;
    }
  }
  return 1;
}

void displayOutput(unsigned long data) {
  unsigned int code = (data >> 3);
  Serial.print(data, HEX);
  Serial.print("\t(");
  Serial.print(code, DEC);
  Serial.println(")");
}

/*
 * Based on the time difference between leading edge of current bit and start bit,
 * Determines which bitnumber this should be and directly updates the data register.
 * Returns the bit number (zero based) that was received. 
 */
int rxbit(unsigned long time_us) {
  int subbit = (time_us - 3500) / 1000;
  int bit = subbit / 6;
  if (1 != subbit % 3) {
    return -1;  // on is too early or late by 1ms.
  }
  if (bit > 23) {
    return -1;  // received later than 24 bits
  }
  data<<=1;  // shift data register.
  if (3 <= subbit %6) {
    data|=1;  // received bit value of true.
  }
  return bit;
}

void loop() {
  while (!wait5ms_off()) {};  // Ensure radio silence between transmissions.
  while (!wait_on()) {};  // Get start bit.
  
  unsigned long start_time = micros();
  int bits = 1;  // Already received first bit
  data = 1;  // First bit was true.

  while (bits < 24) {
    if (bits != rxbit(micros() - start_time)) {
      return;  // bits received at wrong time.
    }
    if (!wait_on()) {
      return;  // timeout on next bit
    }
    bits++;
  }

  displayOutput(data);
}
