Raider:
  .db 40  ;hp
  .db 20  ;atk
  .db 12  ;def
  .db 9  ;int
  .db 12  ;res
  .db 16  ;spd
  .db 3   ;mov
  .db 110 ;sta
  .db 1   ;rng
  .db 21   ;id
  .db 4   ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 10   ;pow
  .db 0   ;is_cmdr
  .db 24, 7, 0 ;attack positions

  ;unit growth
  .db GROWTH_SPEED_DPS

  ;graphic
  .db bank(_musketbtl)
  .dw _musketbtl & $1fff