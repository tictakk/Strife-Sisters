Dancer:
  .db 30  ;hp
  .db 20  ;atk
  .db 11  ;def
  .db 14  ;int
  .db 10  ;res
  .db 23  ;spd
  .db 4   ;mov
  .db 110 ;sta
  .db 1   ;rng
  .db 23  ;id
  .db 32  ;a_type
  .db 0   ;art
  .db 1   ;points
  .db 15  ;pow
  .db 0   ;is_cmdr
  .db 10, 13, 6 ;attack positions

  ;unit growth
  .db GROWTH_DPS

  ;graphic
  .db bank(_dancer_big)
  .dw _dancer_big & $1fff