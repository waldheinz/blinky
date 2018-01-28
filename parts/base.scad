$total_diameter = 155;
$total_diameter_bottom = 170;
$total_height = 50;

/* shade parameters */
$shade_outer_diameter = 150;
$shade_inner_diameter = 147;
$shade_base_overlap = 5;

/* LED carrier parameters */
$ledc_outer_diameter = 110;
$ledc_inner_diameter = 107;
$ledc_base_overlap = 5;

/* computed values */
$shade_outer_radius = $shade_outer_diameter / 2;
$shade_inner_radius = $shade_inner_diameter / 2;
$ledc_outer_radius = $ledc_outer_diameter / 2;
$ledc_inner_radius = $ledc_inner_diameter / 2;
$total_radius = $total_diameter / 2;
$total_radius_bottom = $total_diameter_bottom / 2;

difference() {
    cylinder($total_height, $total_radius_bottom, $total_radius, $fn=180);
    
    /* cut groove for shade */
    translate([0, 0, $total_height - $shade_base_overlap]) difference() {
        cylinder($total_height, $shade_outer_radius, $shade_outer_radius, $fn=180);
        cylinder($total_height, $shade_inner_radius, $shade_inner_radius, $fn=180);
    }
    
    /* cut groove for LED carrier */
    translate([0, 0, $total_height - $ledc_base_overlap]) difference() {
        cylinder($total_height, $ledc_outer_radius, $ledc_outer_radius, $fn=180);
        cylinder($total_height, $ledc_inner_radius, $ledc_inner_radius, $fn=180);
    }
    
    /* cut away material so we have three "feet" */
    for (rz = [0, 120, 240]) {
        rotate([90, 0, rz])
            cylinder($total_radius_bottom, 40, 40, $fn=180);
    }
}
