/* SDL Project By Zarca Dan, L2-B , 13402677 , 10/12/15 */

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

#define FRAME_INVINCIBILITE 90 // let's assume it's a 60 FPS game so it's approximatively 2 seconds of invincibility.
#define CELL 900
#define NB_ENEMY_MAX 10 // Can't have more than 10 enemies.


int true_dir = -1; //Used to set the right direction when moving.

const int FPS = 60; // Used to set the sprites animations speed.Lower it is, faster it goes.

/* Different variables used for the sprite animation*/
int frameTime = 1;
int frameWidth,frameHeight;
int textureWidth,textureHeigh;

SDL_Rect positionz;
SDL_Rect RectB;
SDL_Rect positionB;

int frameWidthB,frameHeightB;
int textureWidthB,textureHeighB;

int textureWidthz;
int textureHeighz;
int frameWidthz;
int frameHeightz;

/* Some timers */
int tempsPrecedent = 0, tempsActuel = 0;
int tempsPrecedent2 = 0, tempsActuel2 = 0;
int tempsPrecedent3 = 0, tempsActuel3 = 0;
int tempsPrecedent4 = 0, tempsActuel4 = 0;
int tempsPrecedent5 = 0, tempsActuel5 = 0;

int deuxieme_tour=0;
int true_vie = 4;

static int fin = -1; // The file start

enum { WIDTH = 20};
enum { HEIGHT = 20 };
enum { ELMTSIZE = 34 };
enum { EMPTY, WALL, GEM, TP,MAZE_MODIFIER,BROKEN_WALL,PORTE,NENT };
enum { LEVEL1, GAMEOVER, NSCREEN};
enum { NORTH, SOUTH, EAST, WEST, NDIR };


/* The game over board */
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

/* The transition board */
int level2[HEIGHT][WIDTH] = {
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 3, 6, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}
};

typedef struct {
    int x;
    int y;
} Point;

Point coor[] = { {0, -1}, {0, 1}, {1, 0}, {-1, 0}, {0, 0}};

/* Used to store floating coordinates, mainly for collisions */
typedef struct {
    float x;
    float y;
} Pointf;

static Point file[256];


/* The Entity structure contains some variable who alternate between 0 or 1 to enable or disable some condition*/
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
    int mange_moi;
    int wall_hack_enemy; // The variable that say if an enemy can destroy a wall or not;
    SDL_Rect Rect;
    Pointf position;
} Entity;

/* The main structure, contains all the variables (not really, since we already saw a bunch of global variables at the begining )*/
typedef struct {

/* The surface who will contain the image */
    SDL_Surface* img_screen;
    SDL_Surface* img_screen_game_over;
    SDL_Surface* img_wall2;
    SDL_Surface* img_herb_camouflage;
    SDL_Surface* img_coeur;
    SDL_Surface* img_ange;
    SDL_Surface* img_angette;
    SDL_Surface* img_boum;
    SDL_Surface* numbers;
    SDL_Surface* score;
    SDL_Surface* level;
    SDL_Surface** tab_enemies;
    SDL_Surface** tab_item;
/* The sound */
    Mix_Chunk *ambient_music;
    Mix_Chunk *hurt_music;
    Mix_Chunk *rupee_music;
    Mix_Chunk *hey_music;
    Mix_Chunk *hey2_music;
    Mix_Chunk *portal_music;
    Mix_Chunk *low_health_music;
    Mix_Chunk *wall_music;
    Mix_Chunk *enemy_death_music;

    Entity** enemies;
    Entity hero;

    int maze[HEIGHT][WIDTH];
    int directions[HEIGHT][WIDTH];
    int board[HEIGHT][WIDTH];
    int ent_count[NENT];

    float wall_cd; // The wall velocity variable.

/*  Enable/Disable variables (o || 1) */
    int blit_all;
    int sword;
    int hide;
    int wall_hack_hero;
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
    int bouge_toi;
    int dir_wall;
    int boucle_mur;
    int nb_enemy_down;
    int nb_enemy;
    int screen;
    int WinOrLoss;
    int doubleWV;
    int broken_blit; // Blit the broken_walls are replace them with empty.
    int transition;
    int player_score; // The player score.
    int current_level; // The current level of difficulty.
    int whichEnemy;
    int ne_me_mange_pas;

} Data;

/* File fonctions Push and Pop */

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


/* All the maze stuff,initialization and creation.I unfortunately had no time to make it proper*/

void init_maze(Data * data){

    int a,b;
    for(a = 0; a <= WIDTH  ; a++){
        for(b = 0; b <= HEIGHT ; b++){
            if(a % 2 == 0 || b % 2 == 0)
                data->maze[b][a] = WALL;
            else
                data->maze[b][a] = EMPTY;
        }
    }
}
int is_closed(Data * data, int x, int y){
    if(data->maze[x - 1][y] == WALL
    && data->maze[x][y - 1] == WALL
    && data->maze[x][y + 1] == WALL
    && data->maze[x + 1][y] == WALL
    )
    return 1;

    return 0;
}

