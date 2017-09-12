// 20 - 55  (angle)
module needle(){
    module s()
        cylinder(h=6.5,r=1.8,center=true,$fn=30);
    
    
    translate([-1.5,0,0])
        cube([3,5,8]);
    translate([12.5,0,0])
        cube([3,5,8]);
    difference(){
        cube([14,25,6]);
        translate([7,10,3])
            s();
    }
    difference(){
        hull(){
            translate([0,20,0])
                cube([14,6,6]);
            translate([7,45,25])
                sphere(3);
        }
        translate([7,33,13])
            rotate([-45,0,0])
                cylinder(h=20,r=1,$fn=30);
    }
    
}

needle();
