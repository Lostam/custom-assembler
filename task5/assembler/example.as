; file ps.asps.asps.asps.asps.asps.asps.asps.asps.asps.asps.asps.asps
.entry LENGTH
.extern W , Z , T
mcro m1
      bne W
      bne Z
      bne     T
      sub @r1     ,             @r4
      bne L3
endmcro
      mcro m2
   END: stop
STR: .string "abcdef"
LENGTH: .data    6 ,  -9,  15
endmcro
MAIN: mov @r3 , LENGTH
LOOP: jmp L1
      prn -5
      m1
L1:   inc K              
.entry LOOP
        jmp W
m2
K: .data 22
.extern L3
