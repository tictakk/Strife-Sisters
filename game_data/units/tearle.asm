Tearle:
  .db 60  ;hp
  .db 36  ;atk
  .db 10  ;def
  .db 8  ;int
  .db 14  ;res
  .db 23  ;spd
  .db 4   ;mov
  .db 150 ;sta
  .db 1   ;rng
  .db 33  ;id
  .db 4   ;a_type
  .db 0   ;art    
  .db 3   ;points
  .db 15  ;pow
  .db 0   ;is_cmdr
  .db 11, 10, 7 ;attack positions

  ;unit growth
  .db GROWTH_SPEED_DPS

  ;graphic
  .db bank(_berserkerbtl)
  .dw _berserkerbtl & $1fff