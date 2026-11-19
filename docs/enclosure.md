# Enclosure

Status: requirements only. No CAD file yet, no part measured.

## Approach

- CAD **by code and parametric** ([ADR-0009](adr/0009-parametric-enclosure-as-code.md)). The
  tool is chosen in [spike 0007](spikes/0007-cad-tool.md).
- Every dimension is a variable at the top of the file. The initial values are the published
  dimensions ([hardware.md](hardware.md)); they are replaced by caliper measurements once the
  parts are in hand.
- The EN05 PCB files ("SCH&PCB" archive on the Seeed wiki) give the board outline and the
  connector positions.

## Target

About **33 × 64 × 14 mm**, with a thin battery stacked behind the board
([ADR-0013](adr/0013-enclosure-envelope.md)). This is a paper estimate. It replaces an
earlier target of 31 × 61 × 10 to 12 mm, which the stack below does not allow.

### Thickness budget

Assumed values, none measured:

| Layer | mm |
| --- | --- |
| Front wall | 1.0 |
| Clearance | 0.2 |
| Panel | 1.0 |
| Support behind the panel | 0.5 |
| PCB | 1.2 |
| Tallest components | 5.0 |
| Clearance | 0.3 |
| Battery, 3 mm cell with 0.5 mm for swelling | 3.5 |
| Back wall | 1.2 |
| **Total** | **13.9** |

The magnet plate adds about 1 mm: a 1.2 mm back wall is too thin to set it in. The realistic
total is 14 to 15 mm.

### Footprint

The panel is 59.2 × 29.2 mm. With walls of 1.2 mm, clearance, and room for the fold of the
FPC cable, the footprint comes to about 33 × 64 mm.

## Requirements

### Screen

- Window slightly smaller than the panel. The active area is not centred on the glass: the
  offset is taken from the panel's drawing.
- The panel is thin glass: supported from behind, never clamped.
- A raised lip around the window, so that the glass does not touch a flat surface if the pin
  falls face down.
- Optional PET or acrylic sheet, about 0.5 mm, in front.
- The FPC cable folds behind the board with a gentle curve.

### Buttons and openings

- USB-C and the power switch.
- KEY1 on one long edge, KEY2 and KEY3 on the other **[to verify]**. Caps or flexures built
  into the case.
- The reset button behind a pinhole, not a cap: it faces KEY2.
- A place to rest a finger opposite each button, offset from the button on the other edge,
  so that pressing one does not press the other.

### Inside

- Battery pocket with no pressure point on the cell and room for swelling.
- A cell as long as the board would cover the BLE antenna and may weaken the link. The
  length of the cell is decided after measuring the range with it in place.
- The board's four mounting holes can hold it.
- Magnet pocket at the back. The magnets are kept away from the BLE antenna, whose position
  is not known yet.
- Room reserved at the back for an NFC coil (V2), even though V1 leaves it empty. A magnet
  plate 45 mm long cannot avoid both the BLE antenna and that coil: the layout is decided
  with the real board.

### Manufacturing

- PETG.
- Clearance 0.2 to 0.3 mm.
- M2 screws or clips. No glue.

## Safety

The case holds a lithium cell and magnets against the body. The rules are in
[safety.md](safety.md).
