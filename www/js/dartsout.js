let ws_url = "ws://localhost:8000/websocket"
// let ws_url = "https://dartsout.duckdns.org/websocket"

let socket = null;

let homeCanvas = document.getElementById("home");
let gameCanvas = document.getElementById("game");

let gameId = -1;

const MsgId = {
  GameStatus: 0,
  Cricket:    1,
  X01:        2,
  LastDart:   3,
  Winner:     4,
  GameId:     5,
}

const GameId = {
  Cricket: 0,
  X01:     1,
}

function sendWsMsg() {
  let msg = document.getElementById("wsMsg").value;
  socket.send(msg);
};

document.addEventListener('keydown', function(event) {
  switch(event.key) {
  case 'n':
    nextPlayer();
    break;
  case 't':
    newDart(0);
    break;
  case 'd':
    newDart(1);
    break;
  case 's':
    newDart(2);
    break;
  case 'b':
    back();
    break;
  }
});

window.onload = init();

function init() {
  if (window.location.pathname == "/") {
    homeCreateCanvas(homeCanvas);
  } else if (window.location.pathname == "/game.html") {
    const urlParams = new URLSearchParams(window.location.search);
    gameId = urlParams.get('id');
    if (gameId == null || gameId == "") {
      console.error("Invalid game ID, assigning 0");
      gameId = 0;
    }
    socket = new WebSocket(ws_url + `?id=${gameId}`);
    socket.onopen = () => socket.send(`${gameId};status`);
  } else {
    alert(`Invalid pathname (${window.location.pathname})`);
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
    // TODO: puede pasar, no es un err/warn
    console.warn("Invalid JSON");
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
        // TODO: aquí puede estar el problema de crear varios canvas. Solo debería crearse si:
        // se abre una sesión y hay una partida en juego (?)
        // Solucionado borrando el canvas antes de actualizarlo
        if (game_id == GameId.Cricket) {
          console.log(`Creating cricket canvas... (${game_n_players} players)`);
          cricketCreateCanvas(gameCanvas, game_n_players, options);
        } else if (game_id == GameId.X01) {
          x01CreateCanvas(gameCanvas, game_n_players, options);
        } else {
          console.error("Game not implemented!");
        }
        //TODO: llamar a sondsNewGame()
        // soundsNewGame();
        addHomeModal(gameCanvas);
      }
      //TODO: temp
      soundsNextPlayer();
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
      let game_id2 = json["game_id"];
      let winnerName = json["name"];
      updateTitle(`${winnerName} wins!`);
      if (game_id2 == GameId.Cricket) {
        let nPlayers = json["players"].length;
        let stats = []
        for (let i = 0; i < nPlayers; i++) {
          let p = json["players"][i]
          stats.push([p["name"], p["game_score"], p["mpr"]/100]);
        }
        console.log(stats);
        // TODO: modal add class modal-lg
        // TODO: highlight winner player
        addFinishModal(gameCanvas, 0, stats);
        launchModal("finish-modal");
      }
      break;
    case MsgId.GameId:
      gameId = json["game_id"];
      console.log("GameId", gameId);
      window.location=`game.html?id=${gameId}`;
      break;
    default:
      alert(`Error, invalid message id (${msgId})`);
  }
}

function addHomeModal(canvas) {
    let homeModal = createModal("home-modal");
    canvas.appendChild(homeModal);
    addTitleModal("home-modal", "Exit");
    let homeModalContent = document.createElement("div");
    let homeModalTxt = document.createElement("h5");
    homeModalTxt.innerHTML = "Do you want to finish game?";
    let homeModalTxt2 = document.createElement("p");
    homeModalTxt2.innerHTML = "Game will finish";
    homeModalContent.appendChild(homeModalTxt);
    homeModalContent.appendChild(homeModalTxt2);
    let yesBtn = createButton("Yes, finish game", "homeCb()");
    let noBtn = createButton("Go home", "goHomeCb()");
    homeModalContent.appendChild(yesBtn);
    homeModalContent.appendChild(noBtn);
    addContentModal("home-modal", homeModalContent);
}

function addFinishModal(canvas, winnerPlayerId, mprs) {
    let finishModal = createModal("finish-modal");
    canvas.appendChild(finishModal);
    addTitleModal("finish-modal", "Game finished");
    let finishModalContent = document.createElement("div");
    let finishModalTxt = document.createElement("h5");
    // TODO: define winner WS message
    finishModalTxt.innerHTML = `TODO wins!`;
    finishModalContent.appendChild(finishModalTxt);
    // TODO: repeat game implementation
    let repeatBtn = createButton("Play Again", "homeCb()");
    let homeBtn = createButton("Home", "homeCb()");
    finishModalContent.appendChild(homeBtn);
    finishModalContent.appendChild(repeatBtn);
    addContentModal("finish-modal", finishModalContent);
    // TODO: for now, only MPR. More stats in future
    let tableHeader = ["Player", "Score", "MPR"];
    let table = createTable('finish-table', Object.keys(mprs).length,
        tableHeader.length);
    finishModalContent.appendChild(table);
    updateTable('finish-table', tableHeader, mprs);
    // TODO: highlight winner player
    // highlightTableRow('finish-table', winnerPlayerId);
}
