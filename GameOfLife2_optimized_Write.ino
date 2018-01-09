
//The screen is 1.77" diagonal, with 160 x 128 pixel resolution. 

//Include Libs
#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

// pin definition for the DUE
#define cs   10
#define dc   9
#define rst  8
#define sd_cs 7


TFT TFTscreen = TFT(cs, dc, rst);

//Variables
#define Delay 200

//const byte SIZEX = 160;
//const byte SIZEY = 128;

const byte SIZEX = 80;
const byte SIZEY = 50;

bool grid[SIZEX][SIZEY];
bool nextGrid[SIZEX][SIZEY];

int difference = 0;
int lastdifference = 0;
int boring = 0;
int alive;
int lastalive = 0;

int lifecount;

int generation;
int highscore;

void setup() {

  Serial.begin(9600);
  
  // initialize the display
  TFTscreen.begin();

  // set the stroke color to white
  TFTscreen.stroke(0,0,255);  

  randomSeed(analogRead(0)); //Initialize Random Number

  clearScreen();

  //displaystats();
 
  initialize();
 
  delay(Delay);
 
}

void loop() {

  difference = 0;
  alive = 0;

  //clearScreen();

  lifecycle();

  drawPixels();
  
  //displaystats();

  fillnextgrid();

  checksamegrid();
}

//Empty Screen
void clearScreen(){
  
  TFTscreen.background(0, 0, 0); 
  
}

//Initializes the very first screen with random cells
void initialize(){

  clearScreen();
  //displaystats();

  if (generation > highscore)highscore = (generation - 25);
  
  generation = 0;

  //Turn ALL Led's on 
  TFTscreen.background(0, 0, 255);
  delay(1000);

  //Turn SOME Led's off counting up
  for(int x = 0; x < SIZEX;x++){
     for(int y = 0; y < SIZEY;y++){

        grid[x][y] = random(2);
        
        if(grid[x][y] == 1){
          TFTscreen.stroke(0,255,0);
          TFTscreen.point( x,y);
        }
        else{
          TFTscreen.stroke(0,0,0);
          TFTscreen.point( x,y);          
        }
     }
  } 
}


void lifecycle(){

  for(int x = 0; x < SIZEX; x++){
     for(int y = 0; y < SIZEY; y++){

         //Count all the neighboring cells
         byte count = neighbours(x, y);

         // The gamerules
         
         // born
         if(grid[x][y] == 0 && count == 3){
          nextGrid[x][y] = 1;
          difference++;
          alive++;
         }


         // Alive and stay alive
         if(grid[x][y] == 1 && count == 2 || count == 3){
          nextGrid[x][y] = 1;
          alive++;
         }
         

         // Alive and die
         if(grid[x][y] == 1 && count < 2 || count > 3){
          nextGrid[x][y] = 0;
          difference++;
          alive--;
         }
    }    
  }   
}

//Writes back all the values that lifecycle() calculated into the temporary 'nextGrid-array' into 'grid-array'
void fillnextgrid(){

  for(int x = 0; x < SIZEX; x++){ 
     for(int y = 0; y < SIZEY; y++){

        grid[x][y] = nextGrid[x][y];      
    }    
  }   
}

//Counts the neighbours and gives back the number of neighbours
byte neighbours(byte x, byte y) {
  return grid[(x + 1) % SIZEX][y] + 
    grid[x][(y + 1) % SIZEY] + 
    grid[(x + SIZEX - 1) % SIZEX][y] + 
    grid[x][(y + SIZEY - 1) % SIZEY] + 
    grid[(x + 1) % SIZEX][(y + 1) % SIZEY] + 
    grid[(x + SIZEX - 1) % SIZEX][(y + 1) % SIZEY] + 
    grid[(x + SIZEX - 1) % SIZEX][(y + SIZEY - 1) % SIZEY] + 
    grid[(x + 1) % SIZEX][(y + SIZEY - 1) % SIZEY];
}


//Checks if the grid is the same. It does so by ckecking if a cell is born or has died (difference),
//and by comparing the alive count to a previous value.
void checksamegrid(){
  
  if (difference == lastdifference && alive == lastalive) boring++;  

  if (boring >= 25 || generation >= 3000){
    delay(Delay * 3);
    generation = generation + 25;
    initialize();
    boring = 0;
  }
  
  lastdifference = difference;
  lastalive = alive;

  //if the grid is completely empty, no need to wait for 'boring' to count up. initialize immediately.
  lifecount = 0;
  
  for(int x = 0; x < SIZEX; x++){
     
    for(int y = 0; y < SIZEY; y++){
      
      lifecount = lifecount + grid[x][y];  
    }
  }
  
  if (lifecount == 0){   
    clearScreen();
    delay(Delay * 3);
    initialize();
  }
  else generation++;

  //Serial.println(alive);

}

  /*isplays stats
void displaystats(){

  //Normalstats
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,33);
  display.print("Alive:");
  display.println(lifecount);
  display.print("Gen:");
  display.print(generation);
  display.print(" ");

  //Highscore rotated 90° to display even large numbers
  display.setRotation(1);  // rotate 90 degrees counter clockwise, can also use values of 2 and 3 to go further.
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(45,55);
  display.println("MaxGen:");
  display.print(highscore);
    // revert back to no rotation
  display.setRotation(0);

  //COSMETICS
  //Horizontalline
  display.drawLine(0,30,59,30,BLACK);
  //Verticalline
  display.drawLine(59,0,59,48,BLACK);

  //Box around the game
  display.drawRoundRect(13,0,29,29,2, BLACK);
}*/
  
//Shows the gamescreen

void drawPixels(){

  for(int x = 0; x < SIZEX; x++){
     for(int y = 0; y < SIZEY; y++){
      
      if(grid[x][y] == 1){
        TFTscreen.stroke(0,255,0);
        TFTscreen.point(x, y);
      } 
      if(grid[x][y] == 0){
        TFTscreen.stroke(0,0,0);
        TFTscreen.point( x,y);
      }  
    }    
  }  
}


  
