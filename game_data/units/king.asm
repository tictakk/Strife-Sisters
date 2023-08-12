King:
  .db 50  ;hp
  .db 21  ;atk
  .db 11  ;def
  .db 16  ;int
  .db 10  ;res
  .db 13  ;spd
  .db 3   ;mov
  .db 120 ;sta
  .db 2   ;rng
  .db 31   ;id
  .db 2   ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 15   ;pow
  .db 1   ;is_cmdr
  .db 23, 24, 23 ;attack positions

  ;unit growth
  .db GROWTH_KING

  ;graphic
  .db bank(_attack2)
  .dw _attack2 & $1fff