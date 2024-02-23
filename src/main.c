#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "main.h"
#include "cricket.h"

/* Global variables ***********************************************************/

// mealy_t cricket_fsm;

int kbhit(void)
{
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if(ch != EOF) {
		ungetc(ch, stdin);
		return 1;
	}

	return 0;
}

/* Main function **************************************************************/

int main()
{
	// cricket_fsm_init(&cricket_fsm);
	printf("Start!\n");

	cricket_player_t players[MAX_PLAYERS];
	int i = 0;
	for (i = 0; i < MAX_PLAYERS; i++) {
		char* name = malloc(100);
		printf("Player %d name (enter to start): ", i+1);
		fgets(name, sizeof(name), stdin);
		name[strcspn(name, "\n")] = '\0';
		if (strlen(name) == 0) {
			free(name);
			if (i == 0) {
				printf("Minimum players: 1\n");
				i--;
				continue;
			}
			break;
		}
		players[i].name = name;
	}

	cricket_t cricket;
	cricket_new_game(&cricket, players, i, 200, 25);

	for (;;) {
		while (!kbhit()) {
			usleep(100000);
			// mealy_fire(&cricket_fsm);

		}
		char ch = getchar();
		printf("\n");
		dart_shot_t val;
		val.number = 1;
		val.multiplier = 3;
		if (ch == 'a') {
			val.number = 15;
		} else if (ch == 'b') {
			val.number = 16;
		} else if (ch == 'c') {
			val.number = 17;
		} else if (ch == 'd') {
			val.number = 18;
		} else if (ch == 'e') {
			val.number = 19;
		} else if (ch == 'f') {
			val.number = 20;
		} else if (ch == 'g') {
			val.number = 0;
		}
		cricket_new_dart(&cricket, &val);
		cricket.darts++;
		cricket_player_t* player = &cricket.players[cricket.current_player];
		printf("\n[Cricket] Round: %d/%d, player: %s, score: %d, darts: %d\n\n",
				cricket.round, cricket.max_rounds, player->name,
				player->game_score, cricket.darts);
		if (cricket.darts == MAX_DARTS) {
			cricket_next_player(&cricket);
		}
		cricket_player_t* winner_player = cricket_finish_game(&cricket);
		if (winner_player != NULL) {
			cricket_finish_game(&cricket);
			printf("Winner is %s with %d points\n", winner_player->name,
					winner_player->game_score);
			return 0;
		}
	}

	return 0;
}