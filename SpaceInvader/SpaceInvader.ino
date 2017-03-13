/*
 RST 8
 CS 10
 RS 9
 SDA 11
 SCL 13
 3V3 3.3V
 LED 3.3V
 GND GND
 */
enum{defender,shot,invader};
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <TFT_ILI9163C.h>

#define __CS 10
#define __DC 9
// Color definitions
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0  
#define WHITE   0xFFFF
#define TEAL    0x66FF

#define  NBINS   8
#define ARMY_SIZE 55

TFT_ILI9163C tft = TFT_ILI9163C(__CS, __DC);


/*
 Invader type one
 X is the middle
 {(X-1,Y),(X-2,Y),(X+1,Y),(X+2,Y)
 (X-2,Y-1),(X-2,Y-2)....}
 DrawInvader(X,Y,&vector)
 {
 switch(type)
 {
  case   1:
  
  case   2
  cse    3




 }
 */

//structs used in the Space_invader game
struct pixel{
  char X;
  char Y;
};
struct Invader
{
  char type;
  char number;
  char X;// X of the middle pixle
  char Y;//Y of the middle pixle
};

struct pozitie
{
  char invader_nr;
  char occupation;// 0:not occupied, 1:occupied
};

struct defender
{
  char X;
  char Y;
  char lives;
};


struct shot{
   short X;
   short Y;
};


struct Invader army[ARMY_SIZE];
struct defender st_defender;
struct shot shot_fired;
char X,Y;

unsigned long currentTime,prevTime=0,shotTime,prevShotTime=0;
const long interval=10,interval2=10;


char collision=0;
char edge=1;
char defender_edge=0;
char gameOver=1;
char current_invader=0;
char shot_on=0;


 void initializare_army()
 {
  char i;
  for(i=0;i<ARMY_SIZE;i++)
  {
    army[i].number=0;
    army[i].X=0;
    army[i].Y=0;
  }
 }





void initializare_defender()
{
  st_defender.X=0;
  st_defender.Y=0;
 st_defender.lives=3;
  
}

void initializare_invader(char X, char Y)
{
  army[current_invader].type=1;
  army[current_invader].X=X;
  army[current_invader].Y=Y;
  current_invader++;
  
} 


void shots_fired()
{
 shot_fired.X=st_defender.X;
 shot_fired.Y=st_defender.Y-1;
  tft.drawPixel(shot_fired.X,shot_fired.Y,WHITE);
   tft.drawPixel(shot_fired.X,shot_fired.Y-1,WHITE);
   shot_on=1;
}

void delete_shot()
{
  tft.drawPixel(shot_fired.X,shot_fired.Y,BLACK);
  tft.drawPixel(shot_fired.X,shot_fired.Y-1,BLACK);
  shot_fired.X=0;
  shot_fired.Y=0;
  shot_on=0;
}
void move_shot()
{
  if(shot_fired.X!=0)
  {
  tft.drawPixel(shot_fired.X,shot_fired.Y-3,WHITE);
  tft.drawPixel(shot_fired.X,shot_fired.Y-4,WHITE);
  tft.drawPixel(shot_fired.X,shot_fired.Y,BLACK);
  tft.drawPixel(shot_fired.X,shot_fired.Y-1,BLACK);
  
   shot_fired.Y-=3;
    if(shot_fired.Y<=2)delete_shot();
  }
  check_collision();
  
}

void move_defender_Right()
{
  char X=st_defender.X;
  char Y=st_defender.Y;
 
tft.drawPixel(X,Y-1,BLACK);
tft.drawPixel(X,Y-2,BLACK);
tft.drawPixel(X-2,Y,BLACK);
tft.drawPixel(X+1,Y-1,TEAL);
tft.drawPixel(X+1,Y-2,TEAL);
tft.drawPixel(X+3,Y,TEAL);
st_defender.X+=1;

if(st_defender.X+2>=126)defender_edge=1;

  
}



