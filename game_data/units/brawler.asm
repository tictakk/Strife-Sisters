Brawler:
  .db 50  ;hp
  .db 21  ;atk
  .db 9  ;def
  .db 5  ;int
  .db 13  ;res
  .db 24  ;spd
  .db 3   ;mov
  .db 120 ;sta
  .db 1   ;rng
  .db 25   ;id
  .db 32   ;a_type
  .db 0   ;art
  .db 3   ;points
  .db 15   ;pow
  .db 0   ;is_cmdr
  .db 23, 23, 5 ;attack positions

  ;unit growth
  .db GROWTH_SPEED_DPS

  ;graphic
  .db bank(_brawlerbtl)
  .dw _brawlerbtl & $1fff