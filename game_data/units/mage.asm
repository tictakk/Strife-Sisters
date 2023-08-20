Mage:
  .db 28  ;hp
  .db 17  ;atk
  .db 7  ;def
  .db 27  ;int
  .db 10  ;res
  .db 12  ;spd
  .db 3   ;mov
  .db 100 ;sta
  .db 2   ;rng
  .db 4   ;id
  .db 16   ;a_type
  .db 0   ;art
  .db 2   ;points
  .db 10   ;pow
  .db 0   ;is_cmdr
  .db 24, 7, 1 ;attack positions

  ;unit growth
  .db GROWTH_MAGIC_DPS

  ;graphic
  .db bank(_magebtl)
  .dw _magebtl & $1fff