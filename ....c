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
};// logo



int read_file_txt(int *j)// START of func
{
 FILE *fp;// stands for file pointer

   if(*(j) == F)
   {
    listports = (char*)malloc(F*sizeof(char*));
    fp = fopen("glaros.C_common_ports.txt", "r");
    fgets(listports,F,fp);
   }
   if(*(j) == S)
   { 
    listports = (char*)malloc(S*sizeof(char*));
    fp = fopen("glaros.C_standart_ports.txt","r");
    fgets(listports,S,fp);
   }
   if(*(j) == A)
   {
    listports = (char*)malloc(A*sizeof(char*));
    fp = fopen("glaros.C_all_ports.txt","r");
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
 return -1;
 }
 
 fclose(fp);
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
  
  for(int k = 0; k < 6; k++)// prints logo 'glaros'
  printf("%s",glaros_ascii[k]);
  
  int error; 
  error = read_file_txt(&j);// calls the func read_file_txt()

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
 
int num_length;
for(int k = 0; k != '\0';k++)
{
*((listports + i) + k);
num_length++;
}
char temp[num_length];
char *blya[num_length];
for(int k = 0; k != '\0';k++)
{
temp[num_length] = *((listports + i) + k);
//(blya + k) = temp[k];
}

 for(int k = 0; k != '\0'; k++)// extracts ports from 'listp'
 {// char temp[k] = *(listp + i + k);
 host.sin_port = htons(atoi(*(temp + k)));
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
