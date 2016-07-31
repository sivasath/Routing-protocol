
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<sys/select.h>
#include<sys/time.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/time.h>
#include<stdlib.h>
#include<netdb.h>
#include<sys/wait.h>
#include<signal.h>
#include<netdb.h>
#include "serialize.h" //ref to beej guide

//for strtok
struct token
{
 char arguments[50];
 
};

//structure for update messages
struct message
{
uint16_t noofupdate;
uint16_t senderport;
uint32_t senderip;
uint32_t ip;
uint16_t port;
uint16_t h;
uint16_t id;
uint16_t cost;
};

//structure to store the details about the neighbours
typedef struct details
{
uint16_t updatefields;
uint16_t id;
uint16_t cost;
uint32_t nip;
uint16_t nport;
int expire;
}details;

//structure for routing table
typedef struct dv
{

uint16_t sid;
uint16_t cost;
uint32_t ip;
uint16_t port;
int hopto;
int n;
int nongh;
int16_t presentnode;
int32_t presentip;
}dv;

//structure for updating
typedef struct updating 
{
int16_t to1;
int16_t to2;
int16_t to3;
int16_t to4;
int16_t to5;
}updating;

//summing function for calculating bellmann ford
int sumof(int16_t a1,int16_t b1)
{
 int16_t total;
 if(b1 == 0xffff || a1==0xffff || a1 == -1 || b1 == -1)
  total =0xffff;
 else
 {
 total=a1+b1;}
 return total;
}

//function for finding the minimum
int min(int16_t a,int16_t b)
{
 if(a == 0xffff || a== -1)
  return b;
 else if (b==0xffff || b==-1)
  return a;

 else if(a>b)
 {
 return b;
 }
 else
 {
  return a;
 }
}

//function for updating cost by Bellmann ford equation	
int updaterouting(struct dv *dv1,struct message *sendmsg,struct message *revddata,struct details *neigh)
{
struct updating newcost[5];
char st[INET6_ADDRSTRLEN];
int i=0;
int mincost;
int32_t dest;
int16_t cofxv;
int16_t cofxy[5];
int32_t senderid;
cofxy[0]=neigh[0].cost;
cofxy[1]=neigh[1].cost;
cofxy[2]=neigh[2].cost;
cofxy[3]=neigh[3].cost;
cofxy[4]=neigh[4].cost;
struct sockaddr_in g;
   memcpy(&g.sin_addr,&revddata[0].senderip,sizeof(revddata[0].senderip));
   inet_ntop(AF_INET,&g.sin_addr,st,INET_ADDRSTRLEN);
for(i=0;i<dv1[0].n;i++)
{
 if(revddata[0].senderip==neigh[i].nip)
 {
  cofxv=neigh[i].cost;
  senderid=neigh[i].id;
  //printf("\n sender id %d",senderid);
  }
}
  
 if(revddata[0].senderip==neigh[0].nip)
{
newcost[0].to1=revddata[0].cost;
newcost[0].to2=revddata[1].cost;
newcost[0].to3=revddata[2].cost;
newcost[0].to4=revddata[3].cost;
newcost[0].to5=revddata[4].cost;
}
if(revddata[0].senderip==neigh[1].nip)
{
newcost[0].to1=revddata[0].cost;
newcost[0].to2=revddata[1].cost;
newcost[0].to3=revddata[2].cost;
newcost[0].to4=revddata[3].cost;
newcost[0].to5=revddata[4].cost;
}
if(revddata[0].senderip==neigh[2].nip)
{
newcost[0].to1=revddata[0].cost;
newcost[0].to2=revddata[1].cost;
newcost[0].to3=revddata[2].cost;
newcost[0].to4=revddata[3].cost;
newcost[0].to5=revddata[4].cost;
}
if(revddata[0].senderip==neigh[3].nip)
{
newcost[0].to1=revddata[0].cost;
newcost[0].to2=revddata[1].cost;
newcost[0].to3=revddata[2].cost;
newcost[0].to4=revddata[3].cost;
newcost[0].to5=revddata[4].cost;
}
if(revddata[0].senderip==neigh[4].nip)
{
newcost[0].to1=revddata[0].cost;
newcost[0].to2=revddata[1].cost;
newcost[0].to3=revddata[2].cost;
newcost[0].to4=revddata[3].cost;
newcost[0].to5=revddata[4].cost;
}

 for(i=0;i<dv1[0].n;i++)
 {
   dest=neigh[i].id;
  if(dv1[0].presentnode==dest)
  {
   mincost=0;
   if(neigh[i].id == dv1[0].presentnode)
    {
    neigh[i].cost=mincost;
    }
  }
  else if(dest==neigh[0].id)
  {
   mincost=min(neigh[0].cost,sumof(cofxv,newcost[0].to1));
    if(mincost != neigh[0].cost)
       dv1[0].hopto=senderid;
   neigh[0].cost=mincost;
  }
  else if(dest==neigh[1].id)
  {
   mincost=min(neigh[1].cost,sumof(cofxv,newcost[0].to2));
    if(mincost != neigh[1].cost)
       dv1[1].hopto=senderid;
   neigh[1].cost=mincost;
  }
  else if(dest==neigh[2].id)
  {
   mincost=min(neigh[2].cost,sumof(cofxv,newcost[0].to3));
     if(mincost != neigh[2].cost)
       dv1[2].hopto=senderid;
   neigh[2].cost=mincost;
   }
  else if(dest==neigh[3].id)
  {
   mincost=min(neigh[3].cost,sumof(cofxv,newcost[0].to4));
     if(mincost != neigh[3].cost)
       dv1[3].hopto=senderid;
   neigh[3].cost=mincost;
   }
  else if(dest==neigh[4].id)
  {
   mincost=min(neigh[4].cost,sumof(cofxv,newcost[0].to5));
     if(mincost != neigh[4].cost)
       dv1[4].hopto=senderid;
   neigh[4].cost=mincost;
   }
  
 }
//for copying the data from the neighbour structure to the routing table
for(i=0;i<dv1[0].n;i++)
 {  
 dv1[i].sid=neigh[i].id;
 dv1[i].cost=neigh[i].cost;
 dv1[i].ip=neigh[i].nip;
 dv1[i].port=neigh[i].nport;
 }//for

return 0;
}

