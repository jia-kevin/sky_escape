/*Kevin Jia 		ICS3U		  Ms. Odecki/Ms. Cullum 		 Summative
Platform game where user controls a charcter and, using up, right, and left arrow keys, moves it to the endpoint of multiple levels*/

//Import header files
#include <allegro.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//define constants and speed counter
volatile long speed_counter = 0; 
#define ACCELERATION 1
#define MAXFALLSPEED 8
#define MAXMOVESPEED 6

//levels are comprised of 20x20 blocks of various objects, so create the structure "block" that stores information on one block in a level (type, position)
struct block{
	char blockName[19];
	int xCoordinate;
	int yCoordinate;
};

//function that returns an integer based on the block name, allowing for easier flow control in other functions
//blocks have different properties and thus need to be identified easily
int object_selection(char *objName)
{
	if (strcmp(objName, "ground")==0)
		return(0);
	else if (strcmp(objName, "spikeUp")==0)
		return(1);
	else if (strcmp(objName, "spikeLeft")==0)
		return(2);
	else if (strcmp(objName, "spikeDown")==0)
		return(3);
	else if (strcmp(objName, "spikeRight")==0)
		return(4);
	else if (strcmp(objName, "box")==0)
		return(5);
	else if (strcmp(objName, "fallPlatformStatic")==0)
		return(6);
	else if (strcmp(objName, "fallPlatformMobile")==0)
		return(7);
	else if (strcmp(objName, "shooterOnUp")==0)
		return(8);
	else if (strcmp(objName, "shooterOnLeft")==0)
		return(9);
	else if (strcmp(objName, "shooterOnDown")==0)
		return(10);
	else if (strcmp(objName, "shooterOnRight")==0)
		return(11);
	else if (strcmp(objName, "shooterOffUp")==0)
		return(12);
	else if (strcmp(objName, "shooterOffLeft")==0)
		return(13);
	else if (strcmp(objName, "shooterOffDown")==0)
		return(14);
	else if (strcmp(objName, "shooterOffRight")==0)
		return(15);
	else if (strcmp(objName, "switchOn")==0)
		return(16);
	else if (strcmp(objName, "switchOff")==0)
		return(17);
	else if (strcmp(objName, "finish")==0)
		return(18);
}
END_OF_FUNCTION(object_selection);


//define a function that simulates gravity for non-static objects (including the character)
void gravity_collision(int object_x_coordinate, int object_y_coordinate, block levelData[], int size, int &dy)
{
	//add acceleration
	dy += ACCELERATION;
	
	for (int i=0; i<size; i++){
		
		//find if object will crash over the boundaries of another block and prevent that from happening (i.e collision)
		if (object_selection(levelData[i].blockName)==0 
		|| (5<=object_selection(levelData[i].blockName) 
		&& object_selection(levelData[i].blockName)<=17)){
			
			if (object_y_coordinate<levelData[i].yCoordinate && dy>=0){
				if (((levelData[i].yCoordinate)-(object_y_coordinate+20))<dy 
				&& -20<(levelData[i].xCoordinate-object_x_coordinate) 
				&& (levelData[i].xCoordinate-object_x_coordinate)<20)
					dy = (levelData[i].yCoordinate)-(object_y_coordinate+20);					
			}
			
			else if (object_y_coordinate>levelData[i].yCoordinate && dy<0){
				if (((levelData[i].yCoordinate+20)-(object_y_coordinate))>dy 
				&& -20<(levelData[i].xCoordinate-object_x_coordinate) 
				&& (levelData[i].xCoordinate-object_x_coordinate)<20)
					dy = (levelData[i].yCoordinate+20)-(object_y_coordinate);	
			}
		}
	}
	//implement a max fall speed
	if (dy>MAXFALLSPEED)
		dy = MAXFALLSPEED;
}
END_OF_FUNCTION(gravity_collision);	


//lets the character jump upon pressing up if it is on the ground
void up_move(int object_x_coordinate, int object_y_coordinate, block levelData[], int size, int &dy)
{
	char downCollision = 'n';
	
	for (int i=0; i<size; i++){
		
		if (object_selection(levelData[i].blockName)==0 
		|| (5<=object_selection(levelData[i].blockName) 
		&& object_selection(levelData[i].blockName)<=17)){
			
			if (((levelData[i].yCoordinate)-(object_y_coordinate+20))==0 
			&& -20<(levelData[i].xCoordinate-object_x_coordinate) 
			&& (levelData[i].xCoordinate-object_x_coordinate)<20)
				downCollision = 'y';
		}
	}
	
	if (downCollision == 'y' && dy==0)
		dy -=15;
}
END_OF_FUNCTION(up_move);

