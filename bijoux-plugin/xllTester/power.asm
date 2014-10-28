
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
	push esi
	push edi

; Function Body
	mov eax, 300

; Restore state of all registers
	pop edi
	pop esi
	pop ebx
; Restore Stack and Base Pointer
	mov esp, ebp
	pop ebp
	ret
func_0 ENDP

func_1 PROC
	push ebp
	mov ebp, esp
	push ebx
	push esi
	push edi

; Function Body
	mov eax, dword ptr [ebp+8]
	imul eax, eax, 3

; Restore state of all registers
	pop edi
	pop esi
	pop ebx
; Restore Stack and Base Pointer
	mov esp, ebp
	pop ebp
	ret
func_1 ENDP

func_2 PROC
	push ebp
	mov ebp, esp
	push ebx
	push esi
	push edi

; Function Body
	mov eax, dword ptr [ebp+8]
	add eax, dword ptr [ebp+12]
	;imul eax, eax, 3

; Restore state of all registers
	pop edi
	pop esi
	pop ebx
; Restore Stack and Base Pointer
	mov esp, ebp
	pop ebp
	ret
func_2 ENDP

func_3 PROC
	push ebp
	mov ebp, esp
	push ebx
	push esi
	push edi

; Function Body
	mov eax, dword ptr [ebp+8]
	add eax, dword ptr [ebp+12]
	add eax, dword ptr [ebp+16]
	;imul eax, eax, 3

; Restore state of all registers
	pop edi
	pop esi
	pop ebx
; Restore Stack and Base Pointer
	mov esp, ebp
	pop ebp
	ret
func_3 ENDP

call_function PROC
	push ebp
	mov ebp, esp
	push ebx
	push esi
	push edi

; Function Body
; Push parameter onto the stack
	push dword ptr [ ebp+8]
	push dword ptr [ ebp+12]
	;mov eax, dword ptr [ebp+8]
	call func_2

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