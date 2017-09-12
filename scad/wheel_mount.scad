////    c1=costat llarg   c2=costat curt  h=alçada    g=gordura parets  d=distancia legs    w=distancia roda    ////

c1=65;
c2=28;
h=35;
g=3;
d=50;
w=10;

module s()
        cylinder(h=6.5,r=1.8,center=true,$fn=30);
    
module leg(){
    difference(){
        hull(){
            cylinder(h=g,r=5,center=true,$fn=30);
            translate([d,0,0])
                cylinder(h=g,r=5,center=true,$fn=30);
        }
        translate([d,0,0])
            cylinder(h=6.5,r=1.9,center=true,$fn=30);
    }
}
module sup(){
    rotate([0,90,0])
    rotate([90,0,0])
        leg();
    rotate([0,90,0])
    translate([0,w,0])
        rotate([90,0,0])
            leg();    
    translate([-5,-g/2,-1])
        cube([10,w+g,6]);
    translate([-17,w/2,g-1])
        hull(){
            cylinder(h=2*g,r=5,center=true,$fn=30);
            translate([17,0,0])
                cylinder(h=2*g,r=5,center=true,$fn=30);  
        }
    //translate([4,-0.5,-5])
        //cube([5,17,10]);
}

module wheel(){
    rotate([0,90,0])
        difference(){
            sup();
            translate([0,w/2,g-1])
                s();
            translate([-17,w/2,g-1])
                s();
            //translate([-5,g/2,0])
                //cube([13,w-g,2]);
        }
}

wheel();



