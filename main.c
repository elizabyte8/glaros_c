/************************************************************
*** Title: gLaЯo$ the port scanner **************************
*** Author: cold_summer aka Elizabeth Maslennikova **********
*** Date: 19/01/2021 ****************************************
*** Description: Did you close all your ports? **************
************************************************************/
#include <stdio.h>       // for funcs printf() and fgets()
#include <stdlib.h>      // for func atoi()
#include <string.h>      // for func memset()
#include <unistd.h>      // for func to close() our socket
// #include <netdb.h>    // database for DNS. Contains func gethostbyname()
#include <arpa/inet.h>   // for funcs htons(),htonl(),ntohs(),ntohl()
#include <sys/types.h>   // is a "daddy" for the described bellow libraries (they require sys/types.h). Contains funcs for system calls
#include <sys/socket.h>  // contains structs for socket creation
#include <netinet/in.h>  // for func connect(). Contains tools for Internet domain addresses

#define  F 100   // -F (fast scan)
#define  S 1000  // -S (standart scan)
#define  A 65535 // -A (all ports)

char *glaros_ascii[] =
{
 "     ________.__",
"   /   _____/|  | _____ _______  ____  ______",
"  /     \\  __|  | \\__  \\\\_  __ \\/  _ \\/  ___/",
"  \\      \\_\\ \\  |__/ __ \\|  | \\(  <_> )___ \\      cold_summer ",
"   \\ ______  /____(____  /__|   \\____/_____ >",
"       \\/            \\/                 \\/ ",
};

char listports[10];


int main(int argc, char **argv)// START of MAIN
{
 unsigned int i = 0, j;// vars for (ports) in loop
 
 for(int k = 0; k < 6; k++)
 printf("%s\n", glaros_ascii[k]);

 // -1 scans only one port
 // -R scans RANGE of ports (-R <first> <last>)
 // -F FAST scan (100 common ports)
 // -S STANDART (1000 another common ports)
 // -A ALL (all of 65535 ports. REQUIRES ROOT PRIVILEGES)
 if(*(argv[2]+1) == '1')
  {
   i = atoi(argv[3]);
   j = atoi(argv[3]);
  } 
 if(*(argv[2]+1) == 'R')
  {
   i = atoi(argv[3]); 
   j = atoi(argv[4]);
  }
 if(*(argv[2]+1) == 'F')
  j = F;// 100
 if(*(argv[2]+1) == 'S')
  j = S;// 1000
 if(*(argv[2]+1) == 'A')
  j = A;// 65535
 if(*(argv[2]+1) != '1' && *(argv[2]+1) != 'R' && *(argv[2]+1) != 'F' && *(argv[2]+1) != 'S' && *(argv[2]+1) != 'A') 
  printf("No such an option");

 while(i <= j)// main loop, runs the programm
 {
// extracts ports from file.txt
 if((j) == F)
 {
  FILE *fp;
  fp = fopen("glaros.C_TCP_common_ports.txt","r");
   for(int k = 0; k <= i; k++)
    for(int l = 0; l <='\0'; l++)
     fgets(listports,10,fp);
  fclose(fp);
  }

 if((j) == S)
 {
  FILE *fp;
  fp = fopen("glaros.C_TCP_standart_ports.txt","r");
   for(int k = 0; k <= i; k++)
    for(int l = 0; l <= '\0'; l++)
     fgets(listports,10,fp);
  fclose(fp);
  }

 
 int sockfd = socket(AF_INET, SOCK_STREAM, 0);// creates general socket
  // 'sockfd' stands for socket file descriptor. AF_INET is IPv4, SOCK_STREAM is TCP
  // returns a nonzero value if failure, zero if success
  if(sockfd < 0) 
  {
    perror("socket ");// explains the reason why socket was not created  
   return -1;
  }   
 
 struct sockaddr_in host;// creates socket for remote (Internet) host
 memset(&host,sizeof(host),0); // zeroes the host's struct
 host.sin_family = AF_INET;
 if(*(argv[2]+1) != '1' && *(argv[2]+1) != 'R') 
  host.sin_port = htons(atoi(listports));// htons() converts from little endian byte order to big endian (Network) order
 else
  host.sin_port = htons(i);
 /* inet_aton()  converts IP address from the Internet dot notation to a Network address,
 exp: 192.168.1.1 = 3232235777. The calculation is 
(192*256^3) + (168*256^2) + (1*256^1) + 256 + 1 = 3232235777. The result is stored in second parameter -> &host.sin_addr
returns zero if failure, and a nonzero value if success */

 if(!inet_aton(argv[1],&host.sin_addr))
    perror("network_address ");
  
  int connection = connect(sockfd,(struct sockaddr*)&host, sizeof(struct sockaddr_in)); // connects general socket to the host's one
 // Returns a nonzero value if failure, and zero if success 
  if(connection < 0) 
  { 
    if(*(argv[2]+1) != '1' && *(argv[2]+1) != 'R')
     printf("| The port %u : is closed [-] \n", atoi(listports));
    else
    printf("| The port %d : is closed [-] \n", i);
   close(sockfd);// disconnection
  }
  else
  { 
    if(*(argv[2]+1) != '1' && *(argv[2]+1) != 'R')
     printf("| The port %u : is open [+] \n", atoi(listports));
    else
    printf("| The port %d :  is open  [+] \n", i);
   close(sockfd);// disconnection
  }
  i++;
 }  
return 0;
}// END of MAIN
