// EcoSense@clock: box, part2 (back)

//color("white")

box_thicknes = 2;
panel_width = 64.897;
panel_height = 96.901;
mount_holes_width = 58.547;
mount_holes_height = 90.170;
mount_hole_diameter = 2.5;
mount_hole_z = 8;

vent_hole_height = 0.75;
vent_hole_width = 25;
vent_hole_pos_h = 20;
vent_hole_count = 25;

box_deep = 30;
box2panel_size = box_thicknes + 3;
box_width = panel_width + 2 * box2panel_size;
box_height = panel_height + 2 * box2panel_size;

mount_pos_w = (panel_width - mount_holes_width) / 2;
mount_pos_h = (panel_height - mount_holes_height) / 2;
echo(mount_pos_w=mount_pos_w);

translate([0,0,0]) {
    difference() {
        
        cube([box_width, box_deep, box_height]);
       
        translate([box_thicknes,box_thicknes,box_thicknes])
            cube([(box_width-box_thicknes*2), box_deep, (box_height-box_thicknes*2)]);
        
    // vent holes
    pos_h = box_height - vent_hole_pos_h;
   echo(pos_h=pos_h);

   echo(s=box_height-((vent_hole_height + vent_hole_height*2)*vent_hole_count));
        
    for(h = [
        vent_hole_pos_h:
        (vent_hole_height + vent_hole_height*2):
        box_height - vent_hole_pos_h
        ]) {
        
        echo(h=h);
        
        translate([
                -1
            ,
                box_deep/2 -            vent_hole_width/2
            ,
                h
            ]) 
                cube([box_width*2, vent_hole_width, vent_hole_height]);
        echo(pos_h=pos_h);
    }
    
    for(h = [
        vent_hole_pos_h+box_thicknes:
        (vent_hole_height + vent_hole_height*2):
        box_width - vent_hole_pos_h-4
        ]) {
        
        translate([
                h-1
            ,
                box_deep/2 -            vent_hole_width/2
            ,
                box_height-box_thicknes*2
            ]) 
                cube([vent_hole_height, vent_hole_width, box_thicknes*3]);
    }
    
        // usb
    translate([
        6 + box_thicknes,
        -1,
        10.3
        ]) {
        cube([13.1, box_thicknes*3, 5.7]);
        }
    
    }
}

difference() {
    translate([0,box_deep,0]) {
        cube([box_width, 10, box_height]);
    }
    translate([box_thicknes/2,box_deep-1,box_thicknes/2]) {
        cube([box_width-box_thicknes, 12, box_height-box_thicknes]);
    }
}

translate([
box_thicknes+vent_hole_pos_h - box_thicknes*2-.5,
box_thicknes,
vent_hole_pos_h
]) {
cube([box_thicknes, box_deep/3, box_height - vent_hole_pos_h]);

}

translate([
box_thicknes+vent_hole_pos_h - box_thicknes*2 + 30+ box_thicknes + 0.5,
box_thicknes,
vent_hole_pos_h
]) {
cube([box_thicknes, box_deep/3, box_height - vent_hole_pos_h]);

}

// mb102 rails
translate([
box_thicknes,
box_thicknes,
box_thicknes + 7// 9.3 - 1.3 - box_thicknes/2
]) {
cube([1, 28, 1]);
cube([50, 1, 1]);
}

translate([
box_thicknes,
box_thicknes,
box_thicknes + 5  //9.3 - 1.3 - box_thicknes/2 - 1.2 - box_thicknes/2
]) {
cube([1, 28, 1]);
cube([50, 1, 1]);
}


echo(version=version());
