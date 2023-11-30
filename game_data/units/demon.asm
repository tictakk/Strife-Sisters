Demon:
  .db 55  ;hp
  .db 27  ;atk
  .db 14  ;def
  .db 16   ;int
  .db 9   ;res
  .db 16  ;spd
  .db 3   ;mov
  .db 150 ;sta
  .db 1   ;rng
  .db 5   ;id
  .db 64  ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 15  ;pow
  .db 0   ;is_cmdr
  .db 11, 5, 11 ;attack positions

  ;unit growth
  .db GROWTH_SPEED_DPS

  ;graphic
  .db bank(_demonbtl)
  .dw _demonbtl & $1fff