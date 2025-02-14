/***includes ***/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <errno.h>

/*** defines ***/

#define CTRL_KEY(k) ((k) & 0x1f)


/*** data ***/

struct termios orig_termios;

/*** function declarations ***/

void die(const char* s);
void disableRawMode();
void enableRawMode();
char editorReadKey();
void editorRefreshScreen();
void editorDrawRows();
//void editorProcessKeypress();

/*** terminal ***/

void die(const char *s) {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    perror(s);
    exit(1);
  }

void disableRawMode(){
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode(){
    //printf("passei");
    if(tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
        die("tcgetattr");
    }
    //printf("passei");
    atexit(disableRawMode);
    struct termios raw = orig_termios;

    raw.c_iflag &= ~(ICRNL | IXON | BRKINT | INPCK | ISTRIP);
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

char editorReadKey(){
    int nread;
    char c;

    while((nread = read(STDIN_FILENO, &c, 1)) != 1){
        if(nread == -1 && errno != EAGAIN) die("read");
    }
    printf("%c", c);

    return c;
}



/*** output ***/

void editorDrawRows() {
    int y;
    for (y = 0; y < 24; y++) {
      write(STDOUT_FILENO, "~\r\n", 3);
    }
  }

  void editorRefreshScreen() {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    editorDrawRows();
    write(STDOUT_FILENO, "\x1b[H", 3);
  }

/*** input ***/

void editorProcessKeypress() {
    char c = editorReadKey();

    switch (c) {
      case CTRL_KEY('q'):
        write(STDOUT_FILENO, "\x1b[2J", 4);
        write(STDOUT_FILENO, "\x1b[H", 3);
        exit(0);
        break;
    }
  }

/*** init ***/

int main(){
    //printf("passei");
    write(STDOUT_FILENO, "passei", 6);
    fflush(stdout);
    enableRawMode();
    
    while(1){
        editorRefreshScreen();
        editorProcessKeypress();
    }
    
    return 0;
}
