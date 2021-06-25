#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;
#include <Wire.h>

enum gameMode
{
  gameMenu,
  game,
  gameOver
};
gameMode state = gameMenu;
//player
int player_score = 0;
int player_h = 16;
int player_w = 3;
int player_x = 0;
int player_y = (LCDHEIGHT-player_h)/2;
int player_vy = 2;
//opponent
int opponent_score = 0;
int opponent_h = 16;
int opponent_w = 3;
int opponent_x = LCDWIDTH - opponent_w;
int opponent_y = (LCDHEIGHT-opponent_h)/2;
int opponent_vy = 2;
//ball
int ball_size = 6;
int ball_x = LCDWIDTH - ball_size - opponent_w -1;
int ball_y = (LCDHEIGHT - ball_size)/2;
int ball_vx = 3;
int ball_vy = 3;
int ball_score = 0;

const char strOne[] PROGMEM = "1P VS AI";
const char strTwo[] PROGMEM = "1V1";
const char strAi[] PROGMEM = "AI VS AI";
const char strJuggle[] PROGMEM = "JUGGLE";
const char* const multiPlayerMenu[4] PROGMEM = {
  strOne,
  strTwo, 
  strJuggle, 
  strAi  
};

boolean playerOne = true;
boolean playerTwo = false;
boolean juggle = false;
boolean ai = false;
void setup()
{
  gb.begin();
  initPrograme();
  gb.pickRandomSeed();
  gb.battery.show = false;
  gb.display.fontSize = 2;
}

void initPrograme()
{
  goTitleScreen();
  state = gameMenu;
  gb.battery.show = false;
}

void goTitleScreen()
{
  gb.titleScreen(F("Coding Pong"));
}

