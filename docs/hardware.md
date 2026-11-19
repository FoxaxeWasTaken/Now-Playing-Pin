# Hardware

Status: **nothing has been ordered or measured.** Every value below comes from a public page
or file. Each one says where it comes from. Anything tagged **[to verify]** must be confirmed
on the real part before it is relied on.

Sources read on 2026-11-19:

- Seeed wiki, EN05 page (`wiki.seeedstudio.com/epaper_en05`), last updated 2026-10-20.
- Seeed product page of the panel, ref. 104990845.
- EN05 "SCH&PCB" archive V1.11, linked from the wiki. Read once during a documentation
  review; nothing from it has been checked on a board.

## Parts

| Part | Choice | Known from the sources |
| --- | --- | --- |
| Board | Seeed XIAO ePaper Display Board EN05 (XIAO nRF52840 Plus) | 24-pin FPC connector, JST 2.0 mm battery connector, power slide switch, 3 user buttons and reset, PCB 1.2 mm thick (wiki) |
| Screen | 2.13" black and white e-paper, 122 × 250 px, Seeed ref. 104990845 | 59.2 × 29.2 × 1.00 mm, active area 48.55 × 23.70 mm, controller SSD1680, 3.2 g, 0 to 50 °C in operation **[to verify]**. The product page lists the EN05 as a compatible board, and the wiki lists this panel as supported |
| Battery | 3.7 V LiPo with protection. **Not chosen yet** | See "Battery" below |
| Mount | Badge magnet with 3 neodymium magnets (about 45 × 13 mm) | See [safety.md](safety.md) |

A four-colour 2.13" panel with the same resolution exists and is listed as supported. The
project uses the black and white one: it refreshes faster and supports partial refresh.

## Board

| Item | Value | Source |
| --- | --- | --- |
| PCB outline | 25.00 × 52.16 mm **[to verify]** | PCB file. An earlier figure of about 27 mm probably included the buttons |
| Mounting holes | 4 holes, 2.5 mm **[to verify]** | PCB file |
| Component side | All components on one face **[to verify]** | PCB file |
| Board dimensions on the wiki | Not given | Wiki |
| Charging | "Built-in charging IC" | Wiki. The chip and the charge current are not given |
| NFC | "Onboard NFC interface" | Wiki. No antenna is described |

Two points from the schematic, read during the review and not confirmed **[to verify]**:

- Charging seems to be done by the XIAO module itself. Its documented charge current is
  50 mA by default and 100 mA when selected. Either is acceptable for a cell of 100 mAh or
  more.
- The NFC lines seem to come out on two pads, with no antenna on the board. NFC (V2) would
  then need an external coil.

## Buttons

| Button | Pin | Level | Source |
| --- | --- | --- | --- |
| KEY1 | D1 | Active low | Wiki |
| KEY2 | D2 | Active low | Wiki |
| KEY3 | D9 | Active low | Wiki |

Read on the wiki, **[to verify]** on the board.

Position, from the PCB file **[to verify]**: the reset button and KEY1 are on one long edge;
KEY2, KEY3 and the power switch are on the opposite edge. KEY1 faces KEY3 and the reset
button faces KEY2, so squeezing the pin to press one button can press the one opposite.

The board's power switch cuts the battery.

Other signals exist on the schematic and are not recorded here yet: the switched supply of
the display and the battery measurement. Their pins are read from the schematic when the
firmware needs them.

## Toolchain

- Arduino core: "Seeed nRF52 Boards". Board: "Seeed XIAO nRF52840 Plus".
- Display library: the wiki builds its EN05 examples on **Seeed_GFX2**.
- The configuration for this board with the 2.13" panel is not given on the wiki. See
  "Display driver" in [firmware.md](firmware.md).

## Battery

Not chosen. Criteria:

- Built-in protection circuit.
- Thickness: about 3 mm, to keep the case near 14 mm ([enclosure.md](enclosure.md)).
- Capacity: about 250 mAh. A cell of roughly 20 × 50 × 3 mm is the kind to look for.
- Connector: many small cells come with a 1.25 mm connector, not the board's 2.0 mm one, and
  the polarity of these connectors is not standardised. On the board, pin 1 would be the
  positive **[to verify with a multimeter before plugging anything]**.
- Wires long enough to reach the connector once folded in the case.
- Bought from a seller that provides the transport and safety documents of the cell.

## Cost

| Part | Price | Source |
| --- | --- | --- |
| Panel | 5.85 USD (6.50 list) | Product page, 2026-11-19 |
| Board | About 10 USD **[to verify]** | Reported during the review, not read on the wiki |
| Battery, magnet clip, filament | Unknown | |
| Instrument that measures microamperes, if not already owned | Unknown | |

## To check before ordering

- [x] The panel ref. 104990845 is supported by the EN05: stated on both the wiki and the
      product page.
- [ ] Whether the board is sold with or without the XIAO module and the panel.
- [ ] A test battery: any protected cell of 100 mAh or more with the right connector. The
      final cell is chosen after the parts are measured.

## To check on reception

- [ ] Battery connector polarity with a multimeter, before plugging any battery.
- [ ] Board and panel dimensions with calipers, including the height of the tallest
      components. They feed the enclosure parameters.
- [ ] Which face of the board the panel sits on, and the direction of the FPC fold.
- [ ] Position of the USB-C port, the battery connector and the direction its wires leave.
- [ ] Button positions and active level.
- [ ] Whether holding a button at start-up puts the bootloader in an update mode.
- [ ] The charge current. The XIAO documentation gives 50 or 100 mA, either of which suits a
      cell of 100 mAh or more.
- [ ] Whether the board charges with the power switch off.
- [ ] Where the BLE antenna sits, whether there is an NFC antenna, and the radio range with
      a battery lying over the antenna.
- [ ] Idle current of the board ([spike 0008](spikes/0008-board-idle-current.md)).
