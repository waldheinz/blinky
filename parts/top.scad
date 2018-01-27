
// properties of the translucent shell
$outer_diameter = 150;
$wall_width = 3;
$thickness = 8;
$bar_thickness = 5;
$bar_width = 5;
$bar_spacing = 10;
$rim_width = 10;

$outer_radius = $outer_diameter / 2;

union() {
    difference() {
        union() {
            cylinder($thickness / 2, $outer_radius, $outer_radius, $fa=2);
            cylinder(
                $thickness,
                $outer_radius - $wall_width,
                $outer_radius - $wall_width,
                $fa=2);
        }
        
        translate([0, 0, -5])
            cylinder(
                $thickness + 10,
                $outer_radius - $rim_width,
                $outer_radius - $rim_width,
                $fa=2);
    }
    
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
}
