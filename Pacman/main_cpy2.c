#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <assert.h>
#include <emscripten.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define FRAME_INVINCIBILITE 90 // let's assume it's a 60 FPS game so it's approximatively 2 seconds.
#define CELL 900
const int FPS = 60; // Used to set the sprites animations speed.Lower it is, faster it goes.
int frameTime = 1;
int frameWidth,frameHeight;
int textureWidth,textureHeigh;

int tempsPrecedent = 0, tempsActuel = 0;
int tempsPrecedent2 = 0, tempsActuel2 = 0;

enum { WIDTH = 20};
enum { HEIGHT = 20 };
enum { ELMTSIZE = 34 };

enum {EMPTY, WALL, GEM, TP,MAZE_MODIFIER,HERB_CUT,GEM_HERB_CUT,FAIRY_HERB_CUT,BUISSON_HERB_CUT,NENT };


int level1[HEIGHT][WIDTH+4] = {
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1},
{0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0},
{0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0},
{0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
{0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}
};



typedef struct {
  int x;
  int y;
} Point;

typedef struct {
  float x;
  float y;
} Pointf;

typedef struct {
  int x;
  int y;
  int dir;
  float count_down;
  int old_y;
  int old_x;
  int wallx;
  int wally;
  float speed;
  int basex;
  int basey;
  int vie;
  SDL_Rect Rect;
  Pointf position;
} Entity;


 static int fin = -1;
 static Point file[256];



 void push(Point v) {

    file[++fin] = v;
 }

 Point pop(void) {
    int i;
    Point v= file[0];
 for(i=0;i< fin;i++){
    file[i]= file[i+1];
 }
    fin--;
    return v;
 }

enum {LEVEL1, GAMEOVER, NSCREEN};
enum { NORTH, SOUTH, EAST, WEST, NDIR };

typedef struct {
    int screen;
    SDL_Surface* img_screen;
    SDL_Surface* img_screen_game_over;
    SDL_Surface* img_wall2;
    SDL_Surface* img_herb_camouflage;
    SDL_Surface* img_coeur;
    SDL_Surface* img_ange;
    SDL_Surface* img_angette;
    SDL_Surface* img_demon;
    SDL_Surface* img_demon2;
    SDL_Surface* img_demon3;
    SDL_Surface* img_demon4;
    SDL_Surface* img_demon5;
    SDL_Surface* img_demon6;
    SDL_Surface** tab_item;

   Mix_Chunk *ambient_music;
   Mix_Chunk *hurt_music;
   Mix_Chunk *rupee_music;
   Mix_Chunk *hey_music;
   Mix_Chunk *hey2_music;
   Mix_Chunk *portal_music;
   Mix_Chunk *low_health_music;



    Entity** enemies;
    Entity heroe;
    Entity wall;
    int maze[HEIGHT][WIDTH];
    int directions[HEIGHT][WIDTH];
    int board[HEIGHT][WIDTH];
    int ent_count[NENT];
    int blit_all;
    int sword;
    int hide;
    int wall_hack_heroe;
    int wall_hack_enemy;
    int editeur;
    int editor_mode;
    int clear;
    int item;
    int hunt;
    int keyboard_state[NDIR];
    int pull;
    int speed_hack;
    int gender;
    int invincibility;
    int mange_moi;
    int bouge_toi;
    int dir_wall;
    float wall_cd;
    int boucle_mur;

} Data;




Point coor[] = { {0, -1}, {0, 1}, {1, 0}, {-1, 0}, {0, 0}};

void init_maze(Data * data){
    int a,b;
     for(a = 0; a <= WIDTH  ; a++)
     {
         for(b = 0; b <= HEIGHT ; b++)
         {
             if(a % 2 == 0 || b % 2 == 0)
                 data->maze[b][a] = WALL;
             else
                 data->maze[b][a] = EMPTY;
         }
     }
}
 int is_closed(Data * data, int x, int y){
    if(data->maze[x - 1][y]  == WALL
       && data->maze[x][y - 1] == WALL
       && data->maze[x][y + 1] == WALL
       && data->maze[x + 1][y] == WALL
    )
        return 1;

    return 0;
}

void maze_generator(int index,Data * data, int backtrack_x[CELL], int backtrack_y[CELL], int x, int y, int visited)
{
    if(visited <  100) //Well, I tried some numbers, for my board dimensions, 100 is the perfect one as it ensure to have a perfect maze.
    {
        int neighbour_valid = -1;
        int neighbour_x[4];
        int neighbour_y[4];
        int step[4];

        int x_next;
        int y_next;

        if(x - 2 > 0 && is_closed(data, x - 2, y))  // upside
        {
            neighbour_valid++;
            neighbour_x[neighbour_valid]=x - 2;
            neighbour_y[neighbour_valid]=y;
            step[neighbour_valid]=1;
        }

        if(y - 2 > 0 && is_closed(data, x, y - 2))  // leftside
        {
            neighbour_valid++;
            neighbour_x[neighbour_valid]=x;
            neighbour_y[neighbour_valid]=y - 2;
            step[neighbour_valid]=2;
        }

        if(is_closed(data, x, y + 2))  // rightside
        {
            neighbour_valid++;
            neighbour_x[neighbour_valid]=x;
            neighbour_y[neighbour_valid]=y + 2;
            step[neighbour_valid]=3;

        }

        if(is_closed(data, x + 2, y))  // downside
        {
            neighbour_valid++;
            neighbour_x[neighbour_valid]=x+2;
            neighbour_y[neighbour_valid]=y;
            step[neighbour_valid]=4;
        }

        if(neighbour_valid == -1)
        {
            // backtrack
            x_next = backtrack_x[index];
            y_next = backtrack_y[index];
            index--;
        }

        if(neighbour_valid!=-1)
        {
            int randomization = neighbour_valid + 1;
            int random = rand()%randomization;
            x_next = neighbour_x[random];
            y_next = neighbour_y[random];
            index++;
            backtrack_x[index] = x_next;
            backtrack_y[index] = y_next;

            int rstep = step[random];

            if(rstep == 1)
                data->maze[x_next+1][y_next] = EMPTY;
            else if(rstep == 2)
                data->maze[x_next][y_next + 1] = EMPTY;
            else if(rstep == 3)
                data->maze[x_next][y_next - 1] = EMPTY;
            else if(rstep == 4)
                data->maze[x_next - 1][y_next] = EMPTY;
            visited++;
        }
        maze_generator(index, data, backtrack_x, backtrack_y, x_next, y_next, visited);
    }
}


