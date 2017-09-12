screw_r = 3.2/2;
th = 3;
outer_r = screw_r + th;
dst = 44.2;

module s() { cylinder(r=screw_r, h=100, center=true, $fn=30); }

module base() {
    difference() {
        hull() {
            cylinder(r=outer_r, h=th, $fn=100);
            translate([dst,0,0])
            cylinder(r=outer_r, h=th, $fn=100);
        }
        
        s();
        translate([dst,0,0]) s();
    }
}

base();




module reinforcement(size, r) {
    translate([r,-outer_r,r])
    rotate([0,90,90])
    difference() {
        cube([r,r,size]);
        cylinder(r = r, h = size, $fn=50);
    }
}


module up() {
   updst = 7+th;
    difference() {
        
        hull() {
            translate([0,0,updst])
            rotate([0,90,0])
            cylinder(r=outer_r+2, h=th, center=true);
                    
            cube([th, 2*outer_r, 0.00001], center=true);
        }
        translate([0,0,updst])
        rotate([0,90,0])
        s();
    }
    
    translate([th/2,0,th])
    reinforcement(size=outer_r*2, r = 4.5);
}

translate([dst/2-5,0,0])
up();



