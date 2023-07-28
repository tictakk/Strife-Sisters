Monk:
  .db 60  ;hp
  .db 25  ;atk
  .db 13  ;def
  .db 10  ;int
  .db 16  ;res
  .db 22  ;spd
  .db 4   ;mov
  .db 150 ;sta
  .db 1   ;rng
  .db 10   ;id
  .db 32   ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 20   ;pow
  .db 0   ;is_cmdr
  .db 24, 7, 0 ;attack positions

  ;unit growth
  .db GROWTH_SPEED_DPS