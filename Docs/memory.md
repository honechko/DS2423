1-wire pulse counter
====================

Device memory
-------------

**Memory pages, counters, scratchpad**

Address space of the device consists of 16 memory pages, each 32 bytes in size:

| Page | Addresses | Contents | Counter |
| --- | --- | --- | --- |
| 0   | 0x000 .. 0x01f | Settings and status data | -   |
| 1   | 0x020 .. 0x03f | Reserved | -   |
| 2   | 0x040 .. 0x05f | Reserved | -   |
| 3   | 0x060 .. 0x07f | Reserved | -   |
| 4   | 0x080 .. 0x09f | Reserved | -   |
| 5   | 0x0a0 .. 0x0bf | Scratchpad | -   |
| 6   | 0x0c0 .. 0x0df | User data, status data | -   |
| 7   | 0x0e0 .. 0x0ff | Scratchpad (copy) | -   |
| 8   | 0x100 .. 0x11f | Settings and status data (copy) | -   |
| 9   | 0x120 .. 0x13f | Reserved | -   |
| 10  | 0x140 .. 0x15f | Reserved | -   |
| 11  | 0x160 .. 0x17f | Reserved | -   |
| 12  | 0x180 .. 0x19f | Reserved | -   |
| 13  | 0x1a0 .. 0x1bf | Scratchpad (copy) | -   |
| 14  | 0x1c0 .. 0x1df | User data, status data (copy) | A   |
| 15  | 0x1e0 .. 0x1ff | Scratchpad (copy) | B   |

Each memory page has a 32-bit counter associated with it. Counters A and B are tied to pages 14 and 15, respectively, the counters of the remaining pages are not used and always have a value of 0. The value of any of the 16 counters can be obtained with a special request (0xA5 READ MEM COUNTER), which guarantees atomic reading of the counter. The value of counters A and B can also be obtained by reading bytes 0..7 of pages 0 or 8, in which the values of counters A and B are mapped, but in this case the reading is not performed atomically (increment of counter can occur between sequential reading of counter bytes). Writing counters A and B is possible only by writing to bytes 0..7 of memory page 0, writing will be performed atomically.

Reading data from memory addresses is performed directly (0xA5 READ MEM COUNTER), while writing is performed with the help of a special 32 bytes Scratchpad memory area: first, data is written to Scratchpad (0x0F WRITE SCRATCH), and then atomically copied from Scratchpad to specified memory addresses (0x5A COPY SCRATCH). The contents of Scratchpad can be read either by a special request (0xAA READ SCRATCH) or by reading memory pages 5,7,13 and 15, in which Scratchpad is mapped. Writing to Scratchpad or any of pages 5,7,13 or 15 changes only Scratchpad and its copies in those pages. Writing to other pages will overwrite Scratchpad and its copies with the written data.

**Settings and status data**

The state and settings of the device are stored in a memory area, one part of which is mapped in memory pages 0 and 8 (both are mapped to the same part of the state and settings) and other part in bytes 24..31 of pages 6 and 14 (both are mapped to the same part of the state).

All state and settings bytes are available for reading. Only bytes 0..7 of page 0 (counter values) and bytes 8..13 of page 8 (settings) are available for writing.

All bytes in pages 1..4, 9..12, as well as bytes 14..16, 18..31 in pages 0 and 8 and bytes 25..31 in pages 6 and 14 are reserved.

**Addresses 0x000..0x003 and 0x100..0x103**

<table>
<tr><td>Bit</td><td>7</td><td>6</td><td>5</td><td>4</td><td>3</td><td>2</td><td>1</td><td>0</td></tr>
<tr><td>Offset in page: <b>0</b></td><td colspan=8 align=center>Counter A:[7..0]</td></tr>
<tr><td>Offset in page: <b>1</b></td><td colspan=8 align=center>Counter A:[15..8]</td></tr>
<tr><td>Offset in page: <b>2</b></td><td colspan=8 align=center>Counter A:[23..16]</td></tr>
<tr><td>Offset in page: <b>3</b></td><td colspan=8 align=center>Counter A:[31..24]</td></tr>
<tr><td>Access in page <b>0</b></td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td></tr>
<tr><td>Access in page <b>8</b></td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td></tr>
<tr><td>Default</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td></tr>
</table>

Counter A in normal operating mode (not in Measure mode), 4 bytes, LSB first.

