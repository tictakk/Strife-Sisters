char tone;

void chirp(char t)
{
	tone = t;

	#asm
	pha
	lda _tone
_ChibiSound:			;%NVPPPPPP	N=Noise  V=Volume  P=Pitch
	pha
		;lda #1
		;sta $0800		;Channel Select (0)

		stz $0804		;Channel On/Write - Set 'Data Write'

		;Define the wave data

		ldy #0
		ldx #32
	_ChibiSoundMoreWaves:
		lda _wave_1,y
		iny
		sta $0806		;Waveform Data (5 bit)
		dex
		bne _ChibiSoundMoreWaves

	pla
	bne _ChibiSound_NotSilent
		pla
	bra done
_ChibiSound_NotSilent:
	pha
		and #%01000000	;Volume
		lsr a
		lsr a
		lsr a
		ora #%10010111	;Chanel Op - Set 'Play' & Max Vol
		sta $0804		;Channel On/Write

		lda #255
		sta $0801		;Main Amplitude Level
		lda #%11001100
		sta $0805		;LR Volume
	pla

	pha
		and #%00111100	;Pitch
		lsr a
		lsr a
		sta $0803		;Frequency H
	pla
	pha
		and #%00000011	;Pitch
		clc
		ror a
		ror a
		ror a
		sta $0802		;Frequency L
	pla

	bit #%10000000
	beq _Chibisound_NoNoise

	and #%00111110		;Noise freq (5bits only)
	eor #%00111110
	lsr a
	sec
	ror a			;Top bit to 1
	bra _Chibisound_NoiseRet
_Chibisound_NoNoise:
	lda #0				;Stop the noise
_Chibisound_NoiseRet:
	sta $0807			;Noise Enable
	pla
done:
	#endasm
}

int c_address;
void load_wavetable(int addr)
{
	char i;
	c_address = addr;
	poke(0x804,0b01000000);
	poke(0x804,0);
	for(i=0; i<32; i++)
	{
		poke(0x806,peek(c_address+i));
	}
	poke(0x804,0b10011111);
	// #asm
	// ;pha
	// ;pha
	// _load_wave:
	// 	;pla
	// 	;sta _c_address
	// 	;pla
	// 	;sta _c_address+1
	//
	// 	lda #%01000000
	// 	sta $0804
	// 	lda #%00000000
	// 	sta $0804
	//
	// 	cly
	// ycounter:
	// 	lda _c_address,y
	// 	sta $0806		;Waveform Data (5 bit)
	// 	iny
	// 	cpy #32
	// 	bne ycounter
	// 	lda #%10011111
	// 	sta $0804
	// #endasm
}

void increase_frquency(int freq)
{
	poke(0x802,(char)(freq&0xFF));
	poke(0x803,(char)(freq>>8));
}

void set_channel(char channel)
{
	poke(0x800,channel);
}
// #asm
// _inc_freq:
// 	pha
// 		lda _bbhh
// 		beq _dec_hi
// 		dec
// 		sta _bbhh
// 		sta $802
// 		pla
// 	rts
//
// _dec_hi:
// 	ldy #01
// 	lda #%11111111
// 	sta _bbhh
// 	sta $802
// 	lda _bbhh,y
// 	dec
// 	sta _bbhh,y
// 	sta $803
// 	pla
// 	rts
// #endasm
