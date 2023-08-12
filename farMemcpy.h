
void __fastcall __macro getFarPointer( char far *obj<__fbank:__fptr>, unsigned int bank_p<__ax>, unsigned int addr_p<__bx> );
void __fastcall __macro addOffsetFarPointer( unsigned int bank_p<__ax>, unsigned int addr_p<__bx>, unsigned int offset<__cx> );

//..............................................
//getFarPointer
#asm
_getFarPointer.3 .macro
            lda __fbank
            sta [__ax]
            lda __fptr
            sta [__bx]
            lda __fptr+1
            and #$1f
            ldy #$01
            sta [__bx],y
    .endm
#endasm

//..............................................
//addOffsetFarPointer
#asm
_addOffsetFarPointer.3 .macro

.bank_p = __ax
.addr_p = __bx
.offset = __cx

            ldy #1
            lda [.addr_p]
            clc
            adc <.offset
            sta [.addr_p]
            lda [.addr_p],y
            adc <.offset + 1
              pha
            and #$1f
            sta [.addr_p],y
              pla
            lsr a
            lsr a
            lsr a
            lsr a
            lsr a
            clc
            adc [.bank_p]
            sta [.bank_p]


    .endm
#endasm

void __fastcall __nop setFarMemcpy( unsigned int dest <__ax>, unsigned char bank_p<__cl>, unsigned int addr_p<__bx>, unsigned char size<__ch> );
void __fastcall __nop setFarMemcpyLabel( unsigned int dest <__ax>, char far *obj<__cl:__bx>, unsigned char size<__ch> );

void __fastcall __nop setFarOffsetMemcpy( unsigned int dest <__ax>, unsigned char bank_p<__cl>, unsigned int addr_p<__bx>, unsigned char size<__ch>,  unsigned int offset<__dx>);
void __fastcall __nop setFarOffsetMemcpyLabel( unsigned int dest <__ax>, char far *obj<__cl:__bx>, unsigned char size<__ch>,  unsigned int offset<__dx>);


void farMemcpy( void );
void farOffsetMemcpy( void );