void maze_generator(int index,Data * data, int backtrack_x[CELL], int backtrack_y[CELL], int x, int y, int visited){
    if(visited <  100){
    int neighbour_valid = -1;
    int neighbour_x[4];
    int neighbour_y[4];
    int step[4];

    int x_next;
    int y_next;

    if(x - 2 > 0 && is_closed(data, x - 2, y)){  // upside
        neighbour_valid++;
        neighbour_x[neighbour_valid]=x - 2;
        neighbour_y[neighbour_valid]=y;
        step[neighbour_valid]=1;
    }

    if(y - 2 > 0 && is_closed(data, x, y - 2)){ // leftside
        neighbour_valid++;
        neighbour_x[neighbour_valid]=x;
        neighbour_y[neighbour_valid]=y - 2;
        step[neighbour_valid]=2;
    }

    if(is_closed(data, x, y + 2)){  // rightside
        neighbour_valid++;
        neighbour_x[neighbour_valid]=x;
        neighbour_y[neighbour_valid]=y + 2;
        step[neighbour_valid]=3;
    }

    if(is_closed(data, x + 2, y)){  // downside
        neighbour_valid++;
        neighbour_x[neighbour_valid]=x+2;
        neighbour_y[neighbour_valid]=y;
        step[neighbour_valid]=4;
    }

    if(neighbour_valid == -1){
        x_next = backtrack_x[index];
        y_next = backtrack_y[index];
        index--;
    }

    if(neighbour_valid!=-1){
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

/* Here is the fonction that load an image */
SDL_Surface*load ( char const *filename ){

    SDL_Surface *srfTmp = NULL;
    SDL_Surface *srfDst = NULL;

    srfTmp = SDL_LoadBMP ( filename );
    if ( srfTmp != NULL ){
        srfDst = SDL_DisplayFormatAlpha ( srfTmp );
    }

    SDL_FreeSurface ( srfTmp ); // Don't forget freeing the memory !

    return srfDst;
}

/* Very simple, if lives comes to 0, the player loose the game but if the number of enemies is 0, he wins. Then he can go to the next level*/
int end_of_game(Data* data) {

    if(!data->hero.vie){
        data->WinOrLoss = 0;
          return 1;
    }
     else
        return data->WinOrLoss = data->nb_enemy_down == data->nb_enemy && data->editor_mode==0;
}

/* Add randomly 1 item of the type given by the variable "item" throughout the board*/
void add_items(Data* data, int nb,int item) {
    int i,fin=0;
    SDL_Rect position;

    for(i = 0; i < nb; i++) {
        fin++;
        if(fin == HEIGHT*WIDTH)
            return;
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;
        if(data->board[y][x] == EMPTY  && (y != data->hero.y || x != data->hero.x )) {
            data->board[y][x] = item;
            position.x = x* ELMTSIZE;
            position.y = y* ELMTSIZE;
            SDL_BlitSurface (data->tab_item[item], NULL, data->img_screen, &position);
        }
        else {
            i--;
        }
    }
}
/* Add randomly 1 item of the type given by the variable "item" at the y:x coordinate */
void add_items_pos(Data* data,int item,int x,int y) {
    SDL_Rect position;
    if(data->board[y][x])
        data->ent_count[data->board[y][x]]--;

    if(y != data->hero.y || x != data->hero.x){
        data->board[y][x] = item;
        position.x = x* ELMTSIZE;
        position.y = y*ELMTSIZE;
        SDL_BlitSurface (data->tab_item[item], NULL, data->img_screen, &position);
    }

        data->ent_count[item]++;
}

/* The player loss the game, this is the game over screen */
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
/* Here we load all the data once and set variables once for a further use */
void init_load(Data* data){

    int x;
    SDL_WM_SetCaption("My game,not the best one but I still love it.", NULL);
    data->img_screen = SDL_SetVideoMode(WIDTH * ELMTSIZE, HEIGHT * ELMTSIZE, 32, SDL_ANYFORMAT);
    data->img_screen_game_over = SDL_SetVideoMode(WIDTH * ELMTSIZE, HEIGHT * ELMTSIZE, 32, SDL_ANYFORMAT);
    data->img_coeur = load("/Pacman/heart.png");
    data->img_herb_camouflage=load("/Pacman/camouflage.png");
    data->nb_enemy = 0;
    data->WinOrLoss = 0;

    data->tab_item=NULL;
    data->tab_item=(SDL_Surface**)malloc(sizeof(SDL_Surface*) * NENT);

    data->tab_enemies=NULL;
    data->tab_enemies=(SDL_Surface**)malloc(sizeof(SDL_Surface*) * 5);

    data->enemies=(Entity**)malloc(sizeof(Entity*)*NB_ENEMY_MAX);

    for(x =0;x<NB_ENEMY_MAX;x++)
        data->enemies[x]=(Entity*)malloc(sizeof(Entity));

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1){ //Initialisation de l'API Mixer
        printf("%s", Mix_GetError());
    }

    Mix_AllocateChannels(9);
    data->ambient_music = Mix_LoadWAV("/Pacman/musique.mp3");
    data->hurt_music= Mix_LoadWAV("/Pacman/MC_Link_Hurt.wav");
    data->rupee_music= Mix_LoadWAV("/Pacman/MC_Rupee.wav");
    data->hey_music= Mix_LoadWAV("/Pacman/MC_Zelda_Hey.wav");
    data->hey2_music= Mix_LoadWAV("/Pacman/MC_Minish_Hey1.wav");
    data->portal_music= Mix_LoadWAV("/Pacman/Oracle_Dungeon_Teleport.wav");
    data->low_health_music= Mix_LoadWAV("/Pacman/MC_LowHealth.wav");
    data->enemy_death_music= Mix_LoadWAV("/Pacman/Oracle_Enemy_Die.wav");
    data->wall_music= Mix_LoadWAV("/Pacman/OOT_Boulder_Loop.wav");

    data->tab_item[0] = load("/Pacman/herbe.png");
    data->tab_item[1] = load("/Pacman/buisson_herbe.png");
    data->tab_item[2] = load("/Pacman/gem_herbe.png");
    data->tab_item[3] = load("/Pacman/teleporteur.png");
    data->tab_item[4] = load("/Pacman/maze_modifier.png");
    data->tab_item[5] = load("/Pacman/broken_wall.png");
    data->tab_item[6] = load("/Pacman/porte.png");

    data->img_ange    = load("/Pacman/ange.png");
    data->img_angette = load("/Pacman/angette.png");

    data->tab_enemies[0]    = load("/Pacman/demon.png");
    data->tab_enemies[1]  = load("/Pacman/demon2.png");
    data->tab_enemies[2]  = load("/Pacman/demon3.png");
    data->tab_enemies[3]   = load("/Pacman/demon4.png");
    data->tab_enemies[4]  = load("/Pacman/demon5.png");
    data->tab_enemies[5]   = load("/Pacman/demon6.png");

    data->img_boum = load("/Pacman/explosion.png");

    data->numbers   = load("/Pacman/white_digits.png");
    data->level     = load("/Pacman/level.png");
    data->score     = load("/Pacman/score.png");

    textureWidthB=data->img_boum->w;
    textureHeighB=data->img_boum->h;
    frameWidthB = textureWidthB /9;
    frameHeightB = textureHeighB;
    RectB.h = frameHeightB;
    RectB.w = frameWidthB;
    RectB.x = RectB.y =0;

    textureWidth=data->img_ange->w;
    textureHeigh=data->img_ange->h;
    frameWidth = textureWidth /3;
    frameHeight= textureHeigh /4;
    data->hero.Rect.h = frameHeight;
    data->hero.Rect.w = frameWidth;
    data->hero.Rect.x = data->hero.Rect.y =0;

    textureWidthz=data->numbers->w;
    textureHeighz=data->numbers->h;
    frameWidthz = textureWidthz /10;
    frameHeightz= textureHeighz;

    positionz.h = frameHeightz;
    positionz.w = frameWidthz;
    positionz.x = 0;
    positionz.y = 0;

    for(x=0;x <NB_ENEMY_MAX;x++){
        data->enemies[x]->Rect.x = data->enemies[x]->Rect.y =0;
        data->enemies[x]->Rect.h = frameHeight;
        data->enemies[x]->Rect.w = frameWidth;
        data->enemies[x]->vie =  1;
    }
}

/* Here we initialize all the variables contained in the data structure*/
void init_data(Data* data) {

    int x,y,z,nb_gems=5,nb_teleporter=5,nb_maze_modifier=1;

    for(x = 0; x < WIDTH; x++) {
        for(y = 0; y < HEIGHT; y++) {
            data->board[y][x] = data->maze[y][x];
        }
    }

    for(x=0;x <NB_ENEMY_MAX;x++){

        data->enemies[x]->vie = 1;
        data->enemies[x]->mange_moi = 0;
        data->enemies[x]->dir =1;
        if(data->WinOrLoss){
            if(data->enemies[x]->speed < 2.125)
                data->enemies[x]->speed *= 1.1 ;
            else
                data->enemies[x]->speed = 2.125;
        }
        else  data->enemies[x]->speed = 1;
        data->enemies[x]->count_down = -1;
        data->enemies[x]->wall_hack_enemy = 0;
    }

    data->hero.dir = 0;
    data->hero.x = WIDTH/2-1;
    data->hero.y = HEIGHT-1;
    data->hero.position.x = data->hero.x * ELMTSIZE;
    data->hero.position.y = data->hero.y * ELMTSIZE;
    data->hero.speed = 2.125;
    data->hero.basex=WIDTH/2-1;
    data->hero.basey=HEIGHT-1;
    data->invincibility=0;
    data->hide=0;
    data->doubleWV =1;
    data->gender =0;
    data->speed_hack=0;
    data->pull=0;
    if(!data->WinOrLoss){
    data->hero.vie = 2;
    true_vie = 4;
    data->player_score = 0;
    data->current_level = 0;
    }
    data->current_level++;
    data->sword=0;
    data->editeur=0;
    data->editor_mode=0;
    data->blit_all=1;
    data->clear=0;
    data->item=0;
    data->ne_me_mange_pas=0;
    data->transition = 1;
    data->hero.count_down = -1;
    data->bouge_toi =0;
    data->broken_blit=0;
    data->dir_wall=0;
    data->wall_hack_hero=0;
    data->hero.wallx=0;
    data->hero.wally=0;
    data->wall_cd = -1;
    data->boucle_mur = 0;
    data->nb_enemy_down=0;
    if(data->nb_enemy < NB_ENEMY_MAX && data->WinOrLoss)
        data->nb_enemy++;
    else
        data->nb_enemy = 1;

    data->hunt=0;

    for(x = 0; x < NENT; x++)
        data->ent_count[x] = 0;

    data->ent_count[GEM] = nb_gems;
    data->ent_count[TP] = nb_teleporter;
    data->ent_count[MAZE_MODIFIER]= nb_maze_modifier;

    z=0;
    data->whichEnemy = rand()%6;

    while(z != NB_ENEMY_MAX){
        x =(rand()% WIDTH)+1;
        y =(rand()% HEIGHT)+1;
        if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && data->board[y][x] == EMPTY && (x != data->hero.x && y != data->hero.y)){
            data->enemies[z]->x=x;
            data->enemies[z]->y=y;
            z++;
        }
    }

    for(x = 0; x < NDIR; x++)
        data->keyboard_state[x]=0;

    data->screen = LEVEL1;
}

/* The function who write the number given in argument. While the number is different than 0,
   we divide it by 10 and use the modulo of the previous number multiplied by the frame width of the image that contain the numbers,
   then we blit the image (the Rect of the image) to the screen,the next blit will be performed a little slither to the left and so on  */

void write_number(Data* data,int number){

    SDL_Rect position;

    position.x = (WIDTH/2+1) * ELMTSIZE;
    position.y =  ELMTSIZE;
    int tmp;

    if(!number){
       positionz.x = 0;
      SDL_BlitSurface (data->numbers, &positionz, data->img_screen, &position);
    }

    while(number){
        position.x += coor[3].x * 15 ;
        positionz.x = 0;

        tmp= number % 10;
        number/=10;

        positionz.x += frameWidthz* tmp;
        SDL_BlitSurface (data->numbers, &positionz, data->img_screen, &position);
    }
}


/* Very big function here, fully commented inside */
void draw_entity(Data* data) {

    SDL_Rect position;
    int x,y;

/* If this is not the transition screen, if a wall is actually moving, we take his floating coordinates and compare them with the floating coordinates of all the enemies,
   if they are approximatively the same,the enemy life become 0 and the player score is increased */

    if(!data->transition)
        if(data->bouge_toi){
            Pointf positionMur;
            float resx,resy;
            positionMur.x = (data->hero.wallx + ((data->boucle_mur+1) * coor[data->dir_wall].x)) * ELMTSIZE - data->wall_cd  * coor[data->dir_wall].x;
            positionMur.y = (data->hero.wally + ((data->boucle_mur+1) * coor[data->dir_wall].y)) * ELMTSIZE - data->wall_cd  * coor[data->dir_wall].y;

            for(x=0;x <data->nb_enemy;x++){
                resx = fabs(positionMur.x - data->enemies[x]->position.x);
                resy = fabs(positionMur.y - data->enemies[x]->position.y);

                if(resx < 20.0f && resy < 20.0f && data->enemies[x]->vie ){
                    Mix_PlayChannel(-1,data->enemy_death_music,0);
                    data->enemies[x]->vie=0;
                    data->nb_enemy_down++;
                    data->player_score+= 100 * data->current_level;

                }
            }
        }

/* If a wall is moving, we blit the screen at the wall position to refresh the board */
    if(data->bouge_toi){
        x=(data->hero.wallx + (data->boucle_mur * coor[data->dir_wall].x));
        y=(data->hero.wally + (data->boucle_mur * coor[data->dir_wall].y));

        position.x = x * ELMTSIZE;
        position.y = y * ELMTSIZE;
        SDL_BlitSurface (data->tab_item[data->board[(data->hero.wally + (data->boucle_mur * coor[data->dir_wall].y))][(data->hero.wallx + (data->boucle_mur  * coor[data->dir_wall].x))]],NULL, data->img_screen, &position);
    }
/* If it is not the transition screen, we refresh all the cells close to the hero position*/
    if(!data->transition)
        for(x=0;x<5;x++){
            position.x = (data->hero.old_x + coor[x].x) * ELMTSIZE;
            position.y = (data->hero.old_y + coor[x].y) * ELMTSIZE;
            if(data->hero.old_x + coor[x].x >= 0 && data->hero.old_x + coor[x].x < WIDTH && data->hero.old_y + coor[x].y >= 0 && data->hero.old_y + coor[x].y < HEIGHT && data->hero.vie)
                SDL_BlitSurface (data->tab_item[data->board[data->hero.old_y + coor[x].y][data->hero.old_x + coor[x].x]], NULL, data->img_screen, &position);
        }

/* If it is not the transition screen, we refresh all the cells close to the enemies position*/
    if(!data->transition)
        for(x=0;x<5;x++){
            for(y=0;y<data->nb_enemy;y++)  {
                position.x = (data->enemies[y]->old_x + coor[x].x) * ELMTSIZE;
                position.y = (data->enemies[y]->old_y + coor[x].y) * ELMTSIZE;
                if(data->enemies[y]->old_x + coor[x].x >= 0 && data->enemies[y]->old_x + coor[x].x < WIDTH && data->enemies[y]->old_y + coor[x].y >= 0 && data->enemies[y]->old_y + coor[x].y < HEIGHT && data->enemies[y]->vie)
                    SDL_BlitSurface (data->tab_item[data->board[data->enemies[y]->old_y + coor[x].y][data->enemies[y]->old_x + coor[x].x]], NULL, data->img_screen, &position);
            }
        }

    position.y = 0;

/* Refresh the cells before the lives are blit*/
    if(!data->transition)
        for(x=0;x<data->hero.vie+2;x++){
            position.x = x * ELMTSIZE;
            SDL_BlitSurface (data->tab_item[data->board[0][x]], NULL, data->img_screen, &position);
        }

/* For each lives, we blit a heart, each consecutive hearts are blit at the right of the previous one */
    for(x=0;x<data->hero.vie;x++){
        position.x = 37 + (x * coor[2].x) * ELMTSIZE;
        position.y = 5 + (x * coor[2].y) * ELMTSIZE;
        SDL_BlitSurface (data->img_coeur, NULL, data->img_screen, &position);
    }

/* The player win the current level, all the board becomes EMPTY and the door the next level appears, so we blit it*/
    if(data->WinOrLoss){
        data->clear=1;
            data->board[0][8]=6;
            position.x = 8 * ELMTSIZE;
            position.y =0;
            SDL_BlitSurface (data->tab_item[data->board[0][8]], NULL, data->img_screen, &position);

    }

/* Drawing the hero slowly, so his speed his based on the frame rate  */
    if(data->hero.count_down > -1) {

        data->hero.position.x = data->hero.x * ELMTSIZE - data->hero.count_down * coor[data->hero.dir].x;
        data->hero.position.y = data->hero.y * ELMTSIZE - data->hero.count_down * coor[data->hero.dir].y;
        position.x = data->hero.x * ELMTSIZE - data->hero.count_down * coor[data->hero.dir].x;
        position.y = data->hero.y * ELMTSIZE - data->hero.count_down * coor[data->hero.dir].y;
        data->hero.count_down-= data->hero.speed;
    }

    else {
        data->hero.position.x = data->hero.x * ELMTSIZE;
        data->hero.position.y = data->hero.y * ELMTSIZE;
        position.x = data->hero.x * ELMTSIZE;
        position.y = data->hero.y * ELMTSIZE;
    }

    /* If the hero invisibility frame is on, we blit the hero with a timer, so he'll blink, cool effect :3, only if the hero is a male, I was too lazy for doing the same for the girl */
    if(!data->hide){
            if(data->invincibility){
                tempsActuel2 = SDL_GetTicks();
                if (tempsActuel2 - tempsPrecedent2 > 30){
                     SDL_BlitSurface (data->gender ? data->img_angette : data->img_ange, &data->hero.Rect, data->img_screen, &position);
                     tempsPrecedent2 = tempsActuel2;
                }
            }
            else
                SDL_BlitSurface (data->gender ? data->img_angette : data->img_ange, &data->hero.Rect, data->img_screen, &position);
    }
    else
        SDL_BlitSurface (data->img_herb_camouflage, NULL, data->img_screen, &position);

/* If it is not the transition screen, if the enemies lives are greater than 0, we blit them slowly, same process than the hero*/
    if(!data->transition)
        for(y=0;y<data->nb_enemy;y++){
            if(data->enemies[y]->vie){
                if(data->enemies[y]->count_down > -1) {
                    data->enemies[y]->position.x = data->enemies[y]->x * ELMTSIZE - data->enemies[y]->count_down * coor[data->enemies[y]->dir].x;
                    data->enemies[y]->position.y = data->enemies[y]->y * ELMTSIZE - data->enemies[y]->count_down * coor[data->enemies[y]->dir].y;
                    position.x = data->enemies[y]->x * ELMTSIZE - data->enemies[y]->count_down * coor[data->enemies[y]->dir].x;
                    position.y = data->enemies[y]->y * ELMTSIZE - data->enemies[y]->count_down * coor[data->enemies[y]->dir].y;
                    data->enemies[y]->count_down-= data->enemies[y]->speed;
                }

                else {
                    data->enemies[y]->position.x = data->enemies[y]->x * ELMTSIZE;
                    data->enemies[y]->position.y = data->enemies[y]->y * ELMTSIZE;
                    position.x = data->enemies[y]->x * ELMTSIZE;
                    position.y = data->enemies[y]->y * ELMTSIZE;
                }
                SDL_BlitSurface (data->tab_enemies[data->whichEnemy], &data->enemies[y]->Rect, data->img_screen, &position);
            }
        }

/* Same process than before, if a wall is moving, we blit it slowly using some variables to get it coordinates*/
    if(data->bouge_toi){

        if(data->wall_cd > -1.0) {
            position.x = (data->hero.wallx + ((data->boucle_mur+1) * coor[data->dir_wall].x)) * ELMTSIZE - data->wall_cd  * coor[data->dir_wall].x;
            position.y = (data->hero.wally + ((data->boucle_mur+1) * coor[data->dir_wall].y)) * ELMTSIZE - data->wall_cd  * coor[data->dir_wall].y;
            data->wall_cd-=4.25 * data->doubleWV; // ELMSIZE/8 == 4.25 , doubleWallVelocity, first it is 1, if "c" is pressed, it will be 2, so the wall will move 2x faster.
        }

        else {
            position.x = (data->hero.wallx + ((data->boucle_mur+1) * coor[data->dir_wall].x)) * ELMTSIZE;
            position.y = (data->hero.wally + ((data->boucle_mur+1) * coor[data->dir_wall].y)) * ELMTSIZE;
        }

        SDL_BlitSurface (data->tab_item[1],NULL, data->img_screen, &position);
        Mix_PlayChannel(8,data->wall_music,0); // The noisy sound playing when a wall is moving.
        Mix_Volume(8, MIX_MAX_VOLUME/4); // So noisy that I divided his volume by 4 !
    }

/* If transition screen is on, here we blit all the explosions. So shiny ! */
    if(data->transition){
        for(x=0;x<8;x++){
            position.x = x *34;
            for(y=7;y<HEIGHT;y++){
                position.y = y * 34;
                SDL_BlitSurface (data->img_boum, &RectB, data->img_screen, &position);
            }
        }

        for(x=12;x<WIDTH;x++){
            position.x = x *34;
            for(y=7;y<HEIGHT;y++){
                position.y = y * 34;
                SDL_BlitSurface (data->img_boum, &RectB, data->img_screen, &position);
            }
        }
    }
}
/* Is position I am trying to access a valid one ?*/
int valid_pos(Data* data, int x, int y) {
    return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && data->board[y][x] != WALL && data->board[y][x] != BROKEN_WALL;
}

/* The function that moves wall ! The wall do not really moves, it is replaced by a 0 until his position his not valid anymore, then when it reach it final travel, it go back to a wall again. Franglish is a beautiful language issn't it ?*/
void move_mur(Data* data){
    int x,y;
    ++data->boucle_mur;
    /* Some variables that are used to save the direction were the wall is moving, mainly, it is why we can't move more than one wall at a time*/
    x = data->hero.wallx + (data->boucle_mur+1) * coor[data->dir_wall].x;
    y = data->hero.wally + (data->boucle_mur+1) * coor[data->dir_wall].y;

    if(valid_pos(data, x, y) && data->board[y][x]== EMPTY) {

        data->board[data->hero.wally + data->boucle_mur * coor[data->dir_wall].y][ data->hero.wallx + data->boucle_mur * coor[data->dir_wall].x]=EMPTY;
        data->wall_cd=ELMTSIZE;
    }
    else{
        data->board[data->hero.wally + data->boucle_mur * coor[data->dir_wall].y][data->hero.wallx + data->boucle_mur* coor[data->dir_wall].x] = WALL;
        data->boucle_mur=0;
        data->bouge_toi=0;
    }
}

/* Clear the directions board used by the bfs function to compute the shortest path between two sets of coordinates*/
void clear_search(Data * data,int HorE){
    int x,y;

    if(!HorE)
    for(x = 0; x < WIDTH; x++) {
        for(y = 0; y < HEIGHT; y++) {
            if(data->board[y][x] != WALL)
                data->directions[y][x] = -2;
            else
                data->directions[y][x] = -10;
        }
    }

    else if(HorE == 1)
        for(x = 0; x < WIDTH; x++) {
            for(y = 0; y < HEIGHT; y++) {

                if(data->board[y][x] == WALL)
                    data->directions[y][x] = -10;
                else
                    if(data->board[y][x] == GEM)
                        data->directions[y][x] = -5;
                else
                    if(data->board[y][x] == TP)
                        data->directions[y][x] = -6;
                else
                    if(data->board[y][x] == MAZE_MODIFIER)
                        data->directions[y][x] = -7;
                else
                    data->directions[y][x] = -2;
            }
        }

    else if(HorE==2)
        for(x = 0; x < WIDTH; x++) {
            for(y = 0; y < HEIGHT; y++) {
                if(data->board[y][x] == WALL)
                    data->directions[y][x] = -10;
                else
                    if(x == data->hero.basex && y == data->hero.basey)
                        data->directions[y][x] = 10;
                else
                    data->directions[y][x] = -2;
            }
        }
}


/* A simple print function, unused */
/*
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
*/

/* The euclidian distance between two sets of coordinates */
float euclid_dist(Entity* e, int x, int y) {
    return sqrtf(powf(x - e->x, 2) + powf(y - e->y, 2));
}

/* The number of valid moves close to an entity*/
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
            if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && data->board[y][x] != WALL) {
                vm[c] = dir;
                c++;
            }
        }
    }
    return c;
}
/* The function who use the euclid_dist to return the direction of the shortest path between two sets of coordinates. With a H ! */
int heuristic_move(Data* data, Entity* e) {
    int vm[4],x,y;
    x=e->x;
    y=e->y;

    if(x == data->hero.x && y == data->hero.y)
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
        float ed = euclid_dist(&data->hero, x, y);
        if(ed < dist_mini){
            dist_mini = ed;
            best = dir;
        }
    }

    return best;
}

