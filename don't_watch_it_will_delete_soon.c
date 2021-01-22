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

char *listports; // for ports in file.txt


char *glaros_ascii[] =
{
 "________.__",
"/  _____/|  | _____ _______  ____  ______",
"/  \\ \\  ___|  | \\__  \\\\_  __ \\/  _ \\/  ___/",
"\\   \\ \\_\\ \\  |__/ __ \\|  | \\(  <_> )___ \\ ",
"\\______  /____(____  /__|   \\____/____  >",
"       \\/          \\/                 \\/ ",
};


int *read_file_txt(int *j)
{
 FILE *fp;// stands for file pointer

   if(*(j) == F)
   {
    listports = (char*)malloc(F*sizeof(char*));
    fp = fopen("glaros.C_TCP_common_ports.txt", "r");
    fgets(listports,F,fp);
   }
   if(*(j) == S)
   { 
    listports = (char*)malloc(S*sizeof(char*));
    fp = fopen("glaros.C_TCP_standart_ports.txt","r");
    fgets(listports,S,fp);
   }
   if(*(j) == A)
   {
    listports = (char*)malloc(A*sizeof(char*));
    fp = fopen("glaros.C_TCP_all_ports.txt","r");
    fgets(listports,A,fp);
   }
 if(fp == NULL)
 {
 /* if(*(j) == F)
 // printf("Error openning file: 'glaros.C_common_ports.txt'");
  if(*(j) == S)
 // printf("Error openning file: 'glaros.C_standart_ports.txt'");
  if(*(j) == A)
  printf("Error openning file: 'glaros.C_all_ports.txt'");
  exit(1);*/
  perror("Error openning file");
 //return -1;
 }
 
 fclose(fp);
//int *bl[*j];
 // for(int i = 0; i < *j; i++)
// (bl + i) = (int*)(listports + i);
return (int*)listports;
}


int main(int argc, char **argv)// START of MAIN
{
 unsigned int i = 1, j;// vars for (ports) in loop
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
  
  for(int k = 0; k < 6; k++)// prints logo 'glaros'
  printf("%s",glaros_ascii[k]);
  
 // int error; 
 // error = read_file_txt(&j);// calls the func read_file_txt()

 for(i; i <= j; i++)
 {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);// creates general socket
  // stands for socket file descriptor. AF_INET is IPv4, SOCK_STREAM is TCP
  // returns a nonzero value if failure, zero if success
  if(sockfd < 0) 
  {
    perror("socket ");// explains the reason why socket was not created  
   return -1;
  }   
 
 struct sockaddr_in host;// creates socket for remote (Internet) host
 memset(&host,sizeof(host),0); // zeroes the host's struct
 host.sin_family = AF_INET;
 int p0rts[j];

 for(int k = 0; k < j; k++)// extracts ports from 'listp'
 {// char temp[k] = *(listp + i + k);
  p0rts[k] = *read_file_txt(&j);
 host.sin_port = htons(p0rts[k]);
// htons() converts from little endian byte order to big endian (Network) order
// printf("%d", temp);
}
 /* inet_aton()  converts IP address from the Internet dot notation to a Network address,
 exp: 192.168.1.1 = 3232235777. The calculation is 
(192*256^3) + (168*256^2) + (1*256^1) + 256 + 1 = 3232235777. The result is stored in second parameter -> &host.sin_addr
returns zero if failure, and a nonzero value if success */
  int network_address;
  network_address = inet_aton(argv[1],&host.sin_addr);
  //if(!inet_aton(argv[1],&host.sin_addr))
   if(!network_address)
    perror("network_address ");
  
  int connection = connect(sockfd,(struct sockaddr*)&host, sizeof(struct sockaddr_in)); // connects general socket to the host's one. 
 // Returns a nonzero value if failure, and zero if success 
  if(connection < 0) 
  { 
    printf("| The port %d : is closed [-] \n",i);
    close(sockfd);// disconnection
  }
  else
  {
    printf("| The port %d :  is open  [+] \n",i);
    close(sockfd);// disconnection
  }
    free(&listports[i]);// returns borrowed memory to OS
 }  
 
