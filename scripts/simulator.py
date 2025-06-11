import argparse
import requests

DARTBOARD_ID = 0xCAFE10

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
    rsp = requests.post(f"{url}new-game", json=data)

    print(f"Result [{rsp.status_code}]: {rsp.text}")

def back(url, board):
    print(f"[BACK] board: 0x{board:X}, base_url: {url}")

    data = {}
    data["board_id"] = board
    rsp = requests.post(f"{url}back", json=data)

    print(f"Result [{rsp.status_code}]: {rsp.text}")

def next_player(url, board):
    print(f"[NEXT PLAYER] board: 0x{board:X}, base_url: {url}")

    data = {}
    data["board_id"] = board
    rsp = requests.post(f"{url}next-player", json=data)

    print(f"Result [{rsp.status_code}]: {rsp.text}")

def new_dart(url, board, number, zone):
    print(f"[NEW DART] Number: {number}, zone: {zone}, board: 0x{board:X}, base_url: {url}")

    zone_int = -1
    if zone == 's':
        zone_int = 2
    elif zone == 'd':
        zone_int = 1
    elif zone == 't':
        zone_int = 0

    data = {}
    data["board_id"] = board
    data["num"] = number
    data["zone"] = zone_int

    rsp = requests.post(f"{url}new-dart", json=data)

    print(f"Result [{rsp.status_code}]: {rsp.text}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    #TODO: cambiar argparse. python3 sumulator next; python3 sumulator back; python3 sumulator dart -n20 -zt
    parser.add_argument("--new", type=str, choices=["cricket", "x01"], help="New game")
    parser.add_argument("--next", action="store_true", help="Next player")
    parser.add_argument("--back", action="store_true", help="Back action")
    parser.add_argument("-u", "--url", type=str, default="http://localhost:8000/", help="Dartsout server URL")
    parser.add_argument("-b", "--board", type=int, default=DARTBOARD_ID, help="Dartboard ID")
    parser.add_argument("-n", "--number", type=int, choices=range(0,20+1), help="Dart number hit")
    parser.add_argument("-z", "--zone", type=str, choices=["t", "d", "s"], help="Dart zone hit")
    args = parser.parse_args()

    print("DartsOut! simulator")

    if args.new:
        players = []
        player = {"name": "Jugador 1", "board_id": args.board}
        players.append(player)
        new_game(args.url, args.new, players, 0)
        exit(0)
    elif args.next:
        next_player(args.url, args.board)
        exit(0)
    elif args.back:
        back(args.url, args.board)
        exit(0)

    new_dart(args.url, args.board, args.number, args.zone)

