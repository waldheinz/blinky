total_height = 8;
total_radius = 80;
mount_screw_radius = 40;
cutout_depth = 3;
cutout_extra = 0.5;

/* outer ring cutout */
outer_ring_start = (150 + cutout_extra) / 2;
outer_ring_end = (144 - cutout_extra) / 2;

/* inner ring cutout */
inner_ring_start = (110 + cutout_extra) / 2;
inner_ring_end = (104 - cutout_extra) / 2;

/* inner plate */
inner_plate_start = 95 / 2;

/* control knob hole */
knob_hole_radius = 7 / 2;

difference() {

    cylinder(total_height, total_radius, total_radius);
    
    translate([0, 0, total_height - cutout_depth]) difference() {
        cylinder(total_height, outer_ring_start, outer_ring_start);
        cylinder(total_height, outer_ring_end, outer_ring_end);
    }
    
    translate([0, 0, total_height - cutout_depth]) difference() {
        cylinder(total_height, inner_ring_start, inner_ring_start);
        cylinder(total_height, inner_ring_end, inner_ring_end);
    }
    
    translate([0, 0, total_height - cutout_depth]) difference() {
        cylinder(total_height, inner_plate_start, inner_plate_start);
        
        for (a = [0, 180]) {
            rotate([0, 0, a])
                translate([0, mount_screw_radius, -1])
                    cylinder(total_height + 2, 9, 9);
        }
    }
    
    translate([0, 0, -1])
        cylinder(total_height + 2, knob_hole_radius, knob_hole_radius);
    
    for (a = [0 : 30 : 360]) {
        rotate([0, 0, a])
            translate([0, (outer_ring_end + inner_ring_start) / 2, -1])
                cylinder(total_height + 2, 6, 6);
    }
    
    for (a = [0, 180]) {
        rotate([0, 0, a]) translate([0, mount_screw_radius, -1]) union() {
            cylinder(5, 9 / 2, 9 / 2);
            translate([0, 0, 3]) cylinder(total_height, 5.5 / 2, 5.5 / 2);
        }
    }
}