SDL_Surface*load ( char const *filename )
{
    SDL_Surface *srfTmp = NULL;
    SDL_Surface *srfDst = NULL;

    srfTmp = SDL_LoadBMP ( filename );
    if ( srfTmp != NULL )
    {

        srfDst = SDL_DisplayFormatAlpha ( srfTmp );
    }

    SDL_FreeSurface ( srfTmp );

    return srfDst;
}

int end_of_game(Data* data) {
int x,acc=-1;
for(x=0;x<6;x++)if(data->enemies[x]->vie ==0) acc++;

if(acc == 5) return 1;
 return (data->ent_count[GEM] == 0 || data->heroe.vie==0 ) && data->editor_mode==0;

}


void add_items(Data* data, int nb,int item) {
  int i,fin=0;
  SDL_Rect position;

  for(i = 0; i < nb; i++) {
    fin++;
  if(fin == HEIGHT*WIDTH)
    return;
    int x = rand() % WIDTH;
    int y = rand() % HEIGHT;
    if((data->board[y][x] == EMPTY || data->board[y][x] == HERB_CUT) && (y != data->heroe.y || x != data->heroe.x )) {
      data->board[y][x] = item;
      position.x = x* ELMTSIZE;
      position.y = y* ELMTSIZE;
      SDL_BlitSurface (data->tab_item[item], NULL, data->img_screen, &position);
    } else {
      i--;
    }
  }



}

void add_items_pos(Data* data,int item,int x,int y) {
SDL_Rect position;
    if((data->board[y][x] != EMPTY || (data->board[y][x] != HERB_CUT)))
       data->ent_count[data->board[y][x]]--;

    if(data->board[y][x] == GEM_HERB_CUT)
       data->ent_count[GEM]--;

    if(y != data->heroe.y || x != data->heroe.x){
      data->board[y][x] = item;

          position.x = x* ELMTSIZE;
          position.y = y*ELMTSIZE;
          SDL_BlitSurface (data->tab_item[item], NULL, data->img_screen, &position);
    }
  if(item == GEM_HERB_CUT)
    data->ent_count[GEM]++;
  else data->ent_count[item]++;

}


void board_game_over(Data* data) {
  int x, y;



 if(data->blit_all){
  SDL_FillRect(data->img_screen_game_over, NULL, SDL_MapRGB(data->img_screen_game_over->format, 64,64, 64));
  for(x = 0; x < WIDTH+4; x++) {
    for(y = 0; y < HEIGHT; y++) {
      SDL_Rect position;
      position.x = x * ELMTSIZE/1.3;
      position.y = y * ELMTSIZE/1.3;
      if(level1[y][x] == WALL)
        SDL_BlitSurface (data->tab_item[WALL], NULL, data->img_screen_game_over, &position);


    }
  }
  data->blit_all=0;
 }
}

void init_load(Data* data){
    int x;
  SDL_WM_SetCaption("My game,not the best one but I still love it.", NULL);
  data->img_screen = SDL_SetVideoMode(WIDTH * ELMTSIZE, HEIGHT * ELMTSIZE, 32, SDL_ANYFORMAT);
  data->img_screen_game_over = SDL_SetVideoMode(WIDTH * ELMTSIZE, HEIGHT * ELMTSIZE, 32, SDL_ANYFORMAT);
  data->img_wall2 = load("/Pacman/buisson.png");
  data->img_coeur = load("/Pacman/heart.png");
  data->img_herb_camouflage=load("/Pacman/camouflage.png");
  data->tab_item=NULL;
  data->tab_item=(SDL_Surface**)malloc(sizeof(SDL_Surface*)*9);
  data->enemies=(Entity**)malloc(sizeof(Entity*)*10);


  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
   {
      printf("%s", Mix_GetError());
   }
   Mix_AllocateChannels(7);
   data->ambient_music = Mix_LoadWAV("/Pacman/musique.mp3");
   data->hurt_music= Mix_LoadWAV("/Pacman/MC_Link_Hurt.wav");
   data->rupee_music= Mix_LoadWAV("/Pacman/MC_Rupee.wav");
   data->hey_music= Mix_LoadWAV("/Pacman/MC_Zelda_Hey.wav");
   data->hey2_music= Mix_LoadWAV("/Pacman/MC_Minish_Hey1.wav");
   data->portal_music= Mix_LoadWAV("/Pacman/MC_Portal_In.wav");
   data->low_health_music= Mix_LoadWAV("/Pacman/MC_LowHealth.wav");


  for(x =0;x<6;x++)
    data->enemies[x]=(Entity*)malloc(sizeof(Entity));

  data->tab_item[0]=load("/Pacman/herbe.png");
  data->tab_item[1]= load("/Pacman/buisson_herbe.png");
  data->tab_item[2] = load("/Pacman/gem_herbe.png");
  data->tab_item[3] = load("/Pacman/teleporteur.png");
  data->tab_item[4] = load("/Pacman/maze_modifier.png");
  data->tab_item[5]=load("/Pacman/herb_cut.png");
  data->tab_item[6]=load("/Pacman/gem_herb_cut.png");
  data->tab_item[7]=load("/Pacman/fairy_herb_cut.png");
  data->tab_item[8]= load("/Pacman/buisson_herb_cut.png");

    data->img_ange = load("/Pacman/ange.png");
    data->img_angette = load("/Pacman/angette.png");
    data->img_demon = load("/Pacman/demon.png");
    data->img_demon2 = load("/Pacman/demon2.png");
    data->img_demon3 = load("/Pacman/demon3.png");
    data->img_demon4 = load("/Pacman/demon4.png");
    data->img_demon5 = load("/Pacman/demon5.png");
    data->img_demon6 = load("/Pacman/demon6.png");



    textureWidth=data->img_ange->w;
    textureHeigh=data->img_ange->h;
    frameWidth = textureWidth /3;
    frameHeight= textureHeigh /4;
    data->heroe.Rect.h = frameHeight;
    data->heroe.Rect.w = frameWidth;
    data->heroe.Rect.x = data->heroe.Rect.y =0;

  for(x=0;x <6;x++){
        data->enemies[x]->Rect.x = data->enemies[x]->Rect.y =0;
        data->enemies[x]->Rect.h =  frameHeight;
        data->enemies[x]->Rect.w =  frameWidth;
        data->enemies[x]->vie =  1;
  }

}

