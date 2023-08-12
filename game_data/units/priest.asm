Priest:
  .db 40  ;hp
  .db 18  ;atk
  .db 8  ;def
  .db 23  ;int
  .db 14  ;res
  .db 20  ;spd
  .db 3   ;mov
  .db 140 ;sta
  .db 2   ;rng
  .db 18   ;id
  .db 16   ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 20   ;pow
  .db 0   ;is_cmdr
  .db 24, 7, 0 ;attack positions

  ;unit growth
  .db GROWTH_MAGIC_SUPPORT

  ;graphic
  .db bank(_musketbtl)
  .dw _musketbtl & $1fff