#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>
#include <unistd.h>

#define DEBUG 1

#include "connect_client.h"
#include "port.h"
#include "query.h"

/* FIXME - figuring out calling malloc and free here is a bad thing 
   pass the buffer? */

int
query_main(QueryData *q, char *host)
{
    int connfd, n, m, i;
    char *myhost;
    char *answer = (char *) calloc(1280,1);
    char *query = q->query;
    int cnt;

    if(host == NULL) {
      myhost = "2001:4f8:3:36:2e0:81ff:fe23:90d3"; // toutatis.taht.net
    } else {
      myhost = host;
    }
    build_query(q); 
    if(q->options.verbose) fprintf(stderr,"Connecting: %s\n",myhost);
    connfd = connect_client(myhost, QUERY_PORT, AF_UNSPEC, SOCK_DGRAM);

    if (connfd < 0) {
         fprintf(stderr,
                 "client error:: could not create connected socket "
                 "socket\n");
         return -1;
    }
    if(q->options.verbose) {
      fprintf(stderr,"Writing query: \"%s\" to socket of length %d\n", 
	      query, strlen(query));
    }
    // fixme - generate legal checksum and check length
    m= write(connfd, query, strlen(query));
    memset(answer, 0, MAX_MTU);
    n = read(connfd,
             answer,
             MAX_MTU); // FIXME, leave running and timeout
    if(q->options.verbose) fprintf(stderr,"Got response: %s\n", answer);
    close(connfd);
    strcpy(q->query,answer); // ycuk - figmeout
    return(answer_parse(q));
}
