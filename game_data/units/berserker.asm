Berserker:
  .db 50  ;hp
  .db 30  ;atk
  .db 12  ;def
  .db 5   ;int
  .db 11  ;res
  .db 22  ;spd
  .db 3   ;mov
  .db 160 ;sta
  .db 1   ;rng
  .db 14  ;id
  .db 4   ;a_type
  .db 0   ;art
  .db 4   ;points
  .db 20  ;pow
  .db 0   ;is_cmdr
  .db 10, 10, 10 ;attack positions

  ;unit growth
  .db GROWTH_DPS

  ;graphic
  .db bank(_berserkerbtl)
  .dw _berserkerbtl & $1fff