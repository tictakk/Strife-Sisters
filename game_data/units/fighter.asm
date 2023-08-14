Fighter:
  .db 55  ;hp
  .db 25  ;atk
  .db 11  ;def
  .db 10  ;int
  .db 10  ;res
  .db 14  ;spd
  .db 3   ;mov
  .db 150 ;sta
  .db 2   ;rng
  .db 24  ;id
  .db 32   ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 15   ;pow
  .db 0   ;is_cmdr
  .db 24, 7, 0 ;attack positions

  ;unit growth
  .db GROWTH_DPS
  
  ;graphic
  .db bank(_fighterbtl)
  .dw _fighterbtl & $1fff