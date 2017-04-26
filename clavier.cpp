#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>
#include <stdio.h>

struct termios orig_termios;

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}


void set_conio_terminal_mode()
{
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

int getch()
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}

int main()
{
int c    ;

    while (1) {
	//printf("touch a key\n");
	set_conio_terminal_mode();
	if (kbhit()){
		c=getch();
		//printf("%d\n",c);
	}
	reset_terminal_mode();
        /* do some work */
	if(c==32){
		printf("well\n");
	c=0;
	}
	if (c==3){
		break;
	}
	//printf("touch a key\n");
	//printf("touch a key\n");
    }
  //  (void)getch(); /* consume the character */

}