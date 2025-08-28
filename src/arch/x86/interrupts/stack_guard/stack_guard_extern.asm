[EXTERN stack_guard_warning_routine]
[EXTERN cancel_panic_ready_routine]

global stack_guard_warning
stack_guard_warning:
    cmp esp, 0x70000
    jb warning
    ret

warning:
    call stack_guard_warning_routine
    ret

global stack_guard_panic
stack_guard_panic:
    cmp esp, 0x60000
    jb overflow

    cmp esp, 0x88000
    ja cancel_panic_ready
    ret
    
cancel_panic_ready:
    call cancel_panic_ready_routine

overflow:
    int 48
    