void init_data(Data* data) {
  int x,y,z,nb_gems=5,nb_teleporter=5,nb_maze_modifier=1,NB_FAIRY_HERB_CUT=0;

  for(x = 0; x < WIDTH; x++) {
    for(y = 0; y < HEIGHT; y++) {
      data->board[y][x] = data->maze[y][x];
    }
  }

  data->enemies[0]->dir = data->enemies[0]->speed = 1 ;
  data->enemies[1]->dir = data->enemies[1]->speed = 1 ;
  data->enemies[2]->dir = data->enemies[2]->speed = 1 ;
  data->enemies[3]->dir = data->enemies[3]->speed = 1 ;
  data->enemies[4]->dir = data->enemies[4]->speed = 1 ;
  data->enemies[5]->dir = data->enemies[5]->speed = 1 ;

  data->enemies[0]->count_down = -1;
  data->enemies[1]->count_down = -1;
  data->enemies[2]->count_down = -1;
  data->enemies[3]->count_down = -1;
  data->enemies[4]->count_down = -1;
  data->enemies[5]->count_down = -1;



  data->heroe.dir = 1;
  data->heroe.x = 1;
  data->heroe.y = 1;

for(x=0;x <6;x++)data->enemies[x]->vie =  1;



  data->heroe.speed = (float)60/34;

  data->heroe.basex=data->heroe.x;
  data->heroe.basey=data->heroe.y;

  data->invincibility=0;
  data->hide=0;
  data->gender =0;
  data->speed_hack=0;
  data->pull=0;
  data->heroe.vie=6;
  data->sword=0;
  data->editeur=0;
  data->editor_mode=0;
  data->blit_all=0;
  data->clear=0;
  data->item=0;
  data->heroe.count_down = -1;
  data->mange_moi=0;
  data->bouge_toi =0;
  data->dir_wall=0;
  data->wall_hack_heroe=0;
  data->wall_hack_enemy=0;
  data->heroe.wallx=0;
  data->heroe.wally=0;
  data->wall_cd = -1;
  data->boucle_mur = 0;

  data->hunt=0;

  for(x = 0; x < NENT; x++)
    data->ent_count[x] = 0;

  data->ent_count[FAIRY_HERB_CUT]=NB_FAIRY_HERB_CUT;
  data->ent_count[GEM] = nb_gems;
  data->ent_count[TP] = nb_teleporter;
  data->ent_count[MAZE_MODIFIER]= nb_maze_modifier;

  add_items(data, nb_gems,GEM);
  add_items(data,nb_teleporter,TP);
  add_items(data,nb_maze_modifier,MAZE_MODIFIER);
   for(x = 0; x < WIDTH; x++) {
    for(y = 0; y < HEIGHT; y++) {
      SDL_Rect position;
      position.x = x * ELMTSIZE;
      position.y = y * ELMTSIZE;

      SDL_BlitSurface (data->tab_item[data->board[y][x]], NULL, data->img_screen, &position);

    }
   }
      z=0;
   while(z !=6){
        x =(rand()% WIDTH)+1;
        y =(rand()% HEIGHT)+1;
        if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && data->board[y][x] == EMPTY && (x != data->enemies[z]->x && y != data->enemies[z]->y)){
            data->enemies[z]->x=x;
            data->enemies[z]->y=y;
           //data->board[y][x] = EMPTY;
            z++;
        }
        }




   for(x = 0; x < NDIR; x++)
       data->keyboard_state[x]=0;

  data->screen = LEVEL1;
}


void draw_entity(Data* data, Entity* e, SDL_Surface* s) {
int x;

SDL_Rect position;


   if(e->count_down > -1) {
        //printf("%f %f \n",e->x * ELMTSIZE - e->count_down * coor[e->dir].x,e->y * ELMTSIZE - e->count_down * coor[e->dir].y);
        e->position.x = e->x * ELMTSIZE - e->count_down * coor[e->dir].x;
        e->position.y = e->y * ELMTSIZE - e->count_down * coor[e->dir].y;

        position.x = e->x * ELMTSIZE - e->count_down * coor[e->dir].x;
        position.y = e->y * ELMTSIZE - e->count_down * coor[e->dir].y;

        e->count_down-=e->speed;
    }
    else {
        e->position.x = e->x * ELMTSIZE;
        e->position.y = e->y * ELMTSIZE;

        position.x = e->x * ELMTSIZE;
        position.y = e->y * ELMTSIZE;

    }

    if(e == &data->heroe && data->hide)SDL_BlitSurface (s,NULL, data->img_screen, &position);
    else SDL_BlitSurface (s, &e->Rect, data->img_screen, &position);
}

int valid_pos(Data* data, int x, int y) {
  return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && data->board[y][x] != WALL && data->board[y][x] != BUISSON_HERB_CUT;
}



void draw_murs(Data* data, SDL_Surface* s) {
     SDL_Rect position;
int x,y;

x=(data->heroe.wallx + (data->boucle_mur * coor[data->dir_wall].x));
y=(data->heroe.wally + (data->boucle_mur * coor[data->dir_wall].y));
  //  for(dir=0;dir < NDIR;dir++){
      //  for(i=0;i<6;i++){
       //    if(x != data->heroe.x+coor[dir].x && x != data->enemies[i]->x +coor[dir].x && y != data->heroe.y+coor[dir].y && y != data->enemies[i]->y +coor[dir].y && x != data->enemies[i]->x && y != data->enemies[i]->y ){
            position.x = x * ELMTSIZE;
            position.y = y * ELMTSIZE;
            SDL_BlitSurface (data->tab_item[data->board[(data->heroe.wally + (data->boucle_mur * coor[data->dir_wall].y))][(data->heroe.wallx + (data->boucle_mur  * coor[data->dir_wall].x))]],NULL, data->img_screen, &position);
       //     dir=NDIR;
        //   }
      //  }
   // }

   if(data->wall_cd > -1.0) {
        position.x = (data->heroe.wallx + ((data->boucle_mur+1) * coor[data->dir_wall].x)) * ELMTSIZE - data->wall_cd  * coor[data->dir_wall].x;
        position.y = (data->heroe.wally + ((data->boucle_mur+1) * coor[data->dir_wall].y)) * ELMTSIZE - data->wall_cd  * coor[data->dir_wall].y;
        data->wall_cd-=4.25;
    }
    else {
        position.x = (data->heroe.wallx + ((data->boucle_mur+1) * coor[data->dir_wall].x)) * ELMTSIZE;
        position.y = (data->heroe.wally + ((data->boucle_mur+1) * coor[data->dir_wall].y)) * ELMTSIZE;
    }

    SDL_BlitSurface (s,NULL, data->img_screen, &position);
}
void move_mur(Data* data){
  int x,y,z;

    SDL_Rect position;



  ++data->boucle_mur;

  x = data->heroe.wallx + (data->boucle_mur+1) * coor[data->dir_wall].x;
  y = data->heroe.wally + (data->boucle_mur+1) * coor[data->dir_wall].y;

  if(valid_pos(data, x, y) && data->board[y][x]== EMPTY) {
    for(z=0;z<6;z++)
      if(data->enemies[z]->y == y && data->enemies[z]->x == x){
          data->enemies[z]->vie=0;
          position.x = x * ELMTSIZE;
          position.y = y * ELMTSIZE;
          SDL_BlitSurface (data->tab_item[data->board[y][x]], NULL, data->img_screen, &position);

      }
         data->board[y][x]=WALL;
         data->board[data->heroe.wally + data->boucle_mur * coor[data->dir_wall].y][ data->heroe.wallx + data->boucle_mur * coor[data->dir_wall].x]=EMPTY;
         data->wall_cd=ELMTSIZE;
  }
else{data->boucle_mur=0;data->bouge_toi=0;data->board[data->heroe.wally + coor[data->dir_wall].y][data->heroe.wallx +coor[data->dir_wall].x]=EMPTY;}
}






