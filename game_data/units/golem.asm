Golem:
  .db 80  ;hp
  .db 20  ;atk
  .db 15  ;def
  .db 5  ;int
  .db 16  ;res
  .db 13  ;spd
  .db 3   ;mov
  .db 120 ;sta
  .db 1   ;rng
  .db 26   ;id
  .db 64   ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 15   ;pow
  .db 0   ;is_cmdr
  .db 10, 11, 10 ;attack positions

  ;unit growth
  .db GROWTH_TANK_MELEE

  ;graphic
  .db bank(_golembtl)
  .dw _golembtl & $1fff