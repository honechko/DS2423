#include <DS2423.h>
OneWire ow(10); // DQ on pin 10, You also need 1k resistor between DQ and VCC
DS2423 cnt(&ow);

void setup(void) {
  Serial.begin(9600);
}

void loop(void) {
  byte i;
  byte addr[8];
  unsigned long a, b;

  if (!ow.search(addr)) {
    Serial.println("No more addresses.");
    ow.reset_search();
    delay(5000);
    return;
  }

  Serial.print("ROM =");
  for (i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }
  Serial.println();

  if (addr[0] == 0x1d &&
      cnt.counter(addr, 0, &a) && cnt.counter(addr, 1, &b)) {
    Serial.print("  Counter.A = ");
    Serial.println(a);
    Serial.print("  Counter.B = ");
    Serial.println(b);
  }
}

