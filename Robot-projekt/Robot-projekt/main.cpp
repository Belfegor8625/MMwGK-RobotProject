#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
// sta³e do obs³ugi menu podrêcznego

enum
{
	FULL_WINDOW, // aspekt obrazu - ca³e okno
	ASPECT_1_1, // aspekt obrazu 1:1
	EXIT // wyjœcie
};

// aspekt obrazu

int Aspect = ASPECT_1_1;

// wpó³rzêdne po³o¿enia obserwatora

GLdouble eyex = 0;
GLdouble eyey = 110;
GLdouble eyez = 60;

// wspó³rzêdne punktu w którego kierunku jest zwrócony obserwator

GLdouble centerx = 0;
GLdouble centery = 0;
GLdouble centerz = -500;
GLfloat yLight = 0.0;
GLfloat moving_light_position[] = { 0.0, 100.0 + yLight,-25.0 };

float angle = 0;

//dane dla poruszania robotem
GLfloat movex = 0.0;
GLfloat movey = 0.0;
GLfloat movez = 0.0;
GLfloat rotatey = 0.0;
bool moveForward = false;
bool moveBackward = false;
bool turnLeft = false;
bool turnRight = false;
GLfloat leftLegUpperAngleX = 0.0, leftLegUpperAngleY = 0.0;
GLfloat rightLegUpperAngleX = 0.0, rightLegUpperAngleY = 0.0;
GLfloat leftLegLowerAngleX = 0.0, leftLegLowerAngleY = 0.0;
GLfloat rightLegLowerAngleX = 0.0, rightLegLowerAngleY = 0.0;
GLfloat leftLegFeetAngleX = 0.0, leftLegFeetAngleY = 0.0;
GLfloat rightLegFeetAngleX = 0.0, rightLegFeetAngleY = 0.0;
bool firstHalfOfStep = true;
bool straightenRightKnee = false;
bool rightStepFinisher = false;
bool rightLegStep = false, leftLegStep = false;

float oneStepDistance = 0.0;

// dane dla cylindra - szyi
float radius = 3.0;
float cylinderLength = 5.0;
int slices = 100;

float kneeRadius = 3.5;

