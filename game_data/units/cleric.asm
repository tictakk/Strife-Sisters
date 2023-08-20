Cleric:
  .db 30  ;hp
  .db 16  ;atk
  .db 12  ;def
  .db 24  ;int
  .db 9  ;res
  .db 16  ;spd
  .db 3   ;mov
  .db 100 ;sta
  .db 2   ;rng
  .db 17   ;id
  .db 16   ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 10   ;pow
  .db 0   ;is_cmdr
  .db 23, 10, 11 ;attack positions

  ;unit growth
  .db GROWTH_MAGIC_SUPPORT

  ;graphic
  .db bank(_magebtl)
  .dw _magebtl & $1fff