<table>
<tr><td>Bit</td><td>7</td><td>6</td><td>5</td><td>4</td><td>3</td><td>2</td><td>1</td><td>0</td></tr>
<tr><td>Offset in page: <b>0</b></td><td colspan=8 align=center>Counter A:[7..0]</td></tr>
<tr><td>Offset in page: <b>1</b></td><td colspan=8 align=center>Counter A:[15..8]</td></tr>
<tr><td>Offset in page: <b>2</b></td><td colspan=8 align=center>Measure Min:[7..0]</td></tr>
<tr><td>Offset in page: <b>3</b></td><td colspan=8 align=center>Measure Max:[7..0]</td></tr>
<tr><td>Access in page <b>0</b></td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td></tr>
<tr><td>Access in page <b>8</b></td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td></tr>
<tr><td>Default</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td></tr>
</table>

In the Measure mode, two most significant bytes of counter A are used to accumulate statistics of the minimum and maximum value of the digitized analog signal for samples that have passed the filter (if the analog mode is activated by setting: threshold rising > threshold falling, otherwise min and max values are undefined).

**Addresses 0x004..0x007 and 0x104..0x107**

<table>
<tr><td>Bit</td><td>7</td><td>6</td><td>5</td><td>4</td><td>3</td><td>2</td><td>1</td><td>0</td></tr>
<tr><td>Offset in page: <b>4</b></td><td colspan=8 align=center>Counter B:[7..0]</td></tr>
<tr><td>Offset in page: <b>5</b></td><td colspan=8 align=center>Counter B:[15..8]</td></tr>
<tr><td>Offset in page: <b>6</b></td><td colspan=8 align=center>Counter B:[23..16]</td></tr>
<tr><td>Offset in page: <b>7</b></td><td colspan=8 align=center>Counter B:[31..24]</td></tr>
<tr><td>Access in page <b>0</b></td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td></tr>
<tr><td>Access in page <b>8</b></td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td></tr>
<tr><td>Default</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td></tr>
</table>

Counter B in normal operating mode (not in Measure mode), 4 bytes, LSB first.

<table>
<tr><td>Bit</td><td>7</td><td>6</td><td>5</td><td>4</td><td>3</td><td>2</td><td>1</td><td>0</td></tr>
<tr><td>Offset in page: <b>4</b></td><td colspan=8 align=center>Counter B:[7..0]</td></tr>
<tr><td>Offset in page: <b>5</b></td><td colspan=8 align=center>Counter B:[15..8]</td></tr>
<tr><td>Offset in page: <b>6</b></td><td colspan=8 align=center>Measure Samples:[7..0]</td></tr>
<tr><td>Offset in page: <b>7</b></td><td colspan=8 align=center>Measure Samples:[15..8]</td></tr>
<tr><td>Access in page <b>0</b></td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td></tr>
<tr><td>Access in page <b>8</b></td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td></tr>
<tr><td>Default</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td></tr>
</table>

In the Measure mode, two most significant bytes of counter B are used to accumulate statistics of the number of samples that have passed the filter. Samples, 2 bytes, LSB first.

**Addresses 0x008..0x00b and 0x108..0x10b**

<table>
<tr><td>Bit</td><td>7</td><td>6</td><td>5</td><td>4</td><td>3</td><td>2</td><td>1</td><td>0</td></tr>
<tr><td>Offset in page: <b>8</b></td><td colspan=8 align=center>Threshold falling A:[7..0]</td></tr>
<tr><td>Offset in page: <b>9</b></td><td colspan=8 align=center>Threshold rising A:[7..0]</td></tr>
<tr><td>Offset in page: <b>10</b></td><td colspan=8 align=center>Threshold falling B:[7..0]</td></tr>
<tr><td>Offset in page: <b>11</b></td><td colspan=8 align=center>Threshold rising B:[7..0]</td></tr>
<tr><td>Access in page <b>0</b></td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td></tr>
<tr><td>Access in page <b>8</b></td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td></tr>
<tr><td>Default</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td></tr>
</table>

Thresholds - values used to convert digitized analog value to logical state:

| Digitized analog value on input is V | Converted logical state of input |
| --- | --- |
| Threshold falling >= Threshold rising | conversion is off, state is read directly from input |
| V < Threshold falling | low |
| V > Threshold rising | high |
| Threshold falling <= V <= Threshold rising | previous state |

**Addresses 0x00c and 0x10c**

<table>
<tr><td>Bit</td><td>7</td><td>6</td><td>5</td><td>4</td><td>3</td><td>2</td><td>1</td><td>0</td></tr>
<tr><td>Offset in page: <b>12</b></td><td>PINCA</td><td>PINCB</td><td>PUDA</td><td>PUDB</td><td>SPWR1</td><td>SPWR0</td><td>PUT1</td><td>PUT0</td></tr>
<tr><td>Access in page <b>0</b></td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td></tr>
<tr><td>Access in page <b>8</b></td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td></tr>
<tr><td>Default</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td></tr>
</table>

PINCA/PINCB - enable counter A/B increment mode on both edges (see table of states in EDGEA/EDGEB bit description).