void robotHeadAndTorso() {
	// G£OWA
	glPushMatrix();
	glBegin(GL_QUADS);
	//  góra
	glColor3f(1.0, 0.0, 0.0);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(-5.0, 100.0, -5.0);
	glVertex3f(5.0, 100.0, -5.0);
	glVertex3f(5.0, 100.0, 5.0);
	glVertex3f(-5.0, 100.0, 5.0);
	// ty³
	glColor3f(0.0, 1.0, 0.0);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(5.0, 90.0, -5.0);
	glVertex3f(5.0, 100.0, -5.0);
	glVertex3f(-5.0, 100.0, -5.0);
	glVertex3f(-5.0, 90.0, -5.0);
	// spód
	glColor3f(1.0, 0.5, 1.0);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(-5.0, 90.0, 5.0);
	glVertex3f(5.0, 90.0, 5.0);
	glVertex3f(5.0, 90.0, -5.0);
	glVertex3f(-5.0, 90.0, -5.0);
	// lewo
	glColor3f(0.0, 1.0, 1.0);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(-5.0, 100.0, -5.0);
	glVertex3f(-5.0, 100.0, 5.0);
	glVertex3f(-5.0, 90.0, 5.0);
	glVertex3f(-5.0, 90.0, -5.0);
	// prawo
	glColor3f(0.0, 0.0, 1.0);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(5.0, 100.0, 5.0);
	glVertex3f(5.0, 100.0, -5.0);
	glVertex3f(5.0, 90.0, -5.0);
	glVertex3f(5.0, 90.0, 5.0);
	// przód
	glColor3f(0.0, 1.0, 0.0);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(-5.0, 100.0, 5.0);
	glVertex3f(5.0, 100.0, 5.0);
	glVertex3f(5.0, 90.0, 5.0);
	glVertex3f(-5.0, 90.0, 5.0);
	glEnd();
	glPopMatrix();

	// SZYJA - cylinder	
	glPushMatrix();
	glTranslatef(0.0, 85.0, 0.0);
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.184, 0.310, 0.310);
	for (int i = 0; i <= slices; i++) {
		float theta = ((float)i)*2.0*M_PI / slices;
		glNormal3f(radius*sin(theta), 0.0, radius*cos(theta));
		glVertex3f(radius*sin(theta), cylinderLength, radius*cos(theta));
		glVertex3f(radius*sin(theta), 0.0, radius*cos(theta));
	}
	glEnd();
	glPopMatrix();

	// TU£ÓW
	glPushMatrix();
	glBegin(GL_QUADS);
	// góra
	glColor3f(0.184, 0.310, 0.310);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(-10.0, 85.0, -10.0);
	glVertex3f(10.0, 85.0, -10.0);
	glVertex3f(10.0, 85.0, 10.0);
	glVertex3f(-10.0, 85.0, 10.0);
	// ty³
	glColor3f(0.0, 1.0, 0.0);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(10.0, 85.0, -10.0);
	glVertex3f(-10.0, 85.0, -10.0);
	glVertex3f(-10.0, 45.0, -10.0);
	glVertex3f(10.0, 45.0, -10.0);
	// spód
	glColor3f(1.0, 0.5, 1.0);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(-10.0, 45.0, -10.0);
	glVertex3f(-10.0, 45.0, 10.0);
	glVertex3f(10.0, 45.0, 10.0);
	glVertex3f(10.0, 45.0, -10.0);
	// lewo
	glColor3f(0.0, 1.0, 1.0);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(-10.0, 85.0, -10.0);
	glVertex3f(-10.0, 85.0, 10.0);
	glVertex3f(-10.0, 45.0, 10.0);
	glVertex3f(-10.0, 45.0, -10.0);
	// prawo
	glColor3f(0.0, 0.0, 1.0);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(10.0, 85.0, 10.0);
	glVertex3f(10.0, 85.0, -10.0);
	glVertex3f(10.0, 45.0, -10.0);
	glVertex3f(10.0, 45.0, 10.0);
	// przód
	glColor3f(0.0, 1.0, 0.0);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(-10.0, 85.0, 10.0);
	glVertex3f(10.0, 85.0, 10.0);
	glVertex3f(10.0, 45.0, 10.0);
	glVertex3f(-10.0, 45.0, 10.0);
	glEnd();
	glPopMatrix();

	// £¥CZNIK Z PRAW¥ NOG¥
	glPushMatrix();
	glTranslated(-9.0, 0.0, 0.0);
	glTranslatef(0.0, 45.0, 0.0);
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.184, 0.310, 0.310);
	for (int i = 0; i <= slices; i++) {
		float theta = ((float)i)*2.0*M_PI / slices;
		glNormal3f(0.0, kneeRadius*cos(theta), kneeRadius*sin(theta));
		glVertex3f(0.0, kneeRadius*cos(theta), kneeRadius*sin(theta));
		glVertex3f(8.0, kneeRadius*cos(theta), kneeRadius*sin(theta));
	}
	glEnd();
	//prawa œcianka
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.184, 0.310, 0.310);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0); // œodek ko³a
	for (int i = 0; i <= slices; i++) {
		float theta = ((float)i)*2.0*M_PI / slices;
		glVertex3f(0.0, kneeRadius*cos(theta), kneeRadius*sin(theta));
	}
	glEnd();
	//lewa œcianka
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.184, 0.310, 0.310);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(8.0, 0.0, 0.0); // œodek ko³a
	for (int i = 0; i <= slices; i++) {
		float theta = ((float)i)*2.0*M_PI / slices;
		glVertex3f(8.0, kneeRadius*cos(theta), -kneeRadius*sin(theta));
	}
	glEnd();
	glPopMatrix();
	// £¥CZNIK Z LEW¥ NOG¥
	glPushMatrix();
	glTranslated(1.0, 0.0, 0.0);
	glTranslatef(0.0, 45.0, 0.0);
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.184, 0.310, 0.310);
	for (int i = 0; i <= slices; i++) {
		float theta = ((float)i)*2.0*M_PI / slices;
		glNormal3f(0.0, kneeRadius*cos(theta), kneeRadius*sin(theta));
		glVertex3f(0.0, kneeRadius*cos(theta), kneeRadius*sin(theta));
		glVertex3f(8.0, kneeRadius*cos(theta), kneeRadius*sin(theta));
	}
	glEnd();
	//prawa œcianka
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.184, 0.310, 0.310);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0); // œodek ko³a
	for (int i = 0; i <= slices; i++) {
		float theta = ((float)i)*2.0*M_PI / slices;
		glVertex3f(0.0, kneeRadius*cos(theta), kneeRadius*sin(theta));
	}
	glEnd();
	//lewa œcianka
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.184, 0.310, 0.310);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(8.0, 0.0, 0.0); // œodek ko³a
	for (int i = 0; i <= slices; i++) {
		float theta = ((float)i)*2.0*M_PI / slices;
		glVertex3f(8.0, kneeRadius*cos(theta), -kneeRadius*sin(theta));
	}
	glEnd();
	glPopMatrix();
}
void robotLeftLeg() {
	// NOGA LEWA - udo
	glPushMatrix();
	glRotatef(180, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -45.0, 0.0);
	glRotatef(-leftLegUpperAngleX, 1.0, 0.0, 0.0);
	glRotatef(leftLegUpperAngleY, 0.0, 1.0, 0.0);

	glBegin(GL_QUADS);
	// ty³
	glColor3f(0.0, 1.0, 0.0);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(8.0, 0.0, -3.0);
	glVertex3f(8.0, 20.0, -3.0);
	glVertex3f(2.0, 20.0, -3.0);
	glVertex3f(2.0, 0.0, -3.0);
	// lewo
	glColor3f(0.0, 1.0, 1.0);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(2.0, 20.0, -3.0);
	glVertex3f(2.0, 20.0, 3.0);
	glVertex3f(2.0, 0.0, 3.0);
	glVertex3f(2.0, 0.0, -3.0);
	// prawo
	glColor3f(0.0, 0.0, 1.0);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(8.0, 20.0, 3.0);
	glVertex3f(8.0, 20.0, -3.0);
	glVertex3f(8.0, 0.0, -3.0);
	glVertex3f(8.0, 0.0, 3.0);
	// przód
	glColor3f(0.0, 0.9, 0.5);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(2.0, 0.0, 3.0);
	glVertex3f(2.0, 20.0, 3.0);
	glVertex3f(8.0, 20.0, 3.0);
	glVertex3f(8.0, 0.0, 3.0);
	glEnd();
	glPopMatrix();
	// KOLANO LEWE
	glPushMatrix();
	glTranslated(1.0, 0.0, 0.0);
	glTranslatef(0.0, 45.0, 0.0);
	glRotatef(-leftLegUpperAngleX, 1.0, 0.0, 0.0);
	glRotatef(leftLegUpperAngleY, 0.0, 1.0, 0.0);
	glTranslatef(0.0, -20.0, 0.0);

	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.184, 0.310, 0.310);
	for (int i = 0; i <= slices; i++) {
		float theta = ((float)i)*2.0*M_PI / slices;
		glNormal3f(0.0, kneeRadius*cos(theta), kneeRadius*sin(theta));
		glVertex3f(0.0, kneeRadius*cos(theta), kneeRadius*sin(theta));
		glVertex3f(8.0, kneeRadius*cos(theta), kneeRadius*sin(theta));
	}
	glEnd();
	//prawa œcianka
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.184, 0.310, 0.310);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0); // œodek ko³a
	for (int i = 0; i <= slices; i++) {
		float theta = ((float)i)*2.0*M_PI / slices;
		glVertex3f(0.0, kneeRadius*cos(theta), kneeRadius*sin(theta));
	}
	glEnd();
	//lewa œcianka
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.184, 0.310, 0.310);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(8.0, 0.0, 0.0); // œodek ko³a
	for (int i = 0; i <= slices; i++) {
		float theta = ((float)i)*2.0*M_PI / slices;
		glVertex3f(8.0, kneeRadius*cos(theta), -kneeRadius*sin(theta));
	}
	glEnd();
	glPopMatrix();
	// NOGA LEWA - pod kolanem
	glPushMatrix();
	glRotatef(180, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -45.0, 0.0);
	glRotatef(-leftLegUpperAngleX, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 20.0, 0.0);
	glRotatef(leftLegLowerAngleX, 1.0, 0.0, 0.0);
	glRotatef(leftLegLowerAngleY, 0.0, 1.0, 0.0);

	glBegin(GL_QUADS);
	// ty³
	glColor3f(0.0, 1.0, 0.0);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(8.0, 0.0, -3.0);
	glVertex3f(8.0, 20.0, -3.0);
	glVertex3f(2.0, 20.0, -3.0);
	glVertex3f(2.0, 0.0, -3.0);
	// lewo
	glColor3f(0.0, 1.0, 1.0);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(2.0, 20.0, -3.0);
	glVertex3f(2.0, 20.0, 3.0);
	glVertex3f(2.0, 0.0, 3.0);
	glVertex3f(2.0, 0.0, -3.0);
	// prawo
	glColor3f(0.0, 0.0, 1.0);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(8.0, 20.0, 3.0);
	glVertex3f(8.0, 20.0, -3.0);
	glVertex3f(8.0, 0.0, -3.0);
	glVertex3f(8.0, 0.0, 3.0);
	// przód
	glColor3f(0.0, 0.9, 0.5);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(2.0, 0.0, 3.0);
	glVertex3f(2.0, 20.0, 3.0);
	glVertex3f(8.0, 20.0, 3.0);
	glVertex3f(8.0, 0.0, 3.0);
	glEnd();
	glPopMatrix();
	// £¥CZNIK ZE STOP¥
	glPushMatrix();
	glTranslated(1.0, 0.0, 0.0);
	glTranslatef(0.0, 45.0, 0.0);
	glRotatef(-leftLegUpperAngleX, 1.0, 0.0, 0.0);
	glRotatef(leftLegUpperAngleY, 0.0, 1.0, 0.0);
	glTranslatef(0.0, -20.0, 0.0);
	glRotatef(leftLegLowerAngleX, 1.0, 0.0, 0.0);
	glRotatef(leftLegLowerAngleY, 0.0, 1.0, 0.0);
	glTranslatef(0.0, -20.0, 0.0);
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.184, 0.310, 0.310);
	for (int i = 0; i <= slices; i++) {
		float theta = ((float)i)*2.0*M_PI / slices;
		glNormal3f(0.0, kneeRadius*cos(theta), kneeRadius*sin(theta));
		glVertex3f(0.0, kneeRadius*cos(theta), kneeRadius*sin(theta));
		glVertex3f(8.0, kneeRadius*cos(theta), kneeRadius*sin(theta));
	}
	glEnd();
	//prawa œcianka
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.184, 0.310, 0.310);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0); // œodek ko³a
	for (int i = 0; i <= slices; i++) {
		float theta = ((float)i)*2.0*M_PI / slices;
		glVertex3f(0.0, kneeRadius*cos(theta), kneeRadius*sin(theta));
	}
	glEnd();
	//lewa œcianka
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.184, 0.310, 0.310);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(8.0, 0.0, 0.0); // œodek ko³a
	for (int i = 0; i <= slices; i++) {
		float theta = ((float)i)*2.0*M_PI / slices;
		glVertex3f(8.0, kneeRadius*cos(theta), -kneeRadius*sin(theta));
	}
	glEnd();
	glPopMatrix();
	// STOPA LEWA
	glPushMatrix();
	glTranslatef(0.0, 45.0, 0.0);
	glRotatef(-leftLegUpperAngleX, 1.0, 0.0, 0.0);
	glRotatef(leftLegUpperAngleY, 0.0, 1.0, 0.0);
	glTranslatef(0.0, -20.0, 0.0);
	glRotatef(leftLegLowerAngleX, 1.0, 0.0, 0.0);
	glRotatef(leftLegLowerAngleY, 0.0, 1.0, 0.0);
	glTranslatef(0.0, -20.0, 0.0);
	glRotatef(leftLegFeetAngleX, 1.0, 0.0, 0.0);
	glRotatef(leftLegFeetAngleY, 0.0, 1.0, 0.0);
	glTranslatef(0.0, -5.0, 0.0);
	glBegin(GL_QUADS);
	//  góra
	glColor3f(1.0, 0.0, 0.0);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(2.0, 5.0, -3.0);
	glVertex3f(8.0, 5.0, -3.0);
	glVertex3f(8.0, 5.0, 10.0);
	glVertex3f(2.0, 5.0, 10.0);
	// ty³
	glColor3f(0.0, 1.0, 0.0);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(2.0, 0.0, -3.0);
	glVertex3f(8.0, 0.0, -3.0);
	glVertex3f(8.0, 5.0, -3.0);
	glVertex3f(2.0, 5.0, -3.0);
	// spód
	glColor3f(1.0, 0.5, 1.0);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(2.0, 0.0, -3.0);
	glVertex3f(2.0, 0.0, 10.0);
	glVertex3f(8.0, 0.0, 10.0);
	glVertex3f(8.0, 0.0, -3.0);
	// lewo
	glColor3f(0.0, 1.0, 1.0);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(2.0, 0.0, -3.0);
	glVertex3f(2.0, 5.0, -3.0);
	glVertex3f(2.0, 5.0, 10.0);
	glVertex3f(2.0, 0.0, 10.0);
	// prawo
	glColor3f(0.0, 0.0, 1.0);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(8.0, 0.0, 10.0);
	glVertex3f(8.0, 5.0, 10.0);
	glVertex3f(8.0, 5.0, -3.0);
	glVertex3f(8.0, 0.0, -3.0);
	// przód
	glColor3f(0.0, 1.0, 0.0);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(2.0, 0.0, 10.0);
	glVertex3f(2.0, 5.0, 10.0);
	glVertex3f(8.0, 5.0, 10.0);
	glVertex3f(8.0, 0.0, 10.0);
	glEnd();
	glPopMatrix();
}
void robotRightLeg() {
	// NOGA PRAWA - udo
	glPushMatrix();
	glTranslatef(-10.0, 0.0, 0.0); //przeniesienie drugiej nogi
	glPushMatrix();
	glRotatef(180, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -45.0, 0.0);
	glRotatef(-rightLegUpperAngleX, 1.0, 0.0, 0.0);
	glRotatef(rightLegUpperAngleY, 0.0, 1.0, 0.0);
	glBegin(GL_QUADS);
	// ty³
	glColor3f(0.0, 1.0, 0.0);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(8.0, 0.0, -3.0);
	glVertex3f(8.0, 20.0, -3.0);
	glVertex3f(2.0, 20.0, -3.0);
	glVertex3f(2.0, 0.0, -3.0);
	// lewo
	glColor3f(0.0, 1.0, 1.0);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(2.0, 20.0, -3.0);
	glVertex3f(2.0, 20.0, 3.0);
	glVertex3f(2.0, 0.0, 3.0);
	glVertex3f(2.0, 0.0, -3.0);
	// prawo
	glColor3f(0.0, 0.0, 1.0);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(8.0, 20.0, 3.0);
	glVertex3f(8.0, 20.0, -3.0);
	glVertex3f(8.0, 0.0, -3.0);
	glVertex3f(8.0, 0.0, 3.0);
	// przód
	glColor3f(0.0, 0.9, 0.5);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(2.0, 0.0, 3.0);
	glVertex3f(2.0, 20.0, 3.0);
	glVertex3f(8.0, 20.0, 3.0);
	glVertex3f(8.0, 0.0, 3.0);
	glEnd();
	glPopMatrix();
	// KOLANO PRAWE
	glPushMatrix();
	glTranslated(1.0, 0.0, 0.0);
	glTranslatef(0.0, 45.0, 0.0);
	glRotatef(-rightLegUpperAngleX, 1.0, 0.0, 0.0);
	glRotatef(rightLegUpperAngleY, 0.0, 1.0, 0.0);
	glTranslatef(0.0, -20.0, 0.0);
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.184, 0.310, 0.310);
	for (int i = 0; i <= slices; i++) {
		float theta = ((float)i)*2.0*M_PI / slices;
		glNormal3f(0.0, kneeRadius*cos(theta), kneeRadius*sin(theta));
		glVertex3f(0.0, kneeRadius*cos(theta), kneeRadius*sin(theta));
		glVertex3f(8.0, kneeRadius*cos(theta), kneeRadius*sin(theta));
	}
	glEnd();
	//prawa œcianka
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.184, 0.310, 0.310);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0); // œodek ko³a
	for (int i = 0; i <= slices; i++) {
		float theta = ((float)i)*2.0*M_PI / slices;
		glVertex3f(0.0, kneeRadius*cos(theta), kneeRadius*sin(theta));
	}
	glEnd();
	//lewa œcianka
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.184, 0.310, 0.310);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(8.0, 0.0, 0.0); // œodek ko³a
	for (int i = 0; i <= slices; i++) {
		float theta = ((float)i)*2.0*M_PI / slices;
		glVertex3f(8.0, kneeRadius*cos(theta), -kneeRadius*sin(theta));
	}
	glEnd();
	glPopMatrix();
	// NOGA PRAWA - pod kolanem
	glPushMatrix();
	glRotatef(180, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -45.0, 0.0);
	glRotatef(-rightLegUpperAngleX, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 20.0, 0.0);
	glRotatef(rightLegLowerAngleX, 1.0, 0.0, 0.0);
	glRotatef(rightLegLowerAngleY, 0.0, 1.0, 0.0);

	glBegin(GL_QUADS);
	// ty³
	glColor3f(0.0, 1.0, 0.0);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(8.0, 0.0, -3.0);
	glVertex3f(8.0, 20.0, -3.0);
	glVertex3f(2.0, 20.0, -3.0);
	glVertex3f(2.0, 0.0, -3.0);
	// lewo
	glColor3f(0.0, 1.0, 1.0);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(2.0, 20.0, -3.0);
	glVertex3f(2.0, 20.0, 3.0);
	glVertex3f(2.0, 0.0, 3.0);
	glVertex3f(2.0, 0.0, -3.0);
	// prawo
	glColor3f(0.0, 0.0, 1.0);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(8.0, 20.0, 3.0);
	glVertex3f(8.0, 20.0, -3.0);
	glVertex3f(8.0, 0.0, -3.0);
	glVertex3f(8.0, 0.0, 3.0);
	// przód
	glColor3f(0.0, 0.9, 0.5);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(2.0, 0.0, 3.0);
	glVertex3f(2.0, 20.0, 3.0);
	glVertex3f(8.0, 20.0, 3.0);
	glVertex3f(8.0, 0.0, 3.0);
	glEnd();
	glPopMatrix();
	// £¥CZNIK ZE STOP¥
	glPushMatrix();
	glTranslated(1.0, 0.0, 0.0);
	glTranslatef(0.0, 45.0, 0.0);
	glRotatef(-rightLegUpperAngleX, 1.0, 0.0, 0.0);
	glRotatef(rightLegUpperAngleY, 0.0, 1.0, 0.0);
	glTranslatef(0.0, -20.0, 0.0);
	glRotatef(rightLegLowerAngleX, 1.0, 0.0, 0.0);
	glRotatef(rightLegLowerAngleY, 0.0, 1.0, 0.0);
	glTranslatef(0.0, -20.0, 0.0);
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.184, 0.310, 0.310);
	for (int i = 0; i <= slices; i++) {
		float theta = ((float)i)*2.0*M_PI / slices;
		glNormal3f(0.0, kneeRadius*cos(theta), kneeRadius*sin(theta));
		glVertex3f(0.0, kneeRadius*cos(theta), kneeRadius*sin(theta));
		glVertex3f(8.0, kneeRadius*cos(theta), kneeRadius*sin(theta));
	}
	glEnd();
	//prawa œcianka
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.184, 0.310, 0.310);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0); // œodek ko³a
	for (int i = 0; i <= slices; i++) {
		float theta = ((float)i)*2.0*M_PI / slices;
		glVertex3f(0.0, kneeRadius*cos(theta), kneeRadius*sin(theta));
	}
	glEnd();
	//lewa œcianka
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.184, 0.310, 0.310);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(8.0, 0.0, 0.0); // œodek ko³a
	for (int i = 0; i <= slices; i++) {
		float theta = ((float)i)*2.0*M_PI / slices;
		glVertex3f(8.0, kneeRadius*cos(theta), -kneeRadius*sin(theta));
	}
	glEnd();
	glPopMatrix();
	// STOPA PRAWA
	glPushMatrix();
	glTranslatef(0.0, 45.0, 0.0);
	glRotatef(-rightLegUpperAngleX, 1.0, 0.0, 0.0);
	glRotatef(rightLegUpperAngleY, 0.0, 1.0, 0.0);
	glTranslatef(0.0, -20.0, 0.0);
	glRotatef(rightLegLowerAngleX, 1.0, 0.0, 0.0);
	glRotatef(rightLegLowerAngleY, 0.0, 1.0, 0.0);
	glTranslatef(0.0, -20.0, 0.0);
	glRotatef(rightLegFeetAngleX, 1.0, 0.0, 0.0);
	glRotatef(rightLegFeetAngleY, 0.0, 1.0, 0.0);
	glTranslatef(0.0, -5.0, 0.0);
	glBegin(GL_QUADS);
	//  góra
	glColor3f(1.0, 0.0, 0.0);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(2.0, 5.0, -3.0);
	glVertex3f(8.0, 5.0, -3.0);
	glVertex3f(8.0, 5.0, 10.0);
	glVertex3f(2.0, 5.0, 10.0);
	// ty³
	glColor3f(0.0, 1.0, 0.0);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(2.0, 0.0, -3.0);
	glVertex3f(8.0, 0.0, -3.0);
	glVertex3f(8.0, 5.0, -3.0);
	glVertex3f(2.0, 5.0, -3.0);
	// spód
	glColor3f(1.0, 0.5, 1.0);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(2.0, 0.0, -3.0);
	glVertex3f(2.0, 0.0, 10.0);
	glVertex3f(8.0, 0.0, 10.0);
	glVertex3f(8.0, 0.0, -3.0);
	// lewo
	glColor3f(0.0, 1.0, 1.0);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(2.0, 0.0, -3.0);
	glVertex3f(2.0, 5.0, -3.0);
	glVertex3f(2.0, 5.0, 10.0);
	glVertex3f(2.0, 0.0, 10.0);
	// prawo
	glColor3f(0.0, 0.0, 1.0);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(8.0, 0.0, 10.0);
	glVertex3f(8.0, 5.0, 10.0);
	glVertex3f(8.0, 5.0, -3.0);
	glVertex3f(8.0, 0.0, -3.0);
	// przód
	glColor3f(0.0, 1.0, 0.0);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(2.0, 0.0, 10.0);
	glVertex3f(2.0, 5.0, 10.0);
	glVertex3f(8.0, 5.0, 10.0);
	glVertex3f(8.0, 0.0, 10.0);
	glEnd();
	glPopMatrix();
	glPopMatrix();
}
void robotLeftArm() {
	// £¥CZNIK Z RAMIENIEM LEWYM
	glPushMatrix();
	glTranslatef(10.0, 80.0, 0.0);
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.184, 0.310, 0.310);
	for (int i = 0; i <= slices; i++) {
		float theta = ((float)i)*2.0*M_PI / slices;
		glNormal3f(0.0, radius*cos(theta), radius*sin(theta));
		glVertex3f(0.0, radius*cos(theta), radius*sin(theta));
		glVertex3f(cylinderLength, radius*cos(theta), radius*sin(theta));
	}
	glEnd();
	glPopMatrix();
	// RAMIÊ LEWE
	glPushMatrix();
	glTranslatef(0.0, -5.0, 0.0);
	glBegin(GL_QUADS);
	//  góra
	glColor3f(1.0, 0.0, 0.0);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(25.0, 90.0, -5.0);
	glVertex3f(25.0, 90.0, 5.0);
	glVertex3f(15.0, 90.0, 5.0);
	glVertex3f(15.0, 90.0, -5.0);
	// ty³
	glColor3f(0.0, 1.0, 0.0);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(25.0, 90.0, -5.0);
	glVertex3f(15.0, 90.0, -5.0);
	glVertex3f(15.0, 80.0, -5.0);
	glVertex3f(25.0, 80.0, -5.0);
	// spód
	glColor3f(1.0, 0.5, 1.0);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(15.0, 80.0, 5.0);
	glVertex3f(25.0, 80.0, 5.0);
	glVertex3f(25.0, 80.0, -5.0);
	glVertex3f(15.0, 80.0, -5.0);
	// lewo
	glColor3f(0.0, 1.0, 1.0);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(15.0, 90.0, -5.0);
	glVertex3f(15.0, 90.0, 5.0);
	glVertex3f(15.0, 80.0, 5.0);
	glVertex3f(15.0, 80.0, -5.0);
	// prawo
	glColor3f(0.0, 0.0, 1.0);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(25.0, 90.0, 5.0);
	glVertex3f(25.0, 90.0, -5.0);
	glVertex3f(25.0, 80.0, -5.0);
	glVertex3f(25.0, 80.0, 5.0);
	// przód
	glColor3f(0.0, 1.0, 0.0);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(15.0, 80.0, 5.0);
	glVertex3f(15.0, 90.0, 5.0);
	glVertex3f(25.0, 90.0, 5.0);
	glVertex3f(25.0, 80.0, 5.0);
	glEnd();
	glPopMatrix();

	// RÊKA LEWA
	glPushMatrix();
	glTranslatef(0.0, -5.0, 0.0);
	glBegin(GL_QUADS);
	// ty³
	glColor3f(0.0, 1.0, 0.0);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(17.0, 40.0, -3.0);
	glVertex3f(23.0, 40.0, -3.0);
	glVertex3f(23.0, 80.0, -3.0);
	glVertex3f(17.0, 80.0, -3.0);
	// spód
	glColor3f(1.0, 0.5, 1.0);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(17.0, 40.0, -3.0);
	glVertex3f(17.0, 40.0, 3.0);
	glVertex3f(23.0, 40.0, 3.0);
	glVertex3f(23.0, 40.0, -3.0);
	// lewo
	glColor3f(0.0, 1.0, 1.0);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(17.0, 80.0, -3.0);
	glVertex3f(17.0, 80.0, 3.0);
	glVertex3f(17.0, 40.0, 3.0);
	glVertex3f(17.0, 40.0, -3.0);
	// prawo
	glColor3f(0.0, 0.0, 1.0);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(23.0, 80.0, 3.0);
	glVertex3f(23.0, 80.0, -3.0);
	glVertex3f(23.0, 40.0, -3.0);
	glVertex3f(23.0, 40.0, 3.0);
	// przód
	glColor3f(0.0, 1.0, 0.0);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(17.0, 80.0, 3.0);
	glVertex3f(23.0, 80.0, 3.0);
	glVertex3f(23.0, 40.0, 3.0);
	glVertex3f(17.0, 40.0, 3.0);
	glEnd();
	glPopMatrix();
}
void robotRightArm() {
	// £¥CZNIK Z RAMIENIEM PRAWYM
	glPushMatrix();;
	glTranslatef(-15.0, 80.0, 0.0);
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.184, 0.310, 0.310);
	for (int i = 0; i <= slices; i++) {
		float theta = ((float)i)*2.0*M_PI / slices;
		glNormal3f(0.0, radius*cos(theta), radius*sin(theta));
		glVertex3f(0.0, radius*cos(theta), radius*sin(theta));
		glVertex3f(cylinderLength, radius*cos(theta), radius*sin(theta));
	}
	glEnd();
	glPopMatrix();
	// RAMIÊ PRAWE
	glPushMatrix();
	glTranslatef(-40.0, -5.0, 0.0);
	glBegin(GL_QUADS);
	//  góra
	glColor3f(1.0, 0.0, 0.0);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(25.0, 90.0, -5.0);
	glVertex3f(25.0, 90.0, 5.0);
	glVertex3f(15.0, 90.0, 5.0);
	glVertex3f(15.0, 90.0, -5.0);
	// ty³
	glColor3f(0.0, 1.0, 0.0);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(25.0, 90.0, -5.0);
	glVertex3f(15.0, 90.0, -5.0);
	glVertex3f(15.0, 80.0, -5.0);
	glVertex3f(25.0, 80.0, -5.0);
	// spód
	glColor3f(1.0, 0.5, 1.0);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(15.0, 80.0, 5.0);
	glVertex3f(25.0, 80.0, 5.0);
	glVertex3f(25.0, 80.0, -5.0);
	glVertex3f(15.0, 80.0, -5.0);
	// lewo
	glColor3f(0.0, 1.0, 1.0);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(15.0, 90.0, -5.0);
	glVertex3f(15.0, 90.0, 5.0);
	glVertex3f(15.0, 80.0, 5.0);
	glVertex3f(15.0, 80.0, -5.0);
	// prawo
	glColor3f(0.0, 0.0, 1.0);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(25.0, 90.0, 5.0);
	glVertex3f(25.0, 90.0, -5.0);
	glVertex3f(25.0, 80.0, -5.0);
	glVertex3f(25.0, 80.0, 5.0);
	// przód
	glColor3f(0.0, 1.0, 0.0);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(15.0, 80.0, 5.0);
	glVertex3f(15.0, 90.0, 5.0);
	glVertex3f(25.0, 90.0, 5.0);
	glVertex3f(25.0, 80.0, 5.0);
	glEnd();
	glPopMatrix();

	// RÊKA PRAWA
	glPushMatrix();
	glTranslatef(-40.0, -5.0, 0.0);
	glBegin(GL_QUADS);
	// ty³
	glColor3f(0.0, 1.0, 0.0);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(17.0, 40.0, -3.0);
	glVertex3f(23.0, 40.0, -3.0);
	glVertex3f(23.0, 80.0, -3.0);
	glVertex3f(17.0, 80.0, -3.0);
	// spód
	glColor3f(1.0, 0.5, 1.0);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(17.0, 40.0, -3.0);
	glVertex3f(17.0, 40.0, 3.0);
	glVertex3f(23.0, 40.0, 3.0);
	glVertex3f(23.0, 40.0, -3.0);
	// lewo
	glColor3f(0.0, 1.0, 1.0);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(17.0, 80.0, -3.0);
	glVertex3f(17.0, 80.0, 3.0);
	glVertex3f(17.0, 40.0, 3.0);
	glVertex3f(17.0, 40.0, -3.0);
	// prawo
	glColor3f(0.0, 0.0, 1.0);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(23.0, 80.0, 3.0);
	glVertex3f(23.0, 80.0, -3.0);
	glVertex3f(23.0, 40.0, -3.0);
	glVertex3f(23.0, 40.0, 3.0);
	// przód
	glColor3f(0.0, 1.0, 0.0);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(17.0, 80.0, 3.0);
	glVertex3f(23.0, 80.0, 3.0);
	glVertex3f(23.0, 40.0, 3.0);
	glVertex3f(17.0, 40.0, 3.0);
	glEnd();
	glPopMatrix();
}
void renderRobot() {
	robotHeadAndTorso();
	robotLeftArm();
	robotRightArm();
	robotLeftLeg();
	robotRightLeg();

}

