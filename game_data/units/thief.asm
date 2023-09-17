Thief:
  .db 40  ;hp
  .db 24  ;atk
  .db 10  ;def
  .db 8   ;int
  .db 11  ;res
  .db 18  ;spd
  .db 4   ;mov
  .db 150 ;sta
  .db 1   ;rng
  .db 22  ;id
  .db 8   ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 15  ;pow
  .db 0   ;is_cmdr
  .db 10, 7, 13 ;attack positions

  ;unit growth
  .db GROWTH_SPEED_DPS

  ;graphic
  .db bank(_thiefbtl)
  .dw _thiefbtl & $1fff