mov r5 5
loop: inc r8
neg r5
cmp r5 r8
jne loop

.data
data: 7