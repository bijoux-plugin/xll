;.586              ;Target processor.  Use instructions for Pentium class machines
;.MODEL FLAT, C    ;Use the flat memory model. Use C calling conventions
;.STACK            ;Define a stack segment of 1KB (Not required for this example)
;.DATA             ;Create a near data segment.  Local variables are declared after
                  ;this directive (Not required for this example)

.CODE             ;Indicates the start of a code segment.

func_0 PROC
	push rdi

	mov rax, 300

	pop rdi
	ret
func_0 ENDP

func_1 PROC
	push rdi

	imul rcx, 3
	mov rax, rcx

	pop rdi
	ret
func_1 ENDP

func_2 PROC
	push rdi

	mov rax, rcx
	add rax, rdx		

	pop rdi
	ret
func_2 ENDP

func_3 PROC
	push rdi

	mov rax, rcx
	add rax, rdx
	add rax, r8

	pop rdi
	ret
func_3 ENDP

func_4 PROC
	push rdi

	mov rax, rcx
	add rax, rdx
	add rax, r8
	add rax, r9

	pop rdi
	ret
func_4 ENDP

func_5 PROC
	push rdi

	mov eax, dword ptr[rsp+30h]
	add rax, rcx
	add rax, rdx
	add rax, r8
	add rax, r9

	pop rdi
	ret
func_5 ENDP

func_6 PROC
	push rdi

	mov eax, dword ptr[rsp+30h]
	add eax, dword ptr[rsp+38h]
	add rax, rcx
	add rax, rdx
	add rax, r8
	add rax, r9

	pop rdi
	ret
func_6 ENDP

call_function PROC
	push rdi
; 1st 4 integer or pointer parameters are stored in:
; rcx, rdx, r8, r9
; rcx = target function address
; rdx = # of parameters
; r8 = pointer to base address

	sub esp, 32h
; move address of target function to rcx
	;mov rax, rcx
	mov r10, rcx
; move # of parameters to r11
	;mov rbx, rdx
	mov r11, rdx
; move address of array to r12
	mov r12, r8

; if there are 0 parameters jump straight to function call
	;cmp rbx, 0
	cmp r11, 0
	jz call_func
; move first parameter to rcx
	mov ecx, [r12]
	cmp r11, 1
	je call_func
; move second parameter to rdx
	mov edx, [r12+4]
; if there are 2 parameters only, jump to function call
	cmp r11, 2
	je call_func
; move 3rd parameter to r8
	mov r8, [r12+8]
; if there are 3 parameters only, jump to function call
	cmp r11, 3
	je call_func
; move 4th parameter to r9
	mov r9, [r12+12]
; if there are 4 parameters only, jump to function call
	cmp r11, 4
	je call_func
; move 5th parameter to the top of the stack
	mov eax, dword ptr[r12+16]
	mov dword ptr [rsp+28h], eax
; if there are 5 parameters only, jump to function call
	cmp r11, 5
	je call_func
; move 6th parameter to the top of the stack
	mov eax, dword ptr[r12+20]
	mov dword ptr [rsp+20h], eax

call_func:
	;call rax
	call r10
	add esp, 32h

	pop rdi
	ret
call_function ENDP

End