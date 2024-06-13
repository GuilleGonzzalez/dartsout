
function homeCreateCanvas(homeCanvas) {
  let title = createHeader("Dartsout!"); // TODO: no imgs
  homeCanvas.appendChild(title);
  let spacer = createSpacer(30);
  homeCanvas.appendChild(spacer);

  let newPlayerBtn = createButton("Add player",  "new_player()");
  homeCanvas.appendChild(newPlayerBtn);
  let newCricketBtn = createButton("New cricket game",  "newCricketGame((1 << 0))");
  homeCanvas.appendChild(newCricketBtn);
  let newCricketBtnWild = createButton("New cricket wild game",  "newCricketGame((1 << 2))");
  homeCanvas.appendChild(newCricketBtnWild);
  let new301Btn = createButton("New 301 game",  "newX01Game()");
  homeCanvas.appendChild(new301Btn);

  let spacer2 = createSpacer(30);
  homeCanvas.appendChild(spacer2);
  let table = createTable("players-table", 8, 3);
  homeCanvas.appendChild(table);
  console.log("Table created");
  updateTable("players-table", ["#", "Name", "MPR"], false);
  let spacer3 = createSpacer(30);
  homeCanvas.appendChild(spacer3);

  let modal = createModal("exit-modal");
  homeCanvas.appendChild(modal);
  let modalContent = document.createElement("div");
  let radioGroup = createRadio(["Classic", "Cricket wild", "Cricket crazy", "Cricket wild and crazy"]);
  modalContent.appendChild(radioGroup);
  addTitleModal("exit-modal", "Cricket");
  addContentModal("exit-modal", modalContent);
  // launchModal("exit-modal");

  // let modalContent = document.createElement("div");
  // let text = document.createElement("h3");
  // text.innerHTML = "Are you sure you want to finish this game?";
  // let btnYes = createButton("Yes", "home()");
  // let btnNo = createButton("No", 'hideModal("exit-modal")');
  // modalContent.appendChild(text);
  // modalContent.appendChild(btnYes);
  // modalContent.appendChild(btnNo);
}

function homeAddPlayer(json) {
  let players = json["players"];
  let data = [];
  for (let i = 0; i < players.length; i++) {
    data.push([i+1, players[i]["name"], "--"]);
  }
  console.log("data", data);
  
  updateTable("players-table", false, data);
}