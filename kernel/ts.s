! Luke Weber, 11398889
! CptS 460, Lab #3
! Created 09/12/2016

	MTXSEG  = 0x1000
	
       .globl _main,_running,_scheduler
       .globl _proc, _procSize
       .globl _tswitch
	
        jmpi   start,MTXSEG

start:	mov  ax,cs
	mov  ds,ax
	mov  ss,ax
        mov  es,ax
	mov  sp,#_proc
	add  sp,_procSize
		
	call _main

_tswitch:
SAVE:	
	push ax
	push bx
	push cx
	push dx
	push bp
	push si
	push di
	pushf
	mov  bx,_running
	mov  2[bx],sp

FIND:	call _scheduler

RESUME:
	mov  bx,_running
	mov  sp,2[bx]
	popf
	pop  di
	pop  si
	pop  bp
	pop  dx
	pop  cx
	pop  bx
	pop  ax

	ret
