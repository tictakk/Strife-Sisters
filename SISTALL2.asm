;--------Begin SONG1--------
SONG1:
		.db		$3f
		.db		LOW(ENDLEAD)
		.db		HIGH(ENDLEAD)
		.db		LOW(ENDEPIANOA)
		.db		HIGH(ENDEPIANOA)
		.db		LOW(ENDEPIANOB)
		.db		HIGH(ENDEPIANOB)
		.db		LOW(ENDBASS)
		.db		HIGH(ENDBASS)
		.db		LOW(ENDNOISE)
		.db		HIGH(ENDNOISE)
		.db		LOW(ENDKICKS)
		.db		HIGH(ENDKICKS)
;--------End SONG1--------

;--------Begin SONG2--------
SONG2:
		.db		$1f
		.db		LOW(EVILSTRINGS)
		.db		HIGH(EVILSTRINGS)
		.db		LOW(EVILBASS)
		.db		HIGH(EVILBASS)
		.db		LOW(EVILLEAD)
		.db		HIGH(EVILLEAD)
		.db		LOW(EVILLEADY2A)
		.db		HIGH(EVILLEADY2A)
		.db		LOW(EVILLEADY2B)
		.db		HIGH(EVILLEADY2B)
;--------End SONG2--------

;--------Begin SONG3--------
SONG3:
		.db		$3f
		.db		LOW(OVERLEAD)
		.db		HIGH(OVERLEAD)
		.db		LOW(OVERHARP)
		.db		HIGH(OVERHARP)
		.db		LOW(OVERBASS)
		.db		HIGH(OVERBASS)
		.db		LOW(ECHO1)
		.db		HIGH(ECHO1)
		.db		LOW(ECHO2)
		.db		HIGH(ECHO2)
		.db		LOW(OVERKICKS)
		.db		HIGH(OVERKICKS)
;--------End SONG3--------

;--------Begin SONG4--------
SONG4:
		.db		$3f
		.db		LOW(LEADED)
		.db		HIGH(LEADED)
		.db		LOW(GUITARA)
		.db		HIGH(GUITARA)
		.db		LOW(GUITARB)
		.db		HIGH(GUITARB)
		.db		LOW(BASS)
		.db		HIGH(BASS)
		.db		LOW(NOISE)
		.db		HIGH(NOISE)
		.db		LOW(KICKSS)
		.db		HIGH(KICKSS)
;--------End SONG4--------

;-----------Track Index Table------------
TRACK_IX:
		.db		LOW(SONG1)
		.db		HIGH(SONG1)
		.db		LOW(SONG2)
		.db		HIGH(SONG2)
		.db		LOW(SONG3)
		.db		HIGH(SONG3)
		.db		LOW(SONG4)
		.db		HIGH(SONG4)
;-----------End Track Index Table--------
