Paladin:
  .db 65  ;hp
  .db 25  ;atk
  .db 16  ;def
  .db 20  ;int
  .db 14  ;res
  .db 18  ;spd
  .db 3   ;mov
  .db 160 ;sta
  .db 1   ;rng
  .db 9   ;id
  .db 1   ;a_type
  .db 0   ;art
  .db 5   ;points
  .db 20   ;pow
  .db 0   ;is_cmdr
  .db 24, 7, 0 ;attack positions

  ;unit growth
  .db GROWTH_RESIST_MELEE

  ;graphic
  .db bank(_musketbtl)
  .dw _musketbtl & $1fff