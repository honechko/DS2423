#ifndef DS2423_h
#define DS2423_h

#ifndef HAVE_DS2480B
#define HAVE_DS2480B 0
#endif

#if HAVE_DS2480B
#include <DS2480B.h>
#else
#include <OneWire.h>
#endif

class DS2423 {
  private:
#if HAVE_DS2480B
    DS2480B *_ow;
#else
    OneWire *_ow;
#endif

  public:
#if HAVE_DS2480B
    DS2423(DS2480B *ow) {
#else
    DS2423(OneWire *ow) {
#endif
      _ow = ow;
    };

    uint8_t counter(uint8_t *addr, uint8_t num, uint32_t *cp) {
      uint8_t buf[14];
      _ow->reset();
      _ow->select(addr);
      buf[0] = 0xa5; // READ MEM COUNTER
      buf[1] = num ? 0xff : 0xdf;
      buf[2] = 0x01;
      _ow->write_bytes(buf, 3);
      _ow->read_bytes(buf + 3, 11);
      // buf[3] - last byte of page, buf[4..7] - 32 bit counter,
      // buf[8..11] - zeroes, buf[12..13] - crc16
      if (!_ow->check_crc16(buf, 12, &buf[12]))
        return 0;
      *cp = ((uint32_t)buf[7] << 24) | ((uint32_t)buf[6] << 16) |
            ((uint32_t)buf[5] << 8)  |  (uint32_t)buf[4];
      return 1;
    };

#if defined(DS2423_MEMORY) && DS2423_MEMORY
    uint8_t read(uint8_t *addr, uint8_t *buf, uint16_t start, uint8_t cnt) {
      if (start + cnt > (start & 0xffe0) + 0x20)
        cnt = (start & 0xffe0) + 0x20 - start; // limit to page boundary
      _ow->reset();
      _ow->select(addr);
      _ow->write(0xa5); // READ MEM COUNTER
      _ow->write(start & 0xff);
      _ow->write(start >> 8);
      _ow->read_bytes(buf, cnt);
      return cnt;
    };
    uint8_t write(uint8_t *addr, uint8_t *buf, uint16_t start, uint8_t cnt) {
      if (start + cnt > (start & 0xffe0) + 0x20)
        cnt = (start & 0xffe0) + 0x20 - start; // limit to page boundary
      if (!cnt)
        return 0;
      _ow->reset();
      _ow->select(addr);
      _ow->write(0x0f); // WRITE SCRATCHPAD
      _ow->write(start & 0xff);
      _ow->write(start >> 8);
      _ow->write_bytes(buf, cnt);
      _ow->reset();
      _ow->select(addr);
      _ow->write(0x5a); // COPY SCRATCHPAD
      _ow->write(start & 0xff);
      _ow->write(start >> 8);
      _ow->write((start & 0x1f) + (cnt - 1));
      return cnt;
    };
#endif
};

#endif
