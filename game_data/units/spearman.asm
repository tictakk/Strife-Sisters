Spearman:
  .db 40  ;hp
  .db 22  ;atk
  .db 11  ;def 
  .db 10  ;int
  .db 11  ;res
  .db 15  ;spd
  .db 3   ;mov
  .db 110 ;sta
  .db 2   ;rng
  .db 2   ;id
  .db 2   ;a_type
  .db 0   ;art
  .db 1   ;points
  .db 5   ;pow
  .db 0   ;is_cmdr
  .db 10, 12, 10 ;attacks

  ;unit growth
  .db GROWTH_DPS

  ;graphic
  .db bank(_attack2)
  .dw _attack2 & $1fff