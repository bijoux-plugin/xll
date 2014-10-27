
.586              ;Target processor.  Use instructions for Pentium class machines
.MODEL FLAT, C    ;Use the flat memory model. Use C calling conventions
;.STACK            ;Define a stack segment of 1KB (Not required for this example)
;.DATA             ;Create a near data segment.  Local variables are declared after
                  ;this directive (Not required for this example)
.CODE             ;Indicates the start of a code segment.

func_0 PROC
	push ebp
	mov ebp, esp

; Save state of all registers
	push ebx
	push ecx
	push edx
	push esi
	push edi

; Function Body
	mov eax, 123

; Restore state of all registers
	pop edi
	pop esi
	pop edx
	pop ecx
	pop ebx
	pop ebp
	ret
func_0 ENDP

func_1 PROC
	push ebp
	mov ebp, esp

; Save state of all registers
	push ebx
	push ecx
	push edx
	push esi
	push edi

; Function Body
	mov eax, 123

; Restore state of all registers
	pop edi
	pop esi
	pop edx
	pop ecx
	pop ebx
	pop ebp
	ret
func_1 ENDP

func_2 PROC
func_2 ENDP


call_function PROC
;	mov eax, [esp+4]

	push ebp
	mov ebp, esp

; Prepare for call to function in [esp+4]
; 1 parameter
	mov eax, [esp+8]
	mov ebx, 32
	push ebx
; call the function
	call eax
; get the result and place it in eax
	pop ebx
;	mov eax, 4

	pop ebp
	ret

call_function ENDP

swap PROC
    mov ecx, [esp+4]
    mov edx, [esp+8]
    mov eax, [ecx]
	mov ebx, [edx]
	mov [edx], eax
	mov [ecx], ebx
    ;xchg [ecx],eax
    ret
swap ENDP 
END

