; ===== MULTIBOOT HEADER =====
%define ALIGN    (1 << 0)
%define MEMINFO  (1 << 1)
%define FLAGS    (ALIGN | MEMINFO)
%define MAGIC    0x1BADB002
%define CHECKSUM -(MAGIC + FLAGS)

section .multiboot
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM


; ===== STACK =====
section .bss
stack_bottom:
resb 1024 * 16
stack_top:

; ===== MEMORY CONSTANTS =====
%define KERNEL_START 0x10000
%define KERNEL_OFFSET 0xFFFFFF8000000000
%define VIRT_TO_PHYS(addr) ((addr) - KERNEL_OFFSET)
%define PHYS_TO_VIRT(addr) ((addr) + KERNEL_OFFSET)


; ===== GLOBAL STARTING POINT =====
global _start
section .text
bits 32
_start:
    cli
    cld
    mov esp, VIRT_TO_PHYS(stack_top)

    ; Store the data that GRUB passed us
    mov [VIRT_TO_PHYS(grub_memmap)], ebx
    mov [VIRT_TO_PHYS(grub_magic)], eax

    call init_terminal
    push VIRT_TO_PHYS(init_message)
    call print_string
    add esp, 4

    ; TODO: check if cpuid is present
    ; Currently we assume that it is always present which is a de facto
    ; standard for modern CPUs

    ; Check for extended function support
    ; If it is not supported then print the message and jump into endless loop
    mov eax, 0x80000000
    cpuid 
    cmp eax, 0x80000001
    jnb check_long_mode_support
    mov eax, VIRT_TO_PHYS(extended_cpuid_not_present)
    jmp print_and_halt

    ; Check for long mode support
check_long_mode_support:
    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29
    jnz initialize_paging_and_long_mode
    mov eax, VIRT_TO_PHYS(no_long_mode_message)
    jmp print_and_halt

initialize_paging_and_long_mode:
    ; Initialize paging

    ; Disable old paging first(just in case it was enabled)
    mov eax, cr0
    and eax, 0x7FFFFFFF
    mov cr0, eax

    ; Clear all paging tables and load PML4T address into CR3
    ; NOTE : this code assumes that all 4 initial paging structures
    ;        are located continiously in memory and that PML4
    ;        is the first of them
    mov ecx, 4096  ; 1024 repetitions
    xor eax, eax   ; Of 32-bit zero
    mov edi, VIRT_TO_PHYS(pml4t) ; Into PML4T and next 3 structures
    rep stosd      ; Write
    mov eax, VIRT_TO_PHYS(pml4t) ; Load PML4T address
    mov cr3, eax   ; And store it into CR3

    ; Indentity map first 2 MiBs
    ; Structure:
    ; PML4T[0]    -> PDPT
    ; PDPT0[0]     -> PDT0
    ; PDT[0]      -> PT
    ; PT[0..511] -> 0x00000000..0x00200000

    ; Also map first 2 MiBs into 0xFFFFFF8000000000 VA to use higher half kernel
    ; 0xFFFFFF8000000000 corresponds to last entry in PML4T
    ; PML4T[511]  -> PDPT
    
    mov eax, VIRT_TO_PHYS(pdpt)    ; Load address of PDPT into eax
    or eax, 0x03     ; Enable R/W and P flags
    mov [VIRT_TO_PHYS(pml4t)], eax ; Store address of PDPT[0] in PML4T[0]
    mov [VIRT_TO_PHYS(pml4t) + 511 * 8], eax ; Same with PML4T[511]
    mov eax, VIRT_TO_PHYS(pdt)
    or eax, 0x03
    mov [VIRT_TO_PHYS(pdpt)], eax  ; Same as with PML4T
    mov eax, VIRT_TO_PHYS(pt)
    or eax, 0x03
    mov [VIRT_TO_PHYS(pdt)], eax   ; Same as with PML4T
    ; Fill the 0th PT
    mov eax, 0x00200000 ; 2 MiBs
    xor ecx, ecx
    mov edi, VIRT_TO_PHYS(pt)
pt_init_loop:
    mov edx, ecx ; Copy address of current page into edx
    or edx, 0x03 ; Set flags P and R/W
    mov dword [VIRT_TO_PHYS(edi)], edx ; Fill the current PT entry
    add edi, 8 ; Move to next PT entry
    add ecx, 4096 ; Move to next physial page
    cmp eax, ecx ; Test if we are done
    jne pt_init_loop ; Loop

    ; Enable PAE
    mov eax, cr4
    or eax, 1 << 5 ; Set PAE bit
    mov cr4, eax

    ; Enable LM bit
    mov ecx, 0xC0000080 ; EFER MSR
    rdmsr ; Read EFER MSR
    or eax, 1 << 8 ; Set LM bit
    wrmsr

    ; Enable paging
    mov eax, cr0
    or eax, 1 << 31 ; Set PG bit
    mov cr0, eax

    ; Load GDT
    lgdt [VIRT_TO_PHYS(gdt)]
    
    ; Jump into long mode
    jmp 0x08:VIRT_TO_PHYS(long_mode_start)

    ; Prints the string that is pointed to by EAX and halts CPU
