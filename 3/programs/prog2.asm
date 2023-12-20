.text

inc %r2
mov %r4 5
mov %r6 1
syscall %r6 $kus
fsym %r6 z $kus

.data
d: 5
dd: 6 
kus: .ascii ksyusha
#key: .ascii jjjjjjjjjjjjjjjjjjjjj

