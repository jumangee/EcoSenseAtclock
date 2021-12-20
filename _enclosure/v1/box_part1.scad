// EcoSense@clock: box, part1 (front)

//color("white")

box_thicknes = 2;
panel_width = 64.897;
panel_height = 96.901;
mount_holes_width = 58.547;
mount_holes_height = 90.170;
mount_hole_diameter = 2.5;
mount_hole_z = 8;

display_width = 26;
display_height = 13;
display_pos_h = 25;

vent_hole_height = 0.75;
vent_hole_width = 15;
vent_hole_pos_h = 20;
vent_hole_count = 5;

box_deep = 20;
box2panel_size = box_thicknes + 3;
box_width = panel_width + 2 * box2panel_size;
box_height = panel_height + 2 * box2panel_size;

echo(box2panel_size=box2panel_size);
echo(box_width=box_width);
echo(box_height=box_height);

mount_pos_w = (panel_width - mount_holes_width) / 2;
mount_pos_h = (panel_height - mount_holes_height) / 2;
echo(mount_pos_w=mount_pos_w);

translate([0,0,0]) {
    difference() {
        union() {
            cube([box_width, box_deep, box_height]);
            translate([box_thicknes/2,box_deep,box_thicknes/2]) {
                cube([box_width-box_thicknes, 10, box_height-box_thicknes]);
            }
        }
        
        translate([
            (box2panel_size+
            panel_width/2-
            display_width/2)
        ,
            -1
        ,
            box2panel_size + panel_height - display_pos_h - display_height
        ]) 
            cube([display_width, 50, display_height]);
        
        translate([box_thicknes,box_thicknes,box_thicknes])
            cube([(box_width-box_thicknes*2), box_deep*2, (box_height-box_thicknes*2)]);
        
    // vent holes
    pos_h = box_height - vent_hole_pos_h;
        
    for(h = [
        pos_h+((vent_hole_height + vent_hole_height*2)*vent_hole_count):
        -(vent_hole_height + vent_hole_height*2):
        pos_h
        ]) {
        
        
        translate([
                -1
            ,
                box_deep/2 -            vent_hole_width/2
            ,
                h
            ]) 
                cube([box_width*2, vent_hole_width, vent_hole_height]);
    }
    
    /*vent_hole_height = 0.5;
vent_hole_width = 20;
vent_hole_pos_h = 20;
vent_hole_count = 5;*/

    }
}


rotate([90,0,0]) {
    translate([mount_pos_w + box2panel_size,mount_pos_h+box2panel_size,-(box_thicknes+mount_hole_z)])
        difference() {
            cylinder(h=(mount_hole_z),r=mount_hole_diameter);
            translate([0,0,-1])
                cylinder(h=(box_deep / 2),r=(mount_hole_diameter/2));
        }

    translate([mount_pos_w + box2panel_size + mount_holes_width,mount_pos_h+box2panel_size,-(box_thicknes+mount_hole_z)])
        difference() {
            cylinder(h=(mount_hole_z),r=mount_hole_diameter);
            translate([0,0,-1])
                cylinder(h=(box_deep / 2),r=(mount_hole_diameter/2));
        }
        
    translate([mount_pos_w + box2panel_size + mount_holes_width,mount_pos_h+box2panel_size + mount_holes_height,-(box_thicknes+mount_hole_z)])
        difference() {
            cylinder(h=(mount_hole_z),r=mount_hole_diameter);
            translate([0,0,-1])
                cylinder(h=(box_deep / 2),r=(mount_hole_diameter/2));
        }
        
    translate([mount_pos_w + box2panel_size,mount_pos_h+box2panel_size + mount_holes_height,-(box_thicknes+mount_hole_z)])
        difference() {
            cylinder(h=(mount_hole_z),r=mount_hole_diameter);
            translate([0,0,-1])
                cylinder(h=(box_deep / 2),r=(mount_hole_diameter/2));
        }
}


echo(version=version());
