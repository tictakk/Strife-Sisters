Archer:
  .db 25  ;hp
  .db 30  ;atk
  .db 7  ;def
  .db 12  ;int
  .db 9  ;res
  .db 17  ;spd
  .db 3   ;mov
  .db 100 ;sta
  .db 3   ;rng
  .db 3   ;id
  .db 8   ;a_type
  .db 0   ;art
  .db 1   ;points
  .db 10   ;pow
  .db 0   ;is_cmdr
  .db 23, 23, 24 ;attack positions

  ;unit growth
  .db GROWTH_RANGED_DPS

  ;graphic
  .db bank(_musketbtl)
  .dw _musketbtl & $1fff