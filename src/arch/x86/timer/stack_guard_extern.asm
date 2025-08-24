[EXTERN stack_overflow_routine]

global stack_guard
stack_guard:
    cmp esp, 0x60000
    jb overflow
    ret
    
overflow:
    call stack_overflow_routine
    ret