return 0;
}// END of MAIN



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
 "  ________.__",
" /  _____/|  | _____ _______  ____  ______",
" /  \\ \\  ___|  | \\__  \\\\_  __ \\/  _ \\/  ___/",
" \\   \\ \\_\\ \\  |__/ __ \\|  | \\(  <_> )___ \\ ",
" \\______  /____(____  /__|   \\____/____  >",
"        \\/          \\/                 \\/ ",
};

char listports[10];


char *read_file(int *i, int *j)// START of func 
{// extracts ports from file.txt
 if(*(j) == F)
{
 FILE *fp;
 fp = fopen("glaros.C_TCP_common_ports.txt","r");
  for(int k = 0; k <=*i; k++)
   for(int l = 0; l <= '\0'; l++)
    fgets(listports,10,fp);
 fclose(fp);
 }

 if(*(j) == S)
{
  FILE *fp;
  fp = fopen("glaros.C_TCP_common_ports.txt","r");
   for(int k = 0; k <=*i; k++)
    for(int l = 0; l <= '\0'; l++)
     fgets(listports,10,fp);
  fclose(fp);
 }

return listports;
}// END of func


int main(int argc, char **argv)// START of MAIN
{
 unsigned int i = 1, j;// vars for (ports) in loop
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
  /* char p0rts[10];
for(int k = 0; k < 10; k++)
{p0rts[k] = *read_file(&i,&j);
 
printf("blblblb %s", p0rts);} 
 // FILE *fp;
 // char listports[j];

//char listports[10];
   //if(fp != NULL)
   // if((j) == F)
  // {// for(int l = 0; l <= j; l++)
//for(int k = 0; k <= '\0';  k++) //  listports = (char*)malloc(F*sizeof(char*));
 //{
 
 //fp = fopen("glaros.C_TCP_common_ports.txt", "r");
    
  //} 
  /* if((j) == S)
   { 
//    listports = (char*)malloc(S*sizeof(char*));
    fp = fopen("glaros.C_TCP_standart_ports.txt","r");
    fgets(listports,S,fp);
   }
   if((j) == A)
   {
    //listports = (char*)malloc(A*sizeof(char*));
    fp = fopen("glaros.C_TCP_all_ports.txt","r");
    fgets(listports,A,fp);
   }
 //if(fp == NULL)
// {
 /* if(*(j) == F)
 // printf("Error openning file: 'glaros.C_common_ports.txt'");
  if(*(j) == S)
 // printf("Error openning file: 'glaros.C_standart_ports.txt'");
  if(*(j) == A)
  printf("Error openning file: 'glaros.C_all_ports.txt'");
  exit(1);*/
  //perror("Error openning file");
// return -1;}
 




 
 
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);// creates general socket
  // stands for socket file descriptor. AF_INET is IPv4, SOCK_STREAM is TCP
  // returns a nonzero value if failure, zero if success
  if(sockfd < 0) 
  {
    perror("socket ");// explains the reason why socket was not created  
   return -1;
  }   
 
 struct sockaddr_in host;// creates socket for remote (Internet) host
 memset(&host,sizeof(host),0); // zeroes the host's struct
 host.sin_family = AF_INET;
 //int p0rts[j];
 unsigned int p0rts;
char bl;
bl = *read_file(&i,&j);
p0rts = (unsigned int) bl;
 //for(int k = 0; k < j; k++)// extracts ports from 'listp'
 //{// char temp[k] = *(listp + i + k);
 // p0rts[k] = *read_file_txt(&j);
//if(*argv[2]+1 != '1' && *argv[2]+1 != 'R') 
//for(int k = 0; k != '\0'; k++)
 //host.sin_port = htons(p0rts);