void loop()
{
    if(gb.update())
    { 
      
      gb.battery.show = false;
      if(gb.buttons.pressed(BTN_C))
      {
        state = gameMenu;
        opponent_x = LCDWIDTH - opponent_w;
        opponent_y = (LCDHEIGHT-opponent_h)/2;
        player_x = 0;
        player_y = (LCDHEIGHT-player_h)/2;
        goTitleScreen();
      }
      
      switch(state)
      {
        case gameMenu :
        //game menu
        opponent_x = LCDWIDTH - opponent_w;
        opponent_y = (LCDHEIGHT-opponent_h)/2;
        player_x = 0;
        player_y = (LCDHEIGHT-player_h)/2;
        opponent_vy = 2;
        switch(gb.menu(multiPlayerMenu, 4)){
          case 0: //1 vs AI mode
            playerOne = true;
            playerTwo = false;
            ai = false;
            juggle = false;
            player_score = 0;
            opponent_score = 0;
            ball_score = 0;
            break;
          case 1: //1 v 1 mode    
            playerOne = false;
            playerTwo = true;
            ai = false;
            juggle = false;
            player_score = 0;
            opponent_score = 0;
            ball_score = 0;
            break;
          case 2: //Juggle mode
            playerOne = false;
            playerTwo = false;
            ai = false;
            juggle = true;
            player_score = 0;
            opponent_score = 0;
            ball_score = 0;
            break;
          case 3: //AI vs AI mode
            playerOne = false;
            playerTwo = false;
            ai = true;
            juggle = false;
            player_score = 0;
            opponent_score = 0;
            opponent_y = (LCDHEIGHT-opponent_h);
            player_y = 0;
            ball_score = 0;
            opponent_vy = 2.5;
            break;
          if(gb.buttons.pressed(BTN_C)){
          gb.titleScreen(F("Coding Pong"));
          }
          default:
            state = gameMenu;
            break;
         }
          state = game;
        break;
        case game :
        gb.display.fontSize = 2;
        if(ai == true){
        player_y = player_y - player_vy; 
        player_y = max(0, player_y);
        }

        if(ai == true){
          if((player_y+(player_h/2)) < (ball_y+(ball_size/2))){
          player_y = player_y + player_vy;
          player_y = min(LCDHEIGHT-player_h, player_y);
          }   
          else {
          if(ai == true){
          player_y = player_y - player_vy; 
          player_y = max(0, player_y);
          }
          }
        }
        else {
        if(gb.buttons.repeat(BTN_UP, 1)){
        player_y = max(0, player_y - player_vy);
        }
        if(gb.buttons.repeat(BTN_DOWN, 1)){
        player_y = min(LCDHEIGHT - player_h, player_y + player_vy);
        }
        }
          
        
        
        if(gb.buttons.pressed(BTN_C)){
        gb.titleScreen(F("Coding Pong"));
        }
        ball_x = ball_x + ball_vx;
        ball_y = ball_y + ball_vy;
        
        if((ball_y + ball_size) > LCDHEIGHT){
        ball_y = LCDHEIGHT - ball_size;
        ball_vy = -ball_vy;
        gb.sound.playTick();  
        }
        
        if(ball_y < 0){
        ball_y = 0;
        ball_vy = -ball_vy;
        gb.sound.playTick();  
        }
        if(gb.collideRectRect(ball_x, ball_y, ball_size, ball_size, player_x, player_y, player_w, player_h)){
        ball_x = player_x + player_w;
        ball_vx = -ball_vx;
        gb.sound.playTick();
        ball_score = ball_score + 1; 
        }
        if(gb.collideRectRect(ball_x, ball_y, ball_size, ball_size, opponent_x, opponent_y, opponent_w, opponent_h)){
        ball_x = opponent_x - ball_size;
        ball_vx = -ball_vx;
        gb.sound.playTick();
        ball_score = ball_score + 1;
        }
        if(ball_y < 0){ ball_y = 0; ball_vy = -ball_vy; gb.sound.playTick(); } if(gb.collideRectRect(ball_x, ball_y, ball_size, ball_size, player_x, player_y, player_w, player_h)){ ball_x = player_x + player_w; ball_vx = -ball_vx; gb.sound.playTick(); } if(gb.collideRectRect(ball_x, ball_y, ball_size, ball_size, opponent_x, opponent_y, opponent_w, opponent_h)){ ball_x = opponent_x - ball_size; ball_vx = -ball_vx; gb.sound.playTick(); } if(ball_x < 0){ opponent_score = opponent_score + 1; gb.sound.playCancel(); ball_x = LCDWIDTH - ball_size - opponent_w -1; ball_y = random(0,LCDHEIGHT-ball_size); } if((ball_x + ball_size) > LCDWIDTH){
        player_score = player_score + 1;
        if(juggle == true && player_score > 0){
        gb.sound.playCancel();
        state = gameOver;
        }
        delay(50);
        if(juggle == true && opponent_score > 0){
        gb.sound.playCancel();
        state = gameOver;
        }
        gb.sound.playOK();
        delay(700);
        ball_x = LCDWIDTH - ball_size - opponent_w - 16;
        ball_y = random(0,LCDHEIGHT-ball_size);
        ball_vx = -ball_vx;
        }
        

        
        if(playerTwo == true){
        if(gb.buttons.repeat(BTN_B, 1)){
        opponent_y = max(0, opponent_y - opponent_vy);
        }
        if(gb.buttons.repeat(BTN_A, 1)){
        opponent_y = min(LCDHEIGHT - opponent_h, opponent_y + opponent_vy);
        }
        }
        if(juggle == true){
        if(gb.buttons.repeat(BTN_UP, 1)){
        opponent_y = max(0, opponent_y - opponent_vy);
        }
        if(gb.buttons.repeat(BTN_DOWN, 1)){
        opponent_y = min(LCDHEIGHT - opponent_h, opponent_y + opponent_vy);
          }
        }
        else{
        if(ai == true){
        if((opponent_y+(opponent_h/2)) < (ball_y+(ball_size/2))){
        opponent_y = opponent_y + opponent_vy;
        opponent_y = min(LCDHEIGHT-opponent_h, opponent_y);
        }   
        else {
        opponent_y = opponent_y - opponent_vy; 
        opponent_y = max(0, opponent_y);
        }
        }
        if(playerOne == true){
        if((opponent_y+(opponent_h/2)) < (ball_y+(ball_size/2))){
        opponent_y = opponent_y + opponent_vy;
        opponent_y = min(LCDHEIGHT-opponent_h, opponent_y);
        }   
        else {
        opponent_y = opponent_y - opponent_vy; 
        opponent_y = max(0, opponent_y);
        }
        }
        }
        gb.display.fillRect(player_x, player_y, player_w, player_h);
        if (juggle == false){
        gb.display.cursorX = 21;
        gb.display.cursorY = 8;
        gb.display.print(player_score);
        gb.display.cursorX = 57;
        gb.display.cursorY = 8;
        gb.display.print(opponent_score);
        }
        if(juggle == true){
        gb.display.cursorX = LCDWIDTH/2-1;
        gb.display.cursorY = 8;
        gb.display.print(ball_score);
        }
        gb.display.fillRect(opponent_x, opponent_y, opponent_w, opponent_h);
        gb.display.fillRect(ball_x, ball_y, ball_size, ball_size);

        if((player_score == 10) || (opponent_score == 10)){
        state = gameOver;
        }
        if(juggle == true && opponent_score > 0){
        gb.sound.playCancel();
        state = gameOver;
        }
          break;
                case gameOver :
          if(juggle == false){
          gb.display.fontSize = 1.5;
          if(opponent_score > player_score){
          if (playerOne == true){
          gb.display.cursorX = LCDWIDTH/3-1;
          gb.display.cursorY = LCDHEIGHT/3;
          gb.display.print("AI Wins!");
          gb.popup(F("Nice Try"), 3);     
          if(gb.buttons.pressed(BTN_B))
          {state = gameMenu;}       
          }
          else{
          gb.display.cursorX = LCDWIDTH/6;
          gb.display.cursorY = LCDHEIGHT/3;
          gb.display.print("Player 2 Wins!");
          gb.popup(F("Good Game"), 3);
          if(gb.buttons.pressed(BTN_B))
          {state = gameMenu;}
          }
          }
          else{
          gb.display.cursorX = LCDWIDTH/6;
          gb.display.cursorY = LCDHEIGHT/3;
          gb.display.print("Player 1 Wins!");
          gb.popup(F("Nice Job!"), 3);
          if(gb.buttons.pressed(BTN_B))
          {state = gameMenu;}
          }
          }
          else{
          gb.display.fontSize = 2;
          gb.display.cursorX = LCDWIDTH/2.15;
          gb.display.cursorY = LCDHEIGHT/2;
          gb.display.print(ball_score);
          gb.display.cursorX = LCDWIDTH/23;
          gb.display.cursorY = LCDHEIGHT/7;
          gb.display.print("Your Score:");
          if(ball_score > 99){
          gb.popup(F("POGCHAMP!!!"), 3);
          if(gb.buttons.pressed(BTN_B))
          {
            state = gameMenu;
          }
          }
          if(gb.buttons.pressed(BTN_B))
          {
            state = gameMenu;
          }

          break;
}
}
}
}
