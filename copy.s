
.globl	_start
 

.text
_start:
 	movl	$24, %edx
	movl	$msg, %ecx
	movl	$2, %ebx
	movl	$4, %eax
	int	$0x80


	## ld will load the program to some address by mapping the whole file, headers and all.
	## We assume that we can find the stard of the file by rounding down to the begining of
	## the page. 
	mov $_start, %ecx		## _start must be 'near' the begining of the file
        mov $0xfff, %eax		## Pages are 4096 bytes. 0xfff is last byte of page
	not %eax
	and %eax, %ecx			## Round %ecx down to page boundary

	## Now write the contents of this 'program' back to stdout
	movl	$1000, %edx		## Assume program is < 1000 bytes long
	movl	$1, %ebx		## stdout is file number 1.
	movl	$4, %eax		## write is sytem call number 4
	int	$0x80			## Make the system call
 


	movl	$0, %ebx
	movl	$1, %eax
	int	$0x80


.data
msg:
	.ascii "Try executing my output\n"

	
