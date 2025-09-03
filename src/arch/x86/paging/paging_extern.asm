global enable_paging
global enable_global_pages
global load_page_directory

enable_paging:
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ret

enable_global_pages:
    mov eax, cr4
    or eax, 1 << 7
    mov cr4, eax

    ret

load_page_directory:
    mov eax, esp+4
    mov cr3, eax

    ret