void clear_search(Data * data,int HorE){
	int x,y;

    if(!HorE)
      for(x = 0; x < WIDTH; x++) {
        for(y = 0; y < HEIGHT; y++) {
            if(data->board[y][x] != WALL) data->directions[y][x] = -2;
              else data->directions[y][x] = -10;

        }
      }

    else if(HorE == 1)
          for(x = 0; x < WIDTH; x++) {
            for(y = 0; y < HEIGHT; y++) {
                if(data->board[y][x] == WALL) data->directions[y][x] = -10;
                else if(data->board[y][x] == GEM || data->board[y][x] == GEM_HERB_CUT)data->directions[y][x] = -5;
                else if(data->board[y][x] == TP) data->directions[y][x] = -6;
                else if(data->board[y][x] == MAZE_MODIFIER) data->directions[y][x] = -7;
                else data->directions[y][x] = -2;
        }
      }

    else if(HorE==2)
       for(x = 0; x < WIDTH; x++) {
            for(y = 0; y < HEIGHT; y++) {
                if(data->board[y][x] == WALL) data->directions[y][x] = -10;
                else if(x == data->heroe.basex && y == data->heroe.basey) data->directions[y][x] = 10;
                else data->directions[y][x] = -2;
            }
        }
}

void afficher(Data * data){
	int x,y;
	for(y = 0; y < HEIGHT; y++) {
  for(x = 0; x < WIDTH; x++) {

      printf("%5d",data->directions[y][x]);
    }
    printf("\n");
  }
  printf("\n");
}

float euclid_dist(Entity* e, int x, int y) {
  return sqrtf(powf(x - e->x, 2) + powf(y - e->y, 2));
}

int valid_moves(Entity* e, int* vm,Data * data) {
  int dir;
  int c = 0;
  for(dir = 0; dir < NDIR; dir++) {
     int x = e->x + coor[dir].x;
     int y = e->y + coor[dir].y;
  if(!data->hide){
     if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
       vm[c] = dir;
       c++;
     }
  }
  else{

      if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && data->board[y][x] != WALL && data->board[y][x] != BUISSON_HERB_CUT) {
        vm[c] = dir;
        c++;
      }
  }
}
  return c;
}

int euristic_move(Data* data, Entity* e) {
  int vm[4],x,y;
  x=e->x;
  y=e->y;
  if(x == data->heroe.x && y == data->heroe.y)
    return -1;
  int c = valid_moves(e, vm,data);
  if(c == 0)
    return -1;
  int i;
  int best = -1;
  float dist_mini = HEIGHT * WIDTH;
  for(i = 0; i < c; i++) {
    int dir = vm[i];
    int x = e->x + coor[dir].x;
    int y = e->y + coor[dir].y;
    float ed = euclid_dist(&data->heroe, x, y);
    if(ed < dist_mini) {
      dist_mini = ed;
      best = dir;
    }
  }

  return best;
}

void move_entity(Data* data, Entity* e, int dir) {
  int z;


  int x = e->x + coor[dir].x;
  int y = e->y + coor[dir].y;
  e->old_x=e->x;
  e->old_y=e->y;
  if(dir == -1)
     return;





    if(e == &data->heroe){
       if(data->sword)
          data->sword--;
      if(data->wall_hack_heroe){
        if(x>= 0 && x < WIDTH && y>= 0 && y < HEIGHT){
               e->x=x;
               e->y=y;
               e->count_down = ELMTSIZE - 1;
               return;
                }
     }
    }



    if(data->board[y][x]== TP && (x> 0 && x < WIDTH && y> 0 && y < HEIGHT) && e->count_down <10){
        while(!(x> 0 && x < WIDTH && y> 0 && y < HEIGHT) || data->board[y][x] == 8 || data->board[y][x] == 3 || data->board[y][x] == 4 || data->board[y][x] == 1|| data->board[y][x] == 2  || (y == e->y && x == e->x )  || (y == data->heroe.y && x == data->heroe.x )) {
            x = rand() % WIDTH;
            y = rand() % HEIGHT;
        }


           e->x = x;
           e->y = y;
           e->count_down=-1;
	   if(e == &data->heroe)
             //Mix_PlayMusic(data->portal_music, 0);
	     Mix_PlayChannel(-1,data->portal_music, 0);
           return;
    }


    if(e == &data->heroe){
        if(data->board[y][x]== MAZE_MODIFIER && (x> 0 && x < WIDTH && y> 0 && y < HEIGHT)){

              int index = 0;
              int backtrack_x[CELL];
              int backtrack_y[CELL];
              backtrack_x[index] = 0;
              backtrack_y[index] = 0;
              init_maze(data);
              maze_generator(index, data, backtrack_x, backtrack_y, 1,1,1);
              data->blit_all=1;
             /* while(!valid_pos(data,x,y)){
                x = x + coor[e->dir].x;
                y = y + coor[e->dir].y;
              }*/
              e->x = x;
              e->y = y;
              e->count_down=-1;
              return;
        }
    }



  if(valid_pos(data, x, y)) {

        e->x = x;
        e->y = y;
       e->count_down = ELMTSIZE - 1;

  }
  else if(data->board[y][x] == WALL && data->mange_moi){
  		data->mange_moi=0;
  		data->board[y][x] = EMPTY;
	}






}

int random_move(Entity* e,Data * data) {
  int vm[4];
  int c = valid_moves(e, vm,data);
  if(c == 0)
    return -1;
  return vm[rand() % c];
}

