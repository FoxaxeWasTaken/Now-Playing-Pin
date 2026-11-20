// The pin's case (docs/enclosure.md), for OpenSCAD 2021.01 (docs/adr/0016-*.md).
// Every dimension is a variable here, in mm. The values are the paper estimates of
// docs/enclosure.md and ADR-0013: nothing has been measured. Today the model is the envelope
// only, a closed shell: the window, buttons, battery pocket and magnets come with the parts.

case_length = 64;     // along the panel's long side
case_width = 33;
case_thickness = 14;
side_wall = 1.2;
front_wall = 1.0;
back_wall = 1.2;

difference() {
    cube([case_length, case_width, case_thickness]);
    translate([side_wall, side_wall, back_wall])
        cube([case_length - 2 * side_wall, case_width - 2 * side_wall,
              case_thickness - back_wall - front_wall]);
}
