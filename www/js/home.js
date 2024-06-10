
function homeCreateCanvas(homeCanvas) {
  let title = createHeader("Dartsout!"); // TODO: no imgs
  homeCanvas.appendChild(title);
  let spacer = createSpacer(30);
  homeCanvas.appendChild(spacer);

  let newPlayerBtn = createButton("Add player",  "new_player()");
  homeCanvas.appendChild(newPlayerBtn);
  let newCricketBtn = createButton("New cricket game",  "newCricketGame()");
  homeCanvas.appendChild(newCricketBtn);
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