int bfs(Data *data,Entity *e){

    Point a,c;
    int dir,acc=WIDTH*HEIGHT;
    int x,y,xh,yh;
 fin = -1;
    if(data->hide)
        return e->dir =random_move(e,data);
    if(data->wall_hack_enemy)
        return e->dir = euristic_move(data,e);

    clear_search(data,0);
    c.x=e->x;
    c.y=e->y;
    yh=data->heroe.y;
    xh=data->heroe.x;
    push(c);

  /*  for(x=0;x<6;x++)
    if(e != data->enemies[x] && e->vie)
        data->directions[data->enemies[x]->y][data->enemies[x]->x] =-10;
    else         data->directions[data->enemies[x]->y][data->enemies[x]->x] =-2;
*/
    data->directions[e->y][e->x]=-3;
    data->directions[data->heroe.y][data->heroe.x]=-4;

    while(c.x!=xh || c.y!=yh){

        if(!acc) //if wall_hack_heroe is enable.
            return -1;
        a= pop();

        for(dir =0;dir < NDIR;dir++){
            x = a.x + coor[dir].x;
            y = a.y + coor[dir].y;
            if(data->directions[y][x]==-4 && valid_pos(data,x,y)){

                if(data->directions[a.y][a.x]==-3 && valid_pos(data,x,y))
                    return e->dir =dir;
                return  e->dir =data->directions[a.y][a.x];
            }

            if( (data->directions[y][x] ==-2)&& valid_pos(data,x,y)){
                c.x=x;
                c.y=y;
            if(data->directions[a.y][a.x]==-3 && valid_pos(data,x,y) ){
                data->directions[c.y][c.x]=dir;
            }
            else
                data->directions[c.y][c.x]=data->directions[a.y][a.x];
            push(c);

           }

        }
        acc--;
}

return -1;

 }

int deuxieme_tour=0;
int bfs_h(Data *data,Entity *e,int item){
 Point a,c;
  int dir,acc=WIDTH*HEIGHT;
    int x,y;
     fin = -1;
     clear_search(data,1);

    c.x=e->x;
    c.y=e->y;

    push(c);

    if(deuxieme_tour)
        for(x=0;x <6;x++)
          data->directions[data->enemies[x]->y][data->enemies[x]->x]=-2;
    else {
         for(x=0;x <6;x++){
          if(data->enemies[x]->vie) data->directions[data->enemies[x]->y][data->enemies[x]->x]=-10; else data->directions[data->enemies[x]->y][data->enemies[x]->x]=-2;
         for(dir =0;dir < NDIR;dir++)
                 if(data->directions[data->enemies[x]->y+ (2*coor[dir].y)][data->enemies[x]->x+ (2*coor[dir].x)]== -2 && data->directions[data->enemies[x]->y+ coor[dir].y][data->enemies[x]->x+ coor[dir].x] != -10 && data->enemies[x]->vie )
 		    data->directions[data->enemies[x]->y+ (2*coor[dir].y)][data->enemies[x]->x+ (2*coor[dir].x)]=-10;

		 if(data->directions[data->enemies[x]->y+ coor[dir].y][data->enemies[x]->x+ coor[dir].x]== -2 && data->enemies[x]->vie)
                    data->directions[data->enemies[x]->y+ coor[dir].y][data->enemies[x]->x+ coor[dir].x]=-10;
         }
    }

    data->directions[data->heroe.y][data->heroe.x]=-4;

    /*tempsActuel = SDL_GetTicks();
    if (tempsActuel - tempsPrecedent > 10000){
        data->hide=0;
        data->heroe.speed*=1.1;
        tempsPrecedent = tempsActuel;
    }
*/
    while(data->directions[c.y][c.x] != item){

    if(!acc){
        if(item == -7){
	if(!deuxieme_tour){deuxieme_tour=1; return bfs_h(data,e,-5);}
        else return -1;
        }
        else if(item == -5)return bfs_h(data,e,-6);
        else if(item == -6)return bfs_h(data,e,-7);
    }
        a= pop();

       for(dir =0;dir < NDIR;dir++){

        for(x=0;x <6;x++)
             if( (e->y+coor[dir].y==data->enemies[x]->y) && (e->x+coor[dir].x==data->enemies[x]->x) && (e->speed < data->enemies[x]->speed) && !data->hide){
                data->pull =1;
                return -1;
             }
            x = a.x + coor[dir].x;
            y = a.y + coor[dir].y;
            if(data->directions[y][x]==item && valid_pos(data,x,y)){

                if(data->directions[a.y][a.x]==-4 && valid_pos(data,x,y))
                    return data->heroe.dir =dir;
                return  data->heroe.dir =data->directions[a.y][a.x];
            }

            if((data->directions[y][x] ==-2 ||data->directions[y][x] ==-6 ||data->directions[y][x] ==-7) && valid_pos(data,x,y)){
                c.x=x;
                c.y=y;
            if(data->directions[a.y][a.x]==-4 && valid_pos(data,x,y) ){
                data->directions[c.y][c.x]=dir;
            }
            else
                data->directions[c.y][c.x]=data->directions[a.y][a.x];
            push(c);

           }

        }
        acc--;
}

return -1;

 }

int bfs_pull(Data *data,Entity *e,int item){ // item = 9 || 10
 Point a,c;
    int dir,acc=WIDTH*HEIGHT;
    int x,y;
     fin = -1;
     clear_search(data,2);

    c.x=e->x;
    c.y=e->y;

    push(c);
    data->directions[data->heroe.y][data->heroe.x]=-4;

    while(data->directions[c.y][c.x] != item){

    if(!acc)
       return -1;

        a= pop();

       for(dir =0;dir < NDIR;dir++){

            x = a.x + coor[dir].x;
            y = a.y + coor[dir].y;
            if(data->directions[y][x]==item && valid_pos(data,x,y)){

                if(data->directions[a.y][a.x]==-4 && valid_pos(data,x,y))
                    return data->heroe.dir =dir;

                return  data->heroe.dir =data->directions[a.y][a.x];
            }

            if((data->directions[y][x] ==-2 ||data->directions[y][x] ==10) && valid_pos(data,x,y)){
                c.x=x;
                c.y=y;
            if(data->directions[a.y][a.x]==-4 && valid_pos(data,x,y) ){
                data->directions[c.y][c.x]=dir;
            }
            else
                data->directions[c.y][c.x]=data->directions[a.y][a.x];
            push(c);

           }

        }
        acc--;
}

return -1;

 }


