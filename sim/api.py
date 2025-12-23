import requests

class Api:
    def __init__(self, server_url, board_id):
        self.server_url = server_url.rstrip("/")
        self.board_id = board_id

    def update_board_id(self, board_id):
        self.board_id = board_id

    def send_dart(self, num, zone):
        payload = {
            "board_id": self.board_id,
            "num": num,
            "zone": zone
        }
        print(f"[SEND DART] payload = {payload}")
        requests.post(f"{self.server_url}/new-dart", json=payload, timeout=1)

    def back(self):
        payload = {
            "board_id": self.board_id,
        }
        requests.get(f"{self.server_url}/back", json=payload, timeout=1)
        print(f"[BACK] payload = {payload}")

    def next_player(self):
        payload = {
            "board_id": self.board_id,
        }
        requests.get(f"{self.server_url}/next-player", json=payload, timeout=1)
        print(f"[NEXT PLAYER] payload = {payload}")
