#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include<iostream>
#include<stdio.h>

#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif

using namespace std;



GLfloat alpha = 0.0, theta = 0.0, axis_x=0.0, axis_y=0.0;
GLboolean bRotate = false, uRotate = false;
char buffer[10];
int rh=3,rw=12,rl=3,score=0;;
float ex=3.5,ey=2.5,ez=5,lx=3.54,ly=2.8,lz=7;
bool collision =false;
float  vehicleX=4,vehicleY;
float collision_right,collision_middle;
float z;
void draw_solid_cube(GLfloat size)
{
    float x1,y1,z1;
    float x2,y2,z2;
    float x3,y3,z3;
    float x4,y4,z4;

    x1 = -size/2;
    y1 = -size/2;
    z1 = -size/2;

    x2 = size/2;
    y2 = -size/2;
    z2 = -size/2;

    x3 = size/2;
    y3 = -size/2;
    z3 = size/2;

    x4 = -size/2;
    y4 = -size/2;
    z4 = size/2;

    float x5,y5,z5;
    float x6,y6,z6;
    float x7,y7,z7;
    float x8,y8,z8;

    x5 = -size/2;
    y5 = size/2;
    z5 = -size/2;

    x6 = size/2;
    y6 = size/2;
    z6 = -size/2;

    x7 = size/2;
    y7 = size/2;
    z7 = size/2;

    x8 = -size/2;
    y8 = size/2;
    z8 = size/2;

    float n1_x = 0, n1_y = -1, n1_z = 0; //face that faces towards bottom
    float n2_x = 0, n2_y = 1, n2_z = 0; //up
    float n3_x = 0, n3_y = 0, n3_z = -1; //front
    float n4_x = 0, n4_y = 0, n4_z = 1; //back
    float n5_x = 1, n5_y = 0, n5_z = 0; //right
    float n6_x = -1, n6_y = 0, n6_z = 0; //left

    glBegin(GL_QUADS);
    //Quad 1

    glNormal3f(n1_x,n1_y,n1_z);

    glTexCoord2f(0,0);
    glVertex3f(x1,y1,z1);

    glTexCoord2f(1,0);
    glVertex3f(x2,y2,z2);

    glTexCoord2f(1,1);
    glVertex3f(x3,y3,z3);

    glTexCoord2f(0,1);
    glVertex3f(x4,y4,z4);

    //Quad 2

    glNormal3f(n2_x,n2_y,n2_z);

    glTexCoord2f(0,0);
    glVertex3f(x8,y8,z8);

    glTexCoord2f(1,0);
    glVertex3f(x7,y7,z7);

    glTexCoord2f(1,1);
    glVertex3f(x6,y6,z6);

    glTexCoord2f(0,1);
    glVertex3f(x5,y5,z5);
    //Quad 3

    glNormal3f(n3_x,n3_y,n3_z);

    glTexCoord2f(0,0);
    glVertex3f(x5,y5,z5);

    glTexCoord2f(1,0);
    glVertex3f(x6,y6,z6);

    glTexCoord2f(1,1);
    glVertex3f(x2,y2,z2);

    glTexCoord2f(0,1);
    glVertex3f(x1,y1,z1);
    //Quad 4


    glNormal3f(n4_x,n4_y,n4_z);

    glTexCoord2f(0,0);
    glVertex3f(x7,y7,z7);

    glTexCoord2f(1,0);
    glVertex3f(x8,y8,z8);

    glTexCoord2f(1,1);
    glVertex3f(x4,y4,z4);

    glTexCoord2f(0,1);
    glVertex3f(x3,y3,z3);
    //Quad 5

    glNormal3f(n5_x,n5_y,n5_z);

    glTexCoord2f(0,0);
    glVertex3f(x6,y6,z6);

    glTexCoord2f(1,0);
    glVertex3f(x7,y7,z7);

    glTexCoord2f(1,1);
    glVertex3f(x3,y3,z3);

    glTexCoord2f(0,1);
    glVertex3f(x2,y2,z2);

    //Quad 6

    glNormal3f(n6_x,n6_y,n6_z);

    glTexCoord2f(0,0);
    glVertex3f(x1,y1,z1);

    glTexCoord2f(1,0);
    glVertex3f(x4,y4,z4);

    glTexCoord2f(1,1);
    glVertex3f(x8,y8,z8);

    glTexCoord2f(0,1);
    glVertex3f(x5,y5,z5);


    glEnd();
}