//lets the character move right when pressing the right key, performs actions that occur upon unit collision
void right_move(int &character_x_coordinate, int character_y_coordinate, block levelData[], int size, int maxSpeed)
{
	char rightCollision = 'h';
	int dx = 0;
	
	while (rightCollision == 'h'){
		rightCollision = 'n';
		
		//loops through the level to find unit collisions when moving right
		for (int i=0; i<size; i++){
			
			if (character_x_coordinate == (levelData[i].xCoordinate - 20) 
			&& -20<(levelData[i].yCoordinate-character_y_coordinate) 
			&& (levelData[i].yCoordinate-character_y_coordinate)<20){
				
				//if hitting a movable block, move that block as well (at a decreased speed)
				if (object_selection(levelData[i].blockName)==5 
				|| object_selection(levelData[i].blockName)==7){
					if (rightCollision != 'y')
						right_move(levelData[i].xCoordinate, levelData[i].yCoordinate, levelData,  size, maxSpeed/2);
				}
				
				//stop, if hitting a static object
				else if (object_selection(levelData[i].blockName) == 0 
				|| object_selection(levelData[i].blockName) == 7 
				|| (8<=object_selection(levelData[i].blockName) 
				&& object_selection(levelData[i].blockName)<=17)){
					rightCollision = 'y';
					
					//change all the lasers to on/off
					if (object_selection(levelData[i].blockName)==17){
						strcpy(levelData[i].blockName, "switchOn");
						
						for (int j=0; j<size; j++){
							if (object_selection(levelData[j].blockName) == 8)
								strcpy(levelData[j].blockName, "shooterOffUp");
							else if (object_selection(levelData[j].blockName) == 9)
								strcpy(levelData[j].blockName, "shooterOffLeft");
							else if (object_selection(levelData[j].blockName) == 10)
								strcpy(levelData[j].blockName, "shooterOffDown");
							else if (object_selection(levelData[j].blockName) == 11)
								strcpy(levelData[j].blockName, "shooterOffRight");
							else if (object_selection(levelData[j].blockName) == 12)
								strcpy(levelData[j].blockName, "shooterOnUp");
							else if (object_selection(levelData[j].blockName) == 13)
								strcpy(levelData[j].blockName, "shooterOnLeft");
							else if (object_selection(levelData[j].blockName) == 14)
								strcpy(levelData[j].blockName, "shooterOnDown");
							else if (object_selection(levelData[j].blockName) == 15)
								strcpy(levelData[j].blockName, "shooterOnRight");
						}
					}
				}
			}
		}
	}
	//get character speed
	if (rightCollision == 'y')
		dx = 0;
	else if (rightCollision == 'n')
		dx = maxSpeed;
	
	//check if the character goes through a block, and limits it to the edge of those blocks (i.e. unit collision)
	int mindx = dx;
	
	if (dx != 0){
		for (int i=0; i<size; i++){
			if (object_selection(levelData[i].blockName)==0
			||(5<=object_selection(levelData[i].blockName) 
			&& object_selection(levelData[i].blockName)<=17)){
				if ((((levelData[i].xCoordinate - 20)-character_x_coordinate)>0 
				&& mindx>(levelData[i].xCoordinate - 20)-character_x_coordinate) 
				&& (-20<(levelData[i].yCoordinate-character_y_coordinate) 
				&& (levelData[i].yCoordinate-character_y_coordinate)<20))
					mindx = (levelData[i].xCoordinate - 20)-character_x_coordinate;
			}
		}
	}
	
	
	dx = mindx;
	
	//change the unit position
	character_x_coordinate += dx;
}				
END_OF_FUNCTION(right_move);	

