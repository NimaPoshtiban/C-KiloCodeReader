#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>

/***    data    ***/
struct termios orig_termios;

/*** terminal ***/

// prints an error message and exits the program.
void die(const char *s)
{
  perror(s);
  exit(1);
}

// disables RawMode
void disableRawMode(void)
{
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");
}

// enabling raw mode in terminal
void enableRawMode(void)
{
   if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");

  atexit(disableRawMode); // runs at the exit time

  struct termios raw = orig_termios;

  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON); // Disable Ctrl-S and Ctrl-Q && Fix Ctrl-M(carriage return)
  raw.c_oflag &= ~(OPOST);                                  // Turn off all output processing
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG); // reading input byte by byte

  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}


int main(void)
{
  enableRawMode();

  while (1)
  {
    char c = '\0';
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read");
    if (iscntrl(c))
    {
      printf("%d\r\n", c);
    }
    else
    {
      printf("%d ('%c')\r\n", c, c);
    }
    if(c=='q') //  exit if the user pressed q
    {
      break;
    }
  } 
  return 0;
}