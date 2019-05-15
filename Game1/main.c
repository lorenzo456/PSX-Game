#include "constants.h"
#include "controller.h"

int playerScore;
int playerLife;

int startScreen;
int winScreen;
int loseScreen;
int gameScreen;
char *startText ="PRESS START";
int Xprev;

MyBox enemies[20];
MyBox player;

Explosion explosion;
int explosionX;
int explosionY;

Bullet bullets[10];

int playerX;
int playerY;
float playerW;
int playerH;

int iterator;
int innerIterator;

int enemiesW;
int enemiesH;

int highScore;
int round; 
int spawnDistance;

int main() {
	initialize();

	while(1) {
		update();
		draw();
		display();
	}
}

void initialize() {
	resetScreens();
	initializeScreen();
	initializePad();
	
	startScreen = 1;

	round = 0;

	explosionX = 50;
	explosionY = 50;
	explosion = createExplosion(explosionX,explosionY);
	
	setBackgroundColor(createColor(0,0, 0));
	initializeEnemies();
	initializePlayer();
	
	Xprev = 0;
	highScore = playerScore;
}
void initializeEnemies()
{
	spawnDistance = 20;
	
	enemiesW = 10;
	enemiesH = 10;
	
	for(iterator = 0; iterator < 10; iterator++)
	{
		bullets[iterator] = createBullet(-200, -200, 10);
		bullets[iterator].used = 0;
	}
	
	for(iterator = 0; iterator < 20; iterator++)
	{
		enemies[iterator] = createMyBox(createColor(255,0,255), -20,-20,enemiesW,enemiesH);
	}
	
}
void initializePlayer()
{
	playerScore = 0;
	playerLife = 5;
	
	playerX = 200;
	playerY = 200;
	playerW = 10;
	playerH = 10;
	
	player = createMyBox(createColor(200,100,200), playerX,playerY,playerW,playerH);	
}

void resetScreens()
{
	gameScreen = 0;
	startScreen = 0;
}

void update()
{
	FntLoad(960, 256); // load the font from the BIOS into the framebuffer
	SetDumpFnt(FntOpen(5, 20, 320, 240, 0, 512)); // screen X,Y | max text length X,Y | autmatic background clear 0,1 | max characters

	padUpdate();
	checkControllerInput();
		
	if(startScreen == 1)
	{
		FntPrint(startText);
		if(padCheck(Pad1Start))
		{
			resetScreens();
			gameScreen = 1;
			playerLife = 5;
			playerScore = 0;
			round = 1;
			
		}
	}
	
	if(gameScreen == 1)
	{
		if(playerScore > highScore)
		{
			highScore = playerScore;
		}
		
		if(round == 1)
		{
			int lastPos = 0;
			for(iterator = 0; iterator < 20; iterator++)
			{
				if(iterator == 0)
				{
					enemies[iterator] = moveMyBox(enemies[iterator],0,getRandom(-30,0), enemiesW, enemiesH);
					lastPos = (enemies[iterator].width + enemies[iterator].x) + spawnDistance;
					continue;
				}
				enemies[iterator] = moveMyBox(enemies[iterator],(iterator * spawnDistance),getRandom(-30,0), enemiesW, enemiesH);
				lastPos += (enemies[iterator].width + enemies[iterator].x) + spawnDistance;// * enemyIterator;
			}
			round = 0;
		}
		

		FntPrint("THE HIGHSCORE IS: %d \n", highScore);
		FntPrint("PLAYER SCORE: %d \n", playerScore);
		FntPrint("LIVES: %d \n", playerLife);
	//	FntPrint("PLAYER: X: %d Y: %d \n", playerX, playerY);
	//	FntPrint("ENEMY: X: %d Y: %d \n", enemies[0].x, enemies[0].y);

		for(iterator = 0; iterator < 20; iterator++)
		{
			moveMyEnemy(&enemies[iterator], &enemies[iterator].y, &enemies[iterator].x);
			if( &enemies[iterator].y > 200)
			{
				checkPlayerBoxCOllision(&enemies[iterator], &enemies[iterator].x, &enemies[iterator].y, &enemies[iterator].width, &enemies[iterator].height);
			}
		}
		

		for(iterator = 0; iterator < 10; iterator++)
		{
			if(bullets[iterator].Y < 0)
			{
				bullets[iterator].used = 0;
				continue;
			}
			
			if(bullets[iterator].used == 1)
			{
				bullets[iterator].Y -= 2;
				bullets[iterator].Y2 -= 2;
				bullets[iterator] = moveBullet(bullets[iterator], bullets[iterator].X,bullets[iterator].Y, bullets[iterator].H);
				
				for(innerIterator = 0; innerIterator < 20; innerIterator++)
				{
					checkBulletCollision(&enemies[innerIterator], &enemies[innerIterator].x, &enemies[innerIterator].y, &enemies[innerIterator].width,&enemies[innerIterator].height, &bullets[iterator], &bullets[iterator].X, &bullets[iterator].Y);
				}

			}
			

		}
		
		if(playerLife < 0 && gameScreen == 1)
		{
			gameScreen = 0;
			startScreen = 1;
			resetEnemies();
		}
	}	
}

