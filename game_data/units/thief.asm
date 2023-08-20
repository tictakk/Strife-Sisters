Thief:
  .db 40  ;hp
  .db 25  ;atk
  .db 13  ;def
  .db 12  ;int
  .db 12  ;res
  .db 17  ;spd
  .db 3   ;mov
  .db 150 ;sta
  .db 2   ;rng
  .db 22  ;id
  .db 8   ;a_type
  .db 0   ;art    
  .db 3   ;points
  .db 15  ;pow
  .db 0   ;is_cmdr
  .db 23, 20, 26 ;attack positions

  ;unit growth
  .db GROWTH_SPEED_DPS

  ;graphic
  .db bank(_thiefbtl)
  .dw _thiefbtl & $1fff