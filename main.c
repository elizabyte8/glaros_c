#include <stdio.h>
#include <stdlib.h>      // for func atoi()
#include <string.h>      // for func memset()
#include <unistd.h>      // for func to close() our socket
//#include <netdb.h>     // database for DNS. Contains func gethostbyname()
#include <arpa/inet.h>   // for funcs htons(),htonl(),ntohs(),ntohl()
#include <sys/types.h>   // is a "daddy" for the described bellow libraries (they require sys/types.h). Contains funcs for system calls
#include <sys/socket.h>  // contains structs for socket creation
#include <netinet/in.h>  // for func connect(). Contains tools for Internet domain addresses

int main(int argc, char** argv)// START of MAIN
{
 unsigned int i = 1, j;// vars for loop
 // -1 scans only one port
 // -R scans RANGE of ports (-R <first> <last>)
 // -F FAST scan (100 common ports)
 // -S STANDART (1024 ports)
 // -A ALL (65535 ports. REQUIRES ROOT PRIVILEGES)
 
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
   j == 100;
 if(*(argv[2]+1) == 'S')
   j = 1024;
 if(*(argv[2]+1) == 'A')
   j = 65535;
 if(*(argv[2]+1) != '1' && *(argv[2]+1) != 'R' && *(argv[2]+1) != 'F' && *(argv[2]+1) != 'S' && *(argv[2]+1) != 'A') 
 printf("No such an option");

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
 host.sin_port = htons(i);// htons() converts from little endian byte order to big endian (Network) order
 /* inet_aton()  converts IP address from the Internet dot notation to a Network address,
 exp: 192.168.1.1 = 3232235777. The calculation is 
(192*256^3) + (168*256^2) + (1*256^1) + 256 + 1 = 3232235777
returns zero if failure, and a nonzero value if success */
  if(!inet_aton(argv[1],&host.sin_addr)) 
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
 }  
return 0;
}// END of MAIN
