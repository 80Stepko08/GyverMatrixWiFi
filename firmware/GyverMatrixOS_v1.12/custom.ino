// свой список режимов

// ************************ НАСТРОЙКИ ************************
#define SHOW_FULL_TEXT 1    // не переключать режим, пока текст не покажется весь
#define SHOW_TEXT_ONCE 1    // показывать бегущий текст только 1 раз

/*
   Режимы:
    clockRoutine();       // часы на чёрном фоне

   Эффекты:
    sparklesRoutine();    // случайные цветные гаснущие вспышки
    snowRoutine();        // снег
    matrixRoutine();      // "матрица"
    starfallRoutine();    // звездопад (кометы)
    ballRoutine();        // квадратик
    ballsRoutine();       // шарики
    rainbowRoutine();     // радуга во всю матрицу горизонтальная
    rainbowDiagonalRoutine();   // радуга во всю матрицу диагональная
    fireRoutine();        // огонь

  Крутые эффекты "шума":
    madnessNoise();       // цветной шум во всю матрицу
    cloudNoise();         // облака
    lavaNoise();          // лава
    plasmaNoise();        // плазма
    rainbowNoise();       // радужные переливы
    rainbowStripeNoise(); // полосатые радужные переливы
    zebraNoise();         // зебра
    forestNoise();        // шумящий лес
    oceanNoise();         // морская вода

  Игры:
    snakeRoutine();     // змейка
    tetrisRoutine();    // тетрис
    mazeRoutine();      // лабиринт
    runnerRoutine();    // бегалка прыгалка
    flappyRoutine();    // flappy bird
    arkanoidRoutine();  // арканоид

  Бегущая строка:
    fillString("Ваш текст", цвет);    // цвет вида 0x00ff25 или CRGB::Red и проч. цвета
    fillString("Ваш текст", 1);       // радужный перелив текста
    fillString("Ваш текст", 2);       // каждая буква случайным цветом!

  Рисунки и анимации:
    loadImage(<название массива>);    // основная функция вывода картинки
    imageRoutine();                   // пример использования
    animation();                      // пример анимации
*/

// ************************* СВОЙ СПИСОК РЕЖИМОВ ************************
// список можно менять, соблюдая его структуру. Можно удалять и добавлять эффекты, ставить их в
// любой последовательности или вообще оставить ОДИН. Удалив остальные case и break. Cтруктура оч простая:
// case <номер>: <эффект>;
//  break;

void customModes() {
  switch (thisMode) {    
    case DEMO_TEXT_0: 
      text = runningText == "" ? "Gyver Matrix" : runningText;
      fillString(text, CRGB::RoyalBlue);
      break;
    case DEMO_TEXT_1: 
      text = runningText == "" ? "РАДУГА" : runningText;
      fillString(text, 1);
      break;
    case DEMO_TEXT_2: 
      text = runningText == "" ? "RGB LED" : runningText;
      fillString(text, 2);
      break;
    case DEMO_NOISE_MADNESS:       madnessNoise(); break;
    case DEMO_NOISE_CLOUD:         cloudNoise(); break;
    case DEMO_NOISE_LAVA:          lavaNoise(); break;
    case DEMO_NOISE_PLASMA:        plasmaNoise(); break;
    case DEMO_NOISE_RAINBOW:       rainbowNoise(); break;
    case DEMO_NOISE_RAINBOW_STRIP: rainbowStripeNoise(); break;
    case DEMO_NOISE_ZEBRA:         zebraNoise(); break;
    case DEMO_NOISE_FOREST:        forestNoise(); break;
    case DEMO_NOISE_OCEAN:         oceanNoise(); break;
    case DEMO_SNOW:                snowRoutine(); break;
    case DEMO_SPARKLES:            sparklesRoutine(); break;
    case DEMO_MATRIX:              matrixRoutine(); break;
    case DEMO_STARFALL:            starfallRoutine(); break;
    case DEMO_BALL:                ballRoutine(); break;
    case DEMO_BALLS:               ballsRoutine(); break;
    case DEMO_RAINBOW:             rainbowRoutine(); break;
    case DEMO_RAINBOW_DIAG:        rainbowDiagonalRoutine(); break;
    case DEMO_FIRE:                fireRoutine(); break;
    case DEMO_SNAKE:               snakeRoutine(); break;
    case DEMO_TETRIS:              tetrisRoutine(); break;
    case DEMO_MAZE:                mazeRoutine(); break;
    case DEMO_RUNNER:              runnerRoutine(); break;
    case DEMO_FLAPPY:              flappyRoutine(); break;
    case DEMO_ARKANOID:            arkanoidRoutine(); break;
    case DEMO_CLOCK:               clockRoutine(); break;
    case DEMO_ANIMATION:           animation(); break;
  }
}

