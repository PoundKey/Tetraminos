
/*
 *  Pong game
 *  Author: Chafic Najjar <chafic.najjar@gmail.com>
 *  Note: Origin of the coordinate system is the upper left corner of the screen
 */

#include "pong.hpp"
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	pid_t pid = getpid();
	printf("%ld\n", (long)pid);

    Pong pong(argc, argv);
    pong.execute();

    return 0;

}
