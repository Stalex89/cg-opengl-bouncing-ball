#include "stdafx.h" 
#include "conio.h"
#include "glaux.h"


GLfloat rotation = 0.00;
GLfloat rotation_speed = 2.0;


GLfloat velocity = 0.0;
GLfloat velocity2 = 0.004;
GLfloat acceleration = -0.00003;
GLfloat position = 3.0;
GLfloat position2 = 0.0;

GLfloat ball_radius = 0.6;
GLfloat floor = 0.0;


void draw_floor();

void CALLBACK resize(GLsizei width, GLsizei height) 
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 5.0, 500);
	glMatrixMode(GL_MODELVIEW);
}

void CALLBACK display(void)
{

	// Enable Z-buffer test  
	glEnable(GL_DEPTH_TEST);
	
	// Enable internal lighting
	glEnable(GL_LIGHTING);


	glDisable(GL_CULL_FACE);

	//glCullFace — specify whether front- or back-facing polygons can be culled
	glCullFace(GL_FRONT);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glShadeModel(GL_FLAT);

	glClearColor(1.0, 0.4, 0.4, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Switch to modelview matrix manipulation 
	glMatrixMode(GL_MODELVIEW);

	// Enable a single light
	glEnable(GL_LIGHT0);
	static float light_diff[4] = { 3.0, 3.0, 3.0, 1.0 };
	static float light_spec[4] = { 5.0, 3.0, 3.0, 1.0 };
	static float light_pos[4] = { -5.0, -1.0, -5.5, 1.0 };


	// Create the new   matrix on the stack MODELVIEW
	glPushMatrix();

	// Start with unit marix/identity transformation
	glLoadIdentity();

	// Finally - if the observer is in specified point, transform
	//           world coordinates to observer coordinates
	gluLookAt(10, 10, 10, 0, 0, -15, 0, 1, 0);

	// Make the copy of pure World->Eye transformation
	glPushMatrix();

	// Set light position right here if it is defined directly in world coordinates
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);



	glTranslatef(0.0, position, -15.0);
	glRotatef(rotation, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -2.0);

	static float mat1_diff[4] = { 0.1, 0.1, 0.1, 1.0 };
	static float mat1_amb[4] = { 0.1, 0.3, 0.1, 1.0 };
	static float mat1_spec[4] = { 0.0, 0.0, 2.8, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat1_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat1_diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat1_spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 30.0);

	auxSolidSphere(ball_radius);

	glPopMatrix();
	
	
	glPushMatrix();

	// Set light position right here if it is defined directly in world coordinates
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	static float mat2_diff[4] = { 0.1, 0.6, 0.1, 1.0 };
	static float mat2_amb[4] = { 0.1, 0.3, 0.1, 1.0 };
	static float mat2_spec[4] = { 0.0, 0.0, 2.8, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat2_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat2_diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat2_spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 30.0);
	
	glTranslatef(0.0, position2, -15.0);
	glRotatef(-rotation, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -2.0);
	auxSolidSphere(ball_radius);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, -15.0);
	draw_floor();
	glPopMatrix();
	auxSwapBuffers();
	// Only now we can be sure the data for rendering are already "consumed"


}

void CALLBACK bounce_ball(void)
{
	rotation += 0.01 * rotation_speed;
	if (rotation >= 360.0)
		rotation -= 360.0;
	
	velocity += acceleration;
	velocity2 += acceleration;
	position = position + velocity;
	position2 = position2 + velocity2;
	if (position < floor+ball_radius)
	{
		velocity *= -1;
		position = floor+ball_radius;
	}
	if (position2 < floor + ball_radius)
	{
		velocity2 *= -1;
		position2 = floor + ball_radius;
	}
	display();
}

void draw_floor()
{
	static float mat1_diff[4] = { 6.0, 1.0, 1.0, 1.0 };
	static float mat1_amb[4] = { 0.0, 0.0, 0.0, 1.0 };
	static float mat1_spec[4] = { 0.0, 0.0, 0.0, 1.0 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat1_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat1_diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat1_spec);


	glBegin(GL_POLYGON);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(-10.0, floor, -10.0);
	glVertex3f(-10, floor, 0);
	glVertex3f(0,floor, 0);
	glVertex3f(0,floor,-10);
	glEnd();

	static float mat2_diff[4] = { 1.0, 6.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat2_diff);

	glBegin(GL_POLYGON);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(0.0, floor, -10.0);
	glVertex3f(0, floor, 0);
	glVertex3f(10, floor, 0);
	glVertex3f(10, floor, -10);
	glEnd();

	static float mat3_diff[4] = { 1.0, 1.0, 6.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat3_diff);

	glBegin(GL_POLYGON);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(-10.0, floor, 0.0);
	glVertex3f(-10, floor, -10);
	glVertex3f(0, floor, 10);
	glVertex3f(0, floor, 0);
	glEnd();

	static float mat4_diff[4] = { 1.0, 6.0, 6.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat4_diff);

	glBegin(GL_POLYGON);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(0.0, floor, 0.0);
	glVertex3f(0, floor, 10);
	glVertex3f(10, floor, 10);
	glVertex3f(10, floor, 0);
	glEnd();
}

void main(void)
{
	auxInitDisplayMode(AUX_RGBA | AUX_DOUBLE | AUX_DEPTH);
	auxInitPosition(100, 100, 800, 800);
	auxInitWindow((LPCWSTR)"Bouncing Ball");
	auxReshapeFunc(resize);
	auxIdleFunc(bounce_ball);

	auxMainLoop(display);
}












