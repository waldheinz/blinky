$fa = 0.5;
$fs = 0.5;

inlay_total_height = 30;
cutout_extra = 0.5;
inner_ring_end = (104 - cutout_extra) / 2;
mount_screw_radius = 40;

difference() {
    union() {
        cylinder(7, inner_ring_end, inner_ring_end);

        for (a = [0, 180]) {
            rotate([0, 0, a])
                translate([0, mount_screw_radius, 2])
                    cylinder(inlay_total_height + 2, 9, 9);
        }
    }
    
    for (a = [0, 180]) {
        rotate([0, 0, a]) union() {
            translate([0, mount_screw_radius, -1])
                cylinder(inlay_total_height + 2, 10 / 2, 10 / 2, $fn = 6);
            
            translate([0, mount_screw_radius, 30.5])
                cylinder(4, 9 / 2, 6 / 2);
        }
    }
    
    translate([50, 0, 0])
        cube([20, 100, 20], center = true);
}
