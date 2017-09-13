screw_dst = 17.5;

module base() {
    th = 2-.6;
    pad = 0.6;
    thcube = [1,1,1]*(th+pad);
    size = [26,2.1,6];
    
    
    screw_h = size[2]-2;
    screw_pos = [(2*(th+pad)+size[0]-screw_dst)/2,(th+pad)*2+2.1+2,0];
    
    difference() {
        cube(size+thcube*2);

        translate(thcube)
        cube(size+[0,0,10]);
        
        translate([0,0,screw_h]+screw_pos) {
            h=8;
            cylinder(r=4, h=h, $fn=30);
            translate([screw_dst,0,0]) cylinder(r=4, h=h, $fn=30);
        }
    }
    
    // screws
    module screws() {
        module s(h=screw_h) {
            $fn=50;
            r = 3.5/2;
            difference() {
                union() {
                    cylinder(r=r+2, h=h);
                    translate(-[r+2,r+1,0])
                    cube([(r+2)*2,r+1,h]);
                }
                cylinder(r=r, h=h);
            }
        }
        
        translate(screw_pos) {
            translate([screw_dst,0,0]) s();
            s();
        }
    }
    
    screws();
}


base();

module holder() {
    
    
}


holder();