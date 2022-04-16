#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

struct termios orig_termios;

// disables RawMode
void disableRawMode(void)
{
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

// enabling raw mode in terminal
void enableRawMode(void)
{
  tcgetattr(STDIN_FILENO, &orig_termios);

  atexit(disableRawMode); // runs at the exit time

  struct termios raw = orig_termios;

  raw.c_lflag &= ~(ECHO | ICANON); // reading input byte by byte

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(void)
{
  enableRawMode();

  char c;

  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q')
    ; //  exit if the user pressed q
  return 0;
}