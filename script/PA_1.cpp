#include <stdlib.h>
#include <math.h>

#include <glut.h>
#include <gl\GLU.h>

enum state{dot, line, triangle, quadangle, pentagon, idle};
state currentState;
state previousState;

float radius;

bool isIdle;
float idleTimer;
int idleSides;
float idleDuration;

#pragma region Draw

void draw_dot(float size) {
	glPointSize(size);
	glBegin(GL_POINTS);
	
	glVertex3f(0.0f, 0.0f, 0.0f);
	
	glEnd();
}

void draw_line(float length, float width) {
	glLineWidth(width);
	glBegin(GL_LINES);

	glVertex3f(length / -2.0f, 0.0f, 0.0f);
	glVertex3f(length / +2.0f, 0.0f, 0.0f);

	glEnd();
}

#define PI 3.14159265

void draw_polygon(int sides, float radius) {
	// correct value
	sides = fmax(3.0, sides);
	radius = fmax(0.1, radius);

	// draw
	glBegin(GL_POLYGON);

	for(int i = 0; i < sides; i++) {
		float degree = i * (360.0f / sides) ;
		if (sides == 4) degree += 45.0;
		else if (sides % 2 == 1) degree += 90.0 ;
		glVertex3f(radius * cosf(degree * PI / 180.0f), radius * sinf(degree * PI / 180.0f), 0.0f);
	}

	glEnd();
}

#pragma endregion 

#pragma region Idle

void idle_off(void){
	isIdle = false;
	currentState = previousState;
}

#pragma endregion 

#pragma region CallBacks

void display(void){
	glClearDepth(1.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.0f, 1.0f, 1.0f);

	switch (currentState)
	{
	case dot:
		draw_dot(radius * 10);
		break;
	case line:
		draw_line(radius, radius * 10);
		break;
	case triangle:
		draw_polygon(3, radius);
		break;
	case quadangle:
		draw_polygon(4, radius);
		break;
	case pentagon:
		draw_polygon(5, radius);
		break;
	case idle:
		glColor3f(1.0f, 1.0f, 0.0f);
		draw_polygon(idleSides, radius);
		break;
	}

	glutSwapBuffers();
}

void reshape(int w, int h){
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(-20, 20, -20, 20);
	glutPostRedisplay();
}

void myMouseClick(GLint button, GLint state, GLint x, GLint y)
{
	isIdle = false;
	currentState = previousState;
	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		radius = fmax(0.1f, radius - 0.1f);
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		radius = fmin(1.0f, radius + 0.1f);

	glutPostRedisplay();
}

void myMouseMotion(GLint x, GLint y)
{
	idle_off();
}

void myKeyboard(unsigned char key, int x, int y)
{	
	idle_off();

	switch (key)
	{
	case '1':
		currentState = dot;
		previousState = dot;
		break;
	case '2':
		currentState = line;
		previousState = line;
		break;
	case '3':
		currentState = triangle;
		previousState = triangle;
		break;
	case '4':
		currentState = quadangle;
		previousState = quadangle;
		break;
	case '5':
		currentState = pentagon;
		previousState = pentagon;
		break;
	}

	glutPostRedisplay();
}

void myIdle(void)
{
	// begins to idle
	if (currentState != idle && !isIdle){
		isIdle = true;
		idleTimer = 0.0f;
		idleSides = 3;
	}

	glutPostRedisplay();
}


void TimerFunc(int value)
{	
	if (isIdle){
		idleTimer += 0.001;

		if (currentState != idle && idleTimer >= 3.0f){
			previousState = currentState;
			currentState = idle;
			idleTimer = 0.0f;
		}
		else if (currentState == idle && idleTimer >= idleDuration){
			idleSides += 1;
			idleTimer = 0.0f;
		}

		// loop
		if (idleSides > 360)
			idleSides = 3;
	}
		

	glutPostRedisplay();
	glutTimerFunc(1, TimerFunc, 1);
}

#pragma endregion 

void main(int argc, char** argv) {
	currentState = idle;
	previousState = triangle;
	radius = 0.3f;
	isIdle = true;
	idleTimer = 0.0f;
	idleSides = 3;
	idleDuration = 0.05f;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("PA_1");

	glutDisplayFunc(display);
	// glutReshapeFunc(reshape);
	glutMouseFunc(myMouseClick);
	glutMotionFunc(myMouseMotion);
	glutKeyboardFunc(myKeyboard);
	glutTimerFunc(100, TimerFunc, 1);
	glutIdleFunc(myIdle);

	glutMainLoop();
}