//same process as above, but for left move
void left_move(int &character_x_coordinate, int character_y_coordinate, block levelData[], int size, int maxSpeed)
{
	char leftCollision = 'h';
	int dx = 0;
	
	while (leftCollision == 'h'){
		leftCollision = 'n';
		
		for (int i=0; i<size; i++){
			if (character_x_coordinate == (levelData[i].xCoordinate+20) 
			&& -20<(levelData[i].yCoordinate-character_y_coordinate) 
			&& (levelData[i].yCoordinate-character_y_coordinate)<20){
				if (object_selection(levelData[i].blockName)==5 
				|| object_selection(levelData[i].blockName)==7){
					if (leftCollision != 'y')
						left_move(levelData[i].xCoordinate, levelData[i].yCoordinate, levelData,  size, maxSpeed/2);
				}
				
				else if (object_selection(levelData[i].blockName) == 0 
				|| object_selection(levelData[i].blockName) == 7 
				|| (8<=object_selection(levelData[i].blockName) 
				&& object_selection(levelData[i].blockName)<=17)){
					leftCollision = 'y';
					
					if (object_selection(levelData[i].blockName)==16){
						strcpy(levelData[i].blockName, "switchOff");
						
						for (int j=0; j<size; j++){
							if (object_selection(levelData[j].blockName) == 8)
								strcpy(levelData[j].blockName, "shooterOffUp");
							else if (object_selection(levelData[j].blockName) == 9)
								strcpy(levelData[j].blockName, "shooterOffLeft");
							else if (object_selection(levelData[j].blockName) == 10)
								strcpy(levelData[j].blockName, "shooterOffDown");
							else if (object_selection(levelData[j].blockName) == 11)
								strcpy(levelData[j].blockName, "shooterOffRight");
							else if (object_selection(levelData[j].blockName) == 12)
								strcpy(levelData[j].blockName, "shooterOnUp");
							else if (object_selection(levelData[j].blockName) == 13)
								strcpy(levelData[j].blockName, "shooterOnLeft");
							else if (object_selection(levelData[j].blockName) == 14)
								strcpy(levelData[j].blockName, "shooterOnDown");
							else if (object_selection(levelData[j].blockName) == 15)
								strcpy(levelData[j].blockName, "shooterOnRight");
						}
					}
				}
			}
		}
		
		if (leftCollision == 'h'){
			for (int i=0; i<size; i++){
				if (character_x_coordinate == (levelData[i].xCoordinate+20) 
				&& -20<(levelData[i].yCoordinate-character_y_coordinate) 
				&& (levelData[i].yCoordinate-character_y_coordinate)<20){
					if (object_selection(levelData[i].blockName)==5 
					|| object_selection(levelData[i].blockName)==7)
						levelData[i].xCoordinate += dx;
				}
			}
		}
	}
	
	
	if (leftCollision == 'y')
		dx = 0;
	else if (leftCollision == 'n')
		dx = -maxSpeed;
	
	
	int mindx = dx;
	if (dx != 0){
		for (int i=0; i<size; i++){
			if (object_selection(levelData[i].blockName)==0
			||(5<=object_selection(levelData[i].blockName) 
			&& object_selection(levelData[i].blockName)<=17)){
				if ((((levelData[i].xCoordinate+20)-character_x_coordinate)<=0 
				&& mindx<(levelData[i].xCoordinate+20)-character_x_coordinate) 
				&& (-20<(levelData[i].yCoordinate-character_y_coordinate) 
				&& (levelData[i].yCoordinate-character_y_coordinate)<20))
					mindx = (levelData[i].xCoordinate+20)-character_x_coordinate;
			}
		}
	}
	dx = mindx;
	
	character_x_coordinate += dx;
}				
END_OF_FUNCTION(left_move);

//change static falling platforms to mobile falling platforms when the character steps on it
void block_fall(int object_x_coordinate, int object_y_coordinate, int timeFall[], block levelData[], int size, int frameCount)
{
	//loops through the level and sets a delay for the blocks to fall
	for (int i=0; i<size; i++){
		if (object_selection(levelData[i].blockName)==6){
			if (object_y_coordinate-levelData[i].yCoordinate+20==0 
			&& -20<(levelData[i].xCoordinate-object_x_coordinate) 
			&& (levelData[i].xCoordinate-object_x_coordinate)<20){
				if (timeFall[i] == 0){
					timeFall[i] = (frameCount + 20)%240;
					if (timeFall[i] == 0)
						timeFall[i] = 240;
				}
			}
		}
	}
	
	if (frameCount == 0)
		frameCount = 240;
		
	//changes the blocks to mobile after the delay
	for (int i=0; i<size; i++){
		if (object_selection(levelData[i].blockName)==6){
			if (timeFall[i] == frameCount)
				strcpy(levelData[i].blockName, "fallPlatformMobile");
		}
	}
}
END_OF_FUNCTION(block_fall);

