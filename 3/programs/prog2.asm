
mov %r2 1
syscall %r2 $kus
fsym %r6 h $kus  
syscall %r2 $gg
fsym %r7 r $gg

.data
d: 5
dd: 6 
kus: .ascii ksyusha
f: 3
gg: .ascii rrrr