/* That function return the opposite dir of the parameter "dir" */
int opp_dir(int dir){
    if (!dir)
        dir =1;
    else
        if(dir==1)
            dir=0;
    else
        if(dir==2)
            dir=3;
    else
        if(dir==3)
            dir=2;

    return dir;
}
/* The function were the entities or moving (a wall is not an entity !) */
void move_entity(Data* data, Entity* e, int dir) {

    int x,y;


    /* Here we set all the variables we will used to move the wall*/
    if(!data->transition)
        if(!data->ne_me_mange_pas)
            if(data->hero.position.x == data->hero.x * ELMTSIZE  && data->hero.position.y == data->hero.y * ELMTSIZE)
                if(!data->bouge_toi)
                    if(data->board[data->hero.y +coor[data->hero.dir].y][data->hero.x+ coor[data->hero.dir].x]== WALL) {
                        if(data->board[data->hero.y + 2*coor[data->hero.dir].y][data->hero.x+ 2*coor[data->hero.dir].x] == EMPTY &&
                          (data->hero.x+ 2*coor[data->hero.dir].x >= 0 && data->hero.x+ 2*coor[data->hero.dir].x < WIDTH
                           && data->hero.y + 2*coor[data->hero.dir].y >= 0 && data->hero.y + 2*coor[data->hero.dir].y < HEIGHT)) {

                            data->dir_wall=data->hero.dir;
                            data->hero.wallx=data->hero.x;
                            data->hero.wally=data->hero.y;
                            data->bouge_toi=1;
                        }

                        else{
                            /* If the wall can't be moved, destroy it !*/
                            if(!data->broken_blit){
                                data->board[data->hero.y +coor[data->hero.dir].y][data->hero.x+ coor[data->hero.dir].x]= BROKEN_WALL;
                                data->broken_blit=1;
                            }
                        }
                    }

    x= e->x + coor[dir].x;
    y= e->y + coor[dir].y;
    e->old_x=e->x;
    e->old_y=e->y;
   /*   The Golden Door Hitbox   */
    if(data->board[0][8]== PORTE){
        if (  (x == 8  && y == 0)
           || (x == 9  && y == 0)
           || (x == 10 && y == 0)
           || (x == 11 && y == 0)
           || (x == 8  && y == 1)
           || (x == 8  && y == 2)
           || (x == 11 && y == 1)
           || (x == 11 && y == 2))
            return;

        if((x == 9 && y == 2) || (x == 10 && y == 2)){
            data->transition = 1;
            init_data(data);
            return;
        }
    }
/* If the player is on the transition screen, if he walk into the door, transition is over, we blit all the board and teleport the hero at the his base coordinates*/
    if(data->transition){
        if(level2[y][x] == 6 || level2[y][x] == 3 ){
            data->transition = 0;
            e->x =e->basex;
            e->y = e->basey;
            data->blit_all=1;
            return;
        }
        if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && level2[y][x] != WALL && level2[y][x] != BROKEN_WALL) {

        e->x = x;
        e->y = y;
        e->count_down = ELMTSIZE - 1;
        }
        return;
    }

    if(dir == -1)
        return;
