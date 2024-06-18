// let ws_url = "ws://localhost:8000/websocket"
let ws_url = "ws://0.0.0.0:8000/websocket"
// let ws_url = "ws://192.168.0.39:8000/websocket"

let socket = new WebSocket(ws_url);

let homeCanvas = document.getElementById("home");
let gameCanvas = document.getElementById("game");

const MsgId = {
  GameStatus: 0,
  Cricket:    1,
  X01:        2,
  LastDart:   3,
  Winner:     4,
}

const GameId = {
  Cricket: 0,
  X01:     1,
}

function sendWsMsg() {
  let msg = document.getElementById("wsMsg").value;
  socket.send(msg);
};

window.onload = init();

function init() {
  if (window.location.pathname == "/") {
    homeCreateCanvas(homeCanvas);
  }
  getStatus();
}

function getStatus() {
  console.log(window.location.href);
  if (window.location.href.endsWith("game.html")) {
    const game = 0;
    json = JSON.stringify({game});
    api_post("/status", json);
  }
}

socket.onmessage = function(event) {
  let incomingMessage = event.data;
  proccessMessage(incomingMessage);
};

socket.onclose = function(event) {
  console.log(`Closed ${event.code}`);
}

function proccessMessage(message) {
  let messageElem = document.createElement('div');
  messageElem.textContent = "MSG rvc: " + message;
  document.getElementById('messages').prepend(messageElem);
  let json;
  try {
    json = JSON.parse(message);
  } catch {
    console.error("Invalid JSON");
    return;
  }
  let msgId = json["msg_id"];
  switch (msgId) {
    case MsgId.GameStatus:
      let running = json["running"];
      let game_id = json["game_id"];
      let options = json["options"];
      let players = json["players"];
      let game_n_players = json["n_players"];
      if (running) {
        if (game_id == GameId.Cricket) {
          cricketCreateCanvas(gameCanvas, game_n_players);
        } else if (game_id == GameId.X01) {
          x01CreateCanvas(gameCanvas, game_n_players, 301, options);
        } else {
          console.error("Game not implemented!");
        }
      } else {
        homeAddPlayer(json);
      }
      break;
    case MsgId.Cricket:
      cricketProccess(json);
      break;
      case MsgId.X01:
      x01Proccess(json);
      break;
    case MsgId.LastDart:
      let last_dart_valid = json["valid"];
      let last_dart_zone = json["zone"];
      soundsNewDart(last_dart_valid, last_dart_zone);
      break;
    case MsgId.Winner:
      soundsWinner();
      updateTitle(`${json["name"]} wins!`);
      break;
    case MsgId.Player:
      break;
    default:
      alert("Error");
  }
}

function newCricketGame(options) {
  window.location="game.html"; //TODO: not necessary?
  new_game(GameId.Cricket, options);
}

function newX01Game(options) {
  window.location="game.html"; //TODO: not necessary?
  new_game(GameId.X01, options);
}