// ********************* ОСНОВНОЙ ЦИКЛ РЕЖИМОВ *******************

static void nextMode() {
#if (SMOOTH_CHANGE == 1)
  fadeMode = 0;
  modeDir = true;
#else
  nextModeHandler();
#endif
}

static void prevMode() {
#if (SMOOTH_CHANGE == 1)
  fadeMode = 0;
  modeDir = false;
#else
  prevModeHandler();
#endif
}

void nextModeHandler() {
  byte aCnt = 0;
  byte curMode = thisMode;

  while (aCnt < MODES_AMOUNT) {
    // Берем следующий режим по циклу режимов
    aCnt++; thisMode++;  
    if (thisMode >= MODES_AMOUNT) thisMode = 0;

    // Если новый режим отмечен флагом "использовать" - используем его, иначе берем следующий (и проверяем его)
    if (getUsageForMode(thisMode)) break;
    
    // Если перебрали все и ни у адного нет флага "использовать" - не обращаем внимание на флаг, используем следующий
    if (aCnt >= MODES_AMOUNT) {
      thisMode = curMode++;
      if (thisMode >= MODES_AMOUNT) thisMode = 0;
      break;
    }    
  }
  
  loadingFlag = true;
  gamemodeFlag = false;
  autoplayTimer = millis();
  setTimersForMode(thisMode);
  FastLED.clear();
  FastLED.show();
}

void prevModeHandler() {
  byte aCnt = 0;
  byte curMode = thisMode;

  while (aCnt < MODES_AMOUNT) {
    // Берем предыдущий режим по циклу режимов
    aCnt++; thisMode--;  
    if (thisMode < 0) thisMode = MODES_AMOUNT - 1;

    // Если новый режим отмечен флагом "использовать" - используем его, иначе берем следующий (и проверяем его)
    if (getUsageForMode(thisMode)) break;
    
    // Если перебрали все и ни у адного нет флага "использовать" - не обращаем внимание на флаг, используем следующий
    if (aCnt >= MODES_AMOUNT) {
      thisMode = curMode--;
      if (thisMode < 0) thisMode = MODES_AMOUNT - 1;
      break;
    }    
  }
  
  loadingFlag = true;
  gamemodeFlag = false;
  autoplayTimer = millis();
  setTimersForMode(thisMode);
  FastLED.clear();
  FastLED.show();
}

void setTimersForMode(byte aMode) {
  if (aMode == DEMO_TEXT_0 || aMode == DEMO_TEXT_1 || aMode == DEMO_TEXT_2) {
    // Это бегущий текст  
    scrollSpeed = getScrollSpeed();
    scrollTimer.setInterval(scrollSpeed);
  } else {
    byte tmp_effect = mapModeToEffect(aMode);
    if (tmp_effect != 255) {
      effectSpeed = getEffectSpeed(tmp_effect);
      effectTimer.setInterval(effectSpeed);
    } else {
      byte tmp_game = mapModeToGame(aMode);
      if (tmp_effect != 255) {
        gameSpeed = DEMO_GAME_SPEED;
        gameTimer.setInterval(gameSpeed);
      }
    }
  }
}

boolean getUsageForMode(byte aMode) {
  if (aMode == DEMO_TEXT_0 || aMode == DEMO_TEXT_1 || aMode == DEMO_TEXT_2) {
    // Это бегущий текст  
    return getUseTextInDemo();
  } else {
    
    byte tmp_effect = mapModeToEffect(aMode);
    if (tmp_effect != 255) {
      return getEffectUsage(tmp_effect);
    }

    byte tmp_game = mapModeToGame(aMode);
    if (tmp_game != 255) {
      return getGameUsage(tmp_game);
    }

    return true;
  }
}

int fadeBrightness;
int fadeStepCount = 10;     // За сколько шагов убирать/добавлять яркость при смене режимов
int fadeStepValue = 5;      // Шаг убавления яркости

#if (SMOOTH_CHANGE == 1)
void modeFader() {
  if (fadeMode == 0) {
    fadeMode = 1;
    fadeStepValue = fadeBrightness / fadeStepCount;
    if (fadeStepValue < 1) fadeStepValue = 1;
  } else if (fadeMode == 1) {
    if (changeTimer.isReady()) {
      fadeBrightness -= fadeStepValue;
      if (fadeBrightness < 0) {
        fadeBrightness = 0;
        fadeMode = 2;
      }
      FastLED.setBrightness(fadeBrightness);
    }
  } else if (fadeMode == 2) {
    fadeMode = 3;
    if (modeDir) nextModeHandler();
    else prevModeHandler();
  } else if (fadeMode == 3) {
    if (changeTimer.isReady()) {
      fadeBrightness += fadeStepValue;
      if (fadeBrightness > globalBrightness) {
        fadeBrightness = globalBrightness;
        fadeMode = 4;
      }
      FastLED.setBrightness(fadeBrightness);
    }
  }
}
#endif

