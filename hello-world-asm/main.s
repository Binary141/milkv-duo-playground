# https://smist08.wordpress.com/2019/09/07/risc-v-assembly-language-hello-world/

.global _start      # Provide program starting address to linker

# Setup the parameters to print hello world
# and then call Linux to do it.

_start: li  a0, 1      # 1 = StdOut
        la  a1, helloworld # load address of helloworld
        li  a2, 22     # length of our string
        li  a7, 64     # linux write system call
        ecall                # Call linux to output the string

# Setup the parameters to exit the program
# and then call Linux to do it.
        li  a0, 0   # Use 0 return code
        li  a7, 93  # Service command code 93 terminates

        ecall               # Call linux to terminate the program

.data
helloworld:      .ascii "Hello World from Asm!\n"
