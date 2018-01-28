// properties of the translucent shell
$outer_diameter = 150;
$wall_width = 3;
$thickness = 8;
$bar_thickness = 5;
$bar_width = 5;
$bar_spacing = 10;
$rim_width = 10;

// properties of the inner shell / mounting hole for control knob

$inner_wall_width = 3;
$inner_ring_width = 6;
$inner_ring_center = 55 - $inner_wall_width / 2;

// purely computed values
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
            union() {
                for (i = [-$outer_radius : $bar_spacing : $outer_radius]) {
                    translate ([0, i, $thickness / 2])
                        cube([$outer_diameter, $bar_thickness, $thickness], center=true);
                }
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
    }
    
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
}
