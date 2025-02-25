#define MAXBUF 22


.data
buf: .space MAXBUF
msg_res: .asciz "%d\n"
msg_err: .asciz "Error: Invalid input format\n"


.text
.global main

main:

        la a0, buf
        li a1, MAXBUF           
        call gets               # Read input string

        la a0, buf
        call atoi               # Get first number
        mv t0, a0

        li t3, ' '
        la a0, buf
        mv a1, t3
        call strchr             # Find first space

        beq a0, zero, failure
        mv t2, a0

        addi t2, t2, 1          # Increment pointer to next symbol

        lb t4, (t2)
        beq t4, zero, failure   # Terminate zero found
        beq t4, t3, failure     # Second space found

        mv a0, t2               
        call atoi               # Get second num
        mv t1, a0

        add t3, t0, t1          # Find sum of two numbers

        la a0, msg_res
        mv a1, t3
        call printf

        li a7, 2                # Exit system call
        li a0, 0                # Exit status (success)
        ecall

failure:

        li a7, 16               # Write syscall number
        li a0, 2                # stderr fd (2)
        la a1, msg_err          # Error message string address
        li a2, 28               # Error message string length
        ecall                   # Syscall

        li a7, 2                # Exit system call
        li a0, 1                # Exit status (failure)
        ecall                   # Syscall
