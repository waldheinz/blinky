$fa = 1;
$fs = 0.5;

$total_diameter = 155;
$total_diameter_bottom = 170;
$total_height = 40;

extra_cutout = 0.5;

/* shade parameters */
$shade_outer_diameter = 150 + extra_cutout;
$shade_inner_diameter = 144 - extra_cutout;
$shade_base_overlap = 3;

/* LED carrier parameters */
$ledc_outer_diameter = 110 + extra_cutout;
$ledc_inner_diameter = 104 - extra_cutout;
$ledc_base_overlap = 3;

/* felt glider cutout */
$felt_glider_radius = 17 / 2;
$felt_glider_displace = 70;
$felt_glider_depth = 0.5;

/* computed values */
$shade_outer_radius = $shade_outer_diameter / 2;
$shade_inner_radius = $shade_inner_diameter / 2;
$ledc_outer_radius = $ledc_outer_diameter / 2;
$ledc_inner_radius = $ledc_inner_diameter / 2;
$total_radius = $total_diameter / 2;
$total_radius_bottom = $total_diameter_bottom / 2;

difference() {
    cylinder($total_height, $total_radius_bottom, $total_radius);
    
    translate([0, 0, $total_height - 25]) cylinder(
        20,
        $ledc_inner_radius - 2,
        $ledc_inner_radius - 2);
    
    /* cut groove for shade */
    translate([0, 0, $total_height - $shade_base_overlap + 1]) {
        difference() {
            cylinder($shade_base_overlap, $shade_outer_radius, $shade_outer_radius);
            cylinder($shade_base_overlap, $shade_inner_radius, $shade_inner_radius);
        }
        
        rotate_extrude(convexity = 10)
            translate([($shade_outer_radius + $shade_inner_radius) / 2 , 0, 0])
                circle(r = ($shade_outer_radius - $shade_inner_radius) / 2);
    }
    
    /* cut groove for LED carrier */
    translate([0, 0, $total_height - $ledc_base_overlap + 1]) {
        difference() {
            cylinder($shade_base_overlap, $ledc_outer_radius, $ledc_outer_radius);
            cylinder($shade_base_overlap, $ledc_inner_radius, $ledc_inner_radius);
        }
        
        rotate_extrude(convexity = 10)
            translate([($ledc_outer_radius + $ledc_inner_radius) / 2 , 0, 0])
                circle(r = ($ledc_outer_radius - $ledc_inner_radius) / 2);
    }
    
    /* cut away material so we have three "feet" */
    for (rz = [0, 120, 240]) {
        rotate([90, 0, rz]) translate([0, -21, 0])
            cylinder($total_radius_bottom, 45, 45);
    }
    
    /* cut away from one foot to make room for DC jack */
    for (a = [-20]) {
        rotate([0, 0, a]) translate([0, $total_radius + 2, 15]) {
            rotate([-90, 0, 0]) {
                cylinder(20, 12.3 / 2, 12.3 / 2);
                translate([0, 0, -10]) cylinder(30, 11.3 / 2, 11.3 / 2);
                translate([0, 0, -33]) rotate([0, 0, 30]) cylinder(30, 16.3 / 2, 16.3 / 2, $fn=6);
            }
        }
    }
   
    rotate([0, 0, 20]) translate([0, $total_radius - 10, 15]) rotate([-90, 0, 0]) {
        cylinder(40, 3.25, 3.25);
        cylinder(11, 4.25, 4.25);
        cube([9, 14, 19], center = true);
        translate([0, 0, 15]) cylinder(3, 6, 6, center = true);
    }
    
    /* vent holes */
    for (a = [15 : 30 : 360]) {
        rotate([0, 0, a]) translate([
            ($shade_inner_radius + $ledc_outer_radius) / 2, 0, $total_height - 10])
                cylinder(20, 7, 3);
    }
    
    /* save some material */
    translate([0, 0, -7])
        cylinder($total_height, $total_radius_bottom - 5, $total_radius - 5);
    
    /* fuse holder */
    translate([0, 0, $total_height - 10]) {
        cylinder(20, 22.4 / 2, 22.4 / 2);

        for (a = [0, 180]) {
            rotate([0, 0, a]) translate([33.5 / 2, 0, 0])
                cylinder(20, 4.5 / 2, 4.5 / 2);
        }
    }
    
    /* power in */
    translate([0, 0, $total_height - 10]) {
        for (a = [66, 74, 106, 114]) {
            rotate([0, 0, a]) translate([45, 0, 0])
                cylinder(20, 4.5 / 2, 4.5 / 2);
        }
    }
}

/* felt gliders */
for (rz = [0, 120, 240]) {
    rotate([0, 0, 90 + rz]) {
        translate([$felt_glider_displace, 0, 0]) difference() {
            cylinder(
                $total_height - 6,
                $felt_glider_radius + 2,
                $felt_glider_radius - 3);
            
            translate([0, 0, -$felt_glider_depth])
                cylinder(
                    2,
                    $felt_glider_radius,
                    $felt_glider_radius);
        }
    }
}
