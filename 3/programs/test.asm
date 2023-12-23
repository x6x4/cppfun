#  simple test program 

.text
mov %r3 5
binpowrem %r3 2 17
mov %r6 2
binpow %r6 6 

.data
d: 5
dd: 6 

