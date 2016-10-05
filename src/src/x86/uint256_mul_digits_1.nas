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
%define OP2_INDEX		esp+72		; int

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
