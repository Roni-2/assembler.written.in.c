; adds two numbers and saves the result in ram
const FIRST = 10
const SECOND = 32
const ADDRESS = 5

mov reg2, SECOND ; this one changes reg1 too, so it goes first
mov reg1, FIRST
add reg3, reg1, reg2

mov reg2, ADDRESS
mov reg1, reg3
mov [reg2], reg1

