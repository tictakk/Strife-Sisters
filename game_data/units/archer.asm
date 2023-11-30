Archer:
  .db 35  ;hp
  .db 23  ;atk
  .db 10  ;def
  .db 10  ;int
  .db 9   ;res
  .db 14  ;spd
  .db 3   ;mov
  .db 100 ;sta
  .db 3   ;rng
  .db 3   ;id
  .db 8   ;a_type
  .db 0   ;art
  .db 1   ;points
  .db 10   ;pow
  .db 0   ;is_cmdr
  .db 0, 7, 8 ;attack positions

  ;unit growth
  .db GROWTH_RANGED_DPS

  ;graphic
  .db bank(_musketbtl)
  .dw _musketbtl & $1fff