void board(Data* data) {

  int x, y,z,nb_gems,nb_teleporter,nb_maze_modifier;
            SDL_Rect position;
if(data->invincibility) data->invincibility--;

	if(data->bouge_toi){
	draw_murs(data,data->tab_item[1]);
	}


     tempsActuel2 = SDL_GetTicks();
  	if (tempsActuel2 - tempsPrecedent2 > 1000){
if(data->heroe.vie == 1) Mix_PlayChannel(-1,data->low_health_music, 0); //Mix_PlayMusic(data->low_health_music, 0);
	tempsPrecedent2 = tempsActuel2;
    }


	 for(x=0;x <6;x++){
        if(!data->enemies[x]->dir) data->enemies[x]->Rect.y= 3*32;
        else if (data->enemies[x]->dir==2) data->enemies[x]->Rect.y = 2*32;
        else if (data->enemies[x]->dir==1) data->enemies[x]->Rect.y = 0;
        else if (data->enemies[x]->dir==3) data->enemies[x]->Rect.y = 32;

    }

    if(!data->heroe.dir) data->heroe.Rect.y= 3*32;
    else if (data->heroe.dir==2) data->heroe.Rect.y = 2*32;
    else if (data->heroe.dir==1) data->heroe.Rect.y = 0;
    else if (data->heroe.dir==3) data->heroe.Rect.y = 32;








    frameTime++;
    if(FPS / frameTime == 4 ){
    frameTime=1;
    data->heroe.Rect.x += frameWidth;
    for(x=0;x <6;x++)
        data->enemies[x]->Rect.x += frameWidth;
    if(data->heroe.Rect.x >= textureWidth){
        data->heroe.Rect.x =0;
        for(x=0;x <6;x++)
            data->enemies[x]->Rect.x=0;

    }
    }


     //SDL_BlitSurface (data->img_ange,&playerRect, data->img_screen, &position);
 if(data->blit_all){
     for(x = 0; x < WIDTH; x++) {
       for(y = 0; y < HEIGHT; y++) {
          data->board[y][x] = data->maze[y][x];
        }
      }

      nb_maze_modifier = data->ent_count[MAZE_MODIFIER];
      nb_gems = data->ent_count[GEM];
      nb_teleporter = data->ent_count[TP];
      add_items(data, nb_gems,GEM);
      add_items(data,nb_teleporter,TP);
      add_items(data,nb_maze_modifier,MAZE_MODIFIER);

       for(x = 0; x < WIDTH; x++) {
        for(y = 0; y < HEIGHT; y++) {

          position.x = x * ELMTSIZE;
          position.y = y * ELMTSIZE;

          SDL_BlitSurface (data->tab_item[data->board[y][x]], NULL, data->img_screen, &position);

        }
       }
    }
    data->blit_all=0;

  if(data->clear){
    for(x = 0; x < WIDTH; x++) {
       for(y = 0; y < HEIGHT; y++) {

           position.x = x * ELMTSIZE;
           position.y = y * ELMTSIZE;

           data->board[y][x] = EMPTY;
           SDL_BlitSurface (data->tab_item[0], NULL, data->img_screen, &position);
           for(z = 0; z < NENT; z++)
              data->ent_count[z] = 0;
      }
    }
  }

  data->clear=0;


    for(x=0;x<5;x++){
         position.x = (data->heroe.old_x+coor[x].x) * ELMTSIZE;
         position.y = (data->heroe.old_y+coor[x].y)* ELMTSIZE;
         if(data->heroe.old_x+coor[x].x >= 0 && data->heroe.old_x+coor[x].x < WIDTH && data->heroe.old_y+coor[x].y >= 0 && data->heroe.old_y+coor[x].y < HEIGHT)
            SDL_BlitSurface (data->tab_item[data->board[data->heroe.old_y+coor[x].y][data->heroe.old_x+coor[x].x]], NULL, data->img_screen, &position);

         for(y=0;y <6;y++){
             position.x = (data->enemies[y]->old_x+coor[x].x) * ELMTSIZE;
             position.y = (data->enemies[y]->old_y+coor[x].y) * ELMTSIZE;
             if( data->enemies[y]->vie && data->enemies[y]->old_x+coor[x].x >= 0 && data->enemies[y]->old_x+coor[x].x < WIDTH && data->enemies[y]->old_y+coor[x].y >= 0 && data->enemies[y]->old_y+coor[x].y < HEIGHT)
                SDL_BlitSurface (data->tab_item[data->board[data->enemies[y]->old_y+coor[x].y][data->enemies[y]->old_x+coor[x].x]], NULL, data->img_screen, &position);
         }

    }


//if(data->bouge_toi){for(x=0;x<WIDTH;x++){  position.x = (data->heroe.x + x*coor[data->dir_wall].x) * ELMTSIZE;position.y = (data->heroe.y + x*coor[data->dir_wall].y) * ELMTSIZE;SDL_BlitSurface (data->tab_item[data->board[(data->heroe.y + x*coor[data->dir_wall].y) * ELMTSIZE][(data->heroe.x + x*coor[data->dir_wall].x) * ELMTSIZE]], NULL, data->img_screen, &position);}}




     position.y = 0;
for(x=0;x<data->heroe.vie+2;x++){
   position.x = x * ELMTSIZE;
   SDL_BlitSurface (data->tab_item[data->board[0][x]], NULL, data->img_screen, &position);
}


for(x=0;x<data->heroe.vie;x++){
         position.x = 37 + (x * coor[2].x) * ELMTSIZE;
         position.y = 5 + (x * coor[2].y) * ELMTSIZE;

SDL_BlitSurface (data->img_coeur, NULL, data->img_screen, &position);
}

   if(data->enemies[0]->vie)  draw_entity(data, data->enemies[0], data->img_demon);
   if(data->enemies[1]->vie)  draw_entity(data, data->enemies[1], data->img_demon2);
   if(data->enemies[2]->vie)  draw_entity(data, data->enemies[2], data->img_demon3);
   if(data->enemies[3]->vie)  draw_entity(data, data->enemies[3], data->img_demon4);
   if(data->enemies[4]->vie)  draw_entity(data, data->enemies[4], data->img_demon5);
   if(data->enemies[5]->vie)  draw_entity(data, data->enemies[5], data->img_demon6);


  if(data->hide){
        draw_entity(data, &data->heroe, data->img_herb_camouflage);
    return;
}
  //if(!data->sword){
    //if(!data->pull)
      if(!data->gender) draw_entity(data, &data->heroe,data->img_ange ); //data->img_heroe[data->heroe.dir]
      else draw_entity(data, &data->heroe,data->img_angette );
  //}



/*

  else{
        draw_entity(data, &data->heroe,data->img_heroe_sword[data->heroe.dir]);
        x = rand() % NENT;
        position.x =data->heroe.x * ELMTSIZE;
        position.y = data->heroe.y* ELMTSIZE;
        if(data->board[data->heroe.y][data->heroe.x] == EMPTY){

            if(x==6){
                data->board[data->heroe.y][data->heroe.x]=GEM_HERB_CUT;
                data->ent_count[GEM]++;
                SDL_BlitSurface (data->tab_item[6], NULL, data->img_screen, &position);
            }
            else if(x==7){
                data->board[data->heroe.y][data->heroe.x]=FAIRY_HERB_CUT;
                data->ent_count[FAIRY_HERB_CUT]++;
                 SDL_BlitSurface (data->tab_item[7], NULL, data->img_screen, &position);
            }
            else {
                 data->board[data->heroe.y][data->heroe.x]=HERB_CUT;
                 SDL_BlitSurface (data->tab_item[5], NULL, data->img_screen, &position);
            }
        }


  }*/
}

