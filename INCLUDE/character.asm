;------------------------------
;       Character logic flow
;------------------------------
;
; logic
; BBS0 -> death
; BBS1 -> attack 
; BBS2 -> move
;------------------------------

BATTLE_LOOP:
    sei
	stz <vb_count

    lda #$10
    sta $2012

    lda #$08
    sta opp

    lda <army_1_count
    sta <army_count

    lda <army_2_count
    sta <opp_army_count

    stw #$2200, <current_army
    ldx <army_count 

    jsr BATTLE

    lda <army_count
    sta <army_1_count

    lda #$00
    sta opp

    lda <army_2_count
    sta <army_count

    lda <army_1_count
    sta <opp_army_count

    stw #$2208, <current_army
    ldx <army_count

    jsr BATTLE

    lda <army_count
    sta <army_2_count
    jsr Update_SATB_BLOCKTRANSFER
    cli
    rts

BATTLE:
.next:
    BEQ .end_army ;if zero
    PHX
    CLY
    LDA [$10], Y
    BEQ .skip_unit
    JSR GET_FRAME
    ;BBR2 <frame_state, .cont
    JSR GET_STATE
    AND #$10
    BNE .cont_char
    JSR DEATH
    BRA .skip_update_position
.cont_char:
    LDY #$06
    LDA [$10], Y
    AND #%00100000
    BEQ .store_moving
    LDA #$A0
    STA <move_state
    BRA .store_attack
.store_moving
    LDA #$80
    STA <move_state
.store_attack
    CLY
    lda [$10], Y
    sta <unit_y
    iny
    iny
    lda [$10],Y
    sta <unit_x

    JSR ANIMATION
    JSR VERIFY_MOVE

    CLY
    lda $20FF
    BNE .skip_update_position
    ;BRANCH IF CAN'T MOVE .SKIP_UPDATE_POS
    lda <unit_y
    sta [$10],Y
    iny
    iny
    lda <unit_x
    sta [$10],Y
    BRA .skip_update_position

.skip_unit
    addw $2012,<current_army
    PLX
    BRA .next

.skip_update_position
    addw $2012,<current_army
    PLX
    DEX
    BRA .next

.end_army:
    rts

;*--------------------------------*
;|       CHARACTER FUNCTIONS      |
;*--------------------------------*

DEATH:
    CLY
    LDA #$C0
    STA <move_state
    JSR ANIMATE

    BBR2 <frame_state, .dont_kill_yet
    LDA #$00
    STA [$10], Y
    INY
    STA [$10], Y
    INY
    STA [$10], Y
    INY
    STA [$10], Y
    INY
    STA [$10], Y
    INY
    STA [$10], Y
    INY
    STA [$10], Y
    INY
    STA [$10], Y
    DEC <army_count
.dont_kill_yet
    rts

FIND_Y_DIRECTION:
    lda <unit_y
    SEC 
    phy
    ldy <opp_pointer
    sbc $2200,Y;$2208
    ply
    BCS .inc_y

    inc <unit_y
    inc <unit_y
    rts
.inc_y
    dec <unit_y
    dec <unit_y
    rts

FIND_X_DIRECTION:
    lda <unit_x
    SEC
    phy
    ldy <opp_pointer
    iny
    iny
    sbc $2200,Y;<opp_unit_x;$220A
    ply
    BCS .inc_x

    inc <unit_x
    inc <unit_x

    ldy #$07
    lda [$10], Y
    ORA #$08
    sta [$10], Y

    rts
.inc_x
    dec <unit_x
    dec <unit_x

    ldy #$07
    lda [$10], Y
    AND #$F7
    sta [$10], Y

    rts

MOVE:
    pha
    lda <opp_unit_y
    BEQ .change_x

    BSR FIND_Y_DIRECTION

.change_x
    SEC
    lda #$20
    sbc <opp_unit_x
    BCS .too_close
    BEQ .end_move
    
    BSR FIND_X_DIRECTION
    pla
    rts

.too_close
    lda <unit_x
    SEC
    phy
    ldy <opp_pointer
    iny
    iny
    sbc $2200,Y;<opp_unit_x;$220A
    ply
    BCC ._inc_x

    inc <unit_x
    inc <unit_x
    pla
    rts
._inc_x
    dec <unit_x
    dec <unit_x
    pla
    rts

.end_move
    pla
    rts

ANIMATION:
    ;
    lda #$FF
    sta <opp_unit_x
    sta <opp_unit_y
    sta <opp_diff
    lda #$01
    sta <opp_diff+1
    ;
    LDA $0C00
    SEC
    ADC <PRN0
    STA <PRN0
	jsr CHECK_ATTACK
	BEQ .change_frame

.MOVEMENT:
    ;jsr MOVE    

.change_frame:
ANIMATE
    phy
    ldy #$04
    lda [$10], Y
    CLC
    ADC #$08
    SEC
.mod:
	SBC #$20	
	BCS .mod
.step_frame:
	ADC #$20    
	CLC

    ADC <move_state
	;ADC #$80
    sta [$10],Y
    ply
	rts

