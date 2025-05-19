#define HAVE_DS2480B 1
#define DS2423_MEMORY 1

#if HAVE_DS2480B
#include <DS2480B.h>
AltSoftSerial altSerial; // RX on pin 8, TX on pin 9
DS2480B ow(altSerial);
#else
#include <OneWire.h>
OneWire ow(10); // DQ on pin 10, You also need 1k resistor between DQ and VCC
#endif
#include <DS2423.h>
DS2423 cnt(&ow);

void setup(void) {
#if DS2423_MEMORY
  byte addr[8] = {0x1D,0x41,0x42,0x0F,0x48,0x4E,0x59,0x65};
  byte buf[1]  = {0x00};
#endif
  Serial.begin(9600);
#if HAVE_DS2480B
  altSerial.begin(9600);
  ow.begin();
#endif
#if DS2423_MEMORY
  // example how to change sampling interval to 19ms for 1d41420f484e5965
  // read more at http://honey.com.ua/counter/memory_en.htm
  cnt.write(addr, buf, (word)32 * 8 + 13, 1); // page 8, offset 13
#endif
}

void loop(void) {
  byte i;
  byte addr[8];
  unsigned long a, b;
#if DS2423_MEMORY
  byte n;
  byte buf[32];
#endif

  if (!ow.search(addr)) {
    Serial.println("No more addresses.");
    Serial.println();
    ow.reset_search();
    delay(5000);
    return;
  }

  Serial.print("ROM =");
  for (i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }

  if (ow.crc8(addr, 8)) {
    Serial.println("CRC is not valid!");
    return;
  }
  Serial.println();

  // the first ROM byte indicates which chip
  if (addr[0] != 0x1d) {
    Serial.println("Device is not a DS2423 family device.");
    return;
  }

#if DS2423_MEMORY
  // read memory example
  n = cnt.read(addr, buf, 0, 32);
  Serial.print("Page0:");
  for (i = 0; i < n; i++) {
    Serial.write(' ');
    Serial.print(buf[i], HEX);
  }
  Serial.println();
#endif

  if (!cnt.counter(addr, 0, &a)) {
    Serial.println("CRC16 is not valid!");
    return;
  }
  if (!cnt.counter(addr, 1, &b)) {
    Serial.println("CRC16 is not valid!");
    return;
  }
  Serial.print("  Counter.A = ");
  Serial.println(a);
  Serial.print("  Counter.B = ");
  Serial.println(b);
}