struct Image_Data
{
        unsigned long Size_X;
        unsigned long Size_Y;
        unsigned char *Data;
};

int Load_Bitmap(char* Filename, Image_Data* Out_Image_Data)
{
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int width, height;
    unsigned int imageSize;   // = width*height*3

    FILE * file = fopen(Filename,"rb");
    if (!file){ return 0;}

    if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
    return 0;
    }

    if ( header[0]!='B' || header[1]!='M' ){
    return 0;
    }

    // Read ints from the byte array
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)      dataPos=54; // The BMP header is done that way

    // Create a buffer
    Out_Image_Data->Data = new unsigned char [imageSize];

    // Read the actual data from the file into the buffer
    fread(Out_Image_Data->Data,1,imageSize,file);

    //Everything is in memory now, the file can be closed
    fclose(file);

    Out_Image_Data->Size_X = width;
    Out_Image_Data->Size_Y = height;

    return 1;
}

GLuint Textures[100];


void Add_Texture(int ID, Image_Data* Data)
{
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,Data->Size_X,Data->Size_Y,0,GL_BGR,GL_UNSIGNED_BYTE,Data->Data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
}



void drawroad()
{

    glPushMatrix();
    glColor4f(1,1,1,1);
    glTranslatef(3.6,0,-10);
    glScalef(4,0.2,35);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);
    draw_solid_cube(1);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();

    }
float dz=-30;
void draw_divider(float x,float y,float z)
{
    for(int i=2;i<=2000;i=i+2)
    {
     glPushMatrix();
    glColor4f(1,1,1,1);
    glTranslatef(x-.30,y+.1,z+i);
    glScalef(.1,.1,.9);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,3);
    //glRotatef(180,0,1,0);
    draw_solid_cube(1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    }
}
float r_z=-.5;
void draw_roadside(float x,float y,float z)
{
for(int i=2;i<1000;i=i+2)
    //leftside
    {
    glPushMatrix();
    glColor4f(1,1,1,1);
    glTranslatef(x-.4,y-.6,z+i);
    glScalef(10,0.01,20);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2);
    draw_solid_cube(1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    }


}
//Rightside

float l_z=-.5;
void draw_rightside(float x,float y,float z)
{
    for(int i=2;i<=1000;i=i+2)
   {
    glPushMatrix();
    glColor4f(1,1,1,1);
    glTranslatef(x-.2,y-.2,z+i);
    glScalef(10,0.1,20);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2);
    draw_solid_cube(1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
   }
}

