Stalker:
  .db 40  ;hp
  .db 25  ;atk
  .db 12  ;def
  .db 16  ;int
  .db 17  ;res
  .db 12  ;spd
  .db 3   ;mov
  .db 130 ;sta
  .db 3   ;rng
  .db 15   ;id
  .db 8   ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 15   ;pow
  .db 0   ;is_cmdr
  .db 0, 9, 2 ;attack positions

  ;unit growth
  .db GROWTH_RANGED_DPS

  ;graphic
  .db bank(_stalkerbtl)
  .dw _stalkerbtl & $1fff