.entry TTY1

TTY1: mov XYZ,r5
    mov Indx,r4
    mov XYZlen,r0

www: .data 7,-6,9

LP1: cmp #0,r0
    jmp END
    cmp XYZ[*END],XYZ[*LP1]
    jmp END
    mov XYZ[Indx][r3],r1
    mov #-3,r6
    bne LP1
    add r7,www[*END]
END: rts

.extern XYZ
.extern Indx
.extern XYZlen
