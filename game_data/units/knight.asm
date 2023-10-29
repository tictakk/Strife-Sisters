Knight:
  .db 50  ;hp
  .db 24  ;atk
  .db 16  ;def
  .db 11  ;int
  .db 10  ;res
  .db 9  ;spd
  .db 3   ;mov
  .db 130 ;sta
  .db 1   ;rng
  .db 13   ;id
  .db 1   ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 15   ;pow
  .db 0   ;is_cmdr
  .db 10, 11, 10 ;attack positions

  ;unit growth
  .db GROWTH_TANK_MELEE

  ;graphic
  .db bank(_knightbtl)
  .dw _knightbtl & $1fff