void renderTerrain() {
	// POD£O¯E
	glBegin(GL_POLYGON);
	glColor3f(0.627, 0.322, 0.176);
	glVertex3f(-1000.0, 0.0, -1000.0);
	glVertex3f(1000.0, 0.0, -1000.0);
glVertex3f(1000.0, 0.0, 1000.0);
glVertex3f(-1000.0, 0.0, 1000.0);
glEnd();
}

void rightLegAngleChecker() {
	if (rightLegUpperAngleX > 30) {
		rightLegUpperAngleX = 30.0;
	}
	else if (rightLegUpperAngleX < 0) {
		rightLegUpperAngleX = 0.0;
	}

	if (rightLegLowerAngleX > 42) {
		rightLegLowerAngleX = 42.0;
		straightenRightKnee = !straightenRightKnee;
	}
	else if (rightLegLowerAngleX > 30 & rightStepFinisher) {
		rightLegLowerAngleX = 30.0;
		straightenRightKnee = !straightenRightKnee;
	}
	else if (rightLegLowerAngleX < 0) {
		rightLegUpperAngleX = 0.0;
		rightLegLowerAngleX = 0.0;
		rightLegFeetAngleX = 0.0;
		leftLegUpperAngleX = 0.0;
		leftLegLowerAngleX = 0.0;
		leftLegFeetAngleX = 0.0;
		firstHalfOfStep = true;
		straightenRightKnee = false;
		rightStepFinisher = false;
		moveForward = false;
		moveBackward = false;
	}

	if (rightLegFeetAngleX > 25.8) {
		rightLegFeetAngleX = 25.8;
		straightenRightKnee = !straightenRightKnee;
		rightStepFinisher = !rightStepFinisher;
		firstHalfOfStep = !firstHalfOfStep;
	}
	else if (rightLegFeetAngleX < 0) {
		rightLegFeetAngleX = 0.0;
	}

}
void leftLegAngleChecker() {
	if (leftLegUpperAngleX > 0 && firstHalfOfStep) {
		leftLegUpperAngleX = 0.0;
	}
	else if (leftLegUpperAngleX > 10) {
		leftLegUpperAngleX = 10.0;
	}
	else if (leftLegUpperAngleX < -15) {
		leftLegUpperAngleX = -15.0;
	}

	if (leftLegLowerAngleX > 21.9 & firstHalfOfStep) {
		leftLegLowerAngleX = 21.9;
	}
	else if (leftLegLowerAngleX < 0) {
		leftLegLowerAngleX = 0.0;
	}

	if (leftLegFeetAngleX > 10) {
		leftLegFeetAngleX = 10.0;
	}
	else if (leftLegFeetAngleX < -36.9) {
		leftLegFeetAngleX = -36.9;
	}

}
void yAxisChecker() {
	if (movey < -4.7) {
		movey = -4.7;
	}
	else if (movey > 0) {
		movey = 0.0;
	}
}
void moveAnimation(int value) {
	glutTimerFunc(10, moveAnimation, -1);
	if (moveForward) {
		if (firstHalfOfStep) {
			if (straightenRightKnee) {
				movez = movez + cos(angle);
				movex = movex + sin(angle);
				if (rightLegLowerAngleX < 30) {
					rightLegFeetAngleX += 0.3;
				}
				if (rightLegLowerAngleX < 4.2) {
					rightLegLowerAngleX = 4.2;
				}
				leftLegUpperAngleX -= 0.2;
				leftLegLowerAngleX += 0.203;
				rightLegLowerAngleX -= 0.35;
				leftLegFeetAngleX -= 0.35;
				movey -= 0.044;
			}
			else {
				rightLegUpperAngleX += 0.25;
				rightLegLowerAngleX += 0.35;
			}
		}
		else {
			movez = movez + cos(angle);
			movex = movex + sin(angle);
			if (rightStepFinisher) {
				if (straightenRightKnee) {
					if (rightLegUpperAngleX < 10){
						leftLegUpperAngleX -= 0.25;
						leftLegLowerAngleX -= 0.5475;
					}
					else {
						leftLegUpperAngleX += 0.3125;
						leftLegFeetAngleX += 0.11125;
					}
					rightLegUpperAngleX -= 0.25;
					rightLegLowerAngleX -= 0.25;
				}
				else {
					rightLegLowerAngleX += 0.35;
					rightLegFeetAngleX -= 0.35;
					leftLegFeetAngleX += 0.35;
				}
				movey += 0.044;
			}
		}
	}
	if (moveBackward) {
		if (firstHalfOfStep) {
			if (straightenRightKnee) {
				movez = movez - cos(angle);
				movex = movex - sin(angle);
				rightLegFeetAngleX -= 0.3;
				leftLegUpperAngleX += 0.2;
				leftLegLowerAngleX -= 0.203;
				rightLegLowerAngleX += 0.35;
				leftLegFeetAngleX += 0.35;
				movey += 0.044;
			}
			else {
				rightLegUpperAngleX -= 0.25;
				rightLegLowerAngleX -= 0.35;
			}
		}
		else {
			if (rightStepFinisher) {
				if (straightenRightKnee) {
					movez = movez - cos(angle);
					movex = movex - sin(angle);
					if (rightLegUpperAngleX < 10) {
						leftLegUpperAngleX += 0.25;
						leftLegLowerAngleX += 0.5475;
					}
					else {
						leftLegUpperAngleX -= 0.3125;
						leftLegFeetAngleX -= 0.11125;
					}
					rightLegUpperAngleX += 0.25;
					rightLegLowerAngleX += 0.25;
				}
				else {
					rightLegLowerAngleX -= 0.35;
					rightLegFeetAngleX += 0.35;
					leftLegFeetAngleX -= 0.35;
				}
				movey -= 0.044;
			}
		}
	}
	if (turnLeft) {
		rotatey++;
		if (rotatey == 360) {
			rotatey = 0;
		}
	}
	else if (turnRight) {
		rotatey--;
		if (rotatey == -360) {
			rotatey = 0;
		}
	}
	rightLegAngleChecker();
	leftLegAngleChecker();
	yAxisChecker();
	glutPostRedisplay();
}

