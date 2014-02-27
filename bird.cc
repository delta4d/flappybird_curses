#include <time.h>
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>

#define REFRESH_TIME 70000

typedef struct {
	int x, y, d;

	void build() {
		int gaplow = (int)(LINES * 0.3);
		int gaphigh = (int)(LINES * 0.6);
		d = (int)(rand() % (gaphigh - gaplow)) + gaplow;
		x = (int)(COLS * 0.8);
		y = (int)(rand() % (LINES - d));
	}

	void draw() {
		for (int i=0; i<LINES; ++i) {
			if (i < y || i > y + d) {
				mvaddch(i, x, '|');
				mvaddch(i, x+5, '|');
			}
			if (i == y || i == y + d) {
				mvprintw(i, x, "------");
			}
		}
	}
} Wall;

typedef struct {
	int x, y;

	void born() {
		x = COLS >> 2;
		y = LINES >> 1;
	}

	void draw() {
		mvprintw(y, x, "=('>");
	}

	// -2: still safely crossing the wall
	// -1: still on the way to wall
	// 0: unlucky bird
	// 1: beyond the wall
	int hit(const Wall &wall) {
		if (x + 3 < wall.x) {
			return -1;
		}
		if (x > wall.x) {
			return 1;
		}
		for (int i=0; i<4; ++i) {
			if (x + i < wall.x || x + i > wall.x) {
				continue;
			}
			if (y < wall.y || y > wall.y + wall.d) {
				return 0;
			}
		}
		return -2;
	}
} Bird;

int main() {
	bool alive = true;
	int score = 0, key;
	Bird bird;
	Wall wall;

	srand((int)time(NULL));
	initscr();
	curs_set(0);
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	timeout(1);

	bird.born();
	wall.build();

	while (alive) {
		clear();
		mvprintw(0, 0, "score: %d", score);
		bird.draw();
		wall.draw();

		int hit_res = bird.hit(wall);
		--wall.x;
		if (hit_res > 0) {
			++score;
			wall.build();
		} else if (hit_res == 0) {
			alive = false;
		}
		key = getch();
		if (key == KEY_UP) {
			bird.y -= 4;
		} else {
			bird.y += 1;
		}
		if (bird.y >= LINES || bird.y < 0) {
			alive = false;
		}
		usleep(REFRESH_TIME);
		refresh();
	}
	
	clear();
	mvprintw(LINES>>1, (COLS>>1)-8, "Game Over, Score: %d", score);
	mvprintw((LINES>>1)+1, (COLS>>1)-8, "Press any key to quit...");
	refresh();
	timeout(-1);
	getch();

	endwin();

	return 0;
}