/* If wall_hack_hero == 1, then the hero can go move freely with no wall collision */
    if(e == &data->hero){
        if(data->wall_hack_hero){
            if(x>= 0 && x < WIDTH && y>= 0 && y < HEIGHT){
                e->x=x;
                e->y=y;
                e->count_down = ELMTSIZE - 1;
                return;
            }
        }
    }

/* If an entity moves to teleporter, then it teleport a random valid coordinate */
    if(data->board[y][x]== TP && (x> 0 && x < WIDTH && y> 0 && y < HEIGHT) && e->count_down <10){
        while(!(x> 0 && x < WIDTH && y> 0 && y < HEIGHT) || data->board[y][x] != EMPTY || (y == e->y && x == e->x )  || (y == data->hero.y && x == data->hero.x )) {

            x = rand() % WIDTH;
            y = rand() % HEIGHT;
        }

        e->x = x;
        e->y = y;
        e->count_down=-1;
        if(e == &data->hero)
            Mix_PlayChannel(-1,data->portal_music, 0);
        return;
    }

/* If a hero walk into a maze modifier, a new maze is created and we blit the new one to the screen */
    if(e == &data->hero){
        if(data->board[y][x]== MAZE_MODIFIER && (x> 0 && x < WIDTH && y> 0 && y < HEIGHT)){

            int index = 0;
            int backtrack_x[CELL];
            int backtrack_y[CELL];
            backtrack_x[index] = 0;
            backtrack_y[index] = 0;
            init_maze(data);
            maze_generator(index, data, backtrack_x, backtrack_y, 1,1,1);
            data->blit_all=1;
            e->x = x;
            e->y = y;
            e->count_down=-1;
            return;
        }
    }
 /* We do not move dead entities, no undead in this game*/
    if(e != &data->hero && e->vie) {

        if(data->board[y][x]== WALL || data->board[y][x]== BROKEN_WALL){
            if(e->count_down<0){
                data->board[y][x]=BROKEN_WALL;
                data->broken_blit = 1;
                return;
            }
        }
        e->x = x;
        e->y = y;
        e->count_down = ELMTSIZE - 1;
        return;
    }
