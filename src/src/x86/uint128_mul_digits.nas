	global _mg_uint128_mul_digits

section .text

;int mg_uint128_mul_digits()
; RETURN
;   unoverflow: 0, overflow: 1

; PARAMETERS
%define OP1				ebp+8		; const mg_uint128 *
%define OP1_DIGITS		ebp+12		; int
%define OP2				ebp+16		; const mg_uint128 *
%define OP2_DIGITS		ebp+20		; int
%define RET_VALUE		ebp+24		; mg_uint128 *

; LOCAL
%define BUFFER			esp			; uint32_t[9]
%define OP2_INDEX		esp+40		; int

%define STACK_SIZE		44

_mg_uint128_mul_digits:
	push		ebp
	mov			ebp, esp
	
	push		ebx
	push		edi
	push		esi

	sub			esp, STACK_SIZE
	
	pxor		xmm1, xmm1
	movdqu		[BUFFER], xmm1
	movdqu		[BUFFER+16], xmm1

	; ëÂÇ´Ç¢ï˚Çì‡ë§ÉãÅ[ÉvÇ÷
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

	xor			ecx, ecx
_LOOP_OP2:
	lea			edi, [BUFFER+ecx*4]

	mov			[OP2_INDEX], ecx
	mov			eax, [OP2]
	mov			ecx, [eax+ecx*4]

	xor			esi, esi
	xor			ebx, ebx
_LOOP_OP1:
	; OP1[i]
	mov			edx, [OP1]

	; OP2[j]
	mov			eax, ecx

	; OP1[i] * OP2[j]
	mul			dword [edx+esi*4]
	
	add			eax,		[edi+0]
	adc			edx,		[edi+4]
	adc			ebx,		[edi+8]
	mov			[edi+0],	eax	
	mov			[edi+4],	edx	
	mov			[edi+8],	ebx	
	mov			ebx,		0
	setb		bl
	
	lea			edi,	[edi+4]

	inc			esi
	cmp			esi,	[OP1_DIGITS]

	jb			_LOOP_OP1;
	
	mov			ecx, [OP2_INDEX]
	inc			ecx
	cmp			ecx, [OP2_DIGITS]

	jb			_LOOP_OP2;

	mov			eax,	dword [BUFFER+16]
	mov			ebx,	dword [BUFFER+20]
	or			eax,	dword [BUFFER+24]
	or			ebx,	dword [BUFFER+28]
	or			eax,	ebx

	jz			_NOT_OVERFLOW;
	
	; return overflow
	mov			eax,	1
	
	jmp			_NOT_OVERFLOW_END;
_NOT_OVERFLOW:
	; return not overflow
	xor			eax,	eax

	; copy buffer
	mov			edi,			dword [RET_VALUE]

	movdqu		xmm1,			[BUFFER]
	movdqu		[edi+0],		xmm1
_NOT_OVERFLOW_END:

	add			esp,	STACK_SIZE

	pop			esi
	pop			edi
	pop			ebx

	mov			esp,	ebp
	pop			ebp
	ret
