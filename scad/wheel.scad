$fn = 100;
module wheel(h) {
    difference() {
        cylinder(r=10, h=h);
        cylinder(r=3.3/2, h=h);
    }
}

wheel(5);