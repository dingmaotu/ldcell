NR==1 { desc = $0 }
NR==2 { factor = $0 }
NR==3 { a = $1 }
NR==4 { b = $2 }
NR==5 { c = $3 }
NR==6 { 
    for (i=1; i<=NF; i++) {
        elems[i] = $i
    }
}
NR==7 { 
    for (i=1; i<=NF; i++) {
        enums[i] = $i
        total += $i
    }

    for(i in elems) {
        res[i] = toupper(elems[i]) "ION"
        atom[i] = toupper(elems[i]) "1"
    }

    print desc
    print total
}
NR >= 8 {
   at = atnum = NR - 7
   i = 1
   while ( at > enums[i] ) {
       at -= enums[i]
       i++;
   }
   groline(atnum, res[i], atnum, atom[i], $1, $2, $3);
}

END {
   grobox(a, b, c);
}
