#include<stdio.h>
#include<GL/glut.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<pthread.h>
#include<time.h>
#include<unistd.h>
#include<math.h> 
#include<netdb.h>
#include<sys/time.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<stdlib.h>


struct ball
{
 float x2;
 float y2;
};

typedef struct ball balobject;
balobject ballobject;


int dir;
static float x3=0.0,y3=-0.95;
static float x4=0.0,y4=0.85;
int ARGC=0;
char **ARGV;
void *ptr; 
int fd,bytes,fd1;
int clino;
pthread_t thread1,thread2,thread3,thread4,thread5;
struct sockaddr_in srv;		
char buf[512];
static int nbytes,flag=0,flag1=0;



void bat(float x,float y,float l)
{
 glBegin(GL_POLYGON);
	glVertex3f (x, y, 0.0);
	glVertex3f (x+l, y, 0.0);
	glVertex3f (x+l, y+.1, 0.0);
	glVertex3f (x, y+.1, 0.0);
 glEnd();
 glFlush();
}

void init(void)
{
 glClearColor(0.0,0.0,0.0,0.0);
 glShadeModel (GL_SMOOTH); 
 glFlush();
}
 

void drawFilledCircle(float x, float y, float radius)
{
 int i;
 int triangleAmount = 20; 
 GLfloat twicePi = 2.0 *3.14;
 glBegin(GL_TRIANGLE_FAN);
 glVertex2f(x, y);
 for(i = 0; i <= triangleAmount;i++) 
  glVertex2f(x + (radius * cos(i * twicePi / triangleAmount)),y + (radius * sin(i * twicePi / triangleAmount)));
 glEnd();
 glFlush();
}

void mouse1(int button, int state, int x, int y) 
{
 flag=0;
 switch(button)
 {
    case GLUT_LEFT_BUTTON: 
	    if(state == GLUT_DOWN) 
           {   
		x3-=0.1;
                if(x3<=-0.95)
                {
                 x3 =-0.95;
                }
                flag=1;
                glColor3f (1.0, 1.0, 1.0);
                bat(x3,y3,0.5); 
                usleep(10000); 
		glFlush();
                write(fd,&x3,sizeof(float));
                flag=0;
            }
 	    break;
   case GLUT_RIGHT_BUTTON: 
	   if(state == GLUT_DOWN) 
           {     
     	        x3+=0.1;
                  if(x3 >=0.45)
                  {
                   x3 = 0.45;
                  } 
                flag=1;
                glColor3f (1.0, 1.0, 1.0);
                bat(x3,y3,.5); 
                usleep(10000); 
		glFlush();
                write(fd,&x3,sizeof(x3));
                flag=0;
   	    }
  }
} 


void mouse2(int button, int state, int x, int y)
{
 flag1=0;
 switch(button)
 {
    case GLUT_LEFT_BUTTON: 
	    if(state == GLUT_DOWN) 
           {
		x4-=0.1;
                if(x4<=-0.95)
                {
                 x4 =-0.95;
                }
                flag1=1;
                glColor3f (1.0, 1.0, 1.0);
                bat(x4,y4,0.5); 
                usleep(10000); 
		glFlush();	
                write(fd,&x4,sizeof(x4));    
                flag1=0;
 	    break;
            }
   case GLUT_RIGHT_BUTTON: 
	   if(state == GLUT_DOWN) 
           {
     	        x4 +=0.1;
                if(x4 >=0.45)
                  {
                   x4 = 0.45;
                  } 
                flag1=1;
                glColor3f (1.0, 1.0, 1.0);
                bat(x4,y4,.5); 
                usleep(10000); 
		glFlush();
                write(fd,&x4,sizeof(x4));    
                flag1=0;	
 		break;
   	    }
  }
} 

