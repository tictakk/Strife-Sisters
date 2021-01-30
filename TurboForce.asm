;
;  TUTORIAL.ASM  -  single Hsync moving on-screen
;

;----- HEADER & INCLUDES ----------------------
	.nolist
	.nomlist
	.include "INCLUDE\startup.asm"
	.list
	.mlist
;--------------- END INCLUDES ------------------

TOP_LINE	= $0040
BOT_LINE	= $0130

BONKBG_VRAM	 	 = $1000
BONKSP_VRAM	 	 = $3000

SATB_VRAM	= $0F00	;where to put the Sprite Table in VRAM

BATWIDTH	= 32	;Set to 32, 64, or 128
BATHEIGHT	= 32	;Set to 32 or 64.

; Zero-page variables
	.zp
hsync_line:		.ds 2
direction:		.ds 1
vb_count:		.ds 1 ;$03
frame_index		.ds 1 ;$04
army_type		.ds 1
army_1_count	.ds 1
opp_army_type	.ds 1
army_2_count	.ds 1
unit_x			.ds 1
unit_y			.ds 1
opp_unit_x		.ds 1
opp_unit_y		.ds 1
opp_diff		.ds 2
opp				.ds 1
current_army	.ds 2
current_opp		.ds 2
opp_pointer		.ds 1
army_count		.ds 1
opp_army_count	.ds 1
frame_state		.ds 1
PRN0			.ds 1
PRN1			.ds 1
PRN2			.ds 1
PRN3			.ds 1
move_state		.ds 1
general_1		.ds 1
general_2		.ds 1

;Higher RAM variables
	.bss
satb:		 .ds 512	;Sprite Attribute Table Buffer
army_one_offset		= $2200
army_two_offset 	= $2208

;==============================
	.code
	.bank	$0
	.org	$E000


MAIN:
	INTS_OFF		;DISABLE INTERRUPTS!
	stz	<direction
	stw	#$0040,<hsync_line

	map bg1
	vload BONKBG_VRAM, bg1, #$800

	map bg2
	vload $1800, bg2, #$800

	map	bg3
	vload $2000, bg3, #$800


	map BonkPal
	map SPRPal
	;map SprCHR
	;vload	BONKSP_VRAM, SprCHR, $0C00	;Load 2 32x32 sprites

	set_bgpal #1, BonkPal, #1
	set_sprpal #0, SPRPal, #1
	set_sprpal #1, SPRPAlt, #2

	map BGMap
	;jsr	Clear_BAT
	;jsr	Draw_BonkBG  ;E1B0 I think?
	batcpy $0000,#32,BGMap,#32,#32
	jsr	Setup_Sprites

	lda #$01
	sta <frame_index

	BG_CYAN
	SCREEN_ON
	lda #$0A
	sta <army_type ;army 1 type 
	sta <army_1_count ;army 1 count
	lda #$0A
	sta <opp_army_type ;army 2 type
	sta <army_2_count ;army 2 count

.loop            	;Here's an infinite loop...
	bbr3 <vb_count, .loop;ANIMATION
	jsr BATTLE_LOOP
	bra	.loop

	.include "INCLUDE\character.asm";,20,8

MY_VSYNC:
	;BORD_RED
	pha
	pla
	pha
	pla
	;BORD_BLUE
	;BG_BLACK

	inc	<vb_count
	rts

MY_HSYNC:
	;BORD_WHITE
	;BG_CYAN

	vreg	#6	;Scanline compare register
	st1		#$00	;No more Hsyncs.
	st2		#$00

	;BORD_DKBLU
	;BG_GREY2

	rts



;Intro_Text:
      ;0123456789ABCDEF0123456789ABCDEF
 ;.db "THIS IS OUR FIRST ON-SCREEN TEXT"
 ;.db "SCANLINE SPLIT NOW AT: XXX      ",0


;============================================================
; Other includes / banks go here (for now)

	.include "INCLUDE\gfx_work.asm"

;============================================================
;============================================================

	.bank $2
	.org $4000
;BonkBG:  .incchr "INCLUDE\realBigBG.pcx",64,64
BGMap:	.incbin "INCLUDE\trt.MAP"

	.bank $3
	.org $4000
bg1: 	.incchr "INCLUDE\bgmap.pcx",20,8

	.bank $4
	.org $4000
bg2: 	.incchr "INCLUDE\bgmap.pcx",0,64,20,8

	.bank $5
	.org $4000
bg3: 	.incchr "INCLUDE\bgmap.pcx",0,128,20,8

	;.bank $3
	;.org  $4000
;BonkBG1: ;.incchr "INCLUDE\realBigBG.pcx",256,511,32,64

	;.bank $4
	;.org  $4000
;BonkBG2: ;.incchr "INCLUDE\realBigBG.pcx",0,256,64,32

	;.bank $5
	;.org  $4000
;BonkBG3: ;.incchr "INCLUDE\realBigBG.pcx",256,512,64,64

	;.bank $6
	;.org  $4000
;BonkBG4: ;.incchr "INCLUDE\realBigBG.pcx",256,512,64,64

	;.bank $7
	;.org  $4000
;BonkBG5: ;.incchr "INCLUDE\realBigBG.pcx",256,512,64,64

	.bank $A
	.org  $C000
BonkPal: .incpal "INCLUDE\realBigBG.pcx",0,11
SprCHR:
	.incspr "INCLUDE\Soldier.pcx",0,0,2,2
	.incspr "INCLUDE\Soldier.pcx",32,0,2,2
	.incspr "INCLUDE\Soldier.pcx",64,0,2,2
	.incspr "INCLUDE\Soldier.pcx",96,0,2,2
	.incspr "INCLUDE\Soldier.pcx",128,0,2,2
	.incspr "INCLUDE\Soldier.pcx",160,0,2,2
	.incspr "INCLUDE\Soldier.pcx",192,0,2,2
	.incspr "INCLUDE\Soldier.pcx",224,0,2,2
	.incspr "INCLUDE\Soldier.pcx",256,0,2,2
	.incspr "INCLUDE\Soldier.pcx",288,0,2,2
	.incspr "INCLUDE\Soldier.pcx",320,0,2,2
	.incspr "INCLUDE\Soldier.pcx",352,0,2,2

SPRPal:	 .incpal "INCLUDE\BonkSP.pcx"
SPRPAlt: .incpal "INCLUDE\bonkSPAlt.pcx"