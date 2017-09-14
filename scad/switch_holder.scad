size = [19,12,12];
th = 5;
screw_dst = 17.5/2;
screw_r = 3.5;
mount_th = 3;

mount_offset = [16,11];

total_w = th*2+size[0];

bolt_th = 3;


module removeinner() {
    difference() {
        children();
        translate([1,1,-0.001]*th)
        translate([0,0,-150]) 
        cube(size+[0,th+.1,300]);   
   
        // negative y axis
        translate([0,-100,0])
        cube([100,100,100]);
        
        // screws and bolts
        translate([total_w/2, -mount_offset[0]+bolt_th, -mount_offset[1]])
        rotate([-90,0,0]) {
            cylinder(r=3.5, h=100, $fn=30);
            cylinder(r=3.7/2, h=100, $fn=30, center=true);
            
            translate([0,screw_dst,0]) {
                cylinder(r=3.5, h=100, $fn=30);
               cylinder(r=3.7/2, h=100, $fn=30, center=true);
            }
        }
    }
}
module switch() {
        cube(size+[2,2,1]*th);
}

module mount() {
    translate([total_w/2,-mount_offset[0],-mount_offset[1]])
    rotate([-90,0,0])
    difference() {
        hull() {
            cylinder(r=screw_r+mount_th, h=mount_th);
            translate([0,screw_dst,0]) cylinder(r=screw_r+mount_th, h=mount_th);
        }

    }
}

removeinner() {
    hull() {
        switch();
        mount();
        translate([total_w-th,0,-mount_offset[1]]) cube(th);
        translate([0,0,-mount_offset[1]]) cube(th);
    }
}