void get_gem(Data* data) {

    if(data->heroe.count_down <20){
  if(data->board[data->heroe.y][data->heroe.x] == GEM ) {
    data->board[data->heroe.y][data->heroe.x] = EMPTY;
      data->ent_count[GEM]--;
    if(data->heroe.vie <20)
      data->heroe.vie++;
Mix_PlayChannel(-1,data->rupee_music, 0);
//Mix_PlayMusic(data->rupee_music, 0);
  }


    }
}


void loop_level1(Data* data) {

//Mix_PlayMusic(data->ambient_music, -1);
int tmp_xh=0,tmp_yh=0,tmp_dir=0,exchange =0,x,z;
float resx,resy;

  //SDL_FillRect(data->img_screen, NULL,SDL_MapRGB(data->img_screen->format, 72,152,72));
  	if (!data->invincibility)
 	   for(x=0;x <6;x++){
            resx = fabs(data->heroe.position.x - data->enemies[x]->position.x);
            resy = fabs(data->heroe.position.y - data->enemies[x]->position.y);

           if(resx < 15.0f && resy < 15.0f && data->enemies[x]->vie ) //more realistic collision.
     {data->heroe.vie--; data->invincibility = FRAME_INVINCIBILITE;break;} //if(data->heroe.x == data->enemies[x]->x && data->heroe.y == data->enemies[x]->y && !data->hide)

	  }

		if(data->invincibility > FRAME_INVINCIBILITE-2){

		   if (!data->heroe.dir) x= 1;
                    else if (data->heroe.dir==1) x=0;
                    else if (data->heroe.dir==2) x=3;
                    else if (data->heroe.dir==3) x=2;
  		   //data->heroe.speed=6.8;
		    data->heroe.count_down =-1;
 		    move_entity(data, &data->heroe,x);

		    data->heroe.count_down =-1;
Mix_PlayChannel(-1,data->hurt_music, 0);
	//Mix_PlayMusic(data->hurt_music, 0);
		}
		 // data->heroe.speed=(float) 60/34;


  board(data);



  deuxieme_tour=0;
  SDL_Flip(data->img_screen);
  SDL_Event event;
  if ((data->heroe.x == data->heroe.basex && data->heroe.y == data->heroe.basey)&& data->pull){
                        if (!data->heroe.dir) data->heroe.dir =1;
                    else if (data->heroe.dir==1) data->heroe.dir=0;
                    else if (data->heroe.dir==2) data->heroe.dir=3;
                    else if (data->heroe.dir==3) data->heroe.dir=2;
     data->heroe.speed=(float)60/34;
     data->pull =0;
     data->heroe.count_down=-1;
     data->wall_hack_heroe=0;
}

  while(SDL_PollEvent(&event)) {

    switch (event.type) {


     case SDL_MOUSEBUTTONDOWN:
      if(event.button.button == SDL_BUTTON_LEFT && event.button.state == SDL_PRESSED){

               if(data->editor_mode==1){
                    int a,b;
                    a= event.button.x/ELMTSIZE;
                    b=event.button.y/ELMTSIZE;
                    if(data->editeur){
                        printf("Wich item do you want ? Enter a number \n");
                        scanf("%d", &data->item);
                        while(data->item < 0 || data->item >8){
                            if (!(data->item >= 0 && data->item <= 8)){
                               printf("Enter a valid answer \n");
                               printf("0 == EMPTY, 1== WALL, 2== GEM, 3== TP, 4== MAZE_MODIFIER , 5==HERB_CUT,6 ==GEM_HERB_CUT,7==FAIRY_HERB_CUT ,8==BUISSON_HERB_CUT. \n");
                               scanf("%d", &data->item);
                            }
                       }
                       data->editeur=0;
                    }

                  else add_items_pos(data,data->item,a,b);

                }
          }
            break;
      case SDL_KEYDOWN:
        switch(event.key.keysym.sym) {
          case SDLK_RIGHT:
                       if(!data->pull){
             if(!data->keyboard_state[0] && !data->keyboard_state[1] && !data->keyboard_state[2] && !data->keyboard_state[3]) {
                if(data->keyboard_state[0] || data->keyboard_state[1] || data->keyboard_state[2] || data->keyboard_state[3])
                    break;
                data->keyboard_state[2]=1;

                if(data->heroe.dir !=2){
                   data->heroe.dir =2;
                   data->heroe.count_down=-1;

                }
             }
                       }
                break;


          case SDLK_LEFT:
              if(!data->pull){
                if(!data->keyboard_state[0] && !data->keyboard_state[1] && !data->keyboard_state[2] && !data->keyboard_state[3]) {
                   if(data->keyboard_state[0] || data->keyboard_state[1] || data->keyboard_state[2] || data->keyboard_state[3])
                        break;
                    data->keyboard_state[3]=1;

                   if(data->heroe.dir !=3){
                      data->heroe.dir =3;
              //        data->wall.count_down=-1;
                      data->heroe.count_down=-1;

                   }

                }
              }
               break;

          case SDLK_UP:
                       if(!data->pull){
               if(data->keyboard_state[0] || data->keyboard_state[1] || data->keyboard_state[2] || data->keyboard_state[3])
                    break;
             if(!data->keyboard_state[0] && !data->keyboard_state[1] && !data->keyboard_state[2] && !data->keyboard_state[3]) {

                data->keyboard_state[0]=1;

                if(data->heroe.dir){
                   data->heroe.dir =0;
             //      data->wall.count_down=-1;
                  data->heroe.count_down=-1;
                }
             }
                       }
                break;

          case SDLK_DOWN:
                       if(!data->pull){
        if(data->keyboard_state[0] || data->keyboard_state[1] || data->keyboard_state[2] || data->keyboard_state[3])
                    break;
             if(!data->keyboard_state[0] && !data->keyboard_state[1] && !data->keyboard_state[2] && !data->keyboard_state[3]) {


              data->keyboard_state[1]=1;

            if(data->heroe.dir !=1){
               data->heroe.dir =1;
            //   data->wall.count_down=-1;
               data->heroe.count_down=-1;
            }
             }
                       }
            break;

          case SDLK_h://h for hack.
          if(data->wall_hack_heroe) data->wall_hack_heroe--;
          else data->wall_hack_heroe++;
          break;

          case SDLK_e: //e for enemy hack.
          if(data->wall_hack_enemy)
            data->wall_hack_enemy--;
          else data->wall_hack_enemy++;
          break;

          case SDLK_g: //g for gem.
          add_items(data,5,GEM);
          if(data->ent_count[GEM]>=31){
            data->ent_count[GEM]=31;
            break;
          }
         else data->ent_count[GEM] += 5;
          break;

          case SDLK_t: //t for teleporter.
          add_items(data,1,TP);
          data->ent_count[TP] ++;
          break;

	  case SDLK_v: //v for "vie".
	  if(data->heroe.vie <20)
             data->heroe.vie++;
          break;

          case SDLK_m: //m for maze_modifier.
          add_items(data,1,MAZE_MODIFIER);
          data->ent_count[MAZE_MODIFIER] ++;
          break;

          case SDLK_r: //r for reset.
          data->clear=1;
          data->editor_mode=1;
          break;

          case SDLK_o: //o for outspeed.
            if(data->speed_hack){
              data->heroe.speed=1.7;
              data->speed_hack--;
            }
              else{
               data->heroe.speed=6.8;
               data->speed_hack++;
               }

          break;


          case SDLK_f: //f for fu** , I already set e so I can't use it for "editor".
          data->editeur=1;
          break;

          case SDLK_q: //q for quit editor_mode.
          data->editor_mode=0;
          break;



          case SDLK_c: //c for camouflage.
          if(data->sword)
            data->sword--;
          if(data->hide)
            data->hide--;
          else data->hide++;
          break;

          case SDLK_a: //a for attack.
          if(data->hunt)
            data->hunt--;
          else data->hunt++;
          break;

          case SDLK_p: //p for pull;
          if(!data->pull)
            data->pull++;
          break;

          case SDLK_d: //d for destroy;
              if(!data->bouge_toi)
                 if(data->board[data->heroe.y +coor[data->heroe.dir].y][data->heroe.x+ coor[data->heroe.dir].x]== WALL) {
               //data->mange_moi=1;
               data->dir_wall=data->heroe.dir;
               data->heroe.wallx=data->heroe.x;
               data->heroe.wally=data->heroe.y;
               data->bouge_toi=1;
               }
          break;


          case SDLK_b:
          if(!data->gender){
            data->gender++;
	      Mix_PlayChannel(-1,data->hey_music, 0);
	   // Mix_PlayMusic(data->hey_music, 0);
          }
        else{ data->gender--;Mix_PlayChannel(-1,data->hey2_music, 0);}// Mix_PlayMusic(data->hey2_music, 0);
          break;


          default: break;
        }

 case SDL_KEYUP:
        switch(event.key.keysym.sym) {
          case SDLK_RIGHT:
              data->keyboard_state[2]=0;

              break;

          case SDLK_LEFT:
              data->keyboard_state[3]=0;

               break;

          case SDLK_UP:
               data->keyboard_state[0]=0;

                break;

          case SDLK_DOWN:
              data->keyboard_state[1]=0;

            break;
         default: break;
        }
       exchange =0;
       get_gem(data);

       break;
       default: break;
    }


  }
if(data->bouge_toi)if(data->wall_cd <0){move_mur(data);}


if(data->pull){

   data->heroe.speed=5.0;
   data->wall_hack_heroe=1;

  if(data->heroe.count_down<0)
    move_entity(data, &data->heroe,bfs_pull(data,&data->heroe,10));

    return;

}

if(data->invincibility < FRAME_INVINCIBILITE -3){
  if(!data->hunt){
    if(data->heroe.count_down<0){
       move_entity(data, &data->heroe,data->heroe.dir);
    }

 for(x=0;x <6;x++)
    if(data->enemies[x]->count_down < 0 && data->enemies[x]->vie) {
       move_entity(data, data->enemies[x], bfs(data, data->enemies[x]));
    }
 }

  else{
        if(data->heroe.count_down<0){

     move_entity(data, &data->heroe,bfs_h(data,&data->heroe,-5));

    }

for(x=0;x <6;x++)
    if(data->enemies[x]->count_down < 0 && data->enemies[x]->vie) {

        move_entity(data, data->enemies[x], bfs(data, data->enemies[x]));

    }

  }

}
        get_gem(data);
           if(end_of_game(data)){
        SDL_Flip(data->img_screen);
        printf("Well played, now press f5 to play again \n");
        data->blit_all=1;
       data->screen = GAMEOVER;
       }

}
void loop_game_over(Data* data) {




 board_game_over(data);
  //SDL_FillRect(data->img_screen_game_over, NULL, SDL_MapRGB(data->img_screen_game_over->format, a, b, c));

 SDL_Flip(data->img_screen_game_over);

 SDL_Event event;
  if(SDL_PollEvent(&event)) {

    switch (event.type) {

      case SDL_KEYDOWN:
        switch(event.key.keysym.sym) {
           case SDLK_F5:
            SDL_Flip(data->img_screen);
            init_data(data);
            data->screen = LEVEL1;
            break;

        default: break;
    }
     default: break;
    }

  }
}

