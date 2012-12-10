NR <= 256 {
    res="MGION";
    atom="MG1";
    printf("%5i%5s%5s%5i%8.3f%8.3f%8.3f\n", NR, res, atom, NR, $1, $2, $3);
}
NR > 256 && NR <= 512 {
    res="SIION";
    atom="SI1";
    printf("%5i%5s%5s%5i%8.3f%8.3f%8.3f\n", NR, res, atom, NR, $1, $2, $3);
}
NR > 512 {
    res="OION";
    atom="O1";
    printf("%5i%5s%5s%5i%8.3f%8.3f%8.3f\n", NR, res, atom, NR, $1, $2, $3);
}