void customRoutine() {
   
  if (!gamemodeFlag) {

    // Беугщая строка - таймер внутри fillString (runningText.ino)
    if (thisMode == DEMO_TEXT_0 || thisMode == DEMO_TEXT_1 || thisMode == DEMO_TEXT_2) {
      customModes();
      FastLED.show();
    } 

    // Эффекты - возможно наложение часов
    else {
      
      if (effectTimer.isReady()) {
        
#if (OVERLAY_CLOCK == 1 && USE_CLOCK == 1)
        boolean loadFlag2;
        boolean needOverlay = modeCode != MC_TEXT && overlayAllowed();
        if (needOverlay) {
          if (!loadingFlag && needUnwrap()) {
            if (CLOCK_ORIENT == 0)
              clockOverlayUnwrapH(CLOCK_X, CLOCK_Y);
            else
              clockOverlayUnwrapV(CLOCK_X, CLOCK_Y);
          }
          if (loadingFlag) loadFlag2 = true;
        }
#endif

        customModes();                // режимы крутятся, пиксели мутятся

#if (OVERLAY_CLOCK == 1 && USE_CLOCK == 1)
        if (needOverlay) {
          if (CLOCK_ORIENT == 0)
            clockOverlayWrapH(CLOCK_X, CLOCK_Y);
          else  
            clockOverlayWrapV(CLOCK_X, CLOCK_Y);
          if (loadFlag2) {
            setOverlayColors();
            loadFlag2 = false;
          }
        }
        loadingFlag = false;
#endif
        FastLED.show();
      }
    }
  } 

  // Игры - таймер внутри игр
  else {
    customModes();
  }  
}

void checkIdleState() {

#if (SMOOTH_CHANGE == 1)
  modeFader();
#endif
  
  if (idleState) {
    if (fullTextFlag && SHOW_TEXT_ONCE) {
      fullTextFlag = false;
      autoplayTimer = millis();
      if (AUTOPLAY) nextMode();
    }
    
    if (millis() - autoplayTimer > autoplayTime && AUTOPLAY) {    // таймер смены режима
      if (modeCode == MC_TEXT && SHOW_FULL_TEXT) {    // режим текста
        if (fullTextFlag) {
          fullTextFlag = false;
          autoplayTimer = millis();
          nextMode();
        }
      } else {
        autoplayTimer = millis();
        nextMode();
      }
    }
  } else {
    if (idleTimer.isReady() && modeCode != MC_CLOCK) {      // таймер холостого режима. 
      idleState = true;                                     // Если находимся в режиме часов - автоматически из Idle в демо-режим не переходить 
      autoplayTimer = millis();
      gameDemo = true;

      gameSpeed = DEMO_GAME_SPEED;
      gameTimer.setInterval(gameSpeed);

      BTcontrol = false;
      loadingFlag = true;
      runningFlag = false;
      controlFlag = false;                      // После начала игры пока не трогаем кнопки - игра автоматическая 
      drawingFlag = false;
      gamemodeFlag = false;
      gamePaused = false;

      AUTOPLAY = true;
      
      FastLED.clear();
      FastLED.show();
    }
  }  
}

bool isColorEffect(byte effect) {
  // Цветовые эффекты - Дыхание, Цвет или Радуга пикс.
  // Они могут работать с custom демо режимами
  return effect == EFFECT_BREATH || effect == EFFECT_COLOR || effect == EFFECT_RAINBOW_PIX;
}

