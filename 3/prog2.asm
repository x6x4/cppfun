#  simple test program 

.data: 
d1: 5
d2: 6


.text
inc %r6
x: inc %r4 
inc %r2
jmp %r5
inc $d2
t: mov $d1 %r2 