printf("%d", p0rts);}
//else
//for(int k = 0; k != '\0'; k++)
//host.sin_port = htons(i);
// htons() converts from little endian byte order to big endian (Network) order
// printf("%d", temp);
//}
 /* inet_aton()  converts IP address from the Internet dot notation to a Network address,
 exp: 192.168.1.1 = 3232235777. The calculation is 
(192*256^3) + (168*256^2) + (1*256^1) + 256 + 1 = 3232235777. The result is stored in second parameter -> &host.sin_addr
returns zero if failure, and a nonzero value if success */
  //int network_address;
 /*int network_address = inet_aton(argv[1],&host.sin_addr);
  if(!inet_aton(argv[1],&host.sin_addr))
   if(!network_address)
    perror("network_address ");
  
  int connection = connect(sockfd,(struct sockaddr*)&host, sizeof(struct sockaddr_in)); // connects general socket to the host's one. 
 // Returns a nonzero value if failure, and zero if success 
  if(connection < 0) 
  { 
    if(*argv[2]+1 != '1' && *argv[2]+1 != 'R')
     for(int k = 0; k != '\0'; k++)
      printf("| The port %s : is closed [-] \n", (listports));

    //printf("| The port %d : is closed [-] \n",i);
    close(sockfd);// disconnection
  }
  else
  { if(*argv[2]+1 != '1' && *argv[2]+1 != 'R')
     //for(int k = 0; k != '\0'; k++)
     //printf("| The port %d : is open [+] \n", (listports[k]));
    printf("| The port %d :  is open  [+] \n",i);
    close(sockfd);// disconnection
  }
i++;
  //fclose(fp); // free(&listports[i]);// returns borrowed memory to OS
 }*/
//fclose(fp);  
return 0;
}// END of MAIN

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
 "  ________.__",
" /  _____/|  | _____ _______  ____  ______",
" /  \\ \\  ___|  | \\__  \\\\_  __ \\/  _ \\/  ___/",
" \\   \\ \\_\\ \\  |__/ __ \\|  | \\(  <_> )___ \\ ",
" \\______  /____(____  /__|   \\____/____  >",
"        \\/          \\/                 \\/ ",
};

char listports[10];


int main(int argc, char **argv)// START of MAIN
{
 unsigned int i = 0, j, p0rts[10];// vars for (ports) in loop
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
  fp = fopen("glaros.C_TCP_common_ports.txt","r");
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
 
 // down bellow will be written some manipulations with casting of ports (from ascii to int)

//if(j == F)
 
  
  // for(int k = 0; k <= '\0'; k++)
    //{ports[k] = *(read_file(&i,&j) + k);
 
  //p0rts[k] = atoi(ports+k);// this is the desired result, ports in int type
 // host.sin_port = htons(p0rts);// ports converted into big endian byte order
//printf("string -> %s\n" ,ports);
printf("intiger -> %u\n", atoi(listports));
 //printf("htons -> %d\n", host.sin_port);
 /*else
  for(int k = 0; k != '\0'; k++)
   host.sin_port = htons(i);// htons() converts from little endian byte order to big endian (Network) order
*/
 /* inet_aton()  converts IP address from the Internet dot notation to a Network address,
 exp: 192.168.1.1 = 3232235777. The calculation is 
(192*256^3) + (168*256^2) + (1*256^1) + 256 + 1 = 3232235777. The result is stored in second parameter -> &host.sin_addr
returns zero if failure, and a nonzero value if success */
 /* int network_address;
  network_address = inet_aton(argv[1],&host.sin_addr);
  //if(!inet_aton(argv[1],&host.sin_addr))
   if(!network_address)
    perror("network_address ");
  
  int connection = connect(sockfd,(struct sockaddr*)&host, sizeof(struct sockaddr_in)); // connects general socket to the host's one. 
 // Returns a nonzero value if failure, and zero if success 
  if(connection < 0) 
  { 
    //if(*argv[2]+1 != '1' && *argv[2]+1 != 'R')
     printf("| The port %d : is closed [-] \n", p0rts);

    //printf("| The port %d : is closed [-] \n", i);
    close(sockfd);// disconnection
  }
  else
  { 
    //if(*argv[2]+1 != '1' && *argv[2]+1 != 'R')
     printf("| The port %d : is open [+] \n", p0rts);

    //printf("| The port %d :  is open  [+] \n", i);
    close(sockfd);// disconnection
  }*/
i++;
  //fclose(fp); // free(&listports[i]);// returns borrowed memory to OS
 }
//fclose(fp);  
return 0;
}// END of MAIN
