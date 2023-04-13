; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TALLER System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "print.mac"
extern KERNEL_PAGE_DIR
extern GDT_DESC
extern screen_draw_layout
extern IDT_DESC
extern idt_init
extern pic_reset
extern pic_enable
extern mmu_init_kernel_dir
extern mmu_init_task_dir
extern copy_page
extern tss_init
extern GDT_TASK_A_SEL
extern GDT_TASK_B_SEL
extern sched_init
extern task_init
global start


; COMPLETAR - Agreguen declaraciones extern según vayan necesitando

; COMPLETAR - Definan correctamente estas constantes cuando las necesiten
%define CS_RING_0_SEL (0x1 << 3)
%define DS_RING_0_SEL (0x3 << 3) 
%define KERNEL_PAGE_DIR (0x00025000)
;EL DEFINE ESTE PUEDE ROMPER EN UN FUTURO....

BITS 16
;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
start_rm_msg db     'Iniciando kernel en Modo Real'
start_rm_len equ    $ - start_rm_msg

start_pm_msg db     'Iniciando kernel en Modo Protegido'
start_pm_len equ    $ - start_pm_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; COMPLETAR - Deshabilitar interrupciones
    cli


    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; COMPLETAR - Imprimir mensaje de bienvenida - MODO REAL
    ; (revisar las funciones definidas en print.mac y los mensajes se encuentran en la
    ; sección de datos)
    
    print_text_rm start_rm_msg, start_rm_len, 0xD, 0, 0



    ; COMPLETAR - Habilitar A20
    ; (revisar las funciones definidas en a20.asm)
    call A20_check
    call A20_enable
    call A20_check 
    ; COMPLETAR - Cargar la GDT
    lgdt [GDT_DESC]



    ; COMPLETAR - Setear el bit PE del registro CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax



    ; COMPLETAR - Saltar a modo protegido (far jump)
    jmp CS_RING_0_SEL:modo_protegido
    ; (recuerden que un far jmp se especifica como jmp CS_selector:address)
    ; Pueden usar la constante CS_RING_0_SEL definida en este archivo

BITS 32
modo_protegido:
    ; COMPLETAR - A partir de aca, todo el codigo se va a ejectutar en modo protegido
    ; Establecer selectores de segmentos DS, ES, GS, FS y SS en el segmento de datos de nivel 0
    ; Pueden usar la constante DS_RING_0_SEL definida en este archivo
    mov ax, DS_RING_0_SEL
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ; xchg bx, bx
    mov ss, ax

    ; COMPLETAR - Establecer el tope y la base de la pila

    mov ebp, 0x25000
    mov esp, 0x25000


    ; ; COMPLETAR - Imprimir mensaje de bienvenida - MODO PROTEGIDO
    print_text_pm start_pm_msg, start_pm_len, 0xE, 0, 0

    ; COMPLETAR - Inicializar pantalla
    call screen_draw_layout


    ;Activar paginacion
    call mmu_init_kernel_dir

    mov cr3, eax

    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    ;Inicializar y cargar IDT
    call idt_init

    lidt [IDT_DESC]

    ;Inicializar PICs
    call pic_reset
    call pic_enable

    ;Habilitar interrupciones
    sti

    ;Agregar descriptores de TSS a las gdt
    call tss_init


    ;Cambiamos el CR3 al de la tarea inicial
    ;push 0x18000
    ;call mmu_init_task_dir ;devuelve en eax el contenido que se ha de cargar en un registro CR3 para la tarea asociada a esta llamada   
    ;mov cr3, eax
    
    ;Cargamos el TR con el selector de la tarea inicial
    mov ax, 0x0058 ;GDT_IDX_TASK_INITIAL 0x58 (indice en la gdt)  shifteado 3 por tener bits con dpl y table indicator
    LTR ax ; (con ax = selector segmento tarea inicial)

    call sched_init

    call task_init

    ;intercambiar con la tarea idle
    JMP 0x60:0 ;GDT_IDX_TASK_IDLE, 0x60(indice en gdt) shifteado 3

    ; Ciclar infinitamente 
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