// funkcja generuj¹ca scenê 3D
void Display()
{
	// kolor t³a - zawartoœæ bufora koloru
	glClearColor(0.529, 0.808, 0.922, 1.0);
	// czyszczenie bufora koloru
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// wybór macierzy modelowania
	glMatrixMode(GL_MODELVIEW);
	// macierz modelowania = macierz jednostkowa
	glLoadIdentity();
	// ustawienie obserwatora
	gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, 0.0, 10.0, 0.0);

	glPushMatrix();
	angle = rotatey*2.0*M_PI / 360;
	glTranslatef(movex*0.1, movey, movez*0.1);
	glRotatef(rotatey, 0.0, 1.0, 0.0);
	renderRobot();
	glPopMatrix();

	renderTerrain();

	// skierowanie poleceñ do wykonania
	glFlush();

	// zamiana buforów koloru
	glutSwapBuffers();
	glutPostRedisplay();
}

// zmiana wielkoœci okna

void Reshape(int width, int height)
{
	// obszar renderingu - ca³e okno
	glViewport(0, 0, width, height);

	// wybór macierzy rzutowania
	glMatrixMode(GL_PROJECTION);

	// macierz rzutowania = macierz jednostkowa
	glLoadIdentity();

	// parametry bry³y obcinania
	if (Aspect == ASPECT_1_1)
	{
		// szerokoœæ okna wiêksza od wysokoœci okna
		if (width < height && width > 0)
			glFrustum(-2.0, 2.0, -2.0 * height / width, 2.0 * height / width, 1.0, 500.0);
		else

			// szerokoœæ okna wiêksza lub równa wysokoœci okna
			if (width >= height && height > 0)
				glFrustum(-2.0 * width / height, 2.0 * width / height, -2.0, 2.0, 1.0, 500.0);

	}
	else
		glFrustum(-2.0, 2.0, -2.0, 2.0, 1.0, 20.0);
	//glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 5.0);
	// generowanie sceny 3D
	Display();
}