byte mapEffectToMode(byte effect) {
  byte tmp_mode = 255;
  
  switch (effect) {
    case EFFECT_SNOW:                tmp_mode = DEMO_SNOW; break;                 // snowRoutine();
    case EFFECT_BALL:                tmp_mode = DEMO_BALL; break;                 // ballRoutine();
    case EFFECT_RAINBOW:             tmp_mode = DEMO_RAINBOW; break;              // rainbowRoutine();
    case EFFECT_FIRE:                tmp_mode = DEMO_FIRE; break;                 // fireRoutine()
    case EFFECT_MATRIX:              tmp_mode = DEMO_MATRIX; break;               // matrixRoutine();
    case EFFECT_BALLS:               tmp_mode = DEMO_BALLS; break;                // ballsRoutine();
    case EFFECT_CLOCK:               tmp_mode = DEMO_CLOCK; break;                // clockRoutine();
    case EFFECT_STARFALL:            tmp_mode = DEMO_STARFALL; break;             // starfallRoutine()
    case EFFECT_SPARKLES:            tmp_mode = DEMO_SPARKLES; break;             // sparklesRoutine()
    case EFFECT_RAINBOW_DIAG:        tmp_mode = DEMO_RAINBOW_DIAG; break;         // rainbowDiagonalRoutine();
    case EFFECT_NOISE_MADNESS:       tmp_mode = DEMO_NOISE_MADNESS;  break;       // madnessNoise();
    case EFFECT_NOISE_CLOUD:         tmp_mode = DEMO_NOISE_CLOUD;  break;         // cloudNoise();
    case EFFECT_NOISE_LAVA:          tmp_mode = DEMO_NOISE_LAVA;  break;          // lavaNoise();
    case EFFECT_NOISE_PLASMA:        tmp_mode = DEMO_NOISE_PLASMA;  break;        // plasmaNoise();
    case EFFECT_NOISE_RAINBOW:       tmp_mode = DEMO_NOISE_RAINBOW;  break;       // rainbowNoise();
    case EFFECT_NOISE_RAINBOW_STRIP: tmp_mode = DEMO_NOISE_RAINBOW_STRIP;  break; // rainbowStripeNoise();
    case EFFECT_NOISE_ZEBRA:         tmp_mode = DEMO_NOISE_ZEBRA;  break;         // zebraNoise();
    case EFFECT_NOISE_FOREST:        tmp_mode = DEMO_NOISE_FOREST; break;         // forestNoise();
    case EFFECT_NOISE_OCEAN:         tmp_mode = DEMO_NOISE_OCEAN; break;          // oceanNoise();
    case EFFECT_ANIMATION:           tmp_mode = DEMO_ANIMATION; break;            // animation();

    // Нет соответствия - выполняются для текущего режима thisMode
    case EFFECT_BREATH:              // Дыхание
    case EFFECT_COLOR:               // Цвет
    case EFFECT_RAINBOW_PIX:         // Радуга пикс
      break;
  }

  return tmp_mode;
}

byte mapGameToMode(byte game) {
  byte tmp_mode = 255;
  
  switch (game) {
    case GAME_SNAKE:    tmp_mode = DEMO_SNAKE;    break;  // snakeRoutine(); 
    case GAME_TETRIS:   tmp_mode = DEMO_TETRIS;   break;  // tetrisRoutine();
    case GAME_MAZE:     tmp_mode = DEMO_MAZE;     break;  // mazeRoutine();
    case GAME_RUNNER:   tmp_mode = DEMO_RUNNER;   break;  // runnerRoutine();
    case GAME_FLAPPY:   tmp_mode = DEMO_FLAPPY;   break;  // flappyRoutine();
    case GAME_ARKANOID: tmp_mode = DEMO_ARKANOID; break;  // arkanoidRoutine();
  }

  return tmp_mode;
}