print_and_halt:
    push eax
    call print_string
    jmp halt_cpu

halt_cpu:
    cli
    cld
hlt_loop:
    hlt
    jmp hlt_loop

; 64 bit section
extern KernelMain
section .text
bits 64
long_mode_start:
    ; Load all segment registers
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov rax, higher_half_start
    jmp rax

higher_half_start:
    ; Unmap identity mapping of first 2 MiBs(but they're still mapped into higher half)
    xor rax, rax
    mov qword [rel pml4t], rax
    ; Set up stack with new addresses
    mov rbp, stack_top
    mov rsp, rbp
    ; Call KernelMain
    mov rax, grub_memmap
    mov edi, dword [rax]
    add rdi, KERNEL_OFFSET ; Because GRUB gave us physical address, so we have to remap it
    mov rax, grub_magic
    mov esi, dword [rax]
    mov rdx, cr3
    call KernelMain
    jmp long_mode_hlt_loop

long_mode_hlt_loop:
    hlt
    jmp long_mode_hlt_loop

section .data
align 4096
null_descriptor:
    dq 0
code_descriptor:
    dd 0
    dd (1 << 11) | (1 << 12) | (1 << 15) | (1 << 21) | (1 << 9)
data_descriptor:
    dd 0
    dd (1 << 12) | (1 << 15) | (1 << 9)
gdt:
    dw $ - null_descriptor - 1
    dq null_descriptor

init_message:
    db "Got control from GRUB", 0x00
extended_cpuid_not_present:
    db "Extended CPUID function 0x80000001 is not present on this CPU", 0x00
no_long_mode_message:
    db "Long mode is not supported on this CPU", 0x00
something_went_wrong_message:
    db "Ooops... Something went wrong!", 0x00

global pml4t
section .bss
align 4096

pml4t:
    resb 4096
pdpt:
    resb 4096
pdt:
    resb 4096
pt:
    resb 4096

grub_memmap:
    resd 1
grub_magic:
    resd 1


; ===== TERMINAL =====
%define VGA_BASE_ADDRESS       0xB8000
%define VGA_WIDTH              80
%define VGA_HEIGHT             25
%define VGA_SIZE               (VGA_WIDTH * VGA_HEIGHT)
%define DEFAULT_TERMINAL_COLOR 0x07

section .text
bits 32
global init_terminal
global print_string

; void init_terminal(void)
; Clears the VGA text buffer
init_terminal:
    push ebp
    mov ebp, esp
    mov eax, VGA_SIZE
    mov ecx, VGA_BASE_ADDRESS
clear_loop:
    mov byte [ecx], ' '
    inc ecx
    mov byte [ecx], DEFAULT_TERMINAL_COLOR
    inc ecx
    dec eax
    test eax, eax
    jnz clear_loop
    mov esp, ebp
    pop ebp
    ret

; void print_string(const char* string);
print_string:
    push ebp
    mov ebp, esp
    mov ecx, dword [ebp + 8] ; Load address of the string into ecx
    mov edx, [VIRT_TO_PHYS(line)]
    mov eax, VGA_WIDTH
    mul edx ; Calculate line * width of line
    lea eax, [VGA_BASE_ADDRESS + 2 * eax] ; Load address of start of current line 
print_loop:
    mov dl, byte [ecx] ; Load current character of string
    and dl, dl ; Test if it is \0 - end of a string
    jz print_string_ret ; If so then jump to end
    mov byte [eax], dl ; Store character into location of current character in VGA buffer
    inc ecx ; Increase string offset
    inc eax ; Increase VGA buffer offset
    mov byte [eax], DEFAULT_TERMINAL_COLOR ; Write default color attributes into VGA buffer
    inc eax ; Increase VGA buffer offset
    jmp print_loop ; Repeat
print_string_ret:
    inc byte [VIRT_TO_PHYS(line)] ; Move to new line
    cmp byte [VIRT_TO_PHYS(line)], VGA_HEIGHT ; Test if this line is below last
    jne skip_line_zero ; If not then finish
    mov byte [VIRT_TO_PHYS(line)], 0 ; Move to zeroth line
skip_line_zero:
    mov esp, ebp ; Restore stack pointer
    pop ebp ; Restore frame pointer
    ret

section .data
line: db 0