/* Finally, if the coordinates of the cells the entity tries to access are valid ones, then we move it the those coordinates*/
    if(valid_pos(data, x, y)) {

        e->x = x;
        e->y = y;
        e->count_down = ELMTSIZE - 1;
    }
}

/* return a random valid direction using valid_moves */
int random_move(Entity* e,Data * data) {
    int vm[4];
    int c = valid_moves(e, vm,data);
    if(c == 0)
        return -1;
    return vm[rand() % c];
}

/* Return the direction of the shortest path between the an entity and the hero. For this one, we used a file to do it recursively */
int bfs(Data *data,Entity *e){

    Point a,c;
    int dir,acc=WIDTH*HEIGHT;
    int x,y,xh,yh;
    fin = -1; // Don't forget to empty the file before using it.

    /* If the hero is hidden or the entity e can destroy walls, we won't use this function*/
    if(data->hide)
        return e->dir =random_move(e,data);
    if(e->wall_hack_enemy)
        return e->dir = heuristic_move(data,e);

/* Clear the direction board */
    clear_search(data,0);
    c.x=e->x;
    c.y=e->y;
    yh=data->hero.y;
    xh=data->hero.x;
    push(c); // Push the coordinates of the entity into the file.


/* To make the enemies a little "smarter" */

/*
     for(x=0;x<data->nb_enemy;x++)
        if(data->enemies[x]->vie)
            data->directions[data->enemies[x]->y][data->enemies[x]->x] =-10;
        else
            data->directions[data->enemies[x]->y][data->enemies[x]->x] =-2;
*/

    data->directions[e->y][e->x]=-3;
    data->directions[data->hero.y][data->hero.x]=-4;

/* While the current coordinates are different than the heros's coordinates, we push new coordinates into the file and mark them into the direction board so we won't use them again*/
    while(c.x!=xh || c.y!=yh){

        if(!acc){ //if it can't find the hero.
            e->wall_hack_enemy=1;
            return -1;
        }
        a= pop();

        for(dir =0;dir < NDIR;dir++){
            x = a.x + coor[dir].x;
            y = a.y + coor[dir].y;
            if(data->directions[y][x]==-4 && valid_pos(data,x,y)){

                if(data->directions[a.y][a.x]==-3 && valid_pos(data,x,y))
                    return e->dir =dir;
                return  e->dir =data->directions[a.y][a.x];
            }

            if((data->directions[y][x] ==-2)&& valid_pos(data,x,y)){
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

/* Same bfs function, this one find the shortest direction to take to reach an item, this one is used to get gems mainly, then if it can't get gems, it'll try to find something else instead */
int bfs_h(Data *data,Entity *e,int item){

    Point a,c;
    int dir,acc=WIDTH*HEIGHT;
    int x,y;
    fin = -1;
    clear_search(data,1);
    c.x=e->x;
    c.y=e->y;
    push(c);
    /* If it can't avoid a collision with an enemy*/
    if(deuxieme_tour)
        for(x=0;x <data->nb_enemy;x++)
            data->directions[data->enemies[x]->y][data->enemies[x]->x]=-2;
    else {
    for(x=0;x <data->nb_enemy;x++){  /* Turn every enemies into walls, so the hero will not take their way */

        if(data->enemies[x]->vie)
            data->directions[data->enemies[x]->y][data->enemies[x]->x]=-10;
        else
            data->directions[data->enemies[x]->y][data->enemies[x]->x]=-2;

        for(dir =0;dir < NDIR;dir++)
            if(data->directions[data->enemies[x]->y+ (2*coor[dir].y)][data->enemies[x]->x+ (2*coor[dir].x)]== -2 && data->directions[data->enemies[x]->y+ coor[dir].y][data->enemies[x]->x+ coor[dir].x] != -10 && data->enemies[x]->vie )
                data->directions[data->enemies[x]->y+ (2*coor[dir].y)][data->enemies[x]->x+ (2*coor[dir].x)]=-10;

        if(data->directions[data->enemies[x]->y+ coor[dir].y][data->enemies[x]->x+ coor[dir].x]== -2 && data->enemies[x]->vie)
            data->directions[data->enemies[x]->y+ coor[dir].y][data->enemies[x]->x+ coor[dir].x]=-10;
        }
    }

    data->directions[data->hero.y][data->hero.x]=-4;

    while(data->directions[c.y][c.x] != item){
        if(!acc){
            if(item == -7){
                if(!deuxieme_tour){
                    deuxieme_tour=1;
                    return bfs_h(data,e,-5);
                }
                else
                    return -1;
            }

            else
                if(item == -5)
                    return bfs_h(data,e,-6);
            else
                if(item == -6)
                    return bfs_h(data,e,-7);
        }

        a= pop();

        for(dir =0;dir < NDIR;dir++){

            for(x=0;x <data->nb_enemy;x++)
                if( (e->y+coor[dir].y==data->enemies[x]->y) && (e->x+coor[dir].x==data->enemies[x]->x) && (e->speed < data->enemies[x]->speed) && !data->hide){
                    data->pull =1;
                    return -1;
            }

            x = a.x + coor[dir].x;
            y = a.y + coor[dir].y;

            if(data->directions[y][x]==item && valid_pos(data,x,y)){

                if(data->directions[a.y][a.x]==-4 && valid_pos(data,x,y))
                    return data->hero.dir =dir;
                    return  data->hero.dir =data->directions[a.y][a.x];
            }

            if((data->directions[y][x] ==-2 ||data->directions[y][x] ==-6 ||data->directions[y][x] ==-7) && valid_pos(data,x,y)){

                c.x=x;
                c.y=y;
                if(data->directions[a.y][a.x]==-4 && valid_pos(data,x,y) )
                    data->directions[c.y][c.x]=dir;

                else
                    data->directions[c.y][c.x]=data->directions[a.y][a.x];
                push(c);
            }
        }
        acc--;
    }

    return -1;

}

/* Same than before again, this one is used to compute the shortest direction to a fixed coordinates*/
int bfs_pull(Data *data,Entity *e,int item){

    Point a,c;
    int dir,acc=WIDTH*HEIGHT;
    int x,y;
    fin = -1;
    clear_search(data,2);

    c.x=e->x;
    c.y=e->y;

    push(c);
    data->directions[data->hero.y][data->hero.x]=-4;

    while(data->directions[c.y][c.x] != item){

        if(!acc)
            return -1;

        a= pop();

        for(dir =0;dir < NDIR;dir++){

            x = a.x + coor[dir].x;
            y = a.y + coor[dir].y;
            if(data->directions[y][x]==item && valid_pos(data,x,y)){

                if(data->directions[a.y][a.x]==-4 && valid_pos(data,x,y))
                    return data->hero.dir =dir;

                return  data->hero.dir =data->directions[a.y][a.x];
            }

            if((data->directions[y][x] ==-2 ||data->directions[y][x] ==10) && valid_pos(data,x,y)){
                c.x=x;
                c.y=y;
                if(data->directions[a.y][a.x]==-4 && valid_pos(data,x,y) )
                    data->directions[c.y][c.x]=dir;

                else
                    data->directions[c.y][c.x]=data->directions[a.y][a.x];
                push(c);
            }
        }
        acc--;
    }

    return -1;
}


/* The function were all the blits are done, mainly. */
void board(Data* data) {

    int x,y,z,nb_gems,nb_teleporter,nb_maze_modifier;
    SDL_Rect position;

/* If the hero is invincible, decrease the duration until it reach 0 */
    if(data->invincibility)
        data->invincibility--;


/* Timers initialization */
    tempsActuel3 = SDL_GetTicks();

    tempsActuel4 = SDL_GetTicks();

/* If the heros has only one life remaining, play a bip sound every 1500 ms */
    if (tempsActuel3 - tempsPrecedent3 > 1500){
        if(data->hero.vie == 1) Mix_PlayChannel(-1,data->low_health_music, 0);
            tempsPrecedent3 = tempsActuel3;
    }

/* The time used for a broken wall to disappear  */
    if (tempsActuel4 - tempsPrecedent4 > 250){

        if(data->broken_blit){
            for(x = 0; x < WIDTH; x++) {
                for(y = 0; y < HEIGHT; y++) {
                    if(data->board[y][x] == BROKEN_WALL){
                        data->board[y][x] = EMPTY;
                        position.x = x * ELMTSIZE;
                        position.y = y * ELMTSIZE;
                        SDL_BlitSurface (data->tab_item[WALL], NULL, data->img_screen, &position);
                    }
                }
            }

            data->broken_blit=0;
        }

        tempsPrecedent4 = tempsActuel4;
    }

/* Rects for the enmies sprites animations*/
    for(x=0;x < data->nb_enemy ;x++){
        if(!data->enemies[x]->dir)
            data->enemies[x]->Rect.y= 3*32;
        else
            if (data->enemies[x]->dir==2)
                data->enemies[x]->Rect.y = 2*32;
        else
            if(data->enemies[x]->dir==1)
                data->enemies[x]->Rect.y = 0;
        else
            if (data->enemies[x]->dir==3)
                data->enemies[x]->Rect.y = 32;
    }

/* Rects for the hero sprite animation*/
    if(!data->hero.dir)
        data->hero.Rect.y= 3*32;
    else
        if (data->hero.dir==2)
            data->hero.Rect.y = 2*32;
    else
        if (data->hero.dir==1)
            data->hero.Rect.y = 0;
    else
        if (data->hero.dir==3)
            data->hero.Rect.y = 32;

    frameTime++;

/* Goes to next animation then return to the first if needed*/
    if(FPS / frameTime == 4 ){
        frameTime=1;
        data->hero.Rect.x += frameWidth;
        RectB.x += frameWidthB;

        for(x=0;x <data->nb_enemy ;x++)
            data->enemies[x]->Rect.x += frameWidth;
        if(data->hero.Rect.x >= textureWidth){
            data->hero.Rect.x =0;
            for(x=0;x <data->nb_enemy ;x++)
                data->enemies[x]->Rect.x=0;
        }
        if(RectB.x >= textureWidthB)
            RectB.x =0;
    }

/* Here we blit the transition screen, and the player score */
    if(data->transition){

            SDL_FillRect(data->img_screen_game_over, NULL, SDL_MapRGB(data->img_screen_game_over->format, 0,0, 0));
            for(x = 8; x < 12; x++) {
                for(y = 7; y < HEIGHT; y++) {
                    SDL_Rect position;
                    position.x = x * ELMTSIZE;
                    position.y = y * ELMTSIZE;
                    if(level2[y][x] == PORTE){
                    position.x = x * ELMTSIZE-34;
                    position.y = y * 24;
                    }
                    if(level2[y][x] != 3)
                    SDL_BlitSurface (data->tab_item[level2[y][x]], NULL, data->img_screen, &position);
                }
            }
            position.x = (WIDTH/2-1) * ELMTSIZE;
            position.y = 0;
            SDL_BlitSurface (data->score, NULL, data->img_screen, &position);

            write_number(data,data->player_score);
            draw_entity(data);
            return;
    }

/* Blit all the board */

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

/* Then don't blit it again until asked*/
    data->blit_all=0;

/* Clear the board, replace all with 0 and reinitialize all the counters */
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
/* Only clear the board once*/
    data->clear=0;

/* Finally, draw all the entities*/
    draw_entity(data);

    return;
}

/* get the gem and play a sound if the heros walk on it, it also add one life to the heros*/
void get_gem(Data* data) {

    if(data->hero.count_down <20){
        if(data->board[data->hero.y][data->hero.x] == GEM ) {
            data->board[data->hero.y][data->hero.x] = EMPTY;
            data->ent_count[GEM]--;
            if(data->hero.vie < 20 && (true_vie/2) == data->hero.vie){
                data->hero.vie++;
                true_vie+=2;
            }
            Mix_PlayChannel(-1,data->rupee_music, 0);
        }
    }
}

/* The game loop*/
void loop_level1(Data* data) {

    int x;
    float resx,resy;

    /* Just for fun, if the user change manually his life, he'll get a warning */
    if(data->hero.vie != (true_vie/2)){
        SDL_Rect position;
        if(!data->transition)
            for(x=0;x<HEIGHT;x++){
                position.y = 0;
                position.x = x * ELMTSIZE;
                SDL_BlitSurface (data->tab_item[data->board[0][x]], NULL, data->img_screen, &position);
            }
        data->hero.vie = 1;
        true_vie = 2;
        printf("So, you like to play with fire ? \n");
    }

    board(data); // Actualize the board

    /* If the heros is touched by an enemy, he becomes invincible for a short duration and loose one life */
    if(!data->transition)
        if (!data->invincibility && !data->pull)
            for(x=0;x <data->nb_enemy;x++){
                resx = fabs(data->hero.position.x - data->enemies[x]->position.x);
                resy = fabs(data->hero.position.y - data->enemies[x]->position.y);

                if(resx < 15.0f && resy < 15.0f && data->enemies[x]->vie ){ //more realistic collision.
                    data->hero.vie--;
                    true_vie-=2;
                    data->invincibility = FRAME_INVINCIBILITE;
                    break;
                    }
            }

/* The "boum" collision effect.Moves the heros to his opposite direction without changing his real direction*/
    if(data->invincibility > FRAME_INVINCIBILITE-1){

        x = opp_dir(data->hero.dir);
        data->hero.count_down =-1;
        move_entity(data, &data->hero,x);
        data->hero.count_down =-1;
        Mix_PlayChannel(-1,data->hurt_music, 0);
    }

    deuxieme_tour=0;

    SDL_Flip(data->img_screen);

    SDL_Event event;

/* If the heros was pulled to his base position, disable the pull effect and return all variable used to her original state*/
    if ((data->hero.x == data->hero.basex && data->hero.y == data->hero.basey) && data->pull){

        data->hero.dir = opp_dir(data->hero.dir);
        data->hero.speed=(float)60/34;
        data->pull =0;
        data->hero.count_down=-1;
        data->wall_hack_hero=0;
    }
/* All the inputs events*/
    while(SDL_PollEvent(&event)) {

        switch (event.type) {

            /* Let the player created his own level my mouse clicking on the screen after, he chooses an item to add */
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT && event.button.state == SDL_PRESSED){

                    if(data->editor_mode==1){
                        int a,b;
                        a= event.button.x/ELMTSIZE;
                        b=event.button.y/ELMTSIZE;
                        if(data->editeur){
                            printf("Wich item do you want ? Enter a number \n");
                            scanf("%d", &data->item);
                            while(data->item < 0 || data->item >5){
                                if (!(data->item >= 0 && data->item <= 5)){
                                    printf("Enter a valid answer \n");
                                    printf("0 == EMPTY, 1== WALL, 2== GEM, 3== TP, 4== MAZE_MODIFIER,5 == BROKEN_WALL \n");
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
                        if(!data->pull)
                            if(!data->keyboard_state[0] && !data->keyboard_state[1] && !data->keyboard_state[2] && !data->keyboard_state[3]) { //Don't move if another direction key is already pressed.
                               data->keyboard_state[2]=1;
                               true_dir = 2;

                            }

                        break;

                    case SDLK_LEFT:
                        if(!data->pull)
                            if(!data->keyboard_state[0] && !data->keyboard_state[1] && !data->keyboard_state[2] && !data->keyboard_state[3]) {
                                data->keyboard_state[3]=1;
                                true_dir = 3;
                            }

                        break;

                    case SDLK_UP:
                        if(!data->pull)
                            if(!data->keyboard_state[0] && !data->keyboard_state[1] && !data->keyboard_state[2] && !data->keyboard_state[3]) {
                                data->keyboard_state[0]=1;
                                true_dir = 0;
                            }

                        break;

                    case SDLK_DOWN:
                        if(!data->pull)
                            if(!data->keyboard_state[0] && !data->keyboard_state[1] && !data->keyboard_state[2] && !data->keyboard_state[3]) {
                                data->keyboard_state[1]=1;
                                true_dir = 1;
                            }

                        break;

                    case SDLK_h://h for hack.
                        if(data->wall_hack_hero)
                            data->wall_hack_hero--;
                        else
                            data->wall_hack_hero++;
                        break;

                    case SDLK_e: //e for add enemy .
                        if(data->nb_enemy < NB_ENEMY_MAX)
                            data->nb_enemy++;
                        break;

                    case SDLK_g: //g for add gem.
                        add_items(data,5,GEM);

                        if(data->ent_count[GEM]>=31)
                            data->ent_count[GEM]=31;

                        else
                            data->ent_count[GEM] += 5;

                        break;

                    case SDLK_t: //t for add teleporter.
                        add_items(data,1,TP);
                        data->ent_count[TP] ++;
                        break;

                    case SDLK_v: //v for add "vie".
                        if(data->hero.vie <20){
                            data->hero.vie++;
                            true_vie+=2;
                        }
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
                            data->hero.speed=2.125;
                            data->doubleWV = 1;
                            data->speed_hack--;
                        }
                        else{
                            data->hero.speed=6.8;
                            data->speed_hack++;
                            data->doubleWV = 2;
                        }

                    break;

                    case SDLK_f: //f for fu** , I already set e so I can't use it for "editor".
                        data->editeur=1;
                        break;

                    case SDLK_q: //q for quit editor_mode.
                        data->editor_mode=0;
                        break;

                    case SDLK_c: //c for camouflage
                        if(!data->hide)
                            data->hide=1;
                        else
                            data->hide=0;

                        break;

                    case SDLK_w: //w for wall_velocity
                        if(!data->bouge_toi){
                            if(data->doubleWV == 1)
                                data->doubleWV = 2;
                            else
                                data->doubleWV = 1;
                        }
                        break;

                    case SDLK_a: //a for attack.
                        if(data->hunt)
                            data->hunt--;
                        else
                            data->hunt++;
                        break;

                    case SDLK_p: //p for pull;
                        if(!data->pull)
                            data->pull++;
                        break;

                    case SDLK_d:
                        if(data->ne_me_mange_pas)
                            data->ne_me_mange_pas--;
                        else
                            data->ne_me_mange_pas++;
                        break;

                    case SDLK_b:
                        if(!data->gender){
                            data->gender++;
                            Mix_PlayChannel(-1,data->hey_music, 0);
                        }
                        else{
                            data->gender--;
                            Mix_PlayChannel(-1,data->hey2_music, 0);
                        }
                        break;

                    default: break;
            }
            /* If a direction key is released, let us know */
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


            break;
            default: break;
        }
    }
/* move wall only if a wall is actually moving and only if he finished his blit count_down */
    if(data->bouge_toi)
        if(data->wall_cd <0)
            move_mur(data);

    if(!data->transition)
        if(data->pull){
            data->hero.speed=5.0;
            data->wall_hack_hero=1;

            if(data->hero.count_down<0)
                move_entity(data, &data->hero,bfs_pull(data,&data->hero,10));
            return;
        }

    if(data->invincibility < FRAME_INVINCIBILITE-3){
        if(!data->hunt){
           if(data->hero.position.x == data->hero.x * ELMTSIZE && data->hero.position.y == data->hero.y * ELMTSIZE){ // From now, it must wait until the heros has truly finished his walking animation before it moves him again.
              if(true_dir != -1)
                data->hero.dir = true_dir; // The last direction saved when the all the events ended will be the one used to move the heros.

               move_entity(data, &data->hero,data->hero.dir);
           }
        }

        else{
            if(!data->transition)
                if(data->hero.count_down<0)
                    move_entity(data, &data->hero,bfs_h(data,&data->hero,-5));
        }
    }

    tempsActuel5 = SDL_GetTicks();

/* Every 5 seconds, the IA used individually for each enemies will be choose at random */
    if (tempsActuel5 - tempsPrecedent5 > 5000){
        for(x=0;x <data->nb_enemy;x++)
            if(data->enemies[x]->vie)
              data->enemies[x]->wall_hack_enemy = rand() & 1;

        tempsPrecedent5 = tempsActuel5;
    }

/* Move alive enemies */
    if(!data->transition)
        for(x=0;x <data->nb_enemy;x++)
            if(data->enemies[x]->count_down < 0 && data->enemies[x]->vie)
                move_entity(data, data->enemies[x], bfs(data, data->enemies[x]));


    if(!data->transition)
        get_gem(data);

    if(end_of_game(data)){
    /* If WinOrLoss == 0, it's loose,so we blit all the game over board and end the game */
        if(!data->WinOrLoss){
        SDL_Flip(data->img_screen);
        printf("Well played, now press f5 to play again \n");
        data->blit_all=1;
        data->screen = GAMEOVER;
        }
    }

}

/* The game over loop, you can still press F5 to initialize a new game*/
void loop_game_over(Data* data) {

    board_game_over(data);
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
    srand((unsigned)time(NULL)); //Initialize the rand seed;

    /* Initialize the maze*/
    int index = 0;
    int backtrack_x[CELL];
    int backtrack_y[CELL];
    init_maze(data);
    backtrack_x[index] = 0;
    backtrack_y[index] = 0;
    maze_generator(index, data, backtrack_x, backtrack_y, 1,1,1);

    init_load(data); // Load the data;
    init_data(data); // Initialize the variables.

    /* The keyboard keys that can be used */
    printf("Press r to enter the level editor,then press f to choose an item, then you can add items throughout the map by mouse clicking on the map with the left button. Press f again to change your item selection. Press q to quit the level editor. \n");
    printf("0 == EMPTY, 1== WALL, 2== GEM, 3== TP, 4== MAZE_MODIFIER , 5== BROKEN_WALL \n");
    printf("\n");
    printf("Press a to start the hero AI \n");
    printf("Press b to change your gender \n");
    printf("Press o to enable speed hack \n");
    printf("Press h to enable wall hack for your hero\n");
    printf("Press e to add one enemy\n");
    printf("Press g to add 5 gems throughout the map \n");
    printf("Press t to add 1 teleporter throughout the map \n");
    printf("Press m to add 1 maze modifier throughout the map \n");
    printf("Press c to hide the hero from enemies \n");
    printf("Press w to double the wall velocity \n");
    printf("Press v to gain 1 life \n");
    printf("Press p to return your hero at his original position \n");

    Mix_PlayChannel(0,data->ambient_music, -1); // The ambient music
    Mix_Volume(0, MIX_MAX_VOLUME/4);

    emscripten_set_main_loop_arg(renderloop, data,0,0);

    return 0;
}


