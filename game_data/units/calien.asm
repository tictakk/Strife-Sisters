Calien:
  .db 20  ;hp
  .db 17  ;atk
  .db 11  ;def
  .db 11  ;int
  .db 12  ;res
  .db 15  ;spd
  .db 3   ;mov
  .db 110 ;sta
  .db 1   ;rng
  .db 34   ;id
  .db 64  ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 10  ;pow
  .db 1   ;is_cmdr
  .db 10, 10, 10 ;attack positions

  ;unit growth
  .db GROWTH_TANK_MELEE

  ;graphic
  .db bank(_attack)
  .dw _attack & $1fff