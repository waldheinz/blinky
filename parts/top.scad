
union() {
    difference() {
        union() {
            cylinder(5, 150, 150);
            cylinder(10, 147, 147);
        }
        
        translate([0, 0, -1]) cylinder(12, 140, 140);
    }

    for (i = [0 : 18 : 360]) {
        rotate([0, 0, i])
        translate([0, -3.5, 0])
        cube([142, 7, 10]);
    }
}