
const int rxPin = 8;

void setup() {
  pinMode(rxPin, INPUT);
  Serial.begin(115200);
  Serial.println("Ready to receive...");

}

// Waits for a leading edge of the next bit.  Returns 0 on timeout.
int wait_on() {
  unsigned long start = micros();

  // Delay 1.2ms
  while(micros() - start < 1200) {};
  
  // Wait until we recive a low.
  while(digitalRead(rxPin)) {
    if (micros() - start > 1500) return 0;
  }
  // Wait until we recive a high.
  start = micros();
  while(!digitalRead(rxPin)) {
    if (micros() - start > 9000) return 0;
  }
  return 1;
}

// Waits for 5ms ensuring signal stays off.  Returns 1 if we saw silence the whole time.
int wait5ms_off() {
  unsigned long start = micros();
  while(micros() - start < 10000) {
    if (digitalRead(rxPin)) {
      return 0;
    }
  }
  return 1;
}

void loop() {
  while (!wait5ms_off()) {};  // Ensure radio silence between transmissions.
  Serial.println("\nsilence.\n");
  while (!wait_on()) {};  // Get start bit.

  unsigned long start_time = micros()-4500;
  int bits = 0;

  for (int bits=0; bits < 30; bits++) {
    if (micros() - start_time > 150000) break;
    unsigned long t = micros() - start_time;
    Serial.print(" ");
    Serial.print(t/1000);
    wait_on(); 
  }

}
