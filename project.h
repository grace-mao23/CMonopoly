#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
//#include <sys/socket.h>
//#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#define PORT "42069"
#define TEST_IP "127.0.0.1"
#define BUFFER_SIZE 1024
#define INPLAY 0

int server_setup();
int server_connect(int sd);
int client_setup(char *server);
void error_check(int i, char *s) {
    if ( i < 0 ) {
        printf("[%s] error %d: %s\n", s, errno, strerror(errno) );
        exit(1);
    }
}
void taken_setup(int *taken);
int lowest_available(int *taken);
