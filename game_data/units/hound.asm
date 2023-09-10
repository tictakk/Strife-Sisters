Hellhound:
  .db 40  ;hp
  .db 28  ;atk
  .db 10  ;def
  .db 7  ;int
  .db 10  ;res
  .db 20  ;spd
  .db 4   ;mov
  .db 120 ;sta
  .db 1   ;rng
  .db 6   ;id
  .db 64   ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 13   ;pow
  .db 0   ;is_cmdr
  .db 23, 23, 4 ;melee, melee, atk up 

  ;unit growth
  .db GROWTH_SPEED_DPS

  ;graphic
  .db bank(_houndbtl)
  .dw _houndbtl & $1fff