
function homeCreateCanvas(homeCanvas) {

  // Modals
  let cricketModal = createModal("cricket-modal");
  homeCanvas.appendChild(cricketModal);
  let cricketModalContent = document.createElement("div");
  let cricketRadioGroup = createRadio("cricket-radio", ["Classic", "Cricket wild", "Cricket crazy", "Cricket wild and crazy"]);
  cricketModalContent.appendChild(cricketRadioGroup);
  let cricketPlayBtn = createButton("Play!", "newCricketGame()");
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
  let newCricketBtn = createButton("New cricket game", "newCricketGame()");
  homeCanvas.appendChild(newCricketBtn);
  let cricketWildOptions = (1 << 2);
  let newCricketWildBtn = createButton("New cricket wild game",
      `newCricketGame(${cricketWildOptions})`);
  homeCanvas.appendChild(newCricketWildBtn);
  let new301Btn = createButton("New 301 game",  "newX01Game()");
  homeCanvas.appendChild(new301Btn);
  let x01DoubleOutOptions = (1 << 1);
  let x01DoubleInOptions = (1 << 0);
  let new301DOBtn = createButton("New 301 game DInOut",
  `newX01Game(${x01DoubleOutOptions | x01DoubleInOptions})`);
  homeCanvas.appendChild(new301DOBtn);

  let newCricketOptsBtn = createButton("New cricket OPTS",
      'launchModal("cricket-modal")');
  homeCanvas.appendChild(newCricketOptsBtn);
  let newX01OptsBtn = createButton("New X01 OPTS",
      'launchModal("x01-modal")');
  homeCanvas.appendChild(newX01OptsBtn);

  // Players-tavble
  let spacer2 = createSpacer(30);
  homeCanvas.appendChild(spacer2);
  let table = createTable("players-table", 8, 3);
  homeCanvas.appendChild(table);
  updateTable("players-table", ["#", "Name", "MPR"], false);
  let spacer3 = createSpacer(30);
  homeCanvas.appendChild(spacer3);

  // let modalContent = document.createElement("div");
  // let text = document.createElement("h3");
  // text.innerHTML = "Are you sure you want to finish this game?";
  // let btnYes = createButton("Yes", "home()");
  // let btnNo = createButton("No", 'hideModal("exit-modal")');
  // modalContent.appendChild(text);
  // modalContent.appendChild(btnYes);
  // modalContent.appendChild(btnNo);
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

  let selectedChecks = document.querySelectorAll('input[name="x01-check"]:checked');
  for (let i = 0; i < selectedChecks.length; i++) {
    selectedChecks[i]
    let tmp = selectedChecks[i].id.split("_");
    let opt = tmp[tmp.length - 1];
    console.log("OPT: ", opt);
    options |= (1 << opt); // DIN -> opt = 0; DOUT -> opt = 1;
  }

  console.log("Score: ", score);

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