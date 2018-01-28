$total_diameter = 155;
$total_diameter_bottom = 170;
$total_height = 40;

extra_cutout = 0.1;

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
$felt_glider_depth = 1;

/* computed values */
$shade_outer_radius = $shade_outer_diameter / 2;
$shade_inner_radius = $shade_inner_diameter / 2;
$ledc_outer_radius = $ledc_outer_diameter / 2;
$ledc_inner_radius = $ledc_inner_diameter / 2;
$total_radius = $total_diameter / 2;
$total_radius_bottom = $total_diameter_bottom / 2;

difference() {
    cylinder($total_height, $total_radius_bottom, $total_radius, $fn=360);
    
    translate([0, 0, $total_height - 25]) cylinder(
        20,
        $ledc_inner_radius - 2,
        $ledc_inner_radius - 2, $fn=360);
    
    /* cut groove for shade */
    translate([0, 0, $total_height - $shade_base_overlap + 1]) {
        difference() {
            cylinder($shade_base_overlap, $shade_outer_radius, $shade_outer_radius, $fn=180);
            cylinder($shade_base_overlap, $shade_inner_radius, $shade_inner_radius, $fn=180);
        }
        
        rotate_extrude(convexity = 10, $fn=180)
            translate([($shade_outer_radius + $shade_inner_radius) / 2 , 0, 0])
                circle(r = ($shade_outer_radius - $shade_inner_radius) / 2, $fn = 20);
    }
    
    /* cut groove for LED carrier */
    translate([0, 0, $total_height - $ledc_base_overlap + 1]) {
        difference() {
            cylinder($shade_base_overlap, $ledc_outer_radius, $ledc_outer_radius, $fn=180);
            cylinder($shade_base_overlap, $ledc_inner_radius, $ledc_inner_radius, $fn=180);
        }
        
        rotate_extrude(convexity = 10, $fn=180)
            translate([($ledc_outer_radius + $ledc_inner_radius) / 2 , 0, 0])
                circle(r = ($ledc_outer_radius - $ledc_inner_radius) / 2, $fn = 20);
    }
    
    /* cut away material so we have three "feet" */
    for (rz = [0, 120, 240]) {
        rotate([90, 0, rz]) translate([0, -21, 0])
            cylinder($total_radius_bottom, 45, 45, $fn=180);
    }
    
    /* cut away from one foot to make room for DC jack and power switch */
    for (a = [-20]) {
        rotate([0, 0, a]) translate([0, $total_radius+3.6, 10]) {
            //cube([10, 40, 40], center = true);
            rotate([-90, 0, 0]) {
                cylinder(20, 12.1 / 2, 12.1 / 2, $fn=60);
                translate([0, 0, -10]) cylinder(30, 11 / 2, 11 / 2, $fn=60);
                translate([0, 0, -33]) cylinder(30, 16.1 / 2, 16.1 / 2, $fn=6);
            }
        }
    }
   
    /* vent holes */
    for (a = [15 : 30 : 360]) {
        rotate([0, 0, a]) translate([
            ($shade_inner_radius + $ledc_outer_radius) / 2, 0, $total_height - 10])
                cylinder(20, 7, 7, $fn = 60);
    }
    
    /* save some material */
    translate([0, 0, -7])
        cylinder($total_height, $total_radius_bottom - 5, $total_radius - 5, $fn=180);
    
    /* fuse holder */
    translate([0, 0, $total_height - 10]) {
        cylinder(20, 22.4 / 2, 22.4 / 2, $fn=60);

        /* power in */
        for (a = [0, 180, 70]) {
            rotate([0, 0, a]) translate([33.5 / 2, 0, 0])
                cylinder(20, 4.5 / 2, 4.5 / 2, $fn=60);
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
                $felt_glider_radius - 3, $fn=180);
            
            translate([0, 0, -$felt_glider_depth])
                cylinder(
                    2,
                    $felt_glider_radius,
                    $felt_glider_radius, $fn=180);
        }
    }
}
