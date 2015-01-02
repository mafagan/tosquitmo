#include <unistd.h>

static void setnonblock(int fd);
static void setreuseaddr(int fd);
static void setaddress(const char *ip, int port, struct sockaddr_in *addr);
static int new_tcp_listensock();
