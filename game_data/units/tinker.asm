Tinker:
  .db 35  ;hp
  .db 26  ;atk
  .db 13  ;def
  .db 14  ;int
  .db 13  ;res
  .db 17  ;spd
  .db 3   ;mov
  .db 100 ;sta
  .db 3   ;rng
  .db 32  ;id
  .db 8  ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 15  ;pow
  .db 1   ;is_cmdr
  .db 10, 10, 13 ;attack positions

  ;unit growth
  .db GROWTH_RANGED_DPS

  ;graphic
  .db bank(_musketbtl)
  .dw _musketbtl & $1fff