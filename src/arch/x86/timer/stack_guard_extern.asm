global stack_guard
stack_guard:
    cmp esp, 0x60000
    jb overflow
    ret
    
overflow:
    int 48
    ret