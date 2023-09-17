Tinker:
  .db 30  ;hp
  .db 24  ;atk
  .db 12  ;def
  .db 25  ;int
  .db 12 ;res
  .db 12  ;spd
  .db 3   ;mov
  .db 100 ;sta
  .db 1   ;rng
  .db 32   ;id
  .db 32   ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 15   ;pow
  .db 1   ;is_cmdr
  .db 10, 13, 4 ;attack positions

  ;unit growth
  .db GROWTH_VIOLET

  ;graphic
  .db bank(_monkbtl)
  .dw _monkbtl & $1fff