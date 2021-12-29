#define THC 5
#define screenSize 500
#define Myoffset 10

#define snakeThick 5



#include<GL/glut.h>
#include<vector>
#include<Windows.h>
#include<time.h>



int snakeDir = 1; //kierunek poruszania sie snake


int SnakeSpeed = 5;

float x_Snake, y_Snake; // współrzędne głowy snake




struct element  //struktura segmentu snake
{
	float x;
	float y;
};


element apple = { (rand() % 96 + 1) * snakeThick,(rand() % 96 + 1) * snakeThick }; // jabłko 


std::vector<element> snakeTiles; //tablica dynamiczna przechowująca informacje o położeniu wszystkich elementów snake

void Draw_Border() //rysowanie obramówki
{
	glColor3f(0, 0, 0);
	glLineWidth((GLfloat)snakeThick);
	glBegin(GL_LINES);

	//left
	glVertex2f(Myoffset, Myoffset);
	glVertex2f(Myoffset, screenSize - Myoffset);

	//right
	glVertex2f(screenSize - Myoffset, Myoffset);
	glVertex2f(screenSize - Myoffset, screenSize - Myoffset);

	//up
	glVertex2f(Myoffset, screenSize - Myoffset);
	glVertex2f(screenSize - Myoffset, screenSize - Myoffset);

	//down
	glVertex2f(Myoffset, Myoffset);
	glVertex2f(screenSize - Myoffset, Myoffset);

	glEnd();
}






bool AppleGood() //sprawdzanie czy jabłko nie wygenerowało się na snakeu
{
	for (int i = 0; i < snakeTiles.size(); i++)
	{
		if (apple.x == snakeTiles[i].x && apple.y == snakeTiles[i].y)
		{
			return false;
		}
	}
	return true;
}

void NewApple() //tworzenie nowego jabłka
{

	apple.x = (rand() % 96 + 1) * snakeThick;
	apple.y = (rand() % 96 + 1) * snakeThick;

	if (!AppleGood())
	{
		NewApple();
	}


}


void SnakeEat() //tworzenie ogona po zjedzeniu jabłka
{

	element tile = { snakeTiles[0].x,snakeTiles[0].y };

	snakeTiles.push_back(tile);
	snakeTiles.push_back(tile);
	snakeTiles.push_back(tile);

}

void checkAppleEat() //sprawdzanie czy snake nie zjadł jabłka w danej klatce gry
{
	if (snakeTiles[0].x == apple.x && snakeTiles[0].y == apple.y)
	{
		SnakeEat();
		NewApple();
	}
}
bool checkCollision() //sprawdzanie kolizji snake z obranówką planszy i samym sobą
{

	//walnal w sciane
	if (snakeTiles[0].x == Myoffset ||
		snakeTiles[0].x == screenSize - Myoffset ||
		snakeTiles[0].y == screenSize - Myoffset ||
		snakeTiles[0].y == Myoffset) return true;


	for (int i = snakeTiles.size() - 1; i > 0; i--)
	{
		if (snakeTiles[0].x == snakeTiles[i].x && snakeTiles[0].y == snakeTiles[i].y)
		{
			return true;
		}
	}

	checkAppleEat(); // po sprawdzeniu kolicji sprawdzane jest czy snake nie zjadł jabłka
	return false;


}
void DrawApple() //rysowanie jabłka na ekranie
{
	glColor3f(0, 1, 0);
	glLineWidth((GLfloat)snakeThick);
	glBegin(GL_POINTS);
	glVertex2f(apple.x, apple.y);
	glEnd();
}


void DrawSnake() //rysowanie snake na ekranie
{
	glColor3f(1, 0, 0);
	glLineWidth((GLfloat)snakeThick);
	glBegin(GL_POINTS);
	for (element tile : snakeTiles)
	{
		glVertex2f(tile.x, tile.y);
	}
	glEnd();

}

void InputFnc() //zczytywanie wejscia z klawiatury
{
	if (((GetKeyState('A') & 0x8000) || (GetKeyState(VK_LEFT) & 0x8000)) && snakeDir != 2)
	{
		snakeDir = 4;
	}
	else if (((GetKeyState('D') & 0x8000) || (GetKeyState(VK_RIGHT) & 0x8000)) && snakeDir != 4)
	{
		snakeDir = 2;
	}
	else if (((GetKeyState('W') & 0x8000) || (GetKeyState(VK_UP) & 0x8000)) && snakeDir != 3)
	{
		snakeDir = 1;
	}
	else if (((GetKeyState('S') & 0x8000) || (GetKeyState(VK_DOWN) & 0x8000)) && snakeDir != 1)
	{
		snakeDir = 3;
	}
}

void SnakePos(int t) //obliczanie kolejnej pozycji snake w zależności od zadanego kierunku co dany interwał czasowy
{


	for (int i = snakeTiles.size() - 1; i > 0; i--)
	{
		snakeTiles[i].x = snakeTiles[i - 1].x;
		snakeTiles[i].y = snakeTiles[i - 1].y;
	}
	if (snakeDir == 4)
	{

		snakeTiles[0].x -= snakeThick;
	}
	else if (snakeDir == 2)
	{
		snakeTiles[0].x += snakeThick;
	}
	else if (snakeDir == 1)
	{
		snakeTiles[0].y += snakeThick;
	}
	else if (snakeDir == 3)
	{
		snakeTiles[0].y -= snakeThick;
	}

	//glutPostRedisplay();
	if (!checkCollision()) {
		glutTimerFunc(1000 / SnakeSpeed, SnakePos, 0);
	}

}

void display() { //funkcja która wykonuje się co klatkę w pętli podczas działania programu
	glClear(GL_COLOR_BUFFER_BIT);



	Draw_Border();

	DrawSnake();
	DrawApple();

	InputFnc();
	//glutTimerFunc(500, SnakePos, 1);


	glFlush();



	glutPostRedisplay();
	//glutTimerFunc(500, SnakePos, 0);
}

void myinit() { //funkcja inicująca OpenGL oraz wartości wejścniowe do snake, wykonuje się raz przy włączeniu gry

	srand(time(0));
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(5.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 499.0, 0.0, 499.0);

	y_Snake = screenSize / 2; x_Snake = screenSize / 2;
	element st1 = { x_Snake,y_Snake };

	snakeTiles.push_back(st1);
	SnakeEat();
	NewApple();
	glutTimerFunc(1000 / SnakeSpeed, SnakePos, 0);
}

void main(int argc, char** argv) { //main



	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(screenSize, screenSize);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Points");
	glutDisplayFunc(display);


	myinit();




	glutMainLoop();
}