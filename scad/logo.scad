r = 50;

base_th = 2;
model_th = 1;
border_th = 3;

module figura() translate([1,1]*-r) import("logo.dxf");

module inv() {
    difference() {
        circle(r=r+border_th, $fn=100);
        figura();
    }
}

color([1,1,1]) linear_extrude(model_th+base_th) inv();
color([0,143,204]/255.) cylinder($fn=50, r=r, h=base_th);
