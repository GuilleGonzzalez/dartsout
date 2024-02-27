#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "main.h"
#include "cricket.h"
#include "api.h"
#include "db.h"
#include "websocket.h"

/* Global variables ***********************************************************/

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
	printf("Start!\n");

	websocket_init();
	api_init();

	// db_init();
	// db_players_new(2, "Alex"); //TODO: pasarle un tipo player_t
	// db_players_get();
	// db_cricket_new(1, 3.4);
	// db_cricket_get();
	// api_init();

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
	int max_points = 200;
	int max_rounds = 3;
	cricket_new_game(&cricket, players, i, max_points, max_rounds);

	printf("[Cricket] New game: %d players, %d rounds, max %d points\n", i,
			max_rounds, max_points);
	for (int j = 0; j < i; j++) {
		printf("    %s will play in pos %d\n", cricket.players[j].name, j+1);
	}

	printf("[Cricket] %s's turn\n", cricket.players[0].name);

	for (;;) {
		while (!kbhit()) {
			usleep(1000);
			api_fire();
		}
		char ch = getchar();
		printf("\n");
		dart_shot_t val;
		val.number = 1;
		val.multiplier = 2;
		if (ch == '5') {
			val.number = 15;
		} else if (ch == '6') {
			val.number = 16;
		} else if (ch == '7') {
			val.number = 17;
		} else if (ch == '8') {
			val.number = 18;
		} else if (ch == '9') {
			val.number = 19;
		} else if (ch == '0') {
			val.number = 20;
		} else if (ch == '1') {
			val.number = 0;
		}
		cricket_new_dart(&cricket, &val);
		cricket.darts++;
		cricket_player_t* winner_player = cricket_finish_game(&cricket);
		if (winner_player != NULL) {
			cricket_finish_game(&cricket);
			printf("Winner is %s with %d points\n", winner_player->name,
					winner_player->game_score);
			return 0;
		}
		cricket_player_t* player = &cricket.players[cricket.current_player];
		printf("[Cricket] Round: %d/%d, player: %s, score: %d, round score: %d, darts: %d\n\n",
				cricket.round, cricket.max_rounds, player->name,
				player->game_score, player->round_score, cricket.darts);
		if (cricket.darts == MAX_DARTS) {
			cricket_next_player(&cricket);
			printf("[Cricket] %s's turn\n",
					cricket.players[cricket.current_player].name);
		}
	}

	return 0;
}