extern malloc
extern free
extern fprintf

section .rodata
    FORMATO_STRING: db "%s",0
	STRING_VACIA: db "NULL",0
    p1 : db '(',0
	p2: db ')', 0
	flechita: db'->',0
	c1: db '[',0
	c2: db ']',0


section .data

section .text

global strCmp
global strClone
global strDelete
global strPrint
global strLen

; ** String **

; int32_t strCmp(char* a, char* b)
strCmp:


    push rbp
    mov rbp, rsp
    push r12	; nuevamente vamos a utilizar r12 y r13 que son registros que se preservan.		
    push r13
    mov r12, rdi
    mov r13, rsi


    call strLen	; retorna en eax la longitud del string A

 ;	Vamos a usar el registro rcx como índice y al regitro sil de rsi para guardar temporariamente los caracteres de los strings.

 	xor rcx, rcx


 	.ciclo:
 		cmp eax, ecx ; si eax es mas chico que el contador ecx significa que ya se recorrieron todos los caracteres no nulos y los dos strings son iguales
 		jl .igual

 		mov sil, [r12 + rcx]; muevo a sil el caracter del string A. Si es nulo(0), lo cual significa significa que el string de A terminó de recorrerse, o si el caracter de A es menor que el caracter de B significa que A es menor en orden lexicogŕafico.
 		cmp sil, [r13 + rcx] ; comparo sil con el caracter del string B. Si es nulo(0), lo cual significa que el string de B terminó de recorrerse, o si el caracter de A es mayor que el caracter de B significa que es A es mayor en orden lexicogŕafico.
 		jl .menor
 		jg .mayor
 		inc rcx			
 		jmp .ciclo		

 	.igual:		
 		mov eax, 0
 		jmp .fin

 	.menor:
 		mov eax, 1
 		jmp .fin

 	.mayor:
 		mov eax,-1
 		jmp .fin

 	.fin:
 		pop r13
 		pop r12

 		pop rbp
ret

; char* strClone(char* a)
strClone:
 	push rbp
 	mov rbp, rsp

 	push r12		; vamos a utilizar r12 y r13 que son registros que se preservan.			
 	push r13 
 	mov r12, rdi  	; r12 va a guardar el puntero al str a clonar
 	call strLen

 	xor r13, r13 ; limpiamos r13
 	mov r13d, eax ; movemos a la parte baja de r13 la longitud del string
; ;	Necesitamos pedir suficiente memoria para clonar el string y además
; ;   agregar el caracter de fin de cadena: '\0'

; ;	

 	mov rdi, r13      ; movemos r13 a rdi para pedir memoria en malloc. Segun la documentación en 64 bits, el tamaño de size_t es 64bits
 	inc rdi			; agregamos uno para pedir en malloc un byte para el caracter nulo.
 	call malloc  ; quedan en rax la primera direccion y en rdi la cantidad de bytes que se pidieron

 	xor rcx, rcx ; limpiamos el contador

; ;	Comparamos el indice contra la longitud de la cadena a clonar.
; ;	Si llegamos al final, copiamos un byte más pues este sería el
; ;	caracter de fin de cadena.

 	.ciclo:
 		cmp r13d, ecx
 		jl .fin ; es jl en lugar de je para copiar el byte final

 		mov sil, [r12 + rcx]
 		mov [rax + rcx], sil
 		inc rcx
 		jmp .ciclo


 	.fin:
 		pop r13
 		pop r12

 		pop rbp
 		ret



; void strDelete(char* a)
strDelete:


 	push rbp				
 	mov rbp, rsp

 	call free ; esta funcion borra por bloques. no es necesario iterar.

    pop rbp
    ret
; void strPrint(char* a, FILE* pFile)
strPrint:

 	push rbp
 	mov rbp, rsp

 	push r12; ahora guardamos en el registro r12 el puntero al string y en el registro r13 el puntero al archivo.
 	push r13

 	mov r12, rdi
 	mov r13, rsi
	
 	call strLen
 	mov rdi, r13 ; el primer parametro de fprintf es el puntero al archivo
 	mov rsi, FORMATO_STRING; parametro 3 para la funcion fprintf para que lo imprima como string.
    mov rdx, r12
 	cmp rax, 0; nos fijamos si la longitud del string es 0, en caso de serlo, debemos imprimir NULL como dice el enunciado.
 	je .vacia
 	jmp .novacia
	
 	.vacia:
 		mov rdx, STRING_VACIA ; parametro 2 para la funcion fprintf para que imprima NULL
 		call fprintf
 		jmp .fin
 	.novacia:
 		mov rdx, r12 ; parametro 2 para la funcion fprintf para que imprima el string que corresponda
 		call fprintf
 	.fin:
 		pop r13
 		pop r12

 		pop rbp
 		ret


; uint32_t strLen(char* a)
strLen:
 	push rbp
 	mov rbp, rsp
 	xor rax, rax ; limpiamos rax
 	.ciclo:
 		cmp byte [rdi], 0	; comparamos byte a byte a ver si es el nulo.	
 		je .fin ; termina
 		inc eax ; incrementamos el contador
 		inc rdi ; incrementamos la posicion
 		jmp .ciclo

 	.fin: 
 		pop rbp
		ret



   


