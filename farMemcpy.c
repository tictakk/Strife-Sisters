
// Safe but slow version
void farMemcpy( void )
{
    #asm

.src_addr  = __bx
.dest_addr = __ax
.src_bank  = __cl
.src_size  = __ch

        ;// $6000-9fff
        tma #$03
        pha
        tma #$04
        pha

        lda <.src_bank 
        tam #$03
        inc a
        tam #$04

        ;// prep source address for mapped pages
        lda <.src_addr + 1
        and #$1f
        ora #$60
        sta <.src_addr + 1

        ;// Copy data
        cly
.loop
        lda [.src_addr],y
        sta [.dest_addr],y
        iny
        cpy <.src_size
      bcc .loop


        ;// restore pages
        pla
        tam #$04
        pla
        tam #$03

    #endasm

}

// Safe but slow version
void farOffsetMemcpy( void )
{
    #asm

.src_addr  = __bx
.dest_addr = __ax
.src_bank  = __cl
.src_size  = __ch
.src_offset = __dx

        ;// $6000-9fff
        tma #$03
        pha
        tma #$04
        pha

        ;// prep source address for mapped pages
        lda <.src_addr
        clc
        adc <.src_offset
        sta <.src_addr
        lda <.src_addr + 1
        adc <.src_offset + 1
          pha
        and #$1f
        ora #$60
        sta <.src_addr + 1
          pla
        lsr a
        lsr a
        lsr a
        lsr a
        lsr a
        clc
        adc <.src_bank
        tam #$03
        inc a
        tam #$04        

        ;// Copy data
        cly
.loop
        lda [.src_addr],y
        sta [.dest_addr],y
        iny
        cpy <.src_size
      bcc .loop


        ;// restore pages
        pla
        tam #$04
        pla
        tam #$03

    #endasm

}

