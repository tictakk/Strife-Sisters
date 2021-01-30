;
; Graphics routines that I write will go here
;

Clear_BAT:
	vreg	#$00
	stwz	video_data
	vreg	#$02

	ldx		#LOW(BATWIDTH*BATHEIGHT)
	ldy     #HIGH(BATWIDTH*BATHEIGHT)
.clrlp:
	st1	#$20	;"whitespace character"
	st2	#$01
	dex
	bne     .clrlp
	dey
	bne	.clrlp
	rts


;Print_Byte:	;Prints a byte as HEX
;	pha
;	lsr a
;	lsr a
;	lsr a
;	lsr a
		;store char # (0-F) of high nyb
;	ora	#LOW(HEX_VRAM/16)
;	sta	video_data
;	lda	#$00+(HEX_VRAM/4096) ;font pal + tile no.
;	sta	video_data+1
;	pla
;Print_Nyb:
;	pha
;	and	#$0F	;isolate low nyb.
;	ora	#LOW(HEX_VRAM/16)
;	sta	video_data
;	lda	#$00+(HEX_VRAM/4096) ;font pal + tile no.
;	sta	video_data+1
;	pla
;	rts

;Print_Text:			;_si points to zero-terminated text
;	jsr	set_write       ;_di points to VRAM add.
;Print_Tex2:
;	cly
;	ldx	#$00+(ASCII_VRAM/4096)	;Palette 0, $1000 VRAM
;.loop1:
;	lda	[_si],Y
;	beq	.finish
;	sta	video_data
;	stx	video_data+1
;	incw	<_si
;	bra	.loop1
;.finish:
;	rts


Draw_BonkBG:	;draws 16x16 tile box w/ graphics on-screen
	;stw	#(1*$1000)+(BONKBG_VRAM/16),<_si ;Pal 1 + Tile No.
	stw #$1100,<_si
	stw	#$0000,<_di
	jsr	set_write	;set VRAM address

	ldy	#BATWIDTH		;outer loop (vert. lines)
.bonklp0:
	ldx	#BATHEIGHT		;inner loop (horiz. tiles)
.bonklp1:
	stw	<_si,video_data	;lay down tile definition
	incw	<_si
	dex
	bne	.bonklp1
	addw	#BATWIDTH,<_di	;then jump down to next line
	jsr	set_write
	dey
	bne     .bonklp0

	rts


