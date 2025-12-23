import argparse
import requests

DARTBOARD_ID = 0xCAFE10

def post_json(url, json, verbose=True):
    try:
        rsp = requests.post(url=url, json=json)
        if verbose:
            print(f"Result [{rsp.status_code}]: {rsp.text}")
    except requests.exceptions.ConnectionError:
        print(f"ERROR: Cannot connect to {url}")

def new_game(url, game_name, players, options):
    print(f"[NEW GAME] name: {game_name}, options: {options}, players: {players}, base_url: {url}")

    game_int = -1
    if game_name == 'cricket':
        game_int = 0
    elif game_name == 'x01':
        game_int = 1

    data = {}
    data["game"] = game_int
    data["options"] = options
    data["players"] = players
    print(data)
    rsp = post_json(f"{url}new-game", json=data)

def back(url, board):
    print(f"[BACK] board: 0x{board:X}, base_url: {url}")

    data = {}
    data["board_id"] = board
    rsp = post_json(f"{url}back", json=data)

def next_player(url, board):
    print(f"[NEXT PLAYER] board: 0x{board:X}, base_url: {url}")

    data = {}
    data["board_id"] = board
    rsp = post_json(f"{url}next-player", json=data)

def new_dart(url, board, number, zone):
    print(f"[NEW DART] Number: {number}, zone: {zone}, board: 0x{board:X}, base_url: {url}")

    zone_int = -1
    if zone == 's':
        zone_int = 1
    elif zone == 'd':
        zone_int = 2
    elif zone == 't':
        zone_int = 3

    data = {}
    data["board_id"] = board
    data["num"] = number
    data["zone"] = zone_int

    rsp = post_json(f"{url}new-dart", json=data)

def main():
    parser = argparse.ArgumentParser(description="DartsOut! simulator")

    parser.add_argument("-u", "--url", type=str, default="http://localhost:8000/", help="Dartsout server URL")

    subparsers = parser.add_subparsers(dest="command", required=True, help="Command to execute")

    parser_new_game = subparsers.add_parser("new-game", help="Create a new game")
    parser_new_game.add_argument("-n", "--number", type=int, default=1, help="Number of games to create, for test purposes")
    parser_new_game.add_argument("-g", "--game", type=str, required=True, choices=["cricket", "x01"], help="Game type")
    parser_new_game.add_argument("-p", "--players", type=str, default=["P1", "P2"], nargs='+', help="List of players (player1_name player2_name ...)")
    parser_new_game.add_argument("-b", "--boards", type=int, nargs='+', help="List of players boards (player1_board player2_board ...)")
    parser_new_game.add_argument("-o", "--options", type=int, default=0, help="Game options (e.g., '0' for no options)")

    parser_next = subparsers.add_parser("next", help="Next player")
    parser_next.add_argument("-b", "--board", type=int, default=DARTBOARD_ID, help="Dartboard ID")

    parser_back = subparsers.add_parser("back", help="Back action")
    parser_back.add_argument("-b", "--board", type=int, default=DARTBOARD_ID, help="Dartboard ID")

    parser_finish = subparsers.add_parser("finish", help="Finish game")
    parser_finish.add_argument("-b", "--board", type=int, default=DARTBOARD_ID, help="Dartboard ID")

    parser_new_dart = subparsers.add_parser("new-dart", help="Throw a dart")
    parser_new_dart.add_argument("-b", "--board", type=int, default=DARTBOARD_ID, help="Dartboard ID")
    parser_new_dart.add_argument("-n", "--number", type=int, required=True, choices=range(0,20+1), help="Dart number hit")
    parser_new_dart.add_argument("-z", "--zone", type=str, required=True, choices=["t", "d", "s"], help="Dart zone hit")

    args = parser.parse_args()

    print("DartsOut! simulator")

    if args.command == "new-game":
        if not args.boards:
            args.boards = [DARTBOARD_ID] * len(args.players)
        if len(args.players) != len(args.boards):
            print(f"ERROR: players and boards arrays have not same length (players: {len(args.players)}, boards: {len(args.boards)})")
            return
        players = []
        for i, p in enumerate(args.players):
            players.append({"name": p, "board_id": args.boards[i]})
        options = args.options
        for i in range(args.number):
            new_game(args.url, args.game, players, options)
    elif args.command == "next":
        next_player(args.url, args.board)
    elif args.command == "back":
        back(args.url, args.board)
    elif args.command == "finish":
        new_dart(args.url, args.board, 20, 't')
        new_dart(args.url, args.board, 19, 't')
        new_dart(args.url, args.board, 18, 't')
        next_player(args.url, args.board)
        next_player(args.url, args.board)
        new_dart(args.url, args.board, 17, 't')
        new_dart(args.url, args.board, 16, 't')
        new_dart(args.url, args.board, 15, 't')
        next_player(args.url, args.board)
        next_player(args.url, args.board)
        new_dart(args.url, args.board, 0, 't')
    elif args.command == "new-dart":
        new_dart(args.url, args.board, args.number, args.zone)
    else:
        print("ERROR: Unknown command")


if __name__ == "__main__":
    main()
