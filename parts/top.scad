/* properties of the translucent shell */
$outer_diameter = 150;
$wall_width = 3;
$thickness = 8;
$bar_thickness = 5;
$bar_spacing = 15;
$rim_width = 10;

/* properties of the inner ring */
$inner_wall_width = 3;
$inner_ring_width = 6;
$inner_ring_center = 55 - $inner_wall_width / 2;

/* knob plate */
$knob_plate_radius = 20;
$knob_hole_radius = 5.92 / 2; // M7 x 0.75 thread
$knob_plate_thickness = 4;

/* purely computed values */
$inner_ring_start = $inner_ring_center - $inner_ring_width / 2;
$inner_ring_end = $inner_ring_center + $inner_ring_width / 2;
$inner_ring_cutout_start = $inner_ring_center - $inner_wall_width / 2;
$inner_ring_cutout_end = $inner_ring_center + $inner_wall_width / 2;

$outer_radius = $outer_diameter / 2;

difference() {
    
    union() {
        difference() {
            /* outer ring */
            union() {
                cylinder($thickness / 2, $outer_radius, $outer_radius, $fa=2);
                cylinder(
                    $thickness,
                    $outer_radius - $wall_width,
                    $outer_radius - $wall_width,
                    $fa=2);
            }
            
            /* outer ring cutout */
            translate([0, 0, -5])
                cylinder(
                    $thickness + 10,
                    $outer_radius - $rim_width,
                    $outer_radius - $rim_width,
                    $fa=2);
        }
        
        /* bars */
        intersection() {
            for (a = [0 : 20 : 180]) {
                echo(a);
                rotate([0, 0, a]) translate ([0, 0, $thickness / 2])
                    cube([$outer_diameter, $bar_thickness, $thickness], center = true);
            }
            
            translate([0, 0, -5])
                cylinder(
                    $thickness + 10,
                    $outer_radius - $wall_width - 1,
                    $outer_radius - $wall_width - 1, $fa=2);
        }
        
        /* inner ring */
        difference() {
            cylinder($thickness, $inner_ring_end, $inner_ring_end, $fa=2);
            translate([0, 0, -5]) cylinder(
                $thickness + 10,
                $inner_ring_start,
                $inner_ring_start,
                $fa=2);
        }
        
        /* plate for knob */
        cylinder($thickness, $knob_plate_radius, $knob_plate_radius, $fa=2);
    }
    
    union() {
        /* inner ring cutout */
        translate([0, 0, 5]) difference() {
            cylinder(
                $thickness,
                $inner_ring_cutout_end,
                $inner_ring_cutout_end, $fa=2);
            
            cylinder(
                $thickness,
                $inner_ring_cutout_start,
                $inner_ring_cutout_start,
                $fa=2);
        }
        
        /* knob plate hole */
        translate([0, 0, -1]) cylinder(
            $thickness + 2,
            $knob_hole_radius,
            $knob_hole_radius, $fa=2);
        
        /* ensure knob plate thickness */
        translate([0, 0, $knob_plate_thickness]) cylinder(
            $thickness,
            $knob_plate_radius - 5,
            $knob_plate_radius - 5,
            $fa=2);
    }
}
