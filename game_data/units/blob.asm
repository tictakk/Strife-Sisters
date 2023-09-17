Blob:
  .db 40  ;hp
  .db 20  ;atk
  .db 13  ;def
  .db 11  ;int
  .db 12  ;res
  .db 16  ;spd
  .db 3   ;mov
  .db 110 ;sta
  .db 1   ;rng
  .db 7   ;id
  .db 64   ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 10   ;pow
  .db 0   ;is_cmdr
  .db 10, 10, 0 ;attack positions

  ;unit growth
  .db GROWTH_TANK_MELEE

  ;graphic
  .db bank(_blobbattle)
  .dw _blobbattle & $1fff