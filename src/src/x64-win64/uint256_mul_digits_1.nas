	bits 64
	global mg_uint256_mul_digits_1
	
section .data
	JmpTable:	dq	_MUL_0x0, _MUL_1x1, _MUL_2xN, _MUL_3xN, _MUL_4xN, 0, 0, 0

section .text

; void mg_uint256_mul_digits_1();
; PARAMETERS
%define OP1				rbp+16		; const mg_uint256 *
%define OP1_DIGITS		rbp+24		; int
%define OP2				rbp+32		; const mg_uint256 *
%define OP2_DIGITS		rbp+40		; int
%define LOW_VALUE		rbp+48		; mg_uint256 *
%define HIGH_VALUE		rbp+56		; mg_uint256 *

; LOCAL
%define BUFFER			rsp			; uint64_t[9]

%define STACK_SIZE		72

mg_uint256_mul_digits_1:
	push		rbp
	mov			rbp, rsp
	
	; ëÂÇ´Ç¢ï˚Çì‡ë§ÉãÅ[ÉvÇ÷
	cmp			rdx,	r9
	jae			_MUL_BODY
	xchg		rcx,	r8
	xchg		rdx,	r9
_MUL_BODY:
	and			rdx,		0x7
	lea			rax,		[REL JmpTable]
	jmp			[rax+rdx*8]

;----------------------------------------------------------------
; multiply 0 * N
;----------------------------------------------------------------
_MUL_0x0:
	mov		r10,		qword [LOW_VALUE]
	mov		r11,		qword [HIGH_VALUE]

	pxor	xmm0,		xmm0
	movdqu	[r10],		xmm0
	movdqu	[r10+16],	xmm0

	movdqu	[r11],		xmm0
	movdqu	[r11+16],	xmm0

	mov		rsp,	rbp
	pop		rbp
	ret

;----------------------------------------------------------------
; multiply 1 * N
;----------------------------------------------------------------
_MUL_1x1:
	; op1[i]
	mov		rax,		[rcx]
	; op1[i] * op2[j]
	mul		qword [r8]
	
	mov		r10,		qword [LOW_VALUE]
	mov		r11,		qword [HIGH_VALUE]

	pxor	xmm0,		xmm0
	mov		[r10],		rax
	mov		[r10+8],	rdx
	movdqu	[r10+16],	xmm0

	movdqu	[r11],		xmm0
	movdqu	[r11+16],	xmm0

	mov		rsp,	rbp
	pop		rbp
	ret

;----------------------------------------------------------------
; multiply 2 * N
;----------------------------------------------------------------
_MUL_2xN:
	push	rbx
	push	rdi
	push	rsi
	push	r12
	push	r13
	push	r14
	push	r15
	
	sub		rsp,	STACK_SIZE
	
	pxor	xmm0,		xmm0
	movdqu	[BUFFER],		xmm0
	movdqu	[BUFFER+16],	xmm0
	movdqu	[BUFFER+32],	xmm0
	movdqu	[BUFFER+48],	xmm0

	xor		r10,	r10
_MUL_2xN_LOOP_OP2:
	xor		rbx,	rbx

	; &buf[k]
	lea		rdi,	[BUFFER+r10*8]

	; op1[i]
	mov		rax,	[rcx]

	; op1[i] * op2[j]
	mul		qword [r8+r10*8]

	mov		r11,	rax
	add		r11,	[rdi+0]
	mov		r12,	rdx
	adc		r12,	[rdi+8]
	mov		r13,	rbx
	adc		r13,	[rdi+16]
	setb	bl
	
	mov		rax,	[rcx+8]

	; op1[i+1] * op2[j]
	mul		qword [r8+r10*8]
	
	add		r12,	rax
	adc		r13,	rdx
	mov		r14,	rbx
	adc		r14,	[rdi+24]

	mov		[rdi+0],	r11
	mov		[rdi+8],	r12
	mov		[rdi+16],	r13
	mov		[rdi+24],	r14

	inc			r10
	cmp			r10,	r9

	jb			_MUL_2xN_LOOP_OP2
	
	jmp			_EXIT

;----------------------------------------------------------------
; multiply 3 * N
;----------------------------------------------------------------
_MUL_3xN:
	push	rbx
	push	rdi
	push	rsi
	push	r12
	push	r13
	push	r14
	push	r15
	
	sub		rsp,	STACK_SIZE
	
	pxor	xmm0,		xmm0
	movdqu	[BUFFER],		xmm0
	movdqu	[BUFFER+16],	xmm0
	movdqu	[BUFFER+32],	xmm0
	movdqu	[BUFFER+48],	xmm0

	xor		r10,	r10
_MUL_3xN_LOOP_OP2:
	xor		rbx,	rbx

	; &buf[k]
	lea		rdi,	[BUFFER+r10*8]

	; op1[0]
	mov		rax,	[rcx]

	; op1[0] * op2[j]
	mul		qword [r8+r10*8]

	mov		r11,	rax
	add		r11,	[rdi+0]
	mov		r12,	rdx
	adc		r12,	[rdi+8]
	mov		r13,	rbx
	adc		r13,	[rdi+16]
	setb	bl
	
	; op1[1]
	mov		rax,	[rcx+8]

	; op1[1] * op2[j]
	mul		qword [r8+r10*8]
	
	add		r12,	rax
	adc		r13,	rdx
	mov		r14,	rbx
	adc		r14,	[rdi+24]
	setb	bl
	
	; op1[2]
	mov		rax,	[rcx+16]

	; op1[2] * op2[j]
	mul		qword [r8+r10*8]
	
	add		r13,	rax
	adc		r14,	rdx
	mov		r15,	rbx
	adc		r15,	[rdi+32]

	mov		[rdi+0],	r11
	mov		[rdi+8],	r12
	mov		[rdi+16],	r13
	mov		[rdi+24],	r14
	mov		[rdi+32],	r15

	inc			r10
	cmp			r10,	r9

	jb			_MUL_3xN_LOOP_OP2
	
	jmp			_EXIT

;----------------------------------------------------------------
; multiply 4 * N
;----------------------------------------------------------------
_MUL_4xN:
	push	rbx
	push	rdi
	push	rsi
	push	r12
	push	r13
	push	r14
	push	r15
	
	sub		rsp,	STACK_SIZE
	
	pxor	xmm0,		xmm0
	movdqu	[rsp],		xmm0
	movdqu	[rsp+16],	xmm0
	movdqu	[rsp+32],	xmm0
	movdqu	[rsp+48],	xmm0

	xor		r10,	r10
_MUL_4xN_LOOP_OP2:
	xor		rbx,	rbx

	; &buf[k]
	lea		rdi,	[BUFFER+r10*8]

	; op1[0]
	mov		rax,	[rcx]

	; op1[0] * op2[j]
	mul		qword [r8+r10*8]

	mov		r11,	rax
	add		r11,	[rdi+0]
	mov		r12,	rdx
	adc		r12,	[rdi+8]
	mov		r13,	rbx
	adc		r13,	[rdi+16]
	setb	bl

	; op1[1]
	mov		rax,	[rcx+8]

	; op1[1] * op2[j]
	mul		qword [r8+r10*8]
	
	add		r12,	rax
	adc		r13,	rdx
	mov		r14,	rbx
	adc		r14,	[rdi+24]
	setb	bl

	; op1[2]
	mov		rax,	[rcx+16]

	; op1[2] * op2[j]
	mul		qword [r8+r10*8]
	
	add		r13,	rax
	adc		r14,	rdx
	mov		r15,	rbx
	adc		r15,	[rdi+32]
	setb	bl

	; op1[3]
	mov		rax,	[rcx+24]

	; op1[3] * op2[j]
	mul		qword [r8+r10*8]
	
	add		r14,	rax
	adc		r15,	rdx
	mov		rsi,	rbx
	adc		rsi,	[rdi+40]

	mov		[rdi+0],	r11
	mov		[rdi+8],	r12
	mov		[rdi+16],	r13
	mov		[rdi+24],	r14
	mov		[rdi+32],	r15
	mov		[rdi+40],	rsi

	inc			r10
	cmp			r10,	r9

	jb			_MUL_4xN_LOOP_OP2
	
	jmp			_EXIT

;----------------------------------------------------------------
; EXIT
;----------------------------------------------------------------
_EXIT:
	; copy buffer
	mov			rdi,			qword [LOW_VALUE]
	mov			rsi,			qword [HIGH_VALUE]

	movdqu		xmm0,			[BUFFER]
	movdqu		xmm1,			[BUFFER+16]
	movdqu		xmm2,			[BUFFER+32]
	movdqu		xmm3,			[BUFFER+48]
	movdqu		[rdi],			xmm0
	movdqu		[rdi+16],		xmm1
	movdqu		[rsi],			xmm2
	movdqu		[rsi+16],		xmm3

	add			rsp,	STACK_SIZE
	
	pop			r15
	pop			r14
	pop			r13
	pop			r12
	pop			rsi
	pop			rdi
	pop			rbx

	mov			rsp,	rbp
	pop			rbp
	ret
