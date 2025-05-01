// Sonic CD 2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include<iostream> 
#define GLEW_STATIC
#include<glew.h>
#include<glut.h> 
#include<glu.h> 
#include<math.h> 
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
float px, py;  
float speed; 
float objx, objy, objz;
GLuint sonictexture;  
GLuint blocktex1; 
GLuint backgroundtex1;
bool speedcontrol;  
float objrotate;

struct move
{
	bool left, right, up, down;
};  

bool collision(float px,float py,float x1,float x2,float y1,float y2)
{
	return px > x1 && px<x2 && py>y1 && py < y2;
}

move MOV{false,false,false,false};

GLuint loadTexture(const char* filename)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int Twidth, Theight, nrChannels;
	unsigned char* data = stbi_load(filename, &Twidth, &Theight, &nrChannels, 0); // ← detect channels

	if (!data)
	{
		std::cerr << "[ERROR] Failed to load texture: " << filename << std::endl;
		std::cerr << "Ensure the texture file exists and the path is correct!" << std::endl;
		exit(1); // Exit the program if texture fails to load
	}

	GLenum format = (nrChannels == 4) ? GL_RGBA :
		(nrChannels == 3) ? GL_RGB :
		(nrChannels == 1) ? GL_RED : GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, format, Twidth, Theight, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);
//	std::cout << "Loaded texture: " << filename << std::endl;

	return textureID;
}

void playerdraw()
{     
	
	glPushMatrix(); 

	glEnable(GL_TEXTURE_2D); 
	glBindTexture(GL_TEXTURE_2D,sonictexture);
	glTranslatef(px, py, 0);
	glScalef(0.1, 0.3, 0); 
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(1,1);
	glTexCoord2f(1, 0); glVertex2f(-1,1);
	glTexCoord2f(1, 1); glVertex2f(-1,0);
	glTexCoord2f(0, 1); glVertex2f(1,0);
	glEnd();  
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void trueorfalse()
{    
	glTranslatef(-px, -py, 0);
	glPushMatrix();
	if (MOV.up)
	{
		py += 0.1;
	}
	if (MOV.down)
	{
		py -= 0.1;
	}
	if (MOV.right)
	{
		px += speed;  
		speedcontrol = true;
		sonictexture = loadTexture("C:/Users/Andrew Alexander/source/repos/Sonic CD 2/Sonic CD 2/sonk3.png");
	}
	if (MOV.left)
	{
		px -= speed;  
		speedcontrol = true;
		sonictexture = loadTexture("C:/Users/Andrew Alexander/source/repos/Sonic CD 2/Sonic CD 2/sonk2.png");
	} 
	glPopMatrix();
}

void timer(int)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, 0);

	if (speedcontrol == true)
	{
		speed += 0.0005;
	}
	else
	{
		speed = 0.05;
	}
	py -= 0.05; 
	
	if (collision(px, py, objx-1 , objx+1 , objy-0.1 , objy+1 ))
	{
		if (py < objy)
		{
			py = objy;
		}
	}
	
} 

void floor()
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D); 
	glBindTexture(GL_TEXTURE_2D,blocktex1);

	glColor3f(1,1,1); 
	glTranslatef(0, -0.95, 0);
	glBegin(GL_POLYGON); 
	glTexCoord2f(1,1); glVertex2f(1, 0);
	glTexCoord2f(0,1); glVertex2f(-1,0);
	glTexCoord2f(0,0); glVertex2f(-1,1);
	glTexCoord2f(1,0); glVertex2f(1, 1);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
} 

void background()
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, backgroundtex1);

	glColor3f(1, 1, 1);
	glTranslatef(0, 0, 0); 
	glScalef(100,100,0);
	glBegin(GL_POLYGON);
	glTexCoord2f(1, 1); glVertex2f(1, 0);
	glTexCoord2f(0, 1); glVertex2f(-1, 0);
	glTexCoord2f(0, 0); glVertex2f(-1, 1);
	glTexCoord2f(1, 0); glVertex2f(1, 1);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void object()
{     
	glPushMatrix();
	glTranslatef(objx, objy, objz);  
	glRotatef(objrotate,1,1,1);
	glScalef(0.5,0.5,0.5); 
	glColor3f(0,1,0); 
	glutWireCube(1); 
	glPopMatrix();
}
void drawing()
{
	glClear(GL_COLOR_BUFFER_BIT); 
	glLoadIdentity();   
	
	trueorfalse(); 
	background();
	playerdraw();  
	
	floor();   
	
	glPushMatrix();
	glTranslatef(1,0,0); 
	floor(); 
	glPopMatrix(); 

	object();
	glFlush();
	glutSwapBuffers();
}

void initial() 
{
	px = 0; 
	py = 0;   
	objrotate = 10;
	objx = 0, objy = 0, objz = 0;
	speed = 0.05; 
	speedcontrol = false;
	sonictexture = loadTexture("C:/Users/Andrew Alexander/source/repos/Sonic CD 2/Sonic CD 2/sonk2.png");
	blocktex1 = loadTexture("C:/Users/Andrew Alexander/source/repos/Sonic CD 2/Sonic CD 2/block1.png"); 
	backgroundtex1 = loadTexture("C:/Users/Andrew Alexander/source/repos/Sonic CD 2/Sonic CD 2/machineback.png");
} 

void control1(unsigned char keys,int,int)
{
	if (keys == 'w')
	{
		MOV.up = true;
    }  
	if (keys == 's')
	{
		MOV.down = true;
	} 
	if (keys == 'd')
	{
		MOV.right = true;
	}
	if (keys == 'a')
	{
		MOV.left = true;
	}
	glutPostRedisplay();
} 

void control2(unsigned char keys, int, int)
{
	if (keys == 'w')
	{
		MOV.up = false;  
		
		
	}
	if (keys == 's')
	{
		MOV.down = false;
	}
	if (keys == 'd')
	{
		MOV.right = false; 
		speedcontrol = false;
	}
	if (keys == 'a')
	{
		MOV.left = false; 
		speedcontrol = false;
	}
	glutPostRedisplay();
}



int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000, 500);  
	glutInitWindowPosition(100, 100);
	glutCreateWindow("sonic CD 2");  
	glewInit();
	initial();
	glutDisplayFunc(drawing);  
	glutTimerFunc(0,timer,0);
	glutKeyboardFunc(control1); 
	glutKeyboardUpFunc(control2);
	glutMainLoop();
	return 0;
}