void move_defender_Left()
{
  char X=st_defender.X;
  char Y=st_defender.Y;
tft.drawPixel(X,Y-1,BLACK);
tft.drawPixel(X,Y-2,BLACK);
tft.drawPixel(X+2,Y,BLACK);
tft.drawPixel(X-1,Y-1,TEAL);
tft.drawPixel(X-1,Y-2,TEAL);
tft.drawPixel(X-3,Y,TEAL);
st_defender.X-=1;
if(st_defender.X-2<=2)defender_edge=2;

  
}
void spawn_defender()
{
  char X=63;
  char Y=120;
/*st_defender.X=63;
st_defender.Y=120;
tft.drawPixel(X,Y,TEAL);
tft.drawPixel(X+1,Y,TEAL);
tft.drawPixel(X+2,Y,TEAL);
tft.drawPixel(X-1,Y,TEAL);
tft.drawPixel(X-2,Y,TEAL);
tft.drawPixel(X,Y-1,TEAL);
tft.drawPixel(X,Y-2,TEAL);
*/
Draw(X,Y,1);
}



void Spawn_Invader(char X,char Y,char type)
{

  
      initializare_invader(X, Y);
      Draw(X,Y,type);
    
}

void Draw(char X, char Y,char type)
{
  
struct pixel type1 []={{X,Y},{X-1,Y},{X-2,Y},{X-3,Y},{X-4,Y},{X-5,Y},{X-4,Y-1},{X-3,Y-1},{X-3,Y-2},
                        {X-2,Y-2},{X-2,Y-3},{X-3,Y-4},{X-1,Y-1},{X-1,Y-2},{X-5,Y+1},{X-5,Y+2},{X-3,Y+1},
                        {X-3,Y+2},{X-2,Y+1},{X-2,Y+3},{X-1,Y+1},{X-1,Y+2},
                        {X+1,Y},{X+2,Y},{X+3,Y},{X+4,Y},{X+5,Y},{X+4,Y-1},{X+3,Y-1},{X+3,Y-2},  
                        {X+2,Y-2},{X+2,Y-3},{X+3,Y-4},{X+1,Y-1},{X+1,Y-2},{X+5,Y+1},{X+5,Y+2},{X+3,Y+1},
                        {X+3,Y+2},{X+2,Y+1},{X+2,Y+3},{X+1,Y+1},{X+1,Y+2},{X,Y-1},{X,Y-2},{X,Y+1},{-1,-1}};
                        
struct pixel type2 []={{X,Y},{X-1,Y},{X-2,Y},{X-3,Y},{X-3,Y-1},{X-2,Y-1},{X-2,Y-2},{X-2,Y-3},{X-1,Y-2},{X-1,Y-4},{X-1,Y-4}
                        ,{X-2,Y+2},{X-2,Y+3},{X-3,Y+2},
                        {X+1,Y},{X+2,Y},{X+3,Y},{X+3,Y-1},{X+2,Y-1},{X+2,Y-2},{X+2,Y-3},{X+1,Y-2},{X+1,Y-4},{X+1,Y-4}
                        ,{X+2,Y+2},{X+2,Y+3},{X+3,Y+2},
                        {X,Y-1},{X,Y-2},{X,Y-3},{X,Y-4},{X,Y+1}};

struct pixel type3 []={{X,Y},{X,Y-1},{X,Y-2},{X,Y-3},{X-1,Y},{X-2,Y},{X-3,Y},{X-4,Y},{X-5,Y},{X-6,Y},{X-1,Y-1},{X-1,Y-2},{X-1,Y-3},
                       {X-2,Y-2},{X-2,Y-3},{X-3,Y-2},{X-4,Y-1},{X-4,Y-2},{X-4,Y-2},{X-1,Y-1},{X-6,Y+1},{X-1,Y+2},{X-2,Y+1},{X-3,Y+1},{X-3,Y+3},{X-4,Y+1},{X-4,Y+2},{X-4,Y+3},{X-4,Y+2},
                      {X+2,Y},{X+3,Y},{X+4,Y},{X+5,Y},{X+6,Y},{X+1,Y-1},{X+1,Y-2},{X+1,Y-3},{X+1,Y},
                       {X+2,Y-2},{X+2,Y-3},{X+3,Y-2},{X+4,Y-1},{X+4,Y-2},{X+4,Y-2},{X+1,Y-1},{X+6,Y+1},{X+1,Y+2},{X+2,Y+1},{X+3,Y+1},{X+3,Y+3},{X+4,Y+1},{X+4,Y+2},{X+4,Y+3},{X+4,Y+2},
                        {X,Y+2}};
  struct pixel defender []= { {X,Y},{X,Y+1},{X,Y+2},{X,Y+3},{X,Y+4},{X,Y+5},{X,Y+6},{X,Y-1},{X,Y-2},{X,Y-3},{X,Y-4},{X,Y-5},{X,Y-6},{X,Y-7},{X,Y-8},
                             {X-1,Y},{X-2,Y},{X-3,Y},{X-4,Y},{X-6,Y},{X-7,Y},{X-7,Y-2},{X-7,Y-1},{X-6,Y-1},{X-6,Y-2},{X-6,Y-3},
                             {X-5,Y+1},{X-5,Y+2},{X-5,Y+3},{X-5,Y+4},{X-5,Y+5},{X-5,Y+6},
                             {X-2,Y-1},{X-1,Y-1},{X-1,Y-2},{X-1,Y-3},{X-1,Y-4},{X-1,Y-5},{X-1,Y-6},{X-1,Y-7},{X-1,Y+1},{X-1,Y+2},{X-1,Y+3},{X-1,Y+4},{X-1,Y+5},{X-1,Y+6},
                             {X-2,Y+1},{X-2,Y+2},{X-2,Y+3},{X-2,Y+4},{X-2,Y+5},{X-2,Y+6},{X-2,Y},{X-3,Y+1},{X-3,Y+2},{X-3,Y+3},{X-3,Y+4},{X-3,Y+5},{X-3,Y+6},
                             {X-4,Y+1},{X-4,Y+2},{X-4,Y+3},{X-4,Y+4},{X-4,Y+5},{X-4,Y+6},{X-6,Y+1},{X-6,Y+2},{X-6,Y+3},{X-6,Y+4},{X-6,Y+5},{X-6,Y+6},{X-7,Y+1},{X-7,Y+1},
                             {X-7,Y+2},{X-7,Y+3},{X-7,Y+4},{X-7,Y+5},{X-7,Y+6},{X-8,Y+2},{X-8,Y+3},{X-8,Y+4},{X-8,Y+5},{X-8,Y+6},{X-9,Y+3},{X-9,Y+4},{X-9,Y+5},
                             {X+1,Y},{X+2,Y},{X+3,Y},{X+4,Y},{X+7,Y},
                             {X+1,Y+1},{X+1,Y+2},{X+1,Y+3},{X+1,Y+4},{X+1,Y+5},{X+1,Y+6},{X+1,Y-1},{X+1,Y-2},{X+1,Y-3},{X+1,Y-4},{X+1,Y-5},{X+1,Y-6},{X+1,Y-7},
                             {X+2,Y+1},{X+2,Y+2},{X+2,Y+3},{X+2,Y+4},{X+2,Y+5},{X+2,Y+6},{X+3,Y+1},
                             {X+3,Y+2},{X+3,Y+3},{X+3,Y+4},{X+3,Y+5},{X+3,Y+6},{X+4,Y+2},{X+4,Y+1},{X+4,Y+3},{X+4,Y+4},{X+4,Y+5},{X+4,Y+6},{X+4,X+1},
                            {X+5,Y+1},{X+5,Y+2},{X+5,Y+3},{X+5,Y+4},{X+5,Y+5},{X+5,Y+6},{X+6,Y+2},{X+6,Y+3},{X+6,Y+4},{X+6,Y+5},{X+6,Y+6},
                            {X+7,Y+1},{X+7,Y+2},{X+7,Y+3},{X+7,Y+4},{X+7,Y+5},{X+7,Y+6},
                            {X+8,Y+3},{X+8,Y+4},{X+8,Y+5},{X+8,Y+6},{X+9,Y+4},{X+9,Y+5}};
                            
                             
                                       
short i;short marime;
switch(type)
{
  
  case 1:
   marime=sizeof(defender)/sizeof(defender[0]);
  for(i=0;i<marime;i++)
 tft.drawPixel(defender[i].X,defender[i].Y,YELLOW);
  break;
  case 2:
  break;
  case 3:
   marime=sizeof(type1)/sizeof(type1[0]);
  for(i=0;i<marime;i++)
 tft.drawPixel(type1[i].X,type1[i].Y,CYAN);
 break;
 case 4:
   marime=sizeof(type2)/sizeof(type2[0]);
  for(i=0;i<marime;i++)
 tft.drawPixel(type2[i].X,type2[i].Y,TEAL);break;
  case 5:
   marime=sizeof(type3)/sizeof(type3[0]);
  for(i=0;i<marime;i++)
 tft.drawPixel(type3[i].X,type3[i].Y,WHITE);

  break;
  
}
}

