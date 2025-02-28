#ifndef EDITOR_H
#define EDITOR_H

/*** includes ***/

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <errno.h>

/*** defines ***/

#define CTRL_KEY(k) ((k) & 0x1f)
#define KILO_VERSION "0.0.1"
#define KILO_TAB_STOP 8
#define KILO_QUIT_TIMES 3
#define HLDB_ENTRIES (sizeof(HLDB) / sizeof(HLDB[0]))

/*** data ***/

struct editorSyntax {
  char* filetype;
  char **filematch;
  char **keywords;
  char *singleline_comment_start;
  char *multiline_comment_start;
  char *multiline_comment_end;
  int flags;
};

typedef struct erow {
  int idx;
  int size;
  int rsize;
  char *chars;
  char* render;
  unsigned char* hl;
  int hl_open_comment;
} erow;

struct editorConfig {
  int cx, cy;
  int rx;
  int screen_rows;
  int screen_cols;
  int coloff;
  int rowoff;
  struct termios orig_termios;
  char* filename;
  int dirty;
  char status_msg[80];
  time_t status_msg_time;
  struct editorSyntax *syntax;
  int numrows;
  erow *row;
};

/*** append buffer ***/

struct abuf {
  char* b;
  int len;
};

#define ABUF_INIT {NULL,0}

/*** enums ***/

enum editorKey {
  BACKSPACE = 127,
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  DEL_KEY,
  HOME_KEY,
  END_KEY,
  PAGE_UP,
  PAGE_DOWN
};

enum editorHighlight {
  HL_NORMAL = 0,
  HL_NUMBER,
  HL_MATCH,
  HL_KEYWORD1,
  HL_KEYWORD2,
  HL_STRING,
  HL_COMMENT,
  HL_MLCOMMENT
};

#define HL_HIGHLIGHT_NUMBERS (1<<0)
#define HL_HIGHLIGHT_STRINGS (1<<1)

/*** function declarations ***/

void die(const char* s);
void disableRawMode();
void enableRawMode();
int editorReadKey();
void editorRefreshScreen();
void editorDrawRows(struct abuf *ab);
void editorDrawStatusBar(struct abuf *ab);
void editorDrawMessageBar(struct abuf *ab);
void editorSetStatusMessage(const char* fmt, ...);
void editorProcessKeypress();
void editorMoveCursor(int key);
int getCursorPosition(int *rows, int *cols);
int getWindowSize(int* rows, int* cols);
void editorInsertRow(int at, char *s, size_t len);
void editorUpdateRow(erow *row);
void editorRowInsertChar(erow *row, int at, int c);
void editorInsertChar(int c);
void editorInsertNewline();
void editorDelChar();
char *editorRowsToString(int *buflen);
void editorOpen(char *filename);
void editorSave();
void abAppend(struct abuf *ab, const char *s, int len);
void abFree(struct abuf *ab);
void editorScroll();
void initEditor();
void editorFreeRow(erow *row);
void editorDelRow(int at);
void editorRowAppendString(erow *row, char *s, size_t len);
int editorRowCxToRx(erow *row, int cx);
int editorRowRxToCx(erow *row, int rx);
char* editorPrompt(char* prompt, void (*callback)(char*, int));
int is_separator(int c);
void editorUpdateSyntax(erow *row);
int editorSyntaxToColor(int hl);
void editorFindCallback(char *query, int key);
void editorFind();

#endif // EDITOR_H