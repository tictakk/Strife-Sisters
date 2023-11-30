Zaladin:
  .db 50  ;hp
  .db 24  ;atk
  .db 20  ;def
  .db 17   ;int
  .db 19  ;res
  .db 26  ;spd
  .db 3   ;mov
  .db 110 ;sta
  .db 1   ;rng
  .db 37  ;id
  .db 2   ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 10  ;pow
  .db 1   ;is_cmdr
  .db 10, 10, 10 ;attack positions

  ;unit growth
  .db 11

  ;graphic
  .db bank(_knightbtl)
  .dw _knightbtl & $1fff