let ws_url = "ws://localhost:8000/websocket"
// let ws_url = "ws://0.0.0.0:8000/websocket"
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
  } else if (window.location.href.endsWith("game.html")) {
    socket.onopen = () => socket.send("status");
  }
}

socket.onmessage = function(event) {
  console.log("Message received:", event.data);
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
      console.log("Status message!");
      let running = json["running"];
      let game_id = json["game_id"];
      let options = json["options"];
      let players = json["players"];
      let game_n_players = json["n_players"];
      if (running) {
        if (game_id == GameId.Cricket) {
          console.log(`Creating cricket canvas... (${game_n_players} players)`);
          cricketCreateCanvas(gameCanvas, game_n_players, options);
        } else if (game_id == GameId.X01) {
          x01CreateCanvas(gameCanvas, game_n_players, options);
        } else {
          console.error("Game not implemented!");
        }
        addHomeModal(gameCanvas);
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

function addHomeModal(canvas) {
    let homeModal = createModal("home-modal");
    canvas.appendChild(homeModal);
    addTitleModal("home-modal", "Exit");
    let homeModalContent = document.createElement("div");
    let homeModalTxt = document.createElement("h5");
    homeModalTxt.innerHTML = "Are you sure you want to exit?";
    let homeModalTxt2 = document.createElement("p");
    homeModalTxt2.innerHTML = "Game will finish";
    homeModalContent.appendChild(homeModalTxt);
    homeModalContent.appendChild(homeModalTxt2);
    let yesBtn = createButton("Yes", "homeCb()");
    homeModalContent.appendChild(yesBtn);
    addContentModal("home-modal", homeModalContent);
}