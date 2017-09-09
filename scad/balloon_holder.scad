th = 3;
platform_dst = 20;

module cone(a) {
    th = 2;
    gap_h = 15;
    h = 30;
    r=20;
    rotate([a,0,0])
    
    difference() {
        cylinder(r1=0, r2=r, h=h);
        
        translate([0,0,h/10+th])
        cylinder(r1=0, r2=r-th, h=h-th);
        
        cube([30,30,gap_h+th]*2, true);
    }
}

module pairwise_hull(close) {
    for (i = [0:$children-2]) {
        hull() {
            children(i);
            children(i+1);
        }
    }
    if (close) {
        hull() { children(0); children($children-1); }
    }
}


module c(pos) translate(pos) cylinder(r=3.6/2+3, h=th, $fn=30);
module s(pos) translate(pos) cylinder(r=3.6/2, h=th*3, $fn=30,center=true);

module screw(pos) {
    
    %translate(pos) cylinder(r=3.6/2, h=th*8, $fn=30, center=true);
}

module balloon(a) {
    f = 5;
    translate([0,0,10])
    %rotate([a,0,0])
    hull() {
        translate([0,0,30]*f)
        sphere(r=20*f);
        sphere(r=1);
    }
}


balloon(56.5);


module finger() {
    difference() {
        pairwise_hull(true) {
            c([-platform_dst,0,0]);
            c([platform_dst,0,0]);
        }
        s([-platform_dst,0,0]);
        s([platform_dst,0,0]);
    }
    
    screw([-platform_dst,0,0]);
    screw([platform_dst,0,0]);
}


module balloon_holder() {
    difference() {
        pairwise_hull(true) {
            c([-platform_dst,0,0]);
            c([platform_dst,0,0]);
            c([0,platform_dst*1.6,0]);
        }
        s([-platform_dst,0,0]);
        s([platform_dst,0,0]);
        s([0,platform_dst*1.6,0]);
    }
    translate([0,20,0])cone(56.5);
}

translate([0,0,-5]) finger();

balloon_holder();