//to send packets
int makepacket(struct dv *dv1,struct message *sendmsg, struct details *neigh,uint16_t port)
{

int sockfd;
struct addrinfo hi, *servinfo, *t;
unsigned int packetsize=0;
void *addr;
int r;
int num;
uint8_t bufr[68];
char st[INET6_ADDRSTRLEN];

   struct sockaddr_in g;
   memcpy(&g.sin_addr,&sendmsg[0].senderip,sizeof(sendmsg[0].senderip));
   inet_ntop(AF_INET,&g.sin_addr,st,INET_ADDRSTRLEN);


sendmsg[0].noofupdate=dv1[0].n;
sendmsg[0].senderport=port;
sendmsg[0].senderip=dv1[0].presentip;

sendmsg[0].ip=neigh[0].nip;
sendmsg[0].port=neigh[0].nport;
sendmsg[0].h=0x0;
sendmsg[0].id=neigh[0].id;
sendmsg[0].cost=neigh[0].cost;

sendmsg[1].ip=neigh[1].nip;
sendmsg[1].port=neigh[1].nport;
sendmsg[1].h=0x0;
sendmsg[1].id=neigh[1].id;
sendmsg[1].cost=neigh[1].cost;

sendmsg[2].ip=neigh[2].nip;
sendmsg[2].port=neigh[2].nport;
sendmsg[2].h=0x0;
sendmsg[2].id=neigh[2].id;
sendmsg[2].cost=neigh[2].cost;

sendmsg[3].ip=neigh[3].nip;
sendmsg[3].port=neigh[3].nport;
sendmsg[3].h=0x0;
sendmsg[3].id=neigh[3].id;
sendmsg[3].cost=neigh[3].cost;

sendmsg[4].ip=neigh[4].nip;
sendmsg[4].port=neigh[4].nport;
sendmsg[4].h=0x0;
sendmsg[4].id=neigh[4].id;
sendmsg[4].cost=neigh[4].cost;

//serialising //reference to beej
packetsize=pack(bufr,"hhllhhhhlhhhhlhhhhlhhhhlhhhh",sendmsg[0].noofupdate,sendmsg[0].senderport,sendmsg[0].senderip,sendmsg[0].ip,sendmsg[0].port,sendmsg[0].h,sendmsg[0].id,sendmsg[0].cost,sendmsg[1].ip,sendmsg[1].port,sendmsg[1].h,sendmsg[1].id,sendmsg[1].cost,sendmsg[2].ip,sendmsg[2].port,sendmsg[2].h,sendmsg[2].id,sendmsg[2].cost,sendmsg[3].ip,sendmsg[3].port,sendmsg[3].h,sendmsg[3].id,sendmsg[3].cost,sendmsg[4].ip,sendmsg[4].port,sendmsg[4].h,sendmsg[4].id,sendmsg[4].cost);

fflush(stdout);

//refernce to beej
//sending packets to neighbours
int i=0;
for(i=0;i<dv1[0].n;i++)
{
 if(neigh[i].cost != 0xffff && neigh[i].cost != 0 )
 {
 
 
 if(neigh[i].nip==sendmsg[0].ip)
  {
   struct sockaddr_in g;
   memcpy(&g.sin_addr,&sendmsg[0].ip,sizeof(sendmsg[0].ip));
   inet_ntop(AF_INET,&g.sin_addr,st,INET_ADDRSTRLEN);
   
   char portno[5];
   sprintf(portno,"%d",sendmsg[0].port);
   memset(&hi, 0, sizeof hi);
hi.ai_family = AF_UNSPEC; //ref to beej guide
hi.ai_socktype = SOCK_DGRAM;
if ((r = getaddrinfo(st, portno, &hi, &servinfo)) != 0) {
fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
return 1;
}
// loop through all the results and make a socket
for(t = servinfo; t != NULL; t = t->ai_next) {
if ((sockfd = socket(t->ai_family, t->ai_socktype,t->ai_protocol)) == -1) {
perror("talker: socket");
continue;
}
break;
}
if (t == NULL) {
fprintf(stderr, "talker: failed to bind socket STEP\n");
return 2;
}

 if ((num = sendto(sockfd, bufr, sizeof(bufr), 0,t->ai_addr, t->ai_addrlen)) == -1)
 {
  perror("talker: sendto");
  exit(1);
 }

  }
  if(neigh[i].nip==sendmsg[1].ip)
  {
   
   struct sockaddr_in g;
   memcpy(&g.sin_addr,&sendmsg[1].ip,sizeof(sendmsg[1].ip));
   inet_ntop(AF_INET,&g.sin_addr,st,INET_ADDRSTRLEN);
   
   
   char portno[5];
   sprintf(portno,"%d",sendmsg[1].port);
  memset(&hi, 0, sizeof hi);
hi.ai_family = AF_UNSPEC;
hi.ai_socktype = SOCK_DGRAM;
if ((r = getaddrinfo(st, portno, &hi, &servinfo)) != 0) {
fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
return 1;
}
// loop through all the results and make a socket
for(t = servinfo; t != NULL; t = t->ai_next) {
if ((sockfd = socket(t->ai_family, t->ai_socktype,t->ai_protocol)) == -1) {
perror("talker: socket");
continue;
}
break;
}
if (t == NULL) {
fprintf(stderr, "talker: failed to bind socket STEP\n");
return 2;
}

 if ((num = sendto(sockfd, bufr, sizeof(bufr), 0,t->ai_addr, t->ai_addrlen)) == -1)
 {
  perror("talker: sendto");
  exit(1);
 }

  }
  if(neigh[i].nip==sendmsg[2].ip)
  {
   struct sockaddr_in g;
   memcpy(&g.sin_addr,&sendmsg[2].ip,sizeof(sendmsg[2].ip));
   inet_ntop(AF_INET,&g.sin_addr,st,INET_ADDRSTRLEN);
   
   char portno[5];
   sprintf(portno,"%d",sendmsg[2].port);
  memset(&hi, 0, sizeof hi);
hi.ai_family = AF_UNSPEC;
hi.ai_socktype = SOCK_DGRAM;
if ((r = getaddrinfo(st, portno, &hi, &servinfo)) != 0) {
fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
return 1;
}
// loop through all the results and make a socket
for(t = servinfo; t != NULL; t = t->ai_next) {
if ((sockfd = socket(t->ai_family, t->ai_socktype,t->ai_protocol)) == -1) {
perror("talker: socket");
continue;
}
break;
}
if (t == NULL) {
fprintf(stderr, "talker: failed to bind socket STEP\n");
return 2;
}

 if ((num = sendto(sockfd, bufr, sizeof(bufr), 0,t->ai_addr, t->ai_addrlen)) == -1)
 {
  perror("talker: sendto");
  exit(1);
 }

  }
  if(neigh[i].nip==sendmsg[3].ip)
  {
   struct sockaddr_in g;
   memcpy(&g.sin_addr,&sendmsg[3].ip,sizeof(sendmsg[3].ip));
   inet_ntop(AF_INET,&g.sin_addr,st,INET_ADDRSTRLEN);
   
   char portno[5];
   sprintf(portno,"%d",sendmsg[3].port);
   memset(&hi, 0, sizeof hi);
hi.ai_family = AF_UNSPEC;
hi.ai_socktype = SOCK_DGRAM;
if ((r = getaddrinfo(st, portno, &hi, &servinfo)) != 0) {
fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
return 1;
}
// loop through all the results and make a socket
for(t = servinfo; t != NULL; t = t->ai_next) {
if ((sockfd = socket(t->ai_family, t->ai_socktype,t->ai_protocol)) == -1) {
perror("talker: socket");
continue;
}
break;
}
if (t == NULL) {
fprintf(stderr, "talker: failed to bind socket STEP\n");
return 2;
}

 if ((num = sendto(sockfd, bufr, sizeof(bufr), 0,t->ai_addr, t->ai_addrlen)) == -1)
 {
  perror("talker: sendto");
  exit(1);
 }

  }
  if(neigh[i].nip==sendmsg[4].ip)
  {
   struct sockaddr_in g;
   memcpy(&g.sin_addr,&sendmsg[4].ip,sizeof(sendmsg[4].ip));
   inet_ntop(AF_INET,&g.sin_addr,st,INET_ADDRSTRLEN);
   
   char portno[5];
   sprintf(portno,"%d",sendmsg[4].port);  
    memset(&hi, 0, sizeof hi);
hi.ai_family = AF_UNSPEC;
hi.ai_socktype = SOCK_DGRAM;
if ((r = getaddrinfo(st, portno, &hi, &servinfo)) != 0) {
fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
return 1;
}
// loop through all the results and make a socket
for(t = servinfo; t != NULL; t = t->ai_next) {
if ((sockfd = socket(t->ai_family, t->ai_socktype,t->ai_protocol)) == -1) {
perror("talker: socket");
continue;
}
break;
}
if (t == NULL) {
fprintf(stderr, "talker: failed to bind socket STEP\n ");
return 2;
}

 if ((num = sendto(sockfd, &bufr, sizeof(bufr), 0,t->ai_addr, t->ai_addrlen)) == -1)
 {
  perror("talker: sendto");
  exit(1);
 }

}
}
}
return 0;
}