void delete_invader(int nr)
{

  char X=army[nr].X;
  char Y=army[nr].Y;

  army[nr].X=0;
  army[nr].Y=0;
tft.drawPixel(X,Y,BLACK);
tft.drawPixel(X-1,Y,BLACK);
tft.drawPixel(X+1,Y,BLACK);
tft.drawPixel(X,Y-1,BLACK);
}

void moveInvader_Y()
{
  char i;
  for(i=0;i<40;i++)
  {
    if(army[i].X!=0)
    {
    tft.drawPixel(army[i].X-1,army[i].Y,BLACK);
   tft.drawPixel(army[i].X+1,army[i].Y,BLACK);
  tft.drawPixel(army[i].X,army[i].Y-1,BLACK);
  tft.drawPixel(army[i].X,army[i].Y+1,GREEN);
  tft.drawPixel(army[i].X-1,army[i].Y+1,GREEN);
  tft.drawPixel(army[i].X+1,army[i].Y+1,GREEN);
  army[i].Y+=1;
  if(army[i].Y==126)
  {
    gameOver=0;
  }
  }
  }
  
  
}

void moveInvader_XRight(char nr)
{
  if(army[nr].X!=0)
  {
  tft.drawPixel(army[nr].X-1,army[nr].Y,BLACK);
  tft.drawPixel(army[nr].X,army[nr].Y-1,BLACK);
  tft.drawPixel(army[nr].X+2,army[nr].Y,GREEN);
  tft.drawPixel(army[nr].X+1,army[nr].Y-1,GREEN);
  army[nr].X+=1;
  if((army[nr].X+2)==126) 
  {
    moveInvader_Y();
    edge=2;
  }
  
  }
}
void moveInvader_Xleft(char nr)
{
  if(army[nr].X!=0)
  {
  tft.drawPixel(army[nr].X+1,army[nr].Y,BLACK);
  tft.drawPixel(army[nr].X,army[nr].Y-1,BLACK);
  tft.drawPixel(army[nr].X-2,army[nr].Y,GREEN);
  tft.drawPixel(army[nr].X-1,army[nr].Y-1,GREEN);
  army[nr].X-=1;
  if((army[nr].X-2)==2) 
  {
    moveInvader_Y();
    edge=1;
  }}
}


