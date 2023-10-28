#  simple test program 

.data
d: 5
dd: 6


.text
inc %r6
x: inc %r4 
inc %r2
inc $dd
t: mov $dd %r2 

#jmp x