//apply gravity to all non-static blocks
void block_gravity(int blockDy[], block levelData[], int size)
{
	//loops through the level and applies gravity (dy is found in the blockDy[] array)
	for (int i=0; i<size; i++){
		if (object_selection(levelData[i].blockName)==7||object_selection(levelData[i].blockName)==5){
			gravity_collision(levelData[i].xCoordinate, levelData[i].yCoordinate, levelData, size, blockDy[i]);
			levelData[i].yCoordinate += blockDy[i];
		}
	}
}
END_OF_FUNCTION(block_gravity);

//draw and load lasers in the level (lasers fire until hitting an object, and every pixel across the width of the laser is independant)
void load_lasers(int lasers[][20], block levelData[], int size, BITMAP *laserHori, BITMAP *laserVert, BITMAP *buffer)
{
	int j=0, l=0, k=0;
	char stop = 'n';
	
	
	//loops through the level to find all laser shooters, and performs the appropriate action depending on the direction of the laser
	for (int i=0; i<size; i++){
		if (object_selection(levelData[i].blockName)==8){
			//loops through every pixel across the width of the shooter to see how long each one goes
			for (j=0; j<20; j++){
				k=0;
				stop = 'n';
				
				while (stop == 'n'){
					//checks if the laser hits a block in the level, if not, increment the length (k) until it does
					for (l=0; l<size; l++){
						if (object_selection(levelData[l].blockName)==0 
						|| (5<=object_selection(levelData[l].blockName) 
						&& object_selection(levelData[l].blockName)<=17)){
							if ((levelData[l].xCoordinate - (levelData[i].xCoordinate+j))<=0 
							&& (levelData[l].xCoordinate - (levelData[i].xCoordinate+j))>-20 
							&& (levelData[i].yCoordinate-k)==(levelData[l].yCoordinate+20))
								stop = 'y';
						}
						if (levelData[i].yCoordinate-k < 0)
								stop = 'y';
					}
					k++;
					
					//draw a pixel when there is no collision 
					if (stop == 'n')
						blit(laserVert, buffer, j, 0, levelData[i].xCoordinate+j, levelData[i].yCoordinate-k, 1, 1);
				}
				k--;
				
				//save the data of how long the lasers need to fire in an array to check for death
				lasers[i][j] = k;
			}
		}
		//same for others
		else if (object_selection(levelData[i].blockName)==9){
			for (j=0; j<20; j++){
				k=0;
				stop = 'n';
				while (stop == 'n'){
					for (l=0; l<size; l++){
						if (object_selection(levelData[l].blockName)==0 
						|| (5<=object_selection(levelData[l].blockName) 
						&& object_selection(levelData[l].blockName)<=17)
						&& stop == 'n'){
							if ((levelData[l].yCoordinate - (levelData[i].yCoordinate+j))<=0 
							&& (levelData[l].yCoordinate - (levelData[i].yCoordinate+j))>-20 
							&& (levelData[i].xCoordinate-k)==(levelData[l].xCoordinate+20))
								stop = 'y';
								
							if (levelData[i].xCoordinate-k < 0)
								stop = 'y';
						}
					}
					k++;
					
					if (stop == 'n')
						blit(laserHori, buffer, 0, j, levelData[i].xCoordinate-k, levelData[i].yCoordinate+j, 1, 1);
				}
				k--;
				
				lasers[i][j] = k;
			}
		}
		
		else if (object_selection(levelData[i].blockName)==10){
			for (j=0; j<20; j++){
				k=0;
				stop = 'n';
				
				while (stop == 'n'){
					for (l=0; l<size; l++){
						if (object_selection(levelData[l].blockName)==0 
						|| (5<=object_selection(levelData[l].blockName) 
						&& object_selection(levelData[l].blockName)<=17)){
							if ((levelData[l].xCoordinate - (levelData[i].xCoordinate+j))<=0 
							&& (levelData[l].xCoordinate - (levelData[i].xCoordinate+j))>-20 
							&& (levelData[i].yCoordinate+k)==(levelData[l].yCoordinate-20))
								stop = 'y';
						}
						
						if (levelData[i].yCoordinate+k > 540)
								stop = 'y';
					}
					k++;
					
					if (stop == 'n')
						blit(laserVert, buffer, j, 0, levelData[i].xCoordinate+j, levelData[i].yCoordinate+19+k, 1, 1);
				}
				k--;
				
				lasers[i][j] = k;
			}
		}
		
		else if (object_selection(levelData[i].blockName)==11){
			for (j=0; j<20; j++){
				k=0;
				stop = 'n';
				while (stop == 'n'){
					for (l=0; l<size; l++){
						if (object_selection(levelData[l].blockName)==0 
						|| (5<=object_selection(levelData[l].blockName) 
						&& object_selection(levelData[l].blockName)<=17)){
							if ((levelData[l].yCoordinate - (levelData[i].yCoordinate+j))>=0 
							&& (levelData[l].yCoordinate - (levelData[i].yCoordinate+j))<20 
							&& (levelData[i].xCoordinate+k)==(levelData[l].xCoordinate-20))
								stop = 'y';
						}
						
						if (levelData[i].xCoordinate+k > 720)
								stop = 'y';
					}
					k++;
					
					if (stop == 'n')
						blit(laserHori, buffer, 0, j, levelData[i].xCoordinate+19+k, levelData[i].yCoordinate+j, 1, 1);
				}
				k--;
				
				lasers[i][j] = k;
			}
		}
	}
}
END_OF_FUNCTION(load_lasers);