CHECK_ATTACK: ;if able to attack, don't move unit until other unit has died

    JSR FIND_NEAREST_OPP	
    lda <opp_unit_y
    BNE .end_check

    SEC
    lda <opp_unit_x
    sbc #$20
    BNE .end_check

    JSR ATTACK

	BRA .swap_animations
.end_check
	rts

.swap_animations:
	;vload	BONKSP_VRAM, SprAttack, $0400
    LDA #$A0
    STA <move_state
    LDY #$06
    LDA [$10], Y
    ORA #%00100000
    LDA #$00
	rts

FIND_NEAREST_OPP:
    phx
    phy

    lda opp
    TAY
    ldx <opp_army_count

.nearest_loop
    BEQ .end_nearest_loop
    LDA $2200, Y
    BNE .cont_nearest
    INX
    INY
    INY
    BRA .next

.cont_nearest:
    lda <unit_y ;current unit y
    SEC
    SBC $2200,Y ;n unit's Y
    BCS .store_y
    EOR #$FF
    ADC #$01
.store_y
    sta $20FF ;store y

    iny
    iny
    clc
    lda <unit_x ;current unit x
    SEC
    sbc $2200,Y ;n unit's x
    BCS .store_x
    EOR #$FF
    ADC #$01
.store_x    
    sta $20FE ;store x
    
    CLC
    lda $20FF ;y
    adc $20FE ;x
    sta $20FC ;store diff
    stz $20FD
    ROL $20FD
        
    sbws $20FC, <opp_diff, $20FA;<opp_diff, $20FC, $20FA
    BCC .next

    ;have a new closest opponent
    phy
    dey
    dey
    sty <opp_pointer
    ply
    lda $20FF
    sta <opp_unit_y ;difference
    lda $20FE
    sta <opp_unit_x ;difference
    lda $20FC
    sta <opp_diff
    lda $20FD
    sta <opp_diff+1

    ;if he's in attacking range, go ahead and and exit. We don't care if there are other people closer
    lda <opp_unit_y
    BNE .next
    lda <opp_unit_x
    SEC
    SBC #$20 ;CHANGE HERE
    BEQ .end_nearest_loop

.next:
    TYA 
    clc
    adc #$0E
    TAY
    DEX
    BRA .nearest_loop

.end_nearest_loop
    ply
    plx
    rts

VERIFY_MOVE
    phx
    phy
    cly
    
    lda opp ;load the current opponent offset and then XOR it with 8 to get our armies offset 
    EOR #$08
    TAY
    ldx <army_count

.verify_loop
    BEQ .end_verify
    ;This check is just to make sure it's not our unit we're checking position against
    LDA $2200,Y
    BNE .cont_v
    INX
    BRA .next_verify
.cont_v
    CLC
    CPY $2010
    BNE .verify 
    BRA .next_verify

.verify:
    lda <unit_y ;current unit y
    SEC
    SBC $2200,Y ;n unit's Y
    BCS .cont_x_verify
    EOR #$FF
    ADC #$01

.cont_x_verify
    STA $20FF

    iny
    iny
    lda <unit_x ;current unit y
    SEC
    SBC $2200,Y ;n unit's Y
    BCS .cont_verify
    EOR #$FF
    ADC #$01

.cont_verify
    DEY
    DEY
    STA $20FE   
    LDA #$20 ;CHANGE HERE
    CMP $20FF
    BMI .next_verify
    SBC $20FE
    BPL .cant_move

.next_verify
    TYA
    CLC
    ADC #$10
    TAY
    DEX
    stz $20FF
    BRA .verify_loop

.cant_move
    lda #$01
    sta $20FF
.end_verify
    ply
    plx
    rts

ATTACK
    JSR GET_FRAME ;loads frame number into A
    BBR2 <frame_state, .end_attack
    JSR GET_OPP_STATE
    PHA
    AND #$10
    BEQ .end_a ;opp state is already dead, so no need to do anything else
    JSR GET_UNIT_ADVANTAGE
    JSR SCRAMBLE_RAND
    SBC <PRN0
    BCC .end_a
    ;Set opp state to dead, also reset their frame state
    pla
    EOR #$10
    TAX
    LDA <opp_pointer
    CLC
    ADC #$06
    TAY
    TXA 
    STA $2200, Y
    DEY
    DEY
    LDA #$C0 ;store move frame?
    STA $2200, Y
    BRA .end_attack 

.end_a
    pla
.end_attack    
    rts

GET_FRAME ;1-4
    phx
    phy
    CLX 
    LDY #$04
    LDA [$10], Y
    SEC
.reduce_frame
    SBC #$20
    BCS .reduce_frame
    ADC #$20
.divide_frame
    INX
    SBC #$08
    BCS .divide_frame

    STX <frame_state
    ply
    plx
    rts

GET_OPP_STATE
    LDA <opp_pointer
    CLC 
    ADC #$06
    TAY
    LDA $2200, Y
    rts

GET_STATE
    LDY #$06
    LDA [$10], Y
    rts

GET_UNIT_ADVANTAGE
    lda #$10
    rts

SCRAMBLE_RAND ;bettween 0-100
    pha
    LDA <PRN0
    ADC <PRN1
    STA <PRN1
    LDA <PRN1
    pla
    rts