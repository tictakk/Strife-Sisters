Nounit:
  .db 0  ;hp
  .db 1  ;atk
  .db 1  ;def
  .db 1  ;int
  .db 1  ;res
  .db 1  ;spd
  .db 1  ;mov
  .db 1  ;sta
  .db 1  ;rng
  .db 0  ;id
  .db 0  ;a_type
  .db 0  ;art
  .db 0  ;points
  .db 0  ;pow
  .db 0  ;is_cmdr
  .db 0, 0, 0 ;attack positions

  ;unit growth
  .db GROWTH_DPS

  ;graphic
  .db bank(_musketbtl)
  .dw _musketbtl & $1fff