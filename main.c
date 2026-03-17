#include <ncurses.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <unistd.h>

/**

  $$$$    $$$$$$  $$$$  $$$$$$  $$$$$$
    $$    $$  $$  $$$$      $$      $$
    $$    $$  $$        $$$$$$      $$
    $$    $$  $$  $$$$  $$          $$
  $$$$$$  $$$$$$  $$$$  $$$$$$      $$ 78

*/

WINDOW *time;

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
int time_data[] = { 0, 0, 1, 0 };
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

  print_timer();
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
  start_y = (max_y - height) / 2;
  start_x = (max_x - width) / 2;

  time = newwin(height, width, start_y, start_x);
  box(time, 0, 0);

  // on start, print 25:00
  print_n(n2, 4);
  print_n(n5, 12);
  print_dots();
  print_n(n0, 26);
  print_n(n0, 34);

  while (true) {
    curs_set(0);
    int key = wgetch(time);

    switch (key) {
      case 'q':
        return 0;

      case 's':
        run_timer(25);
        break;
    }
  }
  
  endwin();
  return 0;
}
