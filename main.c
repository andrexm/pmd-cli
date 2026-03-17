#include <ncurses.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <unistd.h>

/**

  $$$$    $$$$$$  $$$$  $$$$$$  $$$$$$
    $$    $$  $$  $$$$      $$      $$
    $$    $$  $$        $$$$$$      $$
    $$    $$  $$  $$$$  $$          $$
  $$$$$$  $$$$$$  $$$$  $$$$$$      $$

*/

WINDOW *time;
WINDOW *helpers;

char *n0[] = { "$$$$$$", "$$  $$", "$$  $$", "$$  $$", "$$$$$$" }; 
char *n1[] = { "$$$$  ", "  $$  ", "  $$  ", "  $$  ", "$$$$$$" }; 
char *n2[] = { "$$$$$$", "    $$", "$$$$$$", "$$    ", "$$$$$$" }; 
char *n3[] = { "$$$$$$", "    $$", "$$$$$$", "    $$", "$$$$$$" }; 
char *n4[] = { "$$  $$", "$$  $$", "$$$$$$", "    $$", "    $$" }; 
char *n5[] = { "$$$$$$", "$$    ", "$$$$$$", "    $$", "$$$$$$" }; 
char *n6[] = { "$$$$$$", "$$    ", "$$$$$$", "$$  $$", "$$$$$$" }; 
char *n7[] = { "$$$$$$", "    $$", "    $$", "    $$", "    $$" }; 
char *n8[] = { "$$$$$$", "$$  $$", "$$$$$$", "$$  $$", "$$$$$$" }; 
char *n9[] = { "$$$$$$", "$$  $$", "$$$$$$", "    $$", "$$$$$$" }; 
char *ds[] = { "$$$$", "$$$$", "    ", "$$$$", "$$$$" };

char **digits[] = { n0, n1, n2, n3, n4, n5, n6, n7, n8, n9 };
int time_data[] = { 2, 5, 0, 0 };
int intervals[] = { 25, 5, 25, 5, 25, 5, 25, 15 }; // time in minutes
int active_i = 0;

// prints a digit
void print_n(char *n[], int position_x) {
  int y = 2;
  for (int i = 0; i < 5; i++) {
    mvwprintw(time, y++, position_x, "%s", (char*)n[i]);
  }
}

// print dots between digits
void print_dots() {
  int x = 20;
  int y = 2;
  for (int i = 0; i < 5; i++) {
    mvwprintw(time, y++, x, "%s", ds[i]);
  }
}

// print all digits on the screen
void print_timer() {
  int positions_x[] = { 4, 12, 26, 34 };

  for (int i = 0; i < 4; i++) {
    print_n(digits[time_data[i]], positions_x[i]);
  }
  wrefresh(time);
}

void print_before_start_text() {
  char *helper_lines[6];
  helper_lines[0] = "[s] Start Timer / Resume";
  helper_lines[1] = "[p] Pause";
  helper_lines[2] = "[r] Restart Timer";
  helper_lines[3] = "[n] Next Interval";
  helper_lines[4] = "[t] Test Bell Sound"; // verify file exists and play
  helper_lines[5] = "[q] Quit";

  for (int y = 0; y < 6; y++) {
    mvwprintw(helpers, y, 0, "%s", helper_lines[y]);
  }
  wrefresh(time);
  wrefresh(helpers);
}

// play a sound after finishing timer
void triiimm() {
  system("aplay -q ~/.config/pmd-cli/sounds/magiaz-campainha-331260.wav");
}

// set up next time minutes
void next_phase() {
  active_i++;
  if (active_i == 8) active_i = 0;

  time_data[0] = intervals[active_i] / 10;
  time_data[1] = intervals[active_i] % 10;
  time_data[2] = 0;
  time_data[3] = 0;

  print_timer();
}

// prepare timer to start again current session
void restart_timer() {
  active_i = active_i - 1;
  if (active_i == -1) {
    active_i = 3;
  }

  next_phase();
}

// starts timer
void run_timer(int minutes) {
  int key = 'a';
  int deci = 0;

  while (key != 'p') {
    key = wgetch(time);

    if (deci == 10) {
      deci = 0;
      time_data[3]--;
    }
    if (time_data[3] == -1) {
      time_data[3] = 9;
      time_data[2]--;
    }
    if (time_data[2] == -1) {
      time_data[2] = 5;
      time_data[1]--;
    }
    if (time_data[1] == -1) {
      time_data[1] = 9;
      time_data[0]--;
    }

    print_timer();
    wrefresh(time);
    deci++;

    // finish when everything is zero
    if (time_data[0] == 0 && time_data[1] == 0 && time_data[2] == 0 && time_data[3] == 0) {
      triiimm();
      next_phase();
      break;
    }
  }
}

int main() {
  int start_y, start_x, max_y, max_x;
  int width = 44; // 4 spaces at each x
  int height = 9; // 3 space at each y

  initscr();
  noecho();
  raw();
  cbreak();
  halfdelay(1);

  getmaxyx(stdscr, max_y, max_x);
  start_y = (max_y - height) / 2 - 4;
  start_x = (max_x - width) / 2;

  time = newwin(height, width, start_y, start_x);
  box(time, 0, 0);

  // on start, print 25:00
  print_n(n2, 4);
  print_n(n5, 12);
  print_dots();
  print_n(n0, 26);
  print_n(n0, 34);

  helpers = newwin(6, width - 6, start_y + height + 1, start_x + 9);
  print_before_start_text();
  
  wrefresh(time);

  while (true) {
    curs_set(0);
    int key = wgetch(helpers);

    wrefresh(helpers);
    switch (key) {
      case 'q':
        return 0;

      case 's':
        run_timer(25);
        break;

      case 't':
        triiimm();
        break;

      case 'n':
        next_phase();
        break;

      case 'r':
        restart_timer();
        break;
    }
  }
  
  endwin();
  return 0;
}
