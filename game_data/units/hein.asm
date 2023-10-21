Hein:
  .db 50  ;hp
  .db 24  ;atk
  .db 14  ;def
  .db 7   ;int
  .db 14  ;res
  .db 19  ;spd
  .db 3   ;mov
  .db 110 ;sta
  .db 1   ;rng
  .db 35  ;id
  .db 4   ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 10  ;pow
  .db 1   ;is_cmdr
  .db 10, 10, 10 ;attack positions

  ;unit growth
  .db GROWTH_SPEED_DPS

  ;graphic
  .db bank(_raiderbtl)
  .dw _raiderbtl & $1fff