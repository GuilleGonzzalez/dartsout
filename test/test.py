import os
import sys
import time
import signal
import atexit
import subprocess
from pathlib import Path
PROJECT_ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(PROJECT_ROOT))
from sim.api import Api

DARTSOUT_PATH = os.path.join(PROJECT_ROOT, "dartsout.out")

TEST_SERVER_URL = "http://localhost:8000"
TEST_BOARD_ID = 12345
CRICKET_ID = 0
X01_ID = 1

dartsout_process = None

def launch_dartsout():
    global dartsout_process
    # PIPE for logs, DEVNULL to ignore
    dartsout_process = subprocess.Popen([DARTSOUT_PATH])

def cleanup():
    global dartsout_process
    if dartsout_process and dartsout_process.poll() is None:
        dartsout_process.terminate() # SIGTERM
        try:
            dartsout_process.wait(timeout=3)
            print("Dartsout process terminated.")
        except subprocess.TimeoutExpired:
            dartsout_process.kill() # SIGKILL
            print("Dartsout process killed.")

def handle_sigint(sig, frame):
    cleanup()
    sys.exit(0)

def main():
    signal.signal(signal.SIGINT, handle_sigint)
    atexit.register(cleanup)

    launch_dartsout()
    time.sleep(1) # Give some time for the server to start

    api = Api(TEST_SERVER_URL, TEST_BOARD_ID)
    players = []
    players.append({"name": "P1", "board_id": TEST_BOARD_ID})
    players.append({"name": "P2", "board_id": TEST_BOARD_ID})
    api.new_game(CRICKET_ID, players, 0)

    api.send_dart(20, 3)
    api.send_dart(19, 3)
    api.send_dart(18, 3)
    api.next_player()
    api.next_player()
    api.send_dart(17, 3)
    api.send_dart(16, 3)
    api.send_dart(15, 3)
    api.next_player()
    api.next_player()
    api.send_dart(0, 2)
    api.send_dart(0, 1)

    cleanup()
    print("Done.")

if __name__ == "__main__":
    main()