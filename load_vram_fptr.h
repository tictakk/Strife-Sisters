
void __fastcall __macro load_vram_fptr( unsigned char trash<__al> );

//..............................................
//load_vram_fptr
//
// IN :	_DI = VRAM location
//	    _BL = data bank
//	    _SI = data memory location
//	    _CX = number of words to copy

#asm
_load_vram_fptr .macro
      .fail
    .endm

_load_vram_fptr.1 .macro
      jsr load_vram
    .endm

#endasm

// IN :	_DI = VRAM location
//	    _BL = data bank
//	    _SI = data memory location
//	    _CX = number of words to copy

void __fastcall __nop setFarLoadvram( unsigned int vram <__di>, unsigned char bank_p<__bl>, unsigned int addr_p<__si>, unsigned int size<__cx> );

void __fastcall __macro setFarOffsetLoadvram( unsigned int vram <__di>, unsigned char bank_p<__bl>, unsigned int addr_p<__si>, unsigned int size<__cx>,  unsigned int offset<__dx>);
void __fastcall __macro setFarOffsetLoadvramLabel( unsigned int vram <__di>, char far *obj<__bl:__si>, unsigned int size<__cx>,  unsigned int offset<__dx>);

//..............................................
//addOffsetFarPointer
#asm
_setFarOffsetLoadvram.5 .macro

.vram_p = __di
.size   = __cx

.bank_p = __bl
.addr_p = __si
.offset = __dx

            lda <.addr_p
            clc
            adc <.offset
            sta <.addr_p
            lda <.addr_p + 1
            adc <.offset + 1
              pha
            and #$1f
            sta <.addr_p + 1
              pla
            lsr a
            lsr a
            lsr a
            lsr a
            lsr a
            clc
            adc <.bank_p
            sta <.bank_p

    .endm
#endasm

//..............................................
//addOffsetFarPointer
#asm
_setFarOffsetLoadvramLabel.4 .macro

.vram_p = __di
.size   = __cx

.bank_p = __bl
.addr_p = __si
.offset = __dx

            lda <.addr_p
            clc
            adc <.offset
            sta <.addr_p
            lda <.addr_p + 1
            adc <.offset + 1
              pha
            and #$1f
            sta <.addr_p + 1
              pla
            lsr a
            lsr a
            lsr a
            lsr a
            lsr a
            clc
            adc <.bank_p
            sta <.bank_p

    .endm
#endasm