//checks if, based on the character position, the character dies
int check_death(int character_x_coordinate, int character_y_coordinate, block levelData[], int lasers[][20], int size)
{
	int j = 0;
	char checkDeath = 'n';
	
	//loops through the level's blocks and see if any of the kill the character
	for (int i=0; i<size; i++){
		//see if the character dies by a spike
		if (1<=object_selection(levelData[i].blockName) 
		&& object_selection(levelData[i].blockName)<=4){
			if ((character_x_coordinate - levelData[i].xCoordinate)<20 
			&& (character_x_coordinate - levelData[i].xCoordinate)>-20 
			&& (character_y_coordinate - levelData[i].yCoordinate)<20 
			&& (character_y_coordinate - levelData[i].yCoordinate)>-20)
				checkDeath = 'y';
		}
		
		//check if the lasers, based on the lasers array (from load_lasers) hit the character
		else if (object_selection(levelData[i].blockName)==8){
			for (j=0; j<20; j++){
					if ((levelData[i].xCoordinate+j - character_x_coordinate)>=0
					&& (levelData[i].xCoordinate+j - character_x_coordinate)<20
					&& (levelData[i].yCoordinate - character_y_coordinate)>0
					&& (levelData[i].yCoordinate - character_y_coordinate)<=lasers[i][j])
						checkDeath = 'y';
			}
		}
		
		else if (object_selection(levelData[i].blockName)==9){
			for (j=0; j<20; j++){
				if ((levelData[i].yCoordinate+j - character_y_coordinate)>=0
					&& (levelData[i].yCoordinate+j - character_y_coordinate)<20
					&& (levelData[i].xCoordinate - character_x_coordinate)>0
					&& (levelData[i].xCoordinate - character_x_coordinate)<=lasers[i][j])
						checkDeath = 'y';
			}
		}
		
		else if (object_selection(levelData[i].blockName)==10){
			for (j=0; j<20; j++){
				if ((levelData[i].xCoordinate+j - character_x_coordinate)>=0
					&& (levelData[i].xCoordinate+j - character_x_coordinate)<20
					&& (levelData[i].yCoordinate - character_y_coordinate)<=0
					&& (character_y_coordinate - (levelData[i].yCoordinate+20))<lasers[i][j])
						checkDeath = 'y';
			}
		}
		
		else if (object_selection(levelData[i].blockName)==11){
			for (j=0; j<20; j++){
				if ((levelData[i].yCoordinate+j - character_y_coordinate)>=0
					&& (levelData[i].yCoordinate+j - character_y_coordinate)<20
					&& (character_x_coordinate - (levelData[i].xCoordinate+20))>=0
					&& (character_x_coordinate - (levelData[i].xCoordinate+20))<lasers[i][j])
						checkDeath = 'y';
			}
		}
	}
	
	//check if the character is outside the screen (causing death)
	if (character_x_coordinate<-20 || character_x_coordinate>720||character_y_coordinate<-20||character_y_coordinate>720)
		checkDeath = 'y';
	
	//return value
	if (checkDeath == 'n')
		return(0);
	else if (checkDeath == 'y')
		return(1);
}
END_OF_FUNCTION(check_death);