void draw_sunset(float x,float y,float z)
{
    glPushMatrix();
    glColor4f(1,1,1,1);
    glTranslatef(x,y,z);
    glScalef(58,90,10);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,1);
    draw_solid_cube(1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void draw_car(float x,float y,float z)
{
    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(x-1,y+1.2,z+1);
    glScalef(.3,.3,.8);
    collision_right=z+1;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,4);
    draw_solid_cube(1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //tire right back
    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(x-.815,y+1.1,z+1.25);
    glScalef(.16,.3,0.4);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glutSolidSphere(.3,20,20);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //tire left back
     glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(x-1.16,y+1.1,z+1.25);
    glScalef(.16,.3,0.4);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glutSolidSphere(.3,20,20);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //tire front right

    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(x-.815,y+1.1,z+.7);
    glScalef(.16,.3,0.4);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glutSolidSphere(.3,20,20);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //tire front left
    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(x-1.16,y+1.1,z+.7);
    glScalef(.16,.3,0.4);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glutSolidSphere(.3,20,20);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

}


void collision_check()
{
    if((abs(collision_middle-3)<=.05)&& (abs(vehicleX-4)<=.02))
    {
        collision=true;
        //exit(0);
    }


}



float o_z=-30;
void draw_opponent_rightside_cars(float x,float y,float z,int texture_id)
{
 for(int i=0;i<=2000;i=i+10)

   {
    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(x,y+1.2,z+i);
    glScalef(.3,.3,.9);



    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture_id);
    draw_solid_cube(1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //tire left back
    glPushMatrix();
    glColor3f(0,0,0);
    glTranslatef(x-.15,y+1.1,z-30.3+i);
    glScalef(.16,.3,0.4);
    glutSolidSphere(.3,20,20);
    glPopMatrix();

    //tire right back
     glPushMatrix();
    glColor3f(0,0,0);
    glTranslatef(x+.18,y+1.1,z-30.3+i);
    glScalef(.16,.3,0.4);
    glutSolidSphere(.3,20,20);
    glPopMatrix();

    //tire front left

    glPushMatrix();
    glColor3f(0,0,0);
    glTranslatef(x-.15,y+1.1,z-19.8+i);
    glScalef(.16,.3,0.4);
    glutSolidSphere(.3,20,20);
    glPopMatrix();

    //turn front right
    glPushMatrix();
    glColor3f(0,0,0);
    glTranslatef(x+.16,y+1.1,z-19.8+i);
    glScalef(.16,.3,0.4);
    glutSolidSphere(.3,20,20);
    glPopMatrix();

   }


}

void draw_opponent_middle_cars(float x,float y,float z,int texture_id)
{
 for(int i=0;i<=2000;i=i+25)

   {
    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(x-1,y+1.2,z+i);
    glScalef(.3,.3,.8);
    collision_middle=z+i;
    //collision_check();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture_id);
    draw_solid_cube(1);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //tire left back
    glPushMatrix();
    glColor3f(.5,.8,.8);
    glTranslatef(x-1.17,y+1.1,z-24.7+i);
    glScalef(.16,.3,0.4);
    glutSolidSphere(.3,20,20);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //tire right back
     glPushMatrix();
    glColor3f(.5,.8,.8);
    glTranslatef(x-.82,y+1.1,z-24.7+i);
    glScalef(.16,.3,0.4);
    glutSolidSphere(.3,20,20);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //tire front left

    glPushMatrix();
    glColor3f(.5,.8,.8);
     glTranslatef(x-1.17,y+1.1,z-25.28+i);
    glScalef(.16,.3,0.4);
    glutSolidSphere(.3,20,20);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //turn front right
    glPushMatrix();
    glColor3f(.5,.8,.8);
    glTranslatef(x-.82,y+1.1,z-25.28+i);
    glScalef(.16,.3,0.4);
    glutSolidSphere(.3,20,20);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

   }


}



void Write(char *string) //Write string on the screen
{
    while(*string)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *string++);
}

void GameStatus()
{

    char tmp_str[40];

    // Print the status of the game on the screen
    glColor3f(1, 0, 0);
    glRasterPos2f(3, 4);

        if(!collision)
        {sprintf(tmp_str, "Score: %d ", score);
        Write(tmp_str);
        }
    else
    {
        glColor3f(1, 0, 0);
        sprintf(tmp_str, "Game over");
        Write(tmp_str);
        //
        Sleep(5000);
        exit(0);
    }
}








void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClearColor(1,0,1,1);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    //glFrustum(-5,5,-5,5, 4, 50);
    gluPerspective(65,1,0.50,100);
    //glOrtho(-5,5,-5,5, 4, 50);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    gluLookAt( lx,ly,lz,ex,ey,ez,0,1,0);

    glPushMatrix();

    glRotatef( theta, 1, 0, 0.0 );
    glRotatef( alpha, 0, 1, 0.0 );

    drawroad();
    if(r_z>-20)
    {
        r_z=-20.5;
        //Sleep(300);
    }
    else
    {
        r_z++;
        //Sleep(300);
    }
    draw_roadside(-3,0,r_z);

    if(l_z>-22)
    {
        l_z=-22;
    }
    else
        {
            l_z++;
        }
    draw_rightside(10.6,0,l_z);


    if(dz>-30)
    {
        dz=-30;
        Sleep(300);
    }
    else
    {
        dz+=1;
        Sleep(300);

    }
    draw_divider(4,0,dz);
    draw_car(vehicleX,-.9,2);
    printf("%f %f\n",abs(collision_middle-3),abs(4-collision_right));
   // printf("%f\n",);

    draw_sunset(0,1,-30);

    if(o_z>-60)
    {
        o_z=-100;

    }
    else
    {
        o_z++;
    }
     draw_opponent_rightside_cars(4,-.9,o_z,6);
     draw_opponent_middle_cars(4,-.9,o_z,5);
     score+=10;
    GameStatus();
    glPopMatrix();

    //Score



    glFlush();
    glutSwapBuffers();
}

float mm,xx,yy,zz;
float cam_angle = 90/180.0f * 3.1416;

