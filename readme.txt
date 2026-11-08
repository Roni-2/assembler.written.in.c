Comments:
	; skips to the end of the line
Not:
	not reg2, reg1
Flags:
	-f input file, -o save bytecode, -r run after compiling, -i run a bytecode file, -h help
Math and logic:
	or, add, sub, nand, and, xor, eq, written like add reg3, reg1, reg2
RAM write:
	mov [reg2], reg1 gives 11000001
RAM read:
	mov reg1, [reg2] gives 11000000
Constants into any register:
	mov reg2, 10 gives 00001010 then 10001010
Errors show the token:
	for example Unknown identifier 'banana'.
Multiple errors:
	one error per line, in line order, and no binary comes out
Interpreter:
	interpreter.c, 6 registers and 256 bytes of RAM; prints the final state
Constant declarations:
	const SIZE = 10, and const B = SIZE also works
