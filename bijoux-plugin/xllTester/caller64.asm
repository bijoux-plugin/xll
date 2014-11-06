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
; make room on stack
	sub rsp, 0A00h
	mov rdi, rsp
; 1st 4 integer or pointer parameters are stored in:
; rcx, rdx, r8, r9
	; rcx = target function address
	; rdx = # of parameters
	; r8 = pointer to base address

; move address of target function to rcx
	mov r10, rcx
; move # of parameters to r11
	mov r11, rdx
; move address of array to r12
	mov r12, r8

; if r11 is less than 5, then jump to do_first_parameters
	cmp r11, 4
	jle do_first_parameters

; Otherwise, populate stack first then populate parameters 1-4
; if r11 == 5, then execute the loop once, therefore rcx should be 0
; if r11 == 6, then execute the loop twice, therefore rcx should be 1
	mov rcx, r11
	sub rcx, 4
; Move r12, which contains source array address to register 13
	mov r13, r12 ; r12 == address of array
	add r13, 16 ; add 16 to get the 5th element [4]
; Move stack pointer and add 20h into r14 to push parameters onto stack
	mov r14, rsp ; place Stack Pointer into r14
	add r14, 20h ; add 32 to point to next available spot
; 5 => 20h
; 5 => 20h, 6 => 28h
; 5 => 20h, 6 => 28h, 7 => 30h, 8 => 38h
add_parm_to_stack:
; move 5th parameter to the top of the stack
	mov ebx, [r13] ; r13 = array address
	mov dword ptr[r14], ebx
; if there are 5 parameters only, jump to function call
; Increment r13 by 4 and r14 by 8
	add r13, 4
	add r14, 8
	loop add_parm_to_stack

do_first_parameters:
; if there are 0 parameters jump straight to function call
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

call_func:
	call r10
	add rsp, 0A00h

	pop rdi
	ret
call_function ENDP

call_function64 PROC
	push rdi
; make room on stack
	sub rsp, 0A00h
	mov rdi, rsp
; 1st 4 integer or pointer parameters are stored in:
; rcx, rdx, r8, r9
	; rcx = target function address
	; rdx = # of parameters
	; r8 = pointer to base address

; move address of target function to rcx
	mov r10, rcx
; move # of parameters to r11
	mov r11, rdx
; move address of array to r12
	mov r12, r8

; if r11 is less than 5, then jump to do_first_parameters
	cmp r11, 4
	jle do_first_parameters

; Otherwise, populate stack first then populate parameters 1-4
; if r11 == 5, then execute the loop once, therefore rcx should be 0
; if r11 == 6, then execute the loop twice, therefore rcx should be 1
	mov rcx, r11
	sub rcx, 4
; Move r12, which contains source array address to register 13
	mov r13, r12 ; r12 == address of array
	; [0] = 0, [1] = 24, [2] = 48, [3] = 72, [4] = 96
	add r13, 32 ; add (4 X 8=32)16 to get the 5th element [4]
; Move stack pointer and add 20h into r14 to push parameters onto stack
	mov r14, rsp ; place Stack Pointer into r14
	add r14, 20h ; add 32 to point to next available spot

add_parm_to_stack:
; move 5th parameter to the top of the stack
	mov rbx, r13 ; r13 = array address
	;mov dword ptr[r14], ebx
	mov rax, [r13]
	mov [r14], rax
; if there are 5 parameters only, jump to function call
; Increment r13 by 4 and r14 by 8
	add r13, 8
	add r14, 8
	loop add_parm_to_stack

do_first_parameters:
; if there are 0 parameters jump straight to function call
	cmp r11, 0
	jz call_func
; move first parameter to rcx
	mov rcx, [r12]
	cmp r11, 1
	je call_func
; move second parameter to rdx
	mov rdx, [r12+8]
	;add rdx, 8
; if there are 2 parameters only, jump to function call
	cmp r11, 2
	je call_func
; move 3rd parameter to r8
	mov r8, [r12+16]
	;add r8, 16
; if there are 3 parameters only, jump to function call
	cmp r11, 3
	je call_func
; move 4th parameter to r9
	mov r9, [r12+24]
	;add r9, 24
; if there are 4 parameters only, jump to function call
	cmp r11, 4
	je call_func

call_func:
	call r10
	add rsp, 0A00h

	pop rdi
	ret
call_function64 ENDP
End