void display1(void)
{
 glClearColor(0.0,0.0,0.0,0.0);
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 glColor3f(0.0, 0.9, 0.0); 
 glBegin(GL_LINE_STRIP);
	glVertex3f(-0.95, -0.95, 0);
	glVertex3f(0.95, -0.95, 0);
	glVertex3f(0.95, 0.95, 0);
	glVertex3f(-0.95, 0.95, 0);
	glVertex3f(-0.95, -0.95, 0);
  glEnd();  
 glFlush();
 glColor3f(1.0,1.0,1.0);
 bat(x3,y3,0.5);
 glColor3f (0.0, 1.0, 1.0);
 bat(x4,y4,0.5);
 glColor3f (0.0, 0.0, 1.0);
  drawFilledCircle(ballobject.x2,ballobject.y2,.07);
  usleep(10000); 
 glFlush();
}
void display(void)
{ 
 glClearColor(0.0,0.0,0.0,0.0);
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 glColor3f(0.0, 0.9, 0.0); 
 glBegin(GL_LINE_STRIP);
	glVertex3f(-0.95, -0.95, 0);
	glVertex3f(0.95, -0.95, 0);
	glVertex3f(0.95, 0.95, 0);
	glVertex3f(-0.95, 0.95, 0);
	glVertex3f(-0.95, -0.95, 0);
  glEnd();  
 glFlush();
 glColor3f (1.0, 1.0, 1.0);
 bat(x3,y3,0.5);
 glColor3f (0.0, 1.0, 1.0);
 bat(x4,y4,0.5);
 glFlush();
  glColor3f(0.0,0.0,1.0); 
  drawFilledCircle(ballobject.x2,ballobject.y2,.07);
  usleep(10000); 
  glFlush();
}
 
void NormalKeys(unsigned char key, int x, int y)
 {
	if (key == 27)
	  exit(0);
 }

void *graphics(void *ptr)
{
 glutInit(&ARGC,ARGV);
 glutInitDisplayMode(GLUT_SINGLE |GLUT_RGB);
 glutInitWindowPosition(100,100);
 glutInitWindowSize(500,500);
 glutCreateWindow("PLAYER 1");
 init();
 glutDisplayFunc(display);
 glutIdleFunc(display);  
 glutMouseFunc(mouse1);
 glutKeyboardFunc (NormalKeys);
 glutMainLoop();
}

void *graphics1(void *ptr)
{
 glutInit(&ARGC,ARGV);
 glutInitDisplayMode(GLUT_SINGLE |GLUT_RGB);
 glutInitWindowPosition(800,100);
 glutInitWindowSize(500,500);
 glutCreateWindow("PLAYER 2");
 init();
 glutDisplayFunc(display1);
 glutIdleFunc(display1);  
 glutMouseFunc(mouse2);
 glutKeyboardFunc(NormalKeys);
 glutMainLoop();
} 

void *network1(void *ptr)
{
 int a,b;
 	if((nbytes = read(fd, &ballobject, sizeof(ballobject))) < 0) 
	{
	perror("read");
	exit(1);
	}
        while(1)
        {
          read(fd, &ballobject, sizeof(ballobject));
          usleep(10000);
          write(fd,&flag1,sizeof(int));
          read(fd,&flag,sizeof(int));
          if(flag==1)
          {
           read(fd,&x3,sizeof(float)); 
          }
        }
 }
void *network(void *ptr)
{
 	if((nbytes = read(fd, &ballobject, sizeof(ballobject))) < 0) 
	{
	perror("read");
	exit(1);
	}
        while(1)
        {
         read(fd,&ballobject,sizeof(ballobject));
         usleep(10000);
         write(fd,&flag,sizeof(int));
         read(fd,&flag1,sizeof(int));
         if(flag1==1)
          {
           read(fd,&x4,sizeof(float)); 
          }
       }
}

void main(int argc,char *argv[])
{ 
 ARGC=argc;
 ARGV=argv;
 ballobject.x2=100.0;//write(fd,&flag,sizeof(flag));
 ballobject.y2=100.0;
 srv.sin_family = AF_INET; 
	srv.sin_addr.s_addr = inet_addr(ARGV[1]);
	if(srv.sin_addr.s_addr== (in_addr_t) -1)
	{
		fprintf(stderr,"inet_addr_failed\n");
		exit(1);
	}
	srv.sin_port = htons(2000);

	if((fd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("socket");
		exit(1);
	}
	if(connect(fd, (struct sockaddr*) &srv, sizeof(srv)) < 0) 
	{
	perror("connect"); 
	exit(1);
	}
        read(fd,&clino,sizeof(clino));
        printf("%d \n",clino);
  if(clino==1)
{
 pthread_create(&thread1,NULL,graphics,(void *)ptr);
 pthread_create(&thread2,NULL,network,(void *)ptr); 
 pthread_join(thread1,NULL);
}
 if(clino==2)
{
 pthread_create(&thread3,NULL,graphics1,(void *)ptr);
 pthread_create(&thread4,NULL,network1,(void *)ptr);
 pthread_join(thread3,NULL);
}
 exit(1);
}
