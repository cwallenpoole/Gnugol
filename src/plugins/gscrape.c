/* program sends user enter message from parent to child and back */
/* core contributed by david rowe. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "common.h"

#define EXECV

int gnugol_plugin_gscrape_init(int p2p[2]) // I'm confused, this is an array ref?
{
    int i, pid, p2c[2], c2p[2];
    char send[80], receive[80], *gets();
 
    if ( pipe(p2c) != 0) {
	printf("p2c pipe creation error");
	return(-1);
    }

    if ( pipe(c2p) != 0) {
	printf("c2p pipe creation error");
	return(-1);
    }

    i = 0;
    pid = fork();
    if (pid == -1) {
	printf("fork error\n");
	return(pid);
    }
 
    if (pid == 0) {                        /* CHILD process */
	dup2(c2p[1], 1); // stdout (e.g. printfs) get send to c2p[1]
	dup2(p2c[0], 0); // stdin comes from p2c[0]

#ifdef EXECV
        if(execv("/home/d/bin/googd",NULL)>0) {
		printf("NPG\n"); // No plugin
	    	fprintf(stderr, "No plugin backend available\n");
		exit(-1);
	}
#else	      
	while(1) {
        fgets(receive,80,stdin); 
	    fprintf(stderr, "Child received the following message: %s",receive);
	    printf("Really?\n");
	    fflush(stdout);
	}
#endif
	/* never gets here */
        exit(0);
    }
    p2p[1] = p2c[1];
    p2p[0] = c2p[0];
    return(pid);
}

#ifdef TEST_PLUGIN
int main(argc, argv)
     int argc;
     char *argv[];
{
  int i, pid, p2p[2];
  char send[1280], receive[1280], *gets();
  if((pid = gnugol_plugin_gscrape_init(p2p)) > 0) { 
    sprintf(send, "BEGIN LNK SNP\nthis is a test\nEND\n");
    write(p2p[1],send,strlen(send));
    i = read(p2p[0],receive,1280);       /* get message from child */
    receive[i] = '\0';
    fprintf(stderr,"Parent received: %s",receive);
    
    sprintf(send, "BEGIN LNK SNP\nIPv6 address exaustion\nEND\n");
    write(p2p[1],send,strlen(send));
    i = read(p2p[0],receive,1280);       /* get message from child */
    receive[i] = '\0';
    fprintf(stderr,"Parent received: %s",receive);
    kill(pid,1);
  }
  return 0;
}
#endif 