PUDA/PUDB - disable pull-up resistor between power supply and A/B input:

| PUD | Mode |
| --- | --- |
| 0   | Pull-up enabled |
| 1   | Pull-up disabled |

SPWR\[1..0\] - sensors power supply mode:  

| SPWR1 | SPWR0 | Mode |
| --- | --- | --- |
| 0   | 0   | PWR pin is always off |
| 0   | 1   | Pulse power supply on PWR pin |
| 1   | 0   | PWR pin is used as input B, pulse power supply on B pin |
| 1   | 1   | Power supply on PWR pin is always on |

The PWR pin strength is below 100uA, the B pin strength is below 10mA.

PUT\[1..0\] - delay after turning on pull-ups and powering the sensors and before sampling inputs A and B:  

| PUT1 | PUT0 | Mode |
| --- | --- | --- |
| 0   | 0   | 25 us |
| 0   | 1   | 50 us |
| 1   | 0   | 125 us |
| 1   | 1   | 425 us |

**Addresses 0x00d and 0x10d**

<table>
<tr><td>Bit</td><td>7</td><td>6</td><td>5</td><td>4</td><td>3</td><td>2</td><td>1</td><td>0</td></tr>
<tr><td>Offset in page: <b>13</b></td><td>EDGEA</td><td>EDGEB</td><td>MSRP</td><td>MSRL</td><td>MSRA</td><td>MSRB</td><td>SMP1</td><td>SMP0</td></tr>
<tr><td>Access in page <b>0</b></td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td></tr>
<tr><td>Access in page <b>8</b></td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td><td>R/W</td></tr>
<tr><td>Default</td><td>0</td><td>0</td><td>0</td><td>1</td><td>0</td><td>0</td><td>1</td><td>1</td></tr>
</table>

EDGEA/EDGEB - edge used for counter A/B increment:

| PINC | EDGE | Mode |
| --- | --- | --- |
| 0   | 0   | Increment on falling edges |
| 0   | 1   | Increment on rising edges |
| 1   | any | Increment on both edges |

MSRP - device power mode filter in Measure mode:

| MSRP | Mode |
| --- | --- |
| 0   | Accumulate statistics only when operating on battery power |
| 1   | Accumulate statistics only when operating from external power supply |

MSRL - input logical level filter in Measure mode:  

| MSRL | Mode |
| --- | --- |
| 0   | Accumulate statistics only when measured input is in low logical state |
| 1   | Accumulate statistics only when measured input is in high logical state |

MSRA,MSRB - enable Measure mode for input A/B:

| MSRA | MSRB | Mode |
| --- | --- | --- |
| 0   | 0   | Normal operating mode |
| 0   | 1   | Measure mode for input B |
| 1   | 0   | Measure mode for input A |
| 1   | 1   | Reserved |

SMP\[1..0\] - interval between samples:  

| SMP1 | SMP0 | Mode |
| --- | --- | --- |
| 0   | 0   | 19 ms |
| 0   | 1   | 38 ms |
| 1   | 0   | 75 ms |
| 1   | 1   | 150 ms |

**Addresses 0x011 and 0x111**

<table>
<tr><td>Bit</td><td>7</td><td>6</td><td>5</td><td>4</td><td>3</td><td>2</td><td>1</td><td>0</td></tr>
<tr><td>Offset in page: <b>17</b></td><td>Level A</td><td>Level B</td><td colspan=6 align=center>Reserved</td></tr>
<tr><td>Access in page <b>0</b></td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td></tr>
<tr><td>Access in page <b>8</b></td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td></tr>
<tr><td>Default</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td></tr>
</table>

Level A/B - current logical state of inputs A and B, 0 - low, 1 - high.

**Addresses 0x0d8 and 0x1d8**

<table>
<tr><td>Bit</td><td>7</td><td>6</td><td>5</td><td>4</td><td>3</td><td>2</td><td>1</td><td>0</td></tr>
<tr><td>Offset in page: <b>24</b></td><td colspan=8 align=center>Battery:[7..0]</td></tr>
<tr><td>Access in page <b>6</b></td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td></tr>
<tr><td>Access in page <b>14</b></td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td><td>R</td></tr>
<tr><td>Default</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td></tr>
</table>

Battery - current value representing voltage of the CR2032 battery. The battery voltage in Volts can be calculated using the formula Vbat = (VCC - 0.68V) * Battery / 256. In Measure mode, the Battery value is undefined.

**User data**

Bytes 0..23 of pages 6 and 14 (both pages are a mapping of each other) are available for storing user data.

The entire Scratchpad page can also be used to store user data, but before writing any other memory page, the Scratchpad contents must be read and remembered, and after writing, the remembered contents must be restored on device again. Thus, 56 bytes can be available for storing user data.