//check if the character reaches the end of the level
int check_finish(int character_x_coordinate, int character_y_coordinate, block levelData[], int size)
{
	char checkFinish = 'n';
	
	//find the finish block and see if it collides with the character
	for (int i=0; i<size; i++){
		if (object_selection(levelData[i].blockName)==18){
			if ((character_x_coordinate - levelData[i].xCoordinate)<20 
			&& (character_x_coordinate - levelData[i].xCoordinate)>-20 
			&& (character_y_coordinate - levelData[i].yCoordinate)<20 
			&& (character_y_coordinate - levelData[i].yCoordinate)>-20)
				checkFinish = 'y';
		}
	}
	
	if (checkFinish == 'n')
		return(0);
	else if (checkFinish == 'y')
		return(1);
}
END_OF_FUNCTION(check_finish);

void increment_speed_counter() // A function to increment the speed counter
{
	speed_counter++; // This will just increment the speed counter by one.
}
END_OF_FUNCTION(increment_speed_counter);

//load level from a file
void load_level(char *filename, int size, block levelData[])
{
	//open the file
	FILE *level = fopen(filename, "r");
	int i=0, x_coor, y_coor;
	char objectName[19];
	
	//save the data to an array using the block structure
	for (int i=0; i<size; i++){
		fscanf(level, "%s", &objectName);
		fscanf(level, "%d", &x_coor);
		fscanf(level, "%d", &y_coor);
		strcpy(levelData[i].blockName, objectName);
		levelData[i].xCoordinate = x_coor;
		levelData[i].yCoordinate = y_coor;
	}
	fclose(level);
}
END_OF_FUNCTION(load_level);

//draws level from the level data
void draw_level(block levelData[], int size, BITMAP *buffer, BITMAP *background, BITMAP *objects[])
{
	char objectName[11];
	int x_coor, y_coor;
	
	//draw each block through a loop to the "background"
	for (int i=0; i<size; i++)
		masked_blit(objects[object_selection(levelData[i].blockName)], background,0,0,levelData[i].xCoordinate,levelData[i].yCoordinate,20,20);
	blit(background, buffer, 0,0,0,0,720,540);
	clear_bitmap(background);
}
END_OF_FUNCTION(draw_level);

//initialize an array
void init_Array(int array[], int size)
{
	for (int i=0; i<size; i++)
		array[i] = 0;
}
END_OF_FUNCTION(init_Array);

