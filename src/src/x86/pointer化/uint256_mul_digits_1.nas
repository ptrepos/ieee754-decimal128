	global _mg_uint256_mul_digits_1

section .text

; void mg_uint256_mul_digits_1();
; PARAMETERS
%define OP1				ebp+8		; const mg_uint256 *
%define OP1_DIGITS		ebp+12		; int
%define OP2				ebp+16		; const mg_uint256 *
%define OP2_DIGITS		ebp+20		; int
%define LOW_VALUE		ebp+24		; mg_uint256 *
%define HIGH_VALUE		ebp+28		; mg_uint256 *

; LOCAL
%define BUFFER			esp			; uint32_t[17]
%define BUF_PTR			esp+72		; int
%define OP2_PTR			esp+76		; int

%define STACK_SIZE		80

%define STACK_SIZE		76

_mg_uint256_mul_digits_1:
	push		ebp
	mov			ebp, esp
	
	push		ebx
	push		edi
	push		esi

	sub			esp, STACK_SIZE
	
	pxor		xmm1, xmm1
	movdqu		[BUFFER], xmm1
	movdqu		[BUFFER+16], xmm1
	movdqu		[BUFFER+32], xmm1
	movdqu		[BUFFER+48], xmm1
	
	; 大きい方を内側ループへ
	mov			eax, [OP1_DIGITS]
	mov			edx, [OP2_DIGITS]
	cmp			eax, edx
	jae			_MUL_BODY
	mov			ebx, [OP1]
	mov			ecx, [OP2]

	xchg		eax, edx
	xchg		ebx, ecx

	mov			[OP1_DIGITS], eax
	mov			[OP2_DIGITS], edx
	mov			[OP1], ebx
	mov			[OP2], ecx
_MUL_BODY:
	; DIGITSのポインタ化
	mov			eax,	[OP1]
	mov			ebx,	[OP1_DIGITS]
	lea			ebx,	[eax+ebx*4]
	mov			ecx,	[OP2]
	mov			edx,	[OP2_DIGITS]
	lea			edx,	[ecx+edx*4]

	mov			[OP1_DIGITS], ebx
	mov			[OP2_DIGITS], edx

	lea			edi, [BUFFER]
	mov			[BUF_PTR], edi
	
	mov			ecx, [OP2]
_LOOP_OP2:
	mov			edi, [BUF_PTR]

	mov			[OP2_PTR], ecx
	mov			ecx, [ecx]

	mov			esi, [OP1]
	xor			ebx, ebx
_LOOP_OP1:
	; OP2[j]
	mov			eax, ecx
	; OP1[i] * OP2[j]
	mul			dword [esi]
	
	add			eax, [edi+0]
	adc			edx, [edi+4]
	adc			ebx, [edi+8]
	mov			[edi+0], eax	
	mov			[edi+4], edx	
	mov			[edi+8], ebx	
	mov			ebx,  0
	setb		bl
	
	lea			edi, [edi+4]

	lea			esi, [esi+4]
	cmp			esi, [OP1_DIGITS]

	jb			_LOOP_OP1;
	
	mov			edi, [BUF_PTR]
	lea			edi, [edi+4]
	mov			[BUF_PTR], edi
	
	mov			ecx, [OP2_PTR]
	lea			ecx, [ecx+4]
	cmp			ecx, [OP2_DIGITS]

	jb			_LOOP_OP2;

	; copy BUFFER
	mov			edi,			dword [LOW_VALUE]
	mov			esi,			dword [HIGH_VALUE]

	movdqu		xmm0,			[BUFFER]
	movdqu		xmm1,			[BUFFER+16]
	movdqu		xmm2,			[BUFFER+32]
	movdqu		xmm3,			[BUFFER+48]
	movdqu		[edi+0],		xmm0
	movdqu		[edi+16],		xmm1
	movdqu		[esi+0],		xmm2
	movdqu		[esi+16],		xmm3

	add			esp,	STACK_SIZE

	pop			esi
	pop			edi
	pop			ebx

	mov			esp,	ebp
	pop			ebp
	ret
