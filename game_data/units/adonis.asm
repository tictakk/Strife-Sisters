Rannon:
  .db 55  ;hp
  .db 25  ;atk
  .db 11  ;def
  .db 10  ;int
  .db 10  ;res
  .db 14  ;spd
  .db 3   ;mov
  .db 150 ;sta
  .db 2   ;rng
  .db 36  ;id
  .db 2   ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 15   ;pow
  .db 1   ;is_cmdr
  .db 11, 5, 0 ;attack positions

  ;unit growth
  .db GROWTH_DPS

  ;graphic
  .db bank(_lancerbtl)
  .dw _lancerbtl & $1fff