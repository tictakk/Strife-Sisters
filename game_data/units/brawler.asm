Brawler:
  .db 50  ;hp
  .db 20  ;atk
  .db 12  ;def
  .db 5   ;int
  .db 16  ;res
  .db 16  ;spd
  .db 3   ;mov
  .db 120 ;sta
  .db 1   ;rng
  .db 25  ;id
  .db 32  ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 15  ;pow
  .db 0   ;is_cmdr
  .db 10, 15, 10 ;attack positions

  ;unit growth
  .db GROWTH_TANK_MELEE

  ;graphic
  .db bank(_monkbtl)
  .dw _monkbtl & $1fff