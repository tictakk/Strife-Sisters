Witch:
  .db 40  ;hp
  .db 19  ;atk
  .db 13  ;def
  .db 25  ;int
  .db 12  ;res
  .db 15  ;spd
  .db 3   ;mov
  .db 150 ;sta
  .db 2   ;rng
  .db 20  ;id
  .db 16  ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 15  ;pow
  .db 0   ;is_cmdr
  .db 10, 6, 3 ;attack positions

  ;unit growth
  .db GROWTH_MAGIC_DPS

  ;graphic
  .db bank(_witchbtl)
  .dw _witchbtl & $1fff