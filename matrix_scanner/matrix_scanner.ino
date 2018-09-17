#define SIZE 8
int cols[SIZE] = {16, 15, 14, 10,  9,  8,  7,  6};
int rows[SIZE] = { 2,  3,  4,  5, 18, 19, 20, 21};

void scanRows() {
  for (int i = 0; i < SIZE; i++) {
    if (digitalRead(rows[i]) == HIGH) {
      Serial.write("  1");
    } else {
      Serial.write("  0");
    }
    delay(10);
  }
}

void clearAndHome()
{
  Serial.write(27); // ESC
  Serial.print("[H"); // cursor to home
}

void setup() {
  Serial.begin(115200);
  for (int pin = 0; pin < SIZE; pin++) {
    pinMode(rows[pin], INPUT_PULLUP);
//    digitalWrite(rows[pin], HIGH);
    pinMode(cols[pin], OUTPUT);
    digitalWrite(cols[pin], HIGH);
  }
}

void printPin(int pin) {
  if (pin < 10) {
    Serial.print(0);
  }
  Serial.print(pin, DEC);
  Serial.print(" ");  
}

void loop() {
  Serial.print("    ");
  for (int pin = 0; pin < SIZE; pin++) {  
    printPin(rows[pin]);
  }
  Serial.println();
  
  for (int pin = 0; pin < SIZE; pin++) {
    printPin(cols[pin]);
    digitalWrite(cols[pin], LOW);
    scanRows();
    digitalWrite(cols[pin], HIGH);    
    Serial.println();
  }
  clearAndHome();
  delay(500);
}
