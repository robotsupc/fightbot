th = 3;
screw_dst = 8.75;
screw_r = 3.5/2;
outer_r = th + screw_r;
dst_x = 30;
dst_y = -16;






module s() cylinder($fn=100, r=screw_r, h=200, center=true);


module lm2596_holder() {
    
    module holder(pos) {
        difference() {
            linear_extrude(th)
            hull() {
                square([0.0001,outer_r*2], center=true);
                translate(pos)
                circle(r=outer_r, $fn=50);
            }
            children();
        }
    }
    
    holder([th*2+screw_r+screw_dst,0]) {
        translate([th*2+screw_r,0,0]) s();
        translate([th*2+screw_r+screw_dst,0,0]) s();       
    }
    
    
    module lm() {
        translate([0,0,-10])
        rotate([0,90,0])
        difference() {
            hull() {
                cylinder(r=outer_r, $fn=50, h=3);
                translate([dst_x, dst_y])
                cylinder(r=outer_r, $fn=50, h=3);
            }   
        }
        hull() {
            translate([0,-outer_r,0])
            cube([th*1.5,outer_r*2,th]);
            translate([0,0,-10])
            rotate([0,90,0])
            cylinder(r=outer_r, $fn=50, h=3);
        }
    }
    
    difference() {
        lm();
        translate([0,0,-10])
        rotate([0,90,0]) {
            s();
            translate([dst_x, dst_y]) s();
        }
    }
    
    
    module up(h=th*1.5, r=screw_r+1.2) {
        translate([0,0,-10])
        difference() {
            rotate([0,90,0]) {
                    cylinder(r=r, $fn=30, h=h);
                    translate([dst_x, dst_y])
                    cylinder(r=r, $fn=30, h=h);
            }
            rotate([0,90,0]) {
                s();
                translate([dst_x, dst_y]) s();
            }
        }
    }
        
    
    up();
}

lm2596_holder();