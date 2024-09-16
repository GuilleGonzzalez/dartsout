
function homeCreateCanvas(homeCanvas) {

  // Modals
  let playerModal = createModal("player-modal");
  homeCanvas.appendChild(playerModal);
  let playerModalContent = document.createElement("div");
  // let playerForm = createdPlayerForm("playerCb()");
  // playerModalContent.appendChild(playerForm);
  addTitleModal("player-modal", "Add player");
  addContentModal("player-modal", playerModalContent);

  let cricketModal = createModal("cricket-modal");
  homeCanvas.appendChild(cricketModal);
  let cricketModalContent = document.createElement("div");
  let cricketRadioGroup = createRadio("cricket-radio", ["Normal", "No score", "Cut throat", "Wild", "Crazy", "Wild and crazy"]);
  cricketModalContent.appendChild(cricketRadioGroup);
  let cricketPlayBtn = createButton("Play!", "CricketGameCb()");
  cricketModalContent.appendChild(cricketPlayBtn);
  addTitleModal("cricket-modal", "Cricket");
  addContentModal("cricket-modal", cricketModalContent);

  let x01Modal = createModal("x01-modal");
  homeCanvas.appendChild(x01Modal);
  let x01ModalContent = document.createElement("div");
  let x01RadioGroup = createRadio("x01-radio", ["301", "501", "701", "901", "1001"]);
  x01ModalContent.appendChild(x01RadioGroup);
  let spacer1 = createSpacer(30);
  x01ModalContent.appendChild(spacer1);
  let x01CheckGroup = createCheck("x01-check", ["Double In", "Double Out"]);
  x01ModalContent.appendChild(x01CheckGroup);
  let playBtn = createButton("Play!", "x01GameCb()");
  x01ModalContent.appendChild(playBtn);
  addTitleModal("x01-modal", "X01");
  addContentModal("x01-modal", x01ModalContent);

  // Title
  let title = createHeader("Dartsout!"); // TODO: no imgs
  homeCanvas.appendChild(title);
  let spacer = createSpacer(30);
  homeCanvas.appendChild(spacer);

  // Buttons
  let newPlayerBtn = createButton("Add player", "new_player()");
  homeCanvas.appendChild(newPlayerBtn);
  // let newPlayerBtn2 = createButton("Add player 2", 'launchModal("player-modal")');
  // homeCanvas.appendChild(newPlayerBtn2);
  let newCricketOptsBtn = createButton("Cricket",
      'launchModal("cricket-modal")');
  homeCanvas.appendChild(newCricketOptsBtn);
  let newX01OptsBtn = createButton("X01",
      'launchModal("x01-modal")');
  homeCanvas.appendChild(newX01OptsBtn);

  // Players-table
  let spacer2 = createSpacer(30);
  homeCanvas.appendChild(spacer2);
  let table = createTable("players-table", 8, 3);
  homeCanvas.appendChild(table);
  updateTable("players-table", ["#", "Name", "MPR"], false);
  let spacer3 = createSpacer(30);
  homeCanvas.appendChild(spacer3);

  let gameId = createInputText("gameId");
  let joinBtn = createButton("Join", "joinGame()");
  homeCanvas.appendChild(gameId);
  homeCanvas.appendChild(joinBtn);
}

// function playerCb(alert) {
//   let playerName = document.getElementById("playerName").value;
//   if (playerName == "") {
//     return;
//   }
//   new_player2(playerName);
// }

function CricketGameCb() {
  let options = 0;

  let selectedRadio = document.querySelector('input[name="cricket-radio"]:checked');
  if (!selectedRadio) {
    console.error("No radio selected");
    return;
  }
  let tmp = selectedRadio.id.split("_");
  let mode = tmp[tmp.length - 1];
  options |= (1 << mode);

  // OPTIONS
  // |                             MODE                             |
  // |   8  |   7  |   6  |   5  |   4  |   3  |   2  |   1  |   0  |
  // |  RES |  RES |  RES |  w&c | craz | wild | cthr | nsco | norm |
  

  console.log("Cricket Options: ", options);

  newCricketGame(options);
}

function x01GameCb() {
  let options = 0;

  let selectedRadio = document.querySelector('input[name="x01-radio"]:checked');
  if (!selectedRadio) {
    console.error("No radio selected");
    return;
  }
  let tmp = selectedRadio.id.split("_");
  let score = tmp[tmp.length - 1];
  options |= ((1 << score) << 2);

  // OPTIONS
  // |                       SCORE                    | DOUBLE OPTS |
  // |   8  |   7  |   6  |   5  |   4  |   3  |   2  |   1  |   0  |
  // |  RES |  RES | 1001 |  901 |  701 |  501 |  301 | DOUT |  DIN |
  
  let selectedChecks = document.querySelectorAll('input[name="x01-check"]:checked');
  for (let i = 0; i < selectedChecks.length; i++) {
    let tmp = selectedChecks[i].id.split("_");
    let opt = tmp[tmp.length - 1];
    console.log("OPT: ", opt);
    options |= (1 << opt); // DIN -> opt = 0; DOUT -> opt = 1;
  }

  console.log("X01 Options: ", options);

  newX01Game(options);
}

function homeAddPlayer(json) {
  let players = json["players"];
  let data = [];
  for (let i = 0; i < players.length; i++) {
    data.push([i+1, players[i]["name"], "--"]);
  }
  updateTable("players-table", false, data);
}

function joinGame() {
  let gameId = document.getElementById('gameId');
  if (!gameId || gameId.value == "") {
    console.error("No game ID");
    return;
  }
  console.log(gameId.value);

  // joinGame(gameId.value);
}