Lancer:
  .db 45  ;hp
  .db 20  ;atk
  .db 12  ;def
  .db 10  ;int
  .db 14  ;res
  .db 15  ;spd
  .db 3   ;mov
  .db 150 ;sta
  .db 2   ;rng
  .db 12  ;id
  .db 2   ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 15   ;pow
  .db 0   ;is_cmdr
  .db 12, 10, 10 ;attack positions

  ;unit growth
  .db GROWTH_DPS

  ;graphic
  .db bank(_lancerbtl)
  .dw _lancerbtl & $1fff