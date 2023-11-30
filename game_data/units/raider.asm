Raider:
  .db 40   ;hp
  .db 21   ;atk
  .db 13   ;def
  .db 9    ;int
  .db 13   ;res
  .db 20   ;spd
  .db 4    ;mov
  .db 110  ;sta
  .db 1    ;rng
  .db 21   ;id
  .db 4    ;a_type
  .db 0    ;art
  .db 3    ;points
  .db 10   ;pow
  .db 0    ;is_cmdr
  .db 10, 4, 7 ;attack positions

  ;unit growth
  .db GROWTH_SPEED_DPS

  ;graphic
  .db bank(_raiderbtl)
  .dw _raiderbtl & $1fff