void resetEnemies()
{
	for(iterator = 0; iterator < 20; iterator++)
	{
		enemies[iterator] = moveMyBox(enemies[iterator],(iterator * spawnDistance),getRandom(-30,0), enemiesW, enemiesH);
	}
}

void checkControllerInput()
{
	if(padCheck(Pad1Up) && playerY > 150) {
		playerY -= 2;
		player = moveMyBox(player, playerX, playerY, playerW, playerH);
	}
	if(padCheck(Pad1Down) && playerY < 230) {
		playerY += 2;
		player = moveMyBox(player, playerX, playerY,playerW, playerH);

	}
	if(padCheck(Pad1Right) && playerX < 300) {
		playerX += 2;
		player = moveMyBox(player, playerX, playerY, playerW, playerH);

	}
	if(padCheck(Pad1Left) && playerX > 0) {
		playerX -= 2;
		player = moveMyBox(player, playerX, playerY, playerW, playerH);

	}
	
	if(padCheck(Pad1Cross) )
	{
		if(Xprev == 0 && gameScreen == 1)
		{
			spawnBullet();
		}
		Xprev = 1;
	}else
	{
		Xprev = 0;
	}
	
}

void draw()
{
	if(gameScreen == 1){
	
		for(iterator= 0; iterator < 20; iterator++)
		{
			drawMyBox(enemies[iterator]);
		}
		
		for(iterator = 0; iterator < 10; iterator++)
		{
			if(bullets[iterator].used == 0)
			{
				continue;
			}
			drawBullet(bullets[iterator]);
		}		
		drawMyBox(player);
	}
}

void spawnBullet()
{
	for(iterator = 0; iterator < 10; iterator++)
	{
		if(bullets[iterator].used == 0)
		{
			bullets[iterator].Y = playerY + (playerH * -1);
			bullets[iterator].X = playerX + (playerW * 0.5);
			bullets[iterator].used = 1;
			bullets[iterator] = moveBullet(bullets[iterator], bullets[iterator].X,bullets[iterator].Y, bullets[iterator].H);
			return;
		}
	}
}


void checkPlayerBoxCOllision(MyBox *b, int *bX, int *bY, int *bW, int *bH)
{	
	//AABB COLLISSION
	if(*bX < playerX + playerW && *bX + *bW > playerX && *bY < playerY + playerH && *bH + *bY > playerY)
	{
			explosionX = *bX;
			explosionY = *bY;
			explosion = moveExplosion(explosion,explosionX, explosionY);
			drawExplosion(explosion);
			playerLife--;
			*bY = 250;
	}
}

void checkBulletCollision(MyBox *b, int *bX, int *bY, int *bW, int *bH, Bullet *l, int *lX, int *lY)
{

	if(*lX > *bX && *lX < *bX + *bW && *lY > *bY && *lY < *bY + *bH)
	{
//		FntPrint("COLLISIONNN \n");
		explosionX = *lX;
		explosionY = *lY;
		explosion = moveExplosion(explosion,explosionX, explosionY);
		drawExplosion(explosion);

		*bY = 250;
		*lY = -20;
		playerScore++;
	}

}

int getRandom(int min, int max)
{
	int x;
	x = rand() % (max + 1 - (min)) + (min);	
	return x;
}

void moveMyEnemy(MyBox *b, int *y, int *x)
{
	if(*y >250)
	{
		*y = getRandom(-30,0);
	}else{
		*y += 2;
	}
	*b = moveMyBox(*b, *x, *y, 10, 10);
}