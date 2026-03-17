#include <ncurses.h>
#include <unistd.h>

/**

  $$$$    $$$$$$  $$$$  $$$$$$  $$$$$$
    $$    $$  $$  $$$$      $$      $$
    $$    $$  $$        $$$$$$      $$
    $$    $$  $$  $$$$  $$          $$
  $$$$$$  $$$$$$  $$$$  $$$$$$      $$ 78

*/

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
int time_data[] = { 0, 0, 0, 0 };

// prints a digit
void print_n(WINDOW* win, char *n[], int position_x) {
  int y = 2;
  for (int i = 0; i < 5; i++) {
    mvwprintw(win, y++, position_x, "%s", (char*)n[i]);
  }
}

// print dots between digits
void print_dots(WINDOW* win) {
  int x = 20;
  int y = 2;
  for (int i = 0; i < 5; i++) {
    mvwprintw(win, y++, x, "%s", ds[i]);
  }
}

void print_timer(WINDOW* win) {
  int positions_x[] = { 4, 12, 26, 34 };

  for (int i = 0; i < 4; i++) {
    print_n(win, digits[time_data[i]], positions_x[i]);
  }
}

// starts timer
void run_timer(WINDOW* win, int minutes) {
  int key = 'a';
  int deci = 0;

  while (key != 'p') {
    key = wgetch(win);

    if (deci == 10) {
      deci = 0;
      time_data[3]++;
    }
    if (time_data[3] == 10) {
      time_data[3] = 0;
      time_data[2]++;
    }
    if (time_data[2] == 6) {
      time_data[2] = 0;
      time_data[1]++;
    }
    if (time_data[1] == 10) {
      time_data[1] = 0;
      time_data[0]++;
    }

    //mvwprintw(win, 1, 0, "%d = %d%d:%d%d", deci, time_data[0], time_data[1], time_data[2], time_data[3]);
    print_timer(win);

    wrefresh(win);
    deci++;
    if (time_data[0] == 2 && time_data[1] == 5) {
      time_data[0] = time_data[1] = time_data[2] = time_data[3] = 0;
      break;
    }
  }
}

int main() {
  WINDOW *time;

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
  print_n(time, n0, 4);
  print_n(time, n0, 12);
  print_dots(time);
  print_n(time, n0, 26);
  print_n(time, n0, 34);

  while (true) {
    curs_set(0);
    int key = wgetch(time);

    switch (key) {
      case 'q':
        return 0;

      case 's':
        run_timer(time, 25);
        break;
    }
  }
  
  endwin();
  return 0;
}