// obs³uga klawiatury

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '+': // klawisz +
		eyez -= 1.0;
		break;
	case '-': // klawisz -
		eyez += 1.0;
		break;
	case '8': // klawisz 8
		eyey += 1.0;
		break;
	case '2': // klawisz 2
		eyey -= 1.0;
		break;
	case 'w':
		moveForward = true;
		moveBackward = false;
		break;
	case 's':
		moveBackward = true;
		moveForward = false;
		break;
	default: break;
	}

	// odrysowanie okna
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

// obs³uga klawiszy funkcyjnych i klawiszy kursora

void SpecialKeys(int key, int x, int y)
{
	// kursor w lewo
	if (key == GLUT_KEY_LEFT) {
		turnLeft = true;
	}
	// kursor w górê
	if (key == GLUT_KEY_UP) {
		moveForward = true;
	}
	// kursor w prawo
	if (key == GLUT_KEY_RIGHT) {
		turnRight = true;
	}
	// kursor w dó³
	if (key == GLUT_KEY_DOWN) {
		moveBackward = true;
	}
	//std::cout << straightenRightKnee << " " << rightStepFinisher << " " << std::endl;
	//std::cout << rightLegUpperAngleX << " " << rightLegLowerAngleX << " " << rightLegFeetAngleX << std::endl;
	std::cout << leftLegUpperAngleX << " " << leftLegLowerAngleX << " " << leftLegFeetAngleX << std::endl;
	//std::cout << movex << " " << movez << " " << angle << " " << rotatey << std::endl;
	//std::cout << moveForward << " " << moveBackward << " " << turnLeft << " " << turnRight << std::endl;
	// odrysowanie okna
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}
void SpecialKeysRelease(int key, int x, int y)
{
	// kursor w lewo
	if (key == GLUT_KEY_LEFT) {
		turnLeft = false;
	}
	// kursor w górê
	if (key == GLUT_KEY_UP) {
		moveForward = false;
	}
	// kursor w prawo
	if (key == GLUT_KEY_RIGHT) {
		turnRight = false;
	}
	// kursor w dó³
	if (key == GLUT_KEY_DOWN) {
		moveBackward = false;
	}
	// odrysowanie okna
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

// obs³uga menu podrêcznego

void Menu(int value)
{
	switch (value)
	{
		// obszar renderingu - ca³e okno
	case FULL_WINDOW:
		Aspect = FULL_WINDOW;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		break;

		// obszar renderingu - aspekt 1:1
	case ASPECT_1_1:
		Aspect = ASPECT_1_1;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		break;

		// wyjœcie
	case EXIT:
		exit(0);
	}
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	// oœwietlenie globalne
	GLfloat globalAmbient[] = { 0.3,0.3,0.3,1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
	// pozycjonowane oœwietlenie - s³oñce
	GLfloat light_diffuse[] = { 1.0,1.0,1.0,1.0 }; // jasnoœæ
	GLfloat light_position0[] = { 10.0, 40, 25.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
}

int main(int argc, char * argv[])
{
	// inicjalizacja biblioteki GLUT
	glutInit(&argc, argv);

	// inicjalizacja bufora ramki
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	// rozmiary g³ównego okna programu
	glutInitWindowSize(800, 800);
	// utworzenie g³ównego okna programu
#ifdef WIN32
	glutCreateWindow("Robot");
#else
	glutCreateWindow("Robot");
#endif
	init();
	// do³¹czenie funkcji generuj¹cej scenê 3D
	glutDisplayFunc(Display);
	// do³¹czenie funkcji wywo³ywanej przy zmianie rozmiaru okna
	glutReshapeFunc(Reshape);
	// do³¹czenie funkcji obs³ugi klawiatury
	glutKeyboardFunc(Keyboard);
	// do³¹czenie funkcji obs³ugi klawiszy funkcyjnych i klawiszy kursora
	glutSpecialFunc(SpecialKeys);
	glutSpecialUpFunc(SpecialKeysRelease);
	// utworzenie menu podrêcznego
	glutCreateMenu(Menu);
	// dodanie pozycji do menu podrêcznego
#ifdef WIN32

	glutAddMenuEntry("Aspekt obrazu - ca³e okno", FULL_WINDOW);
	glutAddMenuEntry("Aspekt obrazu 1:1", ASPECT_1_1);
	glutAddMenuEntry("Wyjœcie", EXIT);
#else

	glutAddMenuEntry("Aspekt obrazu - cale okno", FULL_WINDOW);
	glutAddMenuEntry("Aspekt obrazu 1:1", ASPECT_1_1);
	glutAddMenuEntry("Wyjscie", EXIT);
#endif
	// okreœlenie przycisku myszki obs³uguj¹cej menu podrêczne
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	// wprowadzenie programu do obs³ugi pêtli komunikatów

	glutTimerFunc(10, moveAnimation, -1);
	glutMainLoop();
	return 0;
}