Violet:
  .db 35  ;hp
  .db 21  ;atk
  .db 9  ;def
  .db 30  ;int
  .db 12  ;res
  .db 13  ;spd
  .db 3   ;mov
  .db 100 ;sta
  .db 2   ;rng
  .db 30   ;id
  .db 16   ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 15   ;pow
  .db 1   ;is_cmdr
  .db 10, 1, 7  ;attack positions

  ;unit growth
  .db GROWTH_VIOLET

  ;graphic
  .db bank(_witchbtl)
  .dw _witchbtl & $1fff