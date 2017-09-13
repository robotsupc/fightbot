stick_r = 22/2;
pad = 2;

screw_r = 3;
screw_dst = .6;

stick_support_th = 5;

module base(angles, r=(stick_r+pad)*2.5, h=(stick_r+pad)*3) {
    difference() {
        cylinder(r=r, h=h, center=true);
        for (a = angles) {
            rotate(a) {
                rotate([0,90,0]) 
                cylinder(r=stick_r+pad, h=r*3, $fn=80);
                
               translate([r*screw_dst,0,0])
               cylinder(r=screw_r, h=h*3, center=true, $fn=30);
            }
        }
        sphere(r=stick_r+pad);
    }
}



module base2(angles, r=(stick_r+pad)*2.5, h=(stick_r+pad)*3) {
    arm_length = r*1;
    difference() {
        hull() {
            for (a = angles) {
                rotate([0,90,a]) 
                cylinder(r=stick_r+pad+stick_support_th, h=arm_length, $fn=80);
            }
            //sphere(r=stick_r+pad+stick_support_th);
        }
        for (a = angles) {
            rotate([0,90,a]) 
            cylinder(r=stick_r+pad, h=r*3, $fn=80);
            
            rotate(a) translate([arm_length*screw_dst,0,0])
            cylinder(r=screw_r, h=100, center=true, $fn=50);
            
            %rotate(a) translate([arm_length*screw_dst,0,0])
            cylinder(r=screw_r, h=50, center=true, $fn=50);
            
        }
        sphere(r=stick_r+pad);
    }
}


base2([0,120]);