Setup_Sprites:

	stz	satb	;zero out sprite table
	tii	satb,satb+1,511

	;---------------------------;
	; 			ARMY 1  		;
	;---------------------------;
	;stw	#$00DA,satb	;Y-pos
	;stw	#$0040,satb+2	;X-pos
	;stw	#(BONKSP_VRAM/32),satb+4 ;Tile No.
	;stw	#%0_0_01_0_00_1_1_001_0000,satb+6 ;Attributes

	stw	#$00F8,satb+16-16	;Y-pos
	stw	#$0048,satb+2+16-16	;X-pos
	stw	#(BONKSP_VRAM/32),satb+4+16-16 ;Tile No.
	stw	#%0_0_01_0_00_1_1_001_0000,satb+6+16-16 ;Attributes

	stw	#$00D6,satb+32-16	;Y-pos
	stw	#$0050,satb+2+32-16	;X-pos
	stw	#(BONKSP_VRAM/32),satb+4+32-16 ;Tile No.
	stw	#%0_0_01_0_00_1_1_001_0000,satb+6+32-16 ;Attributes

	stw	#$00B4,satb+48-16	;Y-pos
	stw	#$0058,satb+2+48-16	;X-pos
	stw	#(BONKSP_VRAM/32),satb+4+48-16 ;Tile No.
	stw	#%0_0_01_0_00_1_1_001_0000,satb+6+48-16 ;Attributes

	stw	#$0092,satb+64-16	;Y-pos
	stw	#$0060,satb+2+64-16	;X-pos
	stw	#(BONKSP_VRAM/32),satb+4+64-16 ;Tile No.
	stw	#%0_0_01_0_00_1_1_001_0000,satb+6+64-16 ;Attributes

	stw	#$0070,satb+80-16	;Y-pos
	stw	#$0068,satb+2+80-16	;X-pos
	stw	#(BONKSP_VRAM/32),satb+4+80-16 ;Tile No.
	stw	#%0_0_01_0_00_1_1_001_0000,satb+6+80-16 ;Attributes

	;---------------------------;
	; 			ARMY 1  cont	;
	;---------------------------;
	;stw	#$00DA,satb+96	;Y-pos
	;stw	#$0020,satb+2+96	;X-pos
	;stw	#(BONKSP_VRAM/32),satb+4+96 ;Tile No.
	;stw	#%0_0_01_0_00_1_1_001_0000,satb+6+96 ;Attributes

	stw	#$00F8,satb+112-32	;Y-pos
	stw	#$0028,satb+2+112-32	;X-pos
	stw	#(BONKSP_VRAM/32),satb+4+112-32 ;Tile No.
	stw	#%0_0_01_0_00_1_1_001_0000,satb+6+112-32 ;Attributes

	stw	#$00D6,satb+128-32	;Y-pos
	stw	#$0030,satb+2+128-32	;X-pos
	stw	#(BONKSP_VRAM/32),satb+4+128-32 ;Tile No.
	stw	#%0_0_01_0_00_1_1_001_0000,satb+6+128-32 ;Attributes

	stw	#$00B4,satb+144-32	;Y-pos
	stw	#$0038,satb+2+144-32	;X-pos
	stw	#(BONKSP_VRAM/32),satb+4+144-32 ;Tile No.
	stw	#%0_0_01_0_00_1_1_001_0000,satb+6+144-32 ;Attributes

	stw	#$0092,satb+160-32	;Y-pos
	stw	#$0040,satb+2+160-32	;X-pos
	stw	#(BONKSP_VRAM/32),satb+4+160-32;Tile No.
	stw	#%0_0_01_0_00_1_1_001_0000,satb+6+160-32;Attributes

	stw	#$0070,satb+176-32	;Y-pos
	stw	#$0048,satb+2+176-32	;X-pos
	stw	#(BONKSP_VRAM/32),satb+4+176-32 ;Tile No.
	stw	#%0_0_01_0_00_1_1_001_0000,satb+6+176-32 ;Attributes

	;---------------------------;
	; 			ARMY 2  		;
	;---------------------------;
	;stw	#$00DA,satb+8	;Y-pos
	;stw	#$00C0,satb+2+8	;X-pos
	;stw	#(BONKSP_VRAM/32),satb+4+8 ;Tile No.
	;stw	#%0_0_01_1_00_1_1_001_0001,satb+6+8 ;Attributes

	stw	#$00F8,satb+24-16	;Y-pos
	stw	#$00B8,satb+2+24-16	;X-pos
	stw	#(BONKSP_VRAM/32),satb+4+24-16 ;Tile No.
	stw	#%0_0_01_1_00_1_1_001_0001,satb+6+24-16 ;Attributes

	stw	#$00D6,satb+40-16	;Y-pos
	stw	#$00B0,satb+2+40-16	;X-pos
	stw	#(BONKSP_VRAM/32),satb+4+40-16 ;Tile No.
	stw	#%0_0_01_1_00_1_1_001_0001,satb+6+40-16 ;Attributes

	stw	#$00B4,satb+56-16	;Y-pos
	stw	#$00A8,satb+2+56-16	;X-pos
	stw	#(BONKSP_VRAM/32),satb+4+56-16 ;Tile No.
	stw	#%0_0_01_1_00_1_1_001_0001,satb+6+56-16 ;Attributes

	stw	#$0092,satb+72-16	;Y-pos
	stw	#$00A0,satb+2+72-16	;X-pos
	stw	#(BONKSP_VRAM/32),satb+4+72-16 ;Tile No.
	stw	#%0_0_01_1_00_1_1_001_0001,satb+6+72-16 ;Attributes

	stw	#$0070,satb+88-16	;Y-pos
	stw	#$0098,satb+2+88-16	;X-pos
	stw	#(BONKSP_VRAM/32),satb+4+88-16 ;Tile No.
	stw	#%0_0_01_1_00_1_1_001_0001,satb+6+88-16 ;Attributes

	;---------------------------;
	; 			ARMY 2 cont		;
	;---------------------------;
	;stw	#$00DA,satb+104	;Y-pos
	;stw	#$00E0,satb+2+104	;X-pos
	;stw	#(BONKSP_VRAM/32),satb+4+104 ;Tile No.
	;stw	#%0_0_01_1_00_1_1_001_0001,satb+6+104 ;Attributes

	stw	#$00F8,satb+120-32	;Y-pos
	stw	#$00D8,satb+2+120-32	;X-pos
	stw	#(BONKSP_VRAM/32),satb+4+120-32 ;Tile No.
	stw	#%0_0_01_1_00_1_1_001_0001,satb+6+120-32 ;Attributes

	stw	#$00D6,satb+136-32	;Y-pos
	stw	#$00D0,satb+2+136-32	;X-pos
	stw	#(BONKSP_VRAM/32),satb+4+136-32 ;Tile No.
	stw	#%0_0_01_1_00_1_1_001_0001,satb+6+136-32 ;Attributes

	stw	#$00B4,satb+152-32	;Y-pos
	stw	#$00C8,satb+2+152-32	;X-pos
	stw	#(BONKSP_VRAM/32),satb+4+152-32 ;Tile No.
	stw	#%0_0_01_1_00_1_1_001_0001,satb+6+152-32 ;Attributes

	stw	#$0092,satb+168-32	;Y-pos
	stw	#$00C0,satb+2+168-32	;X-pos
	stw	#(BONKSP_VRAM/32),satb+4+168-32 ;Tile No.
	stw	#%0_0_01_1_00_1_1_001_0001,satb+6+168-32 ;Attributes

	stw	#$0070,satb+184-32	;Y-pos
	stw	#$00B8,satb+2+184-32	;X-pos
	stw	#(BONKSP_VRAM/32),satb+4+184-32 ;Tile No.
	stw	#%0_0_01_1_00_1_1_001_0001,satb+6+184-32 ;Attributes

	bsr	Update_SATB_BLOCKTRANSFER
	rts

Update_SATB_BLOCKTRANSFER:	;SATB to VRAM (fast)
	stw	#SATB_VRAM,<_di ;default SATB location
	jsr	set_write
	tia	satb,$0002,$0100 ;Blast SATB to VRAM
	rts