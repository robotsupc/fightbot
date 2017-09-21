
th = 4;
glob_th = th;
screw_dst = 8.75;
screw_r = 3.5/2;

cone_r = 15;
cone_dst = 40;

module cone() {
    th = 2;
    gap_h = 15;
    h = 30;
    r=cone_r;
    pos = [-r,0,cone_dst-17];
    translate(pos)
    difference() {
        hull() {
            cylinder(r1=0, r2=r, h=h);
            //translate(-pos+[0,-((screw_r+th)*8)/2,cone_dst])
            //cube([0.0001,(screw_r+th)*8,13]);
            
            #translate(-pos) cube([th,(screw_r+glob_th)*2, 0.001], true);

        }
        
        translate([0,0,h/10+th])
        cylinder(r1=0, r2=r-th, h=h-th);
        
        translate([-18,0,0])
        cube([30,30,gap_h+th]*2, true);
        
        for (i = [0:3]) 
        translate([0,0,10-7*i]) rotate([0,90,0]) cylinder(r=screw_r, h=100, $fn=30);
        

    }
}

cone();

module holder() {
    cube_dst = screw_dst+11;
    translate([-cube_dst,0,0])
    difference() {
        hull() {
            cylinder(r=th+screw_r, h=th, $fn=30);
            translate([screw_dst,0,0]) cylinder(r=th+screw_r, h=th, $fn=30);
            translate([cube_dst,0,th/2]) cube([.00001,(screw_r+th)*2,th], true);
        }
        cylinder(r=screw_r, h=100, $fn=30);
        translate([screw_dst,0,0])cylinder(r=screw_r, h=100, $fn=30);
    }
    rotate([0,-45,0]) translate([0,0,6])    cube([6,(screw_r+th)*2, 3], true);
}

holder();

