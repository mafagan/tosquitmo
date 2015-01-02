#include <ev.h>

int main(int argv, char **argc)
{
    ev_io listensock;
    struct ev_loop *loop = EV_DEFAULT;
    //TODO
    ev_run(loop, 0);
    return 0;
}
