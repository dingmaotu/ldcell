function groline(nr,res,na,atom,x,y,z) {
    printf("%5d%-5s%5s%5d%8.3f%8.3f%8.3f\n", nr,res,atom,na,x,y,z);
}

function grolinefull(nr,res,na,atom,x,y,z,vx,vy,vz) {
    printf("%5d%-5s%5s%5d%8.3f%8.3f%8.3f%8.4f%8.4f%8.4f\n", nr,res,atom,na,x,y,z,vx,vy,vz);
}
function grobox(v1_x, v2_y, v3_z) {
    printf("%10.5f%10.5f%10.5f\n", v1_x, v2_y, v3_z);
}

# Gromacs only support boxes with v1(y)=v1(z)=v2(z)=0
function groboxfull(v1_x, v2_y, v3_z, v2_x, v3_x, v3_y) {
    printf("%10.5f%10.5f%10.5f%10.5f%10.5f%10.5f%10.5f%10.5f%10.5f", v1_x, v2_y, v3_z, 0, 0, v2_x, 0, v3_x, v3_y);
}
