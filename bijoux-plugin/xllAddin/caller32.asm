.586              ;Target processor.  Use instructions for Pentium class machines
.MODEL FLAT, C    ;Use the flat memory model. Use C calling conventions
;.STACK            ;Define a stack segment of 1KB (Not required for this example)
;.DATA             ;Create a near data segment.  Local variables are declared after
                  ;this directive (Not required for this example)
.CODE             ;Indicates the start of a code segment.


call_function PROC
	push ebp
	mov ebp, esp
	push ebx
	push esi
	push edi

; Function Body
	; [ebp+8] = address of function to call
	; [ebp+12] = number of arguments
	; [ebp+16] = address of array with arguments

; If statement
; Move # of arguments into ecx register
	mov ecx, [ebp+12]
; Move parameter address into eax register
	mov eax, [ebp+16]

; If 0 parameters, skip directly to call the function
	cmp ecx, 0
	jz call_target_func

loop_again:
; Read one array parameter and place onto stack
	push dword ptr[eax]
; Increment pointer to parameters pointer array
	add eax, 4
; Increment stack pointer because we just 
;	add esp, 4
	loop loop_again

; Call the function
call_target_func:
	call dword ptr [ebp+8]

; Restore proper size of stack
	mov ecx, [ebp+12]
	cmp ecx, 0
	jz return_to_caller

restore_stack:
	pop ebx
	loop restore_stack;

return_to_caller:

; Restore state of all registers
	pop edi
	pop esi
	pop ebx
; Restore Stack and Base Pointer
	mov esp, ebp
	pop ebp
	ret

call_function ENDP

END