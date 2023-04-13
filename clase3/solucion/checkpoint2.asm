extern sumar_c
extern restar_c
;########### SECCION DE DATOS
section .data

;########### SECCION DE TEXTO (PROGRAMA)
section .text

;########### LISTA DE FUNCIONES EXPORTADAS

global alternate_sum_4
global alternate_sum_4_simplified
global alternate_sum_8
global product_2_f
global alternate_sum_4_using_c

;########### DEFINICION DE FUNCIONES
; uint32_t alternate_sum_4(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4);
; registros: x1[rdi], x2[rsi], x3[rdx], x4[rcx]
alternate_sum_4:
	;prologo
	push rbp
	mov rbp, rsp
	; COMPLETAR 
	;recordar que si la pila estaba alineada a 16 al hacer la llamada
	;con el push de RIP como efecto del CALL queda alineada a 8
	sub rdi, rsi ; con esto en rdi queda x1 - x2
	add rdi, rdx ; con esto en rdi queda x1 - x2 + x3
	sub rdi, rcx ; con esto en rdi queda x1 - x2 + x3 - x4
	mov rax, rdi
	
	;epilogo
	; COMPLETAR 
	pop rbp
	ret

; uint32_t alternate_sum_4_using_c(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4);
; registros: x1[rdi], x2[rsi], x3[rdx], x4[rcx]
alternate_sum_4_using_c: 

	;prologo
    push rbp ; alineado a 16
    mov rbp,rsp

	call restar_c
	mov rdi, rax
	mov rsi, rdx

	call sumar_c

	mov rdi, rax
	mov rsi, rcx

	call restar_c

	;epilogo
	pop rbp
    ret 



; uint32_t alternate_sum_4_simplified(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4);
; registros: x1[?], x2[?], x3[?], x4[?]

alternate_sum_4_simplified:
	sub rdi, rsi ; con esto en rdi queda x1 - x2
	add rdi, rdx ; con esto en rdi queda x1 - x2 + x3
	sub rdi, rcx ; con esto en rdi queda x1 - x2 + x3 - x4
	mov rax, rdi
	ret


; uint32_t alternate_sum_8(uint32_t x1, uint32_t x2, uint32_t x3, uint32_t x4, uint32_t x5, uint32_t x6, uint32_t x7, uint32_t x8);	
; registros y pila: x1[rdi], x2[rsi], x3[rdx], x4[rcx], x5[r8], x6[r9], x7[rbp+0x10], x8[rbp+0x18]
alternate_sum_8:
	;prologo
	push rbp
	mov rbp, rsp
	sub rsp, 0x10
	; COMPLETAR 
	call alternate_sum_4 ;rax tiene la primer parte de la suma alternada
	add rax, r8 
	mov rdi, rax
	mov rsi, r9
	mov rdx, qword [rbp + 0x10]
	mov rcx, qword [rbp + 0x18]
	call alternate_sum_4
	;epilogo
	add rsp, 0x10 ;nose si esta bien esto
	pop rbp
	ret
	

; SUGERENCIA: investigar uso de instrucciones para convertir enteros a floats y viceversa
;void product_2_f(uint32_t * destination, uint32_t x1, float f1);
;registros: destination[rdi], x1[rsi], f1[xmm0]
product_2_f:
	push rbp
	mov rbp, rsp

	mov rcx, rsi ;me guardo el rsi pero queda cerooooo
	cvtsi2ss xmm1, rcx ;convierte a rcx en scalar single precision float
	mulss xmm0, xmm1 ;Multiply Scalar Single-Precision Floating-Point Values
	cvttss2si rax, xmm0 ;convierte con truncado el float a int64
	mov qword [rdi], rax ;guarda en memoria a rax
	pop rbp
	ret

