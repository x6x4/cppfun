#  simple test program 

.text
inc %r6
x: inc %r4 
inc %r2
mov %r3 5
inc $dd
t: mov $dd %r2 

#jmp x

.data
d: 5
dd: 6

