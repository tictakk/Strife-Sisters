Soldier:
  .db 35 ;hp
  .db 24 ;atk
  .db 15 ;def
  .db 6 ;int 
  .db 8 ;res
  .db 12 ;spd
  .db 3  ;mov
  .db 100;sta
  .db 1  ;rng
  .db 1  ;id
  .db 1  ;a_type
  .db 0  ;art
  .db 1  ;points
  .db 10  ;pow
  .db 0  ;is_cmdr
  .db 10, 10, 0 ;attacks

  ;unit growth
  .db GROWTH_DPS

  ;graphic
  .db bank(_attack)
  .dw _attack & $1fff