void renderloop(void* arg) {

  Data* data = arg;

  switch(data->screen) {
    case LEVEL1:
      loop_level1(data);
      break;
    case GAMEOVER:
      loop_game_over(data);

      break;
    default: break;
  }
}

int main() {
  Data* data = malloc(sizeof *data);
  SDL_Init(SDL_INIT_VIDEO);
  srand((unsigned)time(NULL));

  int index = 0;
  int backtrack_x[CELL];
  int backtrack_y[CELL];
  init_maze(data);
  backtrack_x[index] = 0;
  backtrack_y[index] = 0;
  maze_generator(index, data, backtrack_x, backtrack_y, 1,1,1);
  init_load(data);
  init_data(data);
  printf("Press r to enter the level editor,then press f to choose an item, then you can add items throughout the map by mouse clicking on the map with the left button. Press f again to change your item selection. Press q to quit the level editor. \n");
  printf("0 == EMPTY, 1== WALL, 2== GEM, 3== TP, 4== MAZE_MODIFIER , 5==HERB_CUT,6 ==GEM_HERB_CUT,7==FAIRY_HERB_CUT ,8==BUISSON_HERB_CUT. \n");
  printf("\n");
  //printf("Press x to exchange your position with the enemy position\n");
  printf("Press a to start the heroe AI \n");
  printf("Press b to change your gender \n");
  printf("Press h to enable wall hack for your heroe\n");
  printf("Press e to enable wall hack for the enemy\n");
  printf("Press g to add 5 gems throughout the map \n");
  printf("Press t to add 1 teleporter throughout the map \n");
  printf("Press m to add 1 maze modifier throughout the map \n");
  printf("Press c to hide the heroe from enemies \n");
  //printf("Press s to bring your sword and cut herbs \n");
  printf("Press p to return your heroe at his original position \n");
Mix_PlayChannel(0,data->ambient_music, -1);
Mix_Volume(0, MIX_MAX_VOLUME/4);
  emscripten_set_main_loop_arg(renderloop, data,0,0);


  return 0;
}


