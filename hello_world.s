
.globl	_start
 
.text
_start:
	# Write a string. This is write(1, "Hello, World!\n", strlen("Hello, World!\n")#
	movl	$14, %edx    # How long is the string
	movl	$msg, %ecx   # Where is the string
	movl	$1, %ebx     # File descriptor for standard output (get for free in linux)
	movl	$4, %eax     # Ssystem call number for write is 4
	int	$0x80        # Make system call
 
	# Exit (system call # 0)
	movl	$0, %ebx    
	movl	$1, %eax
	int	$0x80
.data
msg:
	.ascii	"Hello, world!\n"
