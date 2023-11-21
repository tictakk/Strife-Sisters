Cleric:
  .db 35  ;hp
  .db 15  ;atk
  .db 10  ;def
  .db 24  ;int
  .db 9  ;res
  .db 14  ;spd
  .db 3   ;mov
  .db 100 ;sta
  .db 1   ;rng
  .db 17   ;id
  .db 16   ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 10   ;pow
  .db 0   ;is_cmdr
  .db 10, 6, 14 ;attack positions

  ;unit growth
  .db GROWTH_MAGIC_SUPPORT

  ;graphic
  .db bank(_magebtl)
  .dw _magebtl & $1fff