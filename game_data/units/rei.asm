Rei:
  .db 40  ;hp
  .db 26  ;atk
  .db 18  ;def
  .db 13  ;int
  .db 12  ;res
  .db 11  ;spd
  .db 3   ;mov
  .db 120 ;sta
  .db 1   ;rng
  .db 29   ;id
  .db 1   ;a_type
  .db 0   ;art
  .db 1   ;points
  .db 15   ;pow
  .db 1   ;is_cmdr
  .db 10, 10, 5 ;attack positions

  ;unit growth
  .db GROWTH_REI

  ;graphic
  .db bank(_attack)
  .dw _attack & $1fff