byte mapModeToEffect(byte aMode) {
  byte tmp_effect = 255;
  // Если режима нет в списке - ему нет соответствия среди эффектов - значит это игра или бегущий текст
  switch (aMode) {
    case DEMO_NOISE_MADNESS:        tmp_effect = EFFECT_NOISE_MADNESS; break;       // madnessNoise();
    case DEMO_NOISE_CLOUD:          tmp_effect = EFFECT_NOISE_CLOUD; break;         // cloudNoise();
    case DEMO_NOISE_LAVA:           tmp_effect = EFFECT_NOISE_LAVA; break;          // lavaNoise();
    case DEMO_NOISE_PLASMA:         tmp_effect = EFFECT_NOISE_PLASMA; break;        // plasmaNoise();
    case DEMO_NOISE_RAINBOW:        tmp_effect = EFFECT_NOISE_RAINBOW; break;       // rainbowNoise();
    case DEMO_NOISE_RAINBOW_STRIP:  tmp_effect = EFFECT_NOISE_RAINBOW_STRIP; break; // rainbowStripeNoise();
    case DEMO_NOISE_ZEBRA:          tmp_effect = EFFECT_NOISE_ZEBRA; break;         // zebraNoise();
    case DEMO_NOISE_FOREST:         tmp_effect = EFFECT_NOISE_FOREST; break;        // forestNoise();
    case DEMO_NOISE_OCEAN:          tmp_effect = EFFECT_NOISE_OCEAN; break;         // oceanNoise();
    case DEMO_SNOW:                 tmp_effect = EFFECT_SNOW;  break;               // snowRoutine();
    case DEMO_SPARKLES:             tmp_effect = EFFECT_SPARKLES; break;            // sparklesRoutine()
    case DEMO_MATRIX:               tmp_effect = EFFECT_MATRIX;  break;             // matrixRoutine();
    case DEMO_STARFALL:             tmp_effect = EFFECT_STARFALL; break;            // starfallRoutine()
    case DEMO_BALL:                 tmp_effect = EFFECT_BALL;  break;               // ballRoutine();
    case DEMO_BALLS:                tmp_effect = EFFECT_BALLS;  break;              // ballsRoutine();
    case DEMO_RAINBOW:              tmp_effect = EFFECT_RAINBOW;  break;            // rainbowRoutine();
    case DEMO_RAINBOW_DIAG:         tmp_effect = EFFECT_RAINBOW_DIAG; break;        // rainbowDiagonalRoutine();
    case DEMO_FIRE:                 tmp_effect = EFFECT_FIRE;  break;               // fireRoutine()
    case DEMO_ANIMATION:            tmp_effect = EFFECT_ANIMATION; break;           // animation();

    case DEMO_TEXT_0 :  break;      // Бегущий текст
    case DEMO_TEXT_1 :  break;      // Бегущий текст
    case DEMO_TEXT_2 :  break;      // Бегущий текст

    case DEMO_SNAKE: break;         // snakeRoutine(); 
    case DEMO_TETRIS: break;        // tetrisRoutine();
    case DEMO_MAZE: break;          // mazeRoutine();
    case DEMO_RUNNER: break;        // runnerRoutine();
    case DEMO_FLAPPY: break;        // flappyRoutine();
    case DEMO_ARKANOID: break;      // arkanoidRoutine();
    
    case DEMO_CLOCK: break;         // clockRoutine();     
  }
  return tmp_effect;
}

byte mapModeToGame(byte aMode) {
  byte tmp_game = 255;
  // Если режима нет в списке - ему нет соответствия среди тгр - значит это эффект или бегущий текст
  switch (aMode) {
    case DEMO_NOISE_MADNESS:        break;       // madnessNoise();
    case DEMO_NOISE_CLOUD:          break;       // cloudNoise();
    case DEMO_NOISE_LAVA:           break;       // lavaNoise();
    case DEMO_NOISE_PLASMA:         break;       // plasmaNoise();
    case DEMO_NOISE_RAINBOW:        break;       // rainbowNoise();
    case DEMO_NOISE_RAINBOW_STRIP:  break;       // rainbowStripeNoise();
    case DEMO_NOISE_ZEBRA:          break;       // zebraNoise();
    case DEMO_NOISE_FOREST:         break;       // forestNoise();
    case DEMO_NOISE_OCEAN:          break;       // oceanNoise();
    case DEMO_SNOW:                 break;       // snowRoutine();
    case DEMO_SPARKLES:             break;       // sparklesRoutine()
    case DEMO_MATRIX:               break;       // matrixRoutine();
    case DEMO_STARFALL:             break;       // starfallRoutine()
    case DEMO_BALL:                 break;       // ballRoutine();
    case DEMO_BALLS:                break;       // ballsRoutine();
    case DEMO_RAINBOW:              break;       // rainbowRoutine();
    case DEMO_RAINBOW_DIAG:         break;       // rainbowDiagonalRoutine();
    case DEMO_FIRE:                 break;       // fireRoutine()
    case DEMO_ANIMATION:            break;       // animation();

    case DEMO_TEXT_0:               break;       // Бегущий текст
    case DEMO_TEXT_1:               break;       // Бегущий текст
    case DEMO_TEXT_2:               break;       // Бегущий текст

    case DEMO_SNAKE:    tmp_game = GAME_SNAKE;    break;     // snakeRoutine(); 
    case DEMO_TETRIS:   tmp_game = GAME_TETRIS;   break;     // tetrisRoutine();
    case DEMO_MAZE:     tmp_game = GAME_MAZE;     break;     // mazeRoutine();
    case DEMO_RUNNER:   tmp_game = GAME_RUNNER;   break;     // runnerRoutine();
    case DEMO_FLAPPY:   tmp_game = GAME_FLAPPY;   break;     // flappyRoutine();
    case DEMO_ARKANOID: tmp_game = GAME_ARKANOID; break;     // arkanoidRoutine();

    case DEMO_CLOCK:                break;  // clockRoutine();     
  }
  return tmp_game;
}