Soldier:
  .db 45 ;hp
  .db 19 ;atk
  .db 14 ;def
  .db 6  ;int
  .db 9  ;res
  .db 13 ;spd
  .db 3  ;mov
  .db 100;sta
  .db 1  ;rng
  .db 1  ;id
  .db 1  ;a_type
  .db 0  ;art
  .db 1  ;points
  .db 10 ;pow
  .db 0  ;is_cmdr
  .db 10, 17, 0 ;attacks

  ;unit growth
  .db GROWTH_TANK_MELEE

  ;graphic
  .db bank(_attack)
  .dw _attack & $1fff