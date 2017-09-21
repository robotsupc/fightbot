logo_color = [0,143,204]/255.;
r = 50;
f = 0.9;


screw_th = 3;
screw_r = 3.5/2;

base_th = 1;
model_th = 1.2;

total_th = base_th + model_th;
border_th = 3;


final_r = (r+border_th)*f;

module figura() translate([1,1]*-r) import("logo.dxf");
module figura_small() {
    difference() {
        circle(r=50, $fn=100);
        scale([1,1]*.7) translate([1,1]*-r) import("logo-small.dxf");
    }
}


module inv(small=false) {
    difference() {
        circle(r=r+border_th, $fn=100);
        if(small) figura_small();
        else figura();
    }
}


module holders(hs=[90], sr=screw_r, th) {

    module holder(a, sr=screw_r, th=screw_th) {
        dst = sr+th;
        color("white")
        linear_extrude(total_th)
        rotate(a)
        translate([r*f,0,0])
        difference() {
            hull() {
                translate(-[r*f,0,0])
    circle(r=final_r, $fn=100);
                square([0.01,2]*r/4,center=true);
                translate([dst,0,0]) circle(r=sr+th, $fn=30);
            }
#            translate([dst,0,0]) circle(r=sr, $fn=30);
            translate(-[r*f,0,0])
    circle(r=final_r-1, $fn=100);
        }
    }

    a = 30;
    for (h = hs) holder(h, sr=sr, th=th);
}


module attachment() {
    th = 1.5;
    pad = .5;
    
    screw_dst = 84.5;
    screw_l = 40;
    
    module cut() {
        a = 20;
        cylinder(r=final_r+pad, h=total_th+pad*2, center=true, $fn=100);
        cylinder(r=final_r-th, h=3000, center=true, $fn=100);
        rotate(45+a) translate([0,0,-500]) cube(1000);
        rotate(45-a) translate([0,0,-500]) cube(1000);
        cube([2,300,300], true);
        
        translate([0,0,-500-35])
        cube([screw_dst, 1000,1000], true);
    }
    module arc(cut=true) {
        difference() {
            cylinder(r=final_r+th+pad, h=total_th+(th+pad)*2, center=true, $fn=100);
            cut();
        }
    }

    
    module screws() {
        module s(inv) {
            m = inv?[1,0,0]:[0,0,0];
            difference() {
                mirror(m)
                translate([0,27,0])
                translate([screw_dst/2,0,0]) {
                    hull() {
                        translate([0,0, -screw_l])
                        rotate([0,90,0])
                        cylinder(r=screw_r+screw_th, h=3);
                        rotate(30)
                        rotate([0,90,0])
                        cube([6.2/2,10,4], true);
                    }
                    hull() {
                        rotate(30)
                        rotate([0,90,0])
                        cube([6.2,10,4], true);

                        translate([-1,-1,0]*3)
                        rotate(30)
                        rotate([0,90,0])
                        cube([6.2,30,0.001], true);
                    }
                }
                
                mirror(m)
                translate([2,27,0])
                translate([screw_dst/2,0,0])
                translate([0,0, -screw_l])
                rotate([0,90,0])
                cylinder(r=screw_r, h=30, center=true, $fn=50);
                
            }
        }
        s(true);
        s(false);
    }
    
    
    rotate(180) arc();
    difference() {
        screws();
        rotate(180)cut();
    }
    
}







module main(small=false) {
    //translate([0,0,-total_th/2])
    scale([f,f,1]) {
        color([1,1,1]) linear_extrude(model_th+base_th) inv(small=small);
        color(logo_color) cylinder($fn=50, r=r, h=base_th);
    }
}


//rotate([180,0,0])attachment();

module keychain() {
    scalef = 1/3;
    scale([scalef,scalef,1]) {
        main(small=true);
        holders([90], sr=1.5/scalef*screw_r, th=2*1/scalef);
    }
}

keychain();


module keychains(x=4, y=4/2) {
    for (j = [0:y])
    translate([0,63*j,0])
    for (i = [0:x]) {
        translate([35*i,0,0]) keychain();
        translate([35*i+35/2,35,0]) rotate(180) keychain();
    }
}

//keychains();