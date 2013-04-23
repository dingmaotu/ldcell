NR == 1 { desc = $0 }
NR == 2 { natoms = $1 }

NR >= 3 && NR <= 2+natoms {
    str = groline_part($0, "atom")
    str = substr(str, 1, length(str)-1)
    elems[str] += 1
    atom_x[NR-2] = groline_part($0, "x")
    atom_y[NR-2] = groline_part($0, "y")
    atom_z[NR-2] = groline_part($0, "z")
}

NR == 3 + natoms {
    a = $1; b = $2; c = $3;
}

END {
    print desc;
    print 1;
    printf("%f %f %f\n", a, 0, 0)
    printf("%f %f %f\n", 0, b, 0)
    printf("%f %f %f\n", 0, 0, c)
    for ( i in elems) {
	printf("%s ", i)
    }
    printf("\n");
    for ( i in elems) {
	printf("%d ", elems[i])
    }
    printf("\n");

    for ( i = 1; i <= natoms; i++) {
	printf("%f %f %f\n", atom_x[i]/a, atom_y[i]/b, atom_z[i]/c);
    }
}
