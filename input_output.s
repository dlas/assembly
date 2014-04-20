
.globl	_start
 
.text
_start:

	# Print introduction to stderr. 
	movl	$16, %edx	## Lengh of string
	movl	$msg, %ecx	## Address of string
	movl	$2, %ebx	## stderr file number
	movl	$4, %eax	## write command
	int	$0x80		## make system call
 

	## Loop forever copying one byte from stdin to stdout
loop:
	## Get one byte from stdin and stash it in tmp
	movl $1,%edx		## Length is one byte
	movl $tmp, %ecx		## put it at tmp
	movl $3, %eax		## read system call is number 3
	int $0x80		## make the system call

	cmp $1, %eax	## Check if we get one byte
	jne done	## Run away if we didn't


	## Now write it to stdout
	movl $1, %edx		## write length is one byte
	movl $tmp, %ecx		## location is still tmp
	movl $4, %eax		## write system call is 4
	int $0x80

	jmp loop		## Go back to loop
done:
	
	## We're done. Exit
	movl	$0, %ebx
	movl	$1, %eax
	int	$0x80
.data
msg:
	.ascii	"Type something!\n"
tmp:
	.byte 0, 0, 0, 0