//start user interface
int main(int argc, char *argv[])
{
	//initialize all variables, allegro routines, and bitmaps
	
	
	allegro_init(); // Initialize Allegro
	install_keyboard(); // Initialize keyboard routines
	install_timer(); // Initialize the timer routines
	
	LOCK_VARIABLE(speed_counter); //Used to set the timer - which regulates the game's
	LOCK_FUNCTION(increment_speed_counter); //speed.
	install_int_ex(increment_speed_counter, BPS_TO_TIMER(60)); //Set our BPS
	
	set_color_depth(desktop_color_depth()); // Set the color depth
	set_gfx_mode(GFX_AUTODETECT_WINDOWED,720,540,0,0); // Change our graphics mode to 640x480
	
	BITMAP *startScreen = load_bitmap("startScreen.bmp", NULL);
	BITMAP *endScreen = load_bitmap("endScreen.bmp", NULL);
	BITMAP *instructions = load_bitmap("Instructions.bmp", NULL);
	BITMAP *background = load_bitmap("background.bmp", NULL);
	BITMAP *highscore = load_bitmap("highscore.bmp", NULL);
	BITMAP *credits = load_bitmap("credits.bmp", NULL);
	BITMAP *objects[19];
	BITMAP *laserHori = load_bitmap("laserHori.bmp", NULL);
	BITMAP *laserVert = load_bitmap("laserVert.bmp", NULL);
	objects[0] = load_bitmap("platform1.bmp", NULL);  
	objects[1] = load_bitmap("spikeUp.bmp", NULL);
	objects[2] = load_bitmap("spikeLeft.bmp", NULL);
	objects[3] = load_bitmap("spikeDown.bmp", NULL);
	objects[4] = load_bitmap("spikeRight.bmp", NULL);
	objects[5] = load_bitmap("box.bmp", NULL);
	objects[6] = load_bitmap("platform2.bmp", NULL);
	objects[7] = load_bitmap("platform2.bmp", NULL);
	objects[8] = load_bitmap("shooterOnUp.bmp", NULL);
	objects[9] = load_bitmap("shooterOnLeft.bmp", NULL);
	objects[10] = load_bitmap("shooterOnDown.bmp", NULL);
	objects[11] = load_bitmap("shooterOnRight.bmp", NULL);
	objects[12] = load_bitmap("shooterOffUp.bmp", NULL);
	objects[13] = load_bitmap("shooterOffLeft.bmp", NULL);
	objects[14] = load_bitmap("shooterOffDown.bmp", NULL);
	objects[15] = load_bitmap("shooterOffRight.bmp", NULL);
	objects[16] = load_bitmap("switchOn.bmp", NULL);
	objects[17] = load_bitmap("switchOff.bmp", NULL);
	objects[18] = load_bitmap("finish.bmp", NULL);
	BITMAP *buffer = create_bitmap(720,540);
	BITMAP *character = load_bitmap("character.bmp", NULL);
	BITMAP *choose = load_bitmap("choose.bmp", NULL);
	
	//ingame variables/data
	int character_x = 220;
	int character_y = 360;
	int frame_counter = 0;
	int dy = 0;
	int deathCounter = 0;
	int numberofLevels = 6;
	block levels[numberofLevels][150];
	int blockDy[numberofLevels][150];
	int timeFall[numberofLevels][150];
	int lasers[numberofLevels][150][20];
	
	//load levels
	load_level("level1.txt",150,levels[0]);
	load_level("level2.txt",150,levels[1]);
	load_level("level3.txt",150,levels[2]);
	load_level("level4.txt",150,levels[3]);
	load_level("level5.txt",150,levels[4]);
	load_level("level6.txt",150,levels[5]);
	
	//arrays for determining where the character starts
	int character_x_start[6]= {120, 120, 120, 120, 220, 120};
	int character_y_start[6]= {320, 320, 320, 320, 160, 100};

	char stop = 'n';
	
	//menu variables
	int menuSelection = 0;
	int menuSelectionTemp = 0;
	int menu_y_coordinates[5] = {221,297,369,433,488};
	
	//game loop
	while (stop == 'n'){
		menuSelection = 0;
		
		//main menu (selecting an option)
		while(!(key[KEY_ENTER])){
			if (key[KEY_UP])
				menuSelectionTemp--;
			if (key[KEY_DOWN])
				menuSelectionTemp++;
			if (menuSelectionTemp<0)
				menuSelectionTemp = 600;
				
			menuSelection = (menuSelectionTemp/60)%5;
			blit(startScreen, buffer, 0,0,0,0,720,540); 
			draw_sprite(buffer, choose, 420, menu_y_coordinates[menuSelection]);
			blit(buffer, screen, 0,0,0,0,720,540); //Draw the buffer to the screen
			clear_bitmap(buffer);
		}
		
		//switch structure determined by which option the user chooses
		switch (menuSelection){
			
			//play game option
			case 0:
				//loop for all the levels
				for (int i=0; i<numberofLevels; i++)
				{
					character_x = character_x_start[i];
					character_y = character_y_start[i];
					
					//single level loop
					while (check_finish(character_x, character_y, levels[i], 150) == 0)
					{
						//initialize everything (character data, level, etc.)
						dy=0;
						character_x = character_x_start[i];
						character_y = character_y_start[i];
						init_Array(blockDy[i], 150);
						init_Array(timeFall[i], 150);
						load_level("level1.txt",150,levels[0]);
						load_level("level2.txt",150,levels[1]);
						load_level("level3.txt",150,levels[2]);
						load_level("level4.txt",150,levels[3]);
						load_level("level5.txt",150,levels[4]);
						load_level("level6.txt",150,levels[5]);
						
						//keep playing until character dies, in which case he restarts
						while (check_finish(character_x, character_y, levels[i], 150) == 0 && check_death(character_x, character_y, levels[i], lasers[i], 150) == 0)
						{
							//sets the game speed
							while(speed_counter > 0)
							{
								//reads in user input
								if(key[KEY_RIGHT])// If the user hits the right key, change the picture's X coordinate
								{
									right_move(character_x, character_y, levels[i], 150, MAXMOVESPEED);
								}
								if(key[KEY_LEFT])// Ditto' - only for left key
								{
									left_move(character_x, character_y, levels[i], 150, MAXMOVESPEED);
								} 
					
								if(key[KEY_UP])// If the user hits the up key, change the picture's Y coordinate
								{
									up_move(character_x, character_y, levels[i], 150, dy);
								}
									
								speed_counter --;
								/* Increment our frame counter at the same speed of the speed counter. */
								frame_counter ++;
								
								//calculate gravity for all objects
								block_fall(character_x, character_y, timeFall[i], levels[i], 150, frame_counter);
								block_gravity(blockDy[i], levels[i], 150);
								gravity_collision(character_x, character_y, levels[i], 150, dy);	
								character_y += dy;
								
								//loop the frame counter so it doesn't go over memory limit for int
								if(frame_counter > 240) 
								{
									frame_counter = 1;
								}
								
							} 
							
							//draw the level and reset the bitmaps to their prior state
							draw_level(levels[i],150,buffer,background,objects);
							clear_bitmap(background);
							destroy_bitmap(background);
							background = load_bitmap("background.bmp", NULL);
							
							//load lasers
							load_lasers(lasers[i], levels[i], 150, laserHori, laserVert, buffer);
							draw_sprite(buffer, character, character_x, character_y);
							
							//print number of deaths
							textprintf_ex(buffer, font, 620, 520, makecol(0, 0, 0), -1, "Deaths: %d", deathCounter);
							
							//draw buffer to screen
							blit(buffer, screen, 0,0,0,0,720,540); //Draw the buffer to the screen
							clear_bitmap(buffer);
						}
						deathCounter++;
					}
					deathCounter--;
				}
				
				//when done, kill program or return to main page depending on user input
				while (!(key[KEY_ESC]||key[KEY_ENTER]))
				{
					blit(endScreen, buffer, 0,0,0,0,720,540);
					textprintf_ex(buffer, font, 324, 260, makecol(255, 255, 255), -1, "Deaths: %d", deathCounter);
					blit(buffer, screen, 0,0,0,0,720,540); //Draw the buffer to the screen
					clear_bitmap(buffer);
				}
				
				if (key[KEY_ESC])
					stop = 'y';
				break;
			
			//show instructions, return to main screen after pressing enter
			case 1:
				while(!key[KEY_ENTER]){
					blit(instructions, buffer, 0,0,0,0,720,540);
					blit(buffer, screen, 0,0,0,0,720,540); //Draw the buffer to the screen
					clear_bitmap(buffer);
				}
				break;
			
			//show "highscores", return to main screen after pressing enter
			case 2:
				while(!key[KEY_ENTER]){
					blit(highscore, buffer, 0,0,0,0,720,540);
					blit(buffer, screen, 0,0,0,0,720,540); //Draw the buffer to the screen
					clear_bitmap(buffer);
				}
				break;
				
			//show credits, return to main screen after pressing enter
			case 3:
				while(!key[KEY_ENTER]){
					blit(credits, buffer, 0,0,0,0,720,540);
					blit(buffer, screen, 0,0,0,0,720,540); //Draw the buffer to the screen
					clear_bitmap(buffer);
				}
				break;
			
			//exit program
			case 4:
				stop = 'y';
				break;
			}
	}
	
	//destroy all the bitmaps
	destroy_bitmap(startScreen);
	destroy_bitmap(background);
	destroy_bitmap(buffer);
	destroy_bitmap(character);
	destroy_bitmap(laserHori);
	destroy_bitmap(laserVert);
	destroy_bitmap(objects[0]);
	destroy_bitmap(objects[1]);
	destroy_bitmap(objects[2]);
	destroy_bitmap(objects[3]);
	destroy_bitmap(objects[4]);
	destroy_bitmap(objects[5]);
	destroy_bitmap(objects[6]);
	destroy_bitmap(objects[7]);
	destroy_bitmap(objects[8]);
	destroy_bitmap(objects[9]);
	destroy_bitmap(objects[10]);
	destroy_bitmap(objects[11]);
	destroy_bitmap(objects[12]);
	destroy_bitmap(objects[13]);
	destroy_bitmap(objects[14]);
	destroy_bitmap(objects[15]);
	destroy_bitmap(objects[16]);
	destroy_bitmap(objects[17]);
	destroy_bitmap(objects[18]); 
	destroy_bitmap(endScreen);
	
	return (0); // Exit with no errors 
}

END_OF_MAIN() 
