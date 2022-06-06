global WriteByteToPortImpl
global WriteWordToPortImpl
global WriteDwordToPortImpl
global ReadByteFromPortImpl

section .text
bits 64

; extern "C" void WriteByteToPortImpl(uint16_t Port, uint8_t Value);
WriteByteToPortImpl:
	mov dx, di
	mov ax, si
	and ax, 0xFF
	out dx, al
	ret

; extern "C" void WriteWordToPortImpl(uint16_t Port, uint16_t Value);
WriteWordToPortImpl:
	mov dx, di
	mov ax, si
	out dx, ax
	ret

; extern "C" void WriteDwordToPortImpl(uint16_t Port, uint32_t Value);
WriteDwordToPortImpl:
	mov dx, di
	mov eax, esi
	out dx, eax
	ret
; extern "C" uint8_t ReadByteFromPortImpl(uint16_t Port);
ReadByteFromPortImpl:
	mov dx, di
	in ax, dx
	ret