//for getaddrinfo
void *get_in_addr(struct sockaddr *sa)
{
if (sa->sa_family == AF_INET) 
{
return &(((struct sockaddr_in*)sa)->sin_addr);
}
return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

//main function

int main(int argc,char *argv[])
{
FILE *rm;
char *save;
char *tokens;
int timeval;
unsigned int packetsize=0;
int x,y;
char buffer[5000];
char recvbuffer[68];
char str[20];
char filename[30];

int sockfd;
struct addrinfo hints, *servinfo, *p;
void *addr;
int rv;
int numbytes;
int rvdbytes;
struct sockaddr_storage their_addr;
socklen_t addr_len;
char ipstr[INET6_ADDRSTRLEN];
char s[INET6_ADDRSTRLEN];
fd_set master;   
fd_set readfds;
int mm,sel;
struct sockaddr_in b;
char arg1[10],arg2[10],arg3[30],arg4[5],arg5[5]; //for sscanf
uint16_t cost;
int16_t recvdonemorepacket;

struct token pc[50];                //for strtok
struct details neigh[10];           //to store neighbour details
struct message revddata[10];        //structure to store the message
struct message sendmsg[10];         // structure to send the message
struct dv dv1[10];                  //to maintain the routing table


//getting the timout value from the startup command
timeval=atoi(argv[4]);

//initialising 
for(x=0;x<=4;x++)
 {
  
    dv1[x].cost=0xffff;
    neigh[x].cost=0xffff;
    neigh[x].expire=3*timeval;
    sendmsg[x].h=0x0;
   
 }
 
if(argc<5)
{
fflush(stdout);
printf("please follow the syntax \n");
return 1;
}

if((strcmp(argv[1],"-t") != 0) && (strcmp(argv[3],"-i") != 0))
{
fflush(stdout);
printf("please specify the update interval \n");
return 1;
}

 //reading topology file
 strcpy(filename,argv[2]);
 rm = fopen(filename, "r");
 if(rm<=0)
 {
 fflush(stdout);
 printf("error opening the file \n");
 }
 while (!feof(rm)) 
   {
    fread(buffer, 1, (sizeof buffer)-1, rm);
    tokens = strtok_r(buffer, " \n", &save); 
    int count = 0;
    while (tokens != NULL) 
   {
        strcpy(str,tokens);
        strcpy(pc[count].arguments,tokens);
        count++;
        tokens = strtok_r(NULL, " \n", &save);
    }
   
  }
  
  //copying from topology file to variables
  dv1[0].n=atoi(pc[0].arguments);
  dv1[0].nongh = atoi(pc[1].arguments);
  neigh[0].id = atoi(pc[2].arguments);
  
  inet_pton(AF_INET,pc[3].arguments,&b.sin_addr);
  memcpy(&neigh[0].nip,&b.sin_addr,sizeof(b.sin_addr));
  
  neigh[0].nport = atoi(pc[4].arguments);
  neigh[1].id = atoi(pc[5].arguments);
  
  inet_pton(AF_INET,pc[6].arguments,&b.sin_addr);
  memcpy(&neigh[1].nip,&b.sin_addr,sizeof(b.sin_addr));
   
  neigh[1].nport = atoi(pc[7].arguments);
  neigh[2].id = atoi(pc[8].arguments);
  neigh[2].nip= atoi(pc[9].arguments);
  inet_pton(AF_INET,pc[9].arguments,&b.sin_addr);
  memcpy(&neigh[2].nip,&b.sin_addr,sizeof(b.sin_addr));
  neigh[2].nport = atoi(pc[10].arguments);
  neigh[3].id = atoi(pc[11].arguments);
  
  inet_pton(AF_INET,pc[12].arguments,&b.sin_addr);
  memcpy(&neigh[3].nip,&b.sin_addr,sizeof(b.sin_addr));
  neigh[3].nport = atoi(pc[13].arguments);
  neigh[4].id = atoi(pc[14].arguments);
  
  inet_pton(AF_INET,pc[15].arguments,&b.sin_addr);
  memcpy(&neigh[4].nip,&b.sin_addr,sizeof(b.sin_addr));
  neigh[4].nport= atoi(pc[16].arguments);
   

 //initialised
 neigh[0].updatefields=dv1[0].n;
 dv1[0].hopto =neigh[0].id;
 dv1[1].hopto =neigh[1].id;
 dv1[2].hopto =neigh[2].id;
 dv1[3].hopto =neigh[3].id;
 dv1[4].hopto =neigh[4].id;


//cost
  int i=0;
  for(i=0;i<=4;i++)
  {
  if((neigh[i].id == atoi(pc[17].arguments)) || (neigh[i].id == atoi(pc[20].arguments)) || (neigh[i].id == atoi(pc[23].arguments)) || (neigh[i].id == atoi(pc[26].arguments))||(neigh[i].id == atoi(pc[29].arguments)))
   {
   neigh[i].cost=0;
   dv1[0].presentnode=neigh[i].id;
   dv1[0].presentip=neigh[i].nip;
  }
  
  if(neigh[i].id == atoi(pc[18].arguments))
  {
   neigh[i].cost = atoi(pc[19].arguments);
   }
  if(neigh[i].id == atoi(pc[21].arguments))
  {
    neigh[i].cost = atoi(pc[22].arguments);
   }
  if(neigh[i].id == atoi(pc[24].arguments))
  {
    neigh[i].cost = atoi(pc[25].arguments);
   }
   if (neigh[i].id == atoi(pc[27].arguments))
   {
    neigh[i].cost = atoi(pc[28].arguments);
   }
   if (neigh[i].id == atoi(pc[30].arguments))
  {
    neigh[i].cost = atoi(pc[31].arguments);
  }
  }//for

 //cpoying the details to the distance vector table
 for(i=0;i<dv1[0].n;i++)
 {  
 dv1[i].sid=neigh[i].id;
 dv1[i].cost=neigh[i].cost;
 dv1[i].ip=neigh[i].nip;
 dv1[i].port=neigh[i].nport;
 }//for

//reference to beej
//socket
int portn;
int16_t z=0;
for(z=0;z<dv1[0].n;z++)
{
if(dv1[0].presentnode==z)
{
portn=neigh[z-1].nport;
}
}
char portno[5];
sprintf(portno,"%d",portn);
memset(&hints, 0, sizeof hints);
hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
hints.ai_socktype = SOCK_DGRAM;
hints.ai_flags = AI_PASSIVE; // use my IP
if ((rv = getaddrinfo(NULL, portno, &hints, &servinfo)) != 0) {
fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
return 1;
}
// loop through all the results and bind to the first we can
for(p = servinfo; p != NULL; p = p->ai_next) {
if ((sockfd = socket(p->ai_family, p->ai_socktype,
p->ai_protocol)) == -1) {
perror("listener: socket");
continue;
}
if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
close(sockfd);
perror("listener: bind");
continue;
}
break;
}


if (p == NULL) {
fprintf(stderr, "listener: failed to bind socket STEP\n");
return 1;
}

freeaddrinfo(servinfo);


for(;;)
{
 
 FD_ZERO(&master);
 FD_ZERO(&readfds);
 FD_SET(sockfd,&master);
 FD_SET(0,&master);
 readfds = master;
mm=sockfd;

struct timeval time;
time.tv_sec=timeval;
sel =  select((mm+1),&master,NULL,NULL,&time); //reference to recitation material

 //to send an update after every timeout to its neighbouring nodes
 if(sel < 0)      
 {
 fprintf(stdout,"\n couldnt select");
 fflush(stdout);
 return 1;
 }
 
 //after timeout
 if(sel==0)                       //reference to recitation material
 {
 fflush(stdout);
 printf("\ntimed out");
 fflush(stdout);
 for(i=0;i<dv1[0].n;i++)
 {
  if(neigh[i].expire !=0)
  {
  neigh[i].expire =neigh[i].expire-timeval;
  }
 
  if(neigh[i].id == dv1[0].presentnode)
    neigh[i].expire = 0;
  
  if(neigh[i].expire == 0 && neigh[i].id != dv1[0].presentnode)
  {
  neigh[i].cost = 0xffff;
  }
 
 }//for


 //copying the neighbour details into the routing table
 for(i=0;i<dv1[0].n;i++)
 {  
 dv1[i].sid=neigh[i].id;
 dv1[i].cost=neigh[i].cost;
 dv1[i].ip=neigh[i].nip;
 dv1[i].port=neigh[i].nport;
 }//for
 makepacket(dv1,sendmsg,neigh,portn);
 }//if

//if a packet is received
if(FD_ISSET(sockfd,&master))  //reference to beej
{

int rvdbytes;
struct sockaddr_storage their_addr;
socklen_t addr_len;
char s[INET6_ADDRSTRLEN];

addr_len=sizeof(their_addr);
addr_len = sizeof their_addr;

if ((rvdbytes = recvfrom(sockfd, recvbuffer, sizeof(recvbuffer) , 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) 
  {
  perror("recvfrom");
  exit(1);
  }
else
  recvdonemorepacket+=1;


//deserialization //reference to beej
unpack(recvbuffer,"hhllhhhhlhhhhlhhhhlhhhhlhhhh",&revddata[0].noofupdate,&revddata[0].senderport,&revddata[0].senderip,&revddata[0].ip,&revddata[0].port,&revddata[0].h,&revddata[0].id,&revddata[0].cost,&revddata[1].ip,&revddata[1].port,&revddata[1].h,&revddata[1].id,&revddata[1].cost,&revddata[2].ip,&revddata[2].port,&revddata[2].h,&revddata[2].id,&revddata[2].cost,&revddata[3].ip,&revddata[3].port,&revddata[3].h,&revddata[3].id,&revddata[3].cost,&revddata[4].ip,&revddata[4].port,&revddata[4].h,&revddata[4].id,&revddata[4].cost);


//to display the received message on the receiver side
for(i=0;i<dv1[0].n;i++)
{
if(neigh[i].nip==revddata[0].senderip)
{
fflush(stdout);
printf("\n RECEIVED A MESSAGE FROM SERVER <%d>",neigh[i].id);
neigh[i].expire=3*timeval;
}
 int j=0;
  for(j=0;j<dv1[0].n;j++)
  {
 if(revddata[0].senderip==neigh[i].nip)
  {
   if(dv1[0].presentip == neigh[j].nip)
    {
     if(neigh[i].cost>revddata[j].cost)
       neigh[i].cost = revddata[j].cost;
     }//if
  }//if i
 }//for j
 }//for i

//copying the neigh details to the distance vector
 for(i=0;i<dv1[0].n;i++)
 {  
 dv1[i].sid=neigh[i].id;
 dv1[i].cost=neigh[i].cost;
 dv1[i].ip=neigh[i].nip;
 dv1[i].port=neigh[i].nport;
 }//for

updaterouting(dv1,sendmsg,revddata,neigh);
}//fdisset sock

else if(sel>0)   //reference to beej
{
//user command
if(FD_ISSET(0,&master))
{ 
fflush(stdin);
fgets(buffer,150,stdin);
sscanf(buffer,"%s %s %s %s %s",arg1,arg2,arg3,arg4,arg5);
fflush(stdout);
if(strcmp(arg1,"update") == 0)
 {
    
  char prst[5];
  char st[INET6_ADDRSTRLEN];
  struct sockaddr_in g;
  char portno[5];
  sprintf(prst,"%d",dv1[0].presentnode);
  if(strcmp(prst,arg2)==0)
  {
   int i=0;
   for(i=0;i<dv1[0].n;i++)
    {
    char nei[5];
    int16_t packetsize=0;
    int16_t h=0x0;
     sprintf(nei,"%d",neigh[i].id);
    if(strcmp(nei,arg3)==0 && neigh[i].cost != 0xffff)
     { 
      uint8_t bufr[68];
      if(strcmp(arg4,"inf")==0 || strcmp(arg4,"INF")==0 )
         neigh[i].cost=0xffff;
        
    
      else
        
         neigh[i].cost=atoi(arg4);
     
  
   
   packetsize=pack(bufr,"hhllhhhhlhhhhlhhhhlhhhhlhhhh",neigh[0].updatefields,portn,dv1[0].presentip,neigh[0].nip,neigh[0].nport,h,neigh[0].id,neigh[0].cost,neigh[1].nip,neigh[1].nport,h,neigh[1].id,neigh[1].cost,neigh[2].nip,neigh[2].nport,h,neigh[2].id,neigh[2].cost,neigh[3].nip,neigh[3].nport,h,neigh[3].id,neigh[3].cost,neigh[4].nip,neigh[4].nport,h,neigh[4].id,neigh[4].cost); //reference to beej
   
    
    memcpy(&g.sin_addr,&neigh[i].nip,sizeof(details));
    inet_ntop(AF_INET,&g.sin_addr,st,INET_ADDRSTRLEN);
   
  
  //reference to beej
  //UDP socket to send packets
   sprintf(portno,"%d",neigh[i].nport);
   memset(&hints, 0, sizeof hints);
   hints.ai_family = AF_UNSPEC;
   hints.ai_socktype = SOCK_DGRAM;
   if ((rv = getaddrinfo(st, portno, &hints, &servinfo)) != 0)
   {
   fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
   return 1;
   }
   // loop through all the results and make a socket
   for(p = servinfo; p != NULL; p = p->ai_next)
   {
   if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) 
   {
   perror("talker: socket");
   continue;
   }
   break;
   }
   if (p == NULL) 
   {
   fprintf(stderr, "talker: failed to bind socket STEP\n");
   return 2;
   }

   if ((numbytes = sendto(sockfd, bufr, sizeof(bufr), 0,p->ai_addr, p->ai_addrlen)) == -1)
   {
    perror("talker: sendto");
    exit(1);
   }  
   else{
    fflush(stdout);
    printf("update SUCCESS \n");
    }
   }// ifarg3
   }//for
  
  for(i=0;i<dv1[0].n;i++)
  {
  if(atoi(arg3) == neigh[i].id)
  {
   if(neigh[i].cost == 0xffff){
       fflush(stdout);
       printf(" \n UPDATE server is not my neighbour %d \n",neigh[i].id);
         }
  }
  }  
   
     
   }//if arg2
  
 //copying the neigh details to the distance vector
 for(i=0;i<dv1[0].n;i++)
 {  
 dv1[i].sid=neigh[i].id;
 dv1[i].cost=neigh[i].cost;
 dv1[i].ip=neigh[i].nip;
 dv1[i].port=neigh[i].nport;
 }//for
  
 }//if update

 if(strcmp(arg1,"step")==0)
  {
  makepacket(dv1,sendmsg,neigh,portn);
  fflush(stdout);
  printf("\n step SUCCESS");
   if(arg2 !=0)
    printf("\n 'step' please follow the command format");
  //sendupdate();
  }

 if(strcmp(arg1,"display")==0)
  {printf("\nid\tdest\thop\tcost\t\n");
  for(i=0;i<dv1[0].n;i++)
  { fflush(stdout);
    printf("\n%d\t%d\t%d\t%d\t\n",dv1[0].presentnode,dv1[i].sid,dv1[i].hopto,dv1[i].cost);
   }
   fflush(stdout);
   printf("display SUCCESS \n");
   
  }

 if(strcmp(arg1,"crash")==0)
 {
  fflush(stdout);
  printf("\n CRASH SUCCESS \n");
  close(sockfd);
 }
if(strcmp(arg1,"disable")==0)
{
  
  for(i=0;i<dv1[0].n;i++)
  {
     if(atoi(arg2)==neigh[i].id)
     {
    neigh[i].cost=0xffff;
     }
   }//for
 
 //copying the neighbour details t the distance vector table
 for(i=0;i<dv1[0].n;i++)
 {  
 dv1[i].sid=neigh[i].id;
 dv1[i].cost=neigh[i].cost;
 dv1[i].ip=neigh[i].nip;
 dv1[i].port=neigh[i].nport;
 }//for
 fflush(stdout);
 printf("\n disable SUCCESS \n");
  
  if((atoi(arg2)!=neigh[0].id) && (atoi(arg2)!=neigh[1].id) && (atoi(arg2)!=neigh[2].id) && (atoi(arg2)!=neigh[3].id) && (atoi(arg2)!=neigh[4].id) && (atoi(arg2)!=neigh[5].id))
  {
   fflush(stdout);
   printf("\n the server id is not my neighbour DISABLE \n");
  }
 
}//disable

if(strcmp(arg1,"packets")==0)
{
 fflush(stdout);
 printf("\npackets received %d",recvdonemorepacket);
 fflush(stdout);
 printf("\n PACKETS SUCCESS \n");
 if(recvdonemorepacket == 0)
 { fflush(stdout);
  printf("\n PACKETS No packets have been received \n");
 }
 recvdonemorepacket=0;
}
}//STDIN
}//sel
}//infinite loop
return 0;
}

