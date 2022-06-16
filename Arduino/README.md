# DS2423 Library for Arduino

## Dependencies
* OneWire
  https://www.pjrc.com/teensy/td_libs_OneWire.html
* AltSoftSerial
  https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html
* DS2480B
  https://github.com/collin80/DS2480B

## Usage

```
#include <DS2423.h>
OneWire ow(10);
DS2423 cnt(&ow);

byte addr[] = {0x1D,0x41,0x42,0x0F,0x48,0x4E,0x59,0x65};

void setup(void) {
  Serial.begin(9600);
}

void loop(void) {
  unsigned long a, b;
  if (cnt.counter(addr, 0, &a) && cnt.counter(addr, 1, &b)) {
    Serial.print("Counter.A = ");
    Serial.println(a);
    Serial.print("Counter.B = ");
    Serial.println(b);
  } else {
    Serial.println("Failed to read counters");
  }
  delay(1000);
}
```

Look examples subdir for more examples.

## Library functions
* ```uint8_t counter(uint8_t *addr, uint8_t num, uint32_t *counterp)```  
  Reads counter value from device  
  return value: true on success, false on error  
  num: 0 | 1  
  counterp: pointer where to place result  


