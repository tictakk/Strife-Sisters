Knight:
  .db 60  ;hp
  .db 23  ;atk
  .db 15  ;def
  .db 11  ;int
  .db 10  ;res
  .db 13  ;spd
  .db 3   ;mov
  .db 130 ;sta
  .db 1   ;rng
  .db 13   ;id
  .db 1   ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 15   ;pow
  .db 0   ;is_cmdr
  .db 24, 7, 0 ;attack positions

  ;unit growth
  .db GROWTH_DPS

  ;graphic
  .db bank(_knightbtl)
  .dw _knightbtl & $1fff