
const int rxPin = 8;

void setup() {
  pinMode(rxPin, INPUT);
  Serial.begin(115200);
  Serial.println("Ready to receive...");
}

bool wait_off(unsigned long delay) {
  unsigned long start = micros();
  while(micros() - start < delay) {
    if (digitalRead(rxPin)) {
      return false;
    }
  }
  return true;
}

void displayOutput(unsigned long data) {
  unsigned int code = (data >> 3);
  Serial.print(data, HEX);
  Serial.print("\t(");
  Serial.print(code, DEC);
  Serial.println(")");
}

// Returns the time the bit was received.
// Or 0 for timeout or other framing issue.
// A zero start time of 0 means we are waiting for the first bit.
unsigned long time_next_bit(unsigned long start_time) {

  if (start_time == 0) {
    // Receiving first bit.
    // Ensure radio silence between transmissions.
    while (wait_off(8500)) {};  // 8.5uS

    while (!digitalRead(rxPin)) {}; // Wait without Deadline
    unsigned long now = micros();
    delayMicroseconds(1200);  // Wait for bit to pass.
    if (digitalRead(rxPin)) {
      Serial.println("first bit: overrun.");
      return 0;  // Still receiving a signal after 1.2ms.
    }
    return now;
  }

  long delay = start_time - micros();
  if (delay > 0) {
    delayMicroseconds(delay);
  }

  // Receiving a followup bit.
  while(!digitalRead(rxPin)) {
    delay = micros() - start_time;
    if (delay > 4500) {
      Serial.println("followup bit: timeout");
      break;  // Timeout.
    }
  }

  unsigned long now = micros();
  delayMicroseconds(1200);  // Wait for bit to pass.
  if (digitalRead(rxPin)) {
    Serial.println("followup bit: overrun.");
    return 0;  // Still receiving a signal after 1.2ms.
  }
  return now;
}

void loop() {
  unsigned long start_time = 0;

  start_time = time_next_bit(0);
  start_time += 2000;
  unsigned long data = 1;
  int bits = 1;

  while (bits++ < 24) {
    unsigned long next_start = time_next_bit(start_time);
    long delay = next_start - start_time;

    if (500 < delay && delay < 1500) {
      start_time = next_start + 5000;
      data<<=1;  // Push bit value 0.
      continue;
    }

    if (3500 < delay && delay < 4500) {
      start_time = next_start + 2000;
      data<<=1;
      data|=1;  // Push bit value 1.
      continue;
    }

    // Else failure.
    Serial.print("Bit ");
    Serial.print(bits);
    Serial.print(" received at ");
    Serial.print(delay);
    Serial.println(" microseconds.");
    return;
  }

  displayOutput(data);
}