void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 'j':
        cam_angle = cam_angle - 0.01;

        lx = ex + cos(cam_angle)*4;
        ly = ey;
        lz = ez + sin(cam_angle)*4;

        break;
    case 'k':
        cam_angle = cam_angle + 0.01;

        lx = ex + cos(cam_angle)*4;
        ly = ey;
        lz = ez + sin(cam_angle)*4;
        break;

    case 't':
        ex = ex + cos(cam_angle) * 0.5;
        ez = ez + sin(cam_angle) * 0.5;

        lx = ex + cos(cam_angle);
        ly = ey;
        lz = ez + sin(cam_angle);
        break;
    case 'g':
        ex = ex - cos(cam_angle) * 0.5;
        ez = ez - sin(cam_angle) * 0.5;

        lx = ex + cos(cam_angle);
        ly = ey;
        lz = ez + sin(cam_angle);

        break;

    case 'f':
        bRotate = !bRotate;
        uRotate = false;
        axis_x=0.0;
        axis_y=3.0;
        break;

    case 'r':
        uRotate = !uRotate;
        bRotate = false;
        axis_x=1.0;
        axis_y=0.0;
        break;

    case 27:	// Escape key
        exit(1);
    }
}

void animate()
{
    if (bRotate == true)
    {
        theta += 0.1;
        if(theta > 360.0)
            theta -= 360.0*floor(theta/360.0);
    }

    if (uRotate == true)
    {
        alpha += 0.1;
        if(alpha > 360.0)
            alpha -= 360.0*floor(alpha/360.0);
    }
    glutPostRedisplay();

}

void Specialkey(int key, int x, int y)//allow to use navigation key for movement of car
{
    switch(key)
    {

    case GLUT_KEY_LEFT:
        if(vehicleX>3.9)
        {
        vehicleX -= .5;
        printf("%f\n",vehicleX);
        break;
        }
    case GLUT_KEY_RIGHT:
        if(vehicleX<=5.8)
        {
        vehicleX += .2;
        printf("%f\n",vehicleX);
        break;
        }
    case GLUT_KEY_DOWN:
        {
            r_z--;
            l_z--;
            dz--;
            //Sleep(300);
        }
    case GLUT_KEY_UP:
        {
            r_z++;
            l_z++;
            dz+=10;
            //Sleep(300);
        }
    }
    glutPostRedisplay();
}
int main (int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(0,0);
    glutInitWindowSize(800,800);
    glutCreateWindow("CAr game");

    glShadeModel( GL_SMOOTH );
    glutSpecialFunc(Specialkey);
    glutKeyboardFunc(myKeyboardFunc);
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glEnable(GL_NORMALIZE);
    //glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    //glEnable(GL_ALPHA);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glGenTextures(10,Textures);


    Image_Data* Bitmap_1 = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("H:\\4 2\\Lab\\CSE 4208\\3D car\\road1.bmp", Bitmap_1) == 0)
    {
         return 0;
    }
    Image_Data* Bitmap_2 = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("H:\\4 2\\Lab\\CSE 4208\\3D car\\sunset.bmp", Bitmap_2) == 0)
    {
         return 0;
    }

     Image_Data* Bitmap_3 = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("H:\\4 2\\Lab\\CSE 4208\\3D car\\desert.bmp", Bitmap_3) == 0)
    {
         return 0;
    }

     Image_Data* Bitmap_4 = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("H:\\4 2\\Lab\\CSE 4208\\3D car\\white.bmp", Bitmap_4) == 0)
    {
         return 0;
    }

    Image_Data* Bitmap_5 = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("H:\\4 2\\Lab\\CSE 4208\\3D car\\car.bmp", Bitmap_5) == 0)
    {
         return 0;
    }

    Image_Data* Bitmap_6 = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("H:\\4 2\\Lab\\CSE 4208\\3D car\\mitsubishi.bmp", Bitmap_6) == 0)
    {
         return 0;
    }


Image_Data* Bitmap_7 = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("H:\\4 2\\Lab\\CSE 4208\\3D car\\bus.bmp", Bitmap_7) == 0)
    {
         return 0;
    }


    Add_Texture(0,Bitmap_1);
    Add_Texture(1,Bitmap_2);
    Add_Texture(2,Bitmap_3);
    Add_Texture(3,Bitmap_4);
    Add_Texture(4,Bitmap_5);
    Add_Texture(5,Bitmap_6);
    Add_Texture(6,Bitmap_7);
    glutMainLoop();

    return 0;
}
