
;########### SECCION DE DATOS
section .data

;########### SECCION DE TEXTO (PROGRAMA)
section .text

;########### LISTA DE FUNCIONES EXPORTADAS
global complex_sum_z
global packed_complex_sum_z
global product_9_f

;########### DEFINICION DE FUNCIONES
;extern uint32_t complex_sum_z(complex_item *arr, uint32_t arr_length);
;registros: arr[rdi], arr_length[rsi]
complex_sum_z:
	;prologo
	push rbp
	mov rbp, rsp

	mov ecx, esi ;le paso el size del arr a ecx
	mov eax, 0

.cycle:			; etiqueta a donde retorna el ciclo que itera sobre arr
	mov dword r11d, [rdi + 24] ;el atributo z esta a 24 bytes a partir del struct
	add eax, r11d
	add rdi, 32 ;paso al segundo elem del array
	loop .cycle		; decrementa ecx y si es distinto de 0 salta a .cycle

	
	;epilogo
	pop rbp
	ret
	
;extern uint32_t packed_complex_sum_z(packed_complex_item *arr, uint32_t arr_length);
;registros: arr[?], arr_length[?]
packed_complex_sum_z:
	push rbp
	mov rbp, rsp

	mov ecx, esi ;le paso el size del arr a ecx
	mov eax, 0

.cycle:			; etiqueta a donde retorna el ciclo que itera sobre arr
	mov dword r11d, [rdi + 20] ;el atributo z esta a 20 bytes a partir del struct
	add eax, r11d
	add rdi, 24 ;paso al segundo elem del array
	loop .cycle		; decrementa ecx y si es distinto de 0 salta a .cycle

	
	;epilogo
	pop rbp
	ret


;extern void product_9_f(uint32_t * destination
;, uint32_t x1, float f1, uint32_t x2, float f2, uint32_t x3, float f3, uint32_t x4, float f4
;, uint32_t x5, float f5, uint32_t x6, float f6, uint32_t x7, float f7, uint32_t x8, float f8
;, uint32_t x9, float f9);
;registros y pila: destination[rdi], x1[rsi], f1[xmm0], x2[rdx], f2[xmm1], x3[rcx], f3[xmm2], x4[r8], f4[xmm3]
;	, x5[r9], f5[xmm4], x6[rbp+0x10], f6[xmm5], x7[rbp+0x18], f7[xmm6], x8[rbp+0x20], f8[xmm7],
;	, x9[r+0x28], f9[rbp+0x30]
;el res se guarda en xmm0 convertido a double
;floats: xmm0 - xmm7 y pila
;ints:  10
product_9_f:

	;prologo 
	push rbp
	mov rbp, rsp
	
	;convertimos los flotantes de cada registro xmm en doubles con cvtss2sd

	; COMPLETAR 
	cvtss2sd xmm0, xmm0
	cvtss2sd xmm1, xmm1
	mulsd xmm0, xmm1
	cvtss2sd xmm2, xmm2
	mulsd xmm0, xmm2
	cvtss2sd xmm3, xmm3
	mulsd xmm0, xmm3
	cvtss2sd xmm4, xmm4
	mulsd xmm0, xmm4
	cvtss2sd xmm5, xmm5
	mulsd xmm0, xmm5
	cvtss2sd xmm6, xmm6
	mulsd xmm0, xmm6
	cvtss2sd xmm7, xmm7
	mulsd xmm0, xmm7
	movsd xmm1, [rbp + 0x30]
	cvtss2sd xmm1, xmm1
	mulsd xmm0, xmm1


	cvtsi2sd xmm1, rsi
	mulsd xmm0, xmm1

	cvtsi2sd xmm1, rdx
	mulsd xmm0, xmm1

	cvtsi2sd xmm1, rcx
	mulsd xmm0, xmm1

	cvtsi2sd xmm1, r8
	mulsd xmm0, xmm1

	cvtsi2sd xmm1, r9
	mulsd xmm0, xmm1

	cvtsi2sd xmm1, [rbp + 0x10]
	mulsd xmm0, xmm1

	cvtsi2sd xmm1, [rbp + 0x18]
	mulsd xmm0, xmm1

	cvtsi2sd xmm1, [rbp + 0x20]
	mulsd xmm0, xmm1
	
	cvtsi2sd xmm1, [rbp + 0x28]
	mulsd xmm0, xmm1

	;cvtss2sd xmm0, xmm0
	movsd [rdi], xmm0

	;multiplicamos los doubles en xmm0 <- xmm0 * xmm1, xmmo * xmm2 , ...

	; COMPLETAR 

	; convertimos los enteros en doubles y los multiplicamos por xmm0. 

	; COMPLETAR 

	;mov [rdi], xmm0
	; epilogo 
	pop rbp
	ret

