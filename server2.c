#include<netinet/in.h>
#include<sys/time.h>
#include<sys/socket.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>

struct ball    //Ball cordinates
{
 float x2;
 float y2;
};


typedef struct ball balobject;
balobject ballobject;

static float x3,y3;        //lower bat co-ordiantes
static float x4,y4;        //upper bat co-ordinates
int dir=1,client_id,status;
char buf[512];
int flag,flag1;


void main(int argc ,char *argv[])
{
	ballobject.x2=0.0;
        ballobject.y2=0.0;
        int a,newfd[1],fd,i=0;
	struct sockaddr_in srv;
	struct sockaddr_in cli;
	char buff[512];
	int nbytes;
	
	int cli_len=sizeof(cli);
	srv.sin_family = AF_INET;         
	srv.sin_port = htons(2000);
	srv.sin_addr.s_addr = htonl(INADDR_ANY);

	printf("*** SERVER RUNNING ... ***\n");

	if((fd=socket(AF_INET,SOCK_STREAM,0))<0)           //creating socket
	{
	        perror("socket");
	        exit(1);
	}
	if(bind(fd,(struct sockaddr*) &srv,sizeof(srv))<0)        // binding socket
	{
		perror("bind");
		exit(1);
	}
	if(listen(fd,5)<0)                               //  listen to the socket 
	{
		perror("listen");
		exit(1);	
	}
    	
        while(i<=1)
        {
	 newfd[i]=accept(fd,(struct sockaddr*) &cli,&cli_len); //Accept connection from client and create different copy of socket for evry client
	 if(newfd[i] < 0)
	 {
		perror("accept");
		exit(1);
	 }

        i++;

        }

        for(i=0;i<=1;i++)
        {
              client_id=i+1;
              write(newfd[i],&client_id,sizeof(client_id));
	      if (write(newfd[i],&ballobject,sizeof(ballobject))< 0)
       	     perror("Write");
        }


	printf("Request processed\n");
	
        while(1)
	{
         switch(dir)
         {
           case 0:(ballobject.x2)+=.03;
                  (ballobject.y2)+=.01;
                   break;

           case 1:(ballobject.x2)+=.03; 
                  (ballobject.y2)+=.01;
                   break;
  
           case 2:(ballobject.x2)-=.01; 
                  (ballobject.y2)+=.01;
                   break;
 
           case 3:(ballobject.x2)-=.03; 
                  (ballobject.y2)-=.02;
                  break;
  
           case 4:(ballobject.x2)+=.02; 
                  (ballobject.y2)-=.01;
                  break;
        }            // switch case ends

/* 	Ball-Bat Contact condition for UPPER bat 	*/

        if(((ballobject.x2)>=0.9)&&(dir==1))
  	 dir=2;
        if(((ballobject.y2)>=0.75)&&(dir==2)&&(ballobject.x2>=x4)&&(ballobject.x2<=(x4+.5))) 
 	 dir=3;
        if(((ballobject.x2)<=-0.9)&&(dir==3))
 	 dir=4;
        if(((ballobject.y2)>=0.75)&&(dir==0)&&(ballobject.x2>=x4)&&(ballobject.x2<=(x4+.5))) 
 	 dir=4; 
        if(((ballobject.y2)>=0.75)&&(dir==1)&&(ballobject.x2>=x4)&&(ballobject.x2<=(x4+.5))) 
 	 dir=4;


	if(((ballobject.y2)>=0.95)&&((dir==1)||(dir==2))&&((ballobject.x2<=x4)||(ballobject.x2>=(x4+.5)))) 
 	 ballobject.y2++; 

/* 	Ball-Bat Contact condition for LOWER bat 	*/
 
        if(((ballobject.x2)>=0.9)&&(dir==4))
 	 dir=3;
        if(((ballobject.x2)<=-.9)&&(dir==2))
	  dir=1;
        if(((ballobject.y2)<=-0.75)&&(dir==4)&&(ballobject.x2>=x3)&&(ballobject.x2<=(x3+.5)))
	  dir=1;
        if(((ballobject.y2)<=-0.75)&&(dir==3)&&(ballobject.x2>=x3)&&(ballobject.x2<=(x3+.5)))
	  dir=2;


	if(((ballobject.y2)<=-0.95)&&((dir==4)||(dir==3))&&((ballobject.x2>=x3)||(ballobject.x2<=(x3+.5))))
        ballobject.y2--;


        for(i=0;i<=1;i++) 
        {
	      if (write(newfd[i],&ballobject,sizeof(ballobject))< 0)
          perror("Write");
 	     }

         usleep(10000);
         read(newfd[0],&flag,sizeof(int));
         write(newfd[1],&flag,sizeof(int));

         if(flag==1)
         {
           read(newfd[0],&x3,sizeof(float));
           printf("x3 = %f \n", x3);
           write(newfd[1],&x3,sizeof(float));
         }

         read(newfd[1],&flag1,sizeof(int));  
         write(newfd[0],&flag1,sizeof(int));

         if(flag1==1)
         {
           read(newfd[1],&x4,sizeof(float));
           printf("x4 = %f \n", x4);
           write(newfd[0],&x4,sizeof(float));
         }                      //if ends
	}                      //while loop ends

        for(i=0;i<=1;i++) 	
        close(newfd[i]);
	close(fd);
	exit(1);
		
}