void check_collision()
{
char i;
for(i=0;i<40;i++)
{
if((shot_fired.X>army[i].X-2 && shot_fired.X<army[i].X+2) && (shot_fired.Y>army[i].Y-3 && shot_fired.Y<army[i].Y+3))
//if(shot_fired.X==army[i].X && shot_fired.Y==army[i].Y)
  {
   
    delete_invader(i);
    delete_shot();
  }
 else if(army[i].Y>=124) game_over();
    
}}
void game_over()
{
  while(1)
  {
  tft.clearScreen(BLACK);
  tft.setCursor(48,60);
  tft.setTextColor(WHITE);
  tft.print("Game Over");
}}

void moveArmy()
{
  char i,j;

  if(edge==1)
  {
    for(i=0;i<40;i++)
     moveInvader_XRight(i);
  }

    else if(edge==2)
  {
 
    for(i=0;i<40;i++)
     moveInvader_Xleft(i);
     
  }
 
}




void Spawn_Level(char X,char Y)
{
char i,j;
char type;
spawn_defender();
char original_X=X;
for(j=0;j<4;j++)
  {
    if(j<=1)type=3; else if (j==2) type=4; else if(j==3) type=5;
    for(i=0;i<5;i++)
    {
      Spawn_Invader(X,Y,type);
      X+=20;
    }
    X=original_X;
    Y+=20;
  }

spawn_defender();

}



void setup(void) {
  
  Serial.begin(38400);
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  tft.setTextWrap(true);
  tft.setTextColor(WHITE,BLACK);

 

Spawn_Level(7,10);

}


void loop(){
  while(1)
  {
  /*shotTime=millis();
    currentTime=millis();
    if(shotTime-prevShotTime>=interval2)
    {
      prevShotTime=shotTime;
      check_collision();
       if(shot_on==0) shots_fired();
     
    }
    if(currentTime-prevTime>=interval)
    {
      if(defender_edge==0)
      move_defender_Right();
      else if(defender_edge==1)
      move_defender_Left();
      else if(defender_edge==2)
      move_defender_Right();
      move_shot();
      moveArmy();
     prevTime=currentTime;
      */
    
  
  }}
  



