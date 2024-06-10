
function cricketCreateCanvas(gameCanvas, nPlayers) {
  let title = createHeader("Cricket");
  gameCanvas.appendChild(title);
  let spacer = createSpacer(30);
  gameCanvas.appendChild(spacer);
  let grid = cricketCreateGrid(nPlayers);
  gameCanvas.appendChild(grid);
  let spacer2 = createSpacer(30);
  gameCanvas.appendChild(spacer2);

  let winnerAudio = createAudio("winner_audio", "audio/winner.oga");
  let nextPlayerAudio = createAudio("next_player_audio", "audio/next_player.oga");
  let noAudio = createAudio("no_audio", "audio/no.oga");
  let tripleAudio = createAudio("triple_audio", "audio/triple.oga");
  let doubleAudio = createAudio("double_audio", "audio/double.oga");
  let simpleAudio = createAudio("simple_audio", "audio/simple.oga");
  gameCanvas.appendChild(winnerAudio);
  gameCanvas.appendChild(nextPlayerAudio);
  gameCanvas.appendChild(noAudio);
  gameCanvas.appendChild(tripleAudio);
  gameCanvas.appendChild(doubleAudio);
  gameCanvas.appendChild(simpleAudio);

  gameCanvas.appendChild(spacer2);
}

function cricketCreateGrid(nPlayers) {
  let grid = document.createElement("div");
  grid.className = "container-fluid";
  let row = document.createElement("div");
  row.className = "row";
  let colLeft = document.createElement("div");
  colLeft.className = "col col-lg-3";
  let colRight = document.createElement("div");
  colRight.className = "col";
  
  let info = createCardInfo();
  
  let cards = createScoreCards(nPlayers);
  let table = createTable("cricket-table", nPlayers, 8);
  
  colLeft.appendChild(info);
  colRight.appendChild(cards);
  colRight.appendChild(table);
  
  row.appendChild(colLeft);
  row.appendChild(colRight);
  grid.appendChild(row);

  return grid;
}

function cricketProccess(json) {
  let nPlayers = json["n_players"];
  let nDarts = json["darts"];
  let dartScores = json["dart_scores"];
  let scoreables = json["scoreables"];
  let currPlayerIdx = json["current_player"];
  let currPlayer = json["players"][currPlayerIdx];

  let roundStr = `Round: ${json["round"]}/${json["max_rounds"]}`;
  let scoreStr = `Round score: ${currPlayer["round_score"]}`
  updateCardInfo(currPlayer["name"], [roundStr, scoreStr]);
  highlightTableRow("cricket-table", currPlayerIdx);

  let nums = Array(dartScores.length).fill("");
  let zones = Array(dartScores.length).fill("");
  for (let i = 0; i < dartScores.length; i++) {
    if (i < nDarts) {
      nums[i] = getNumStr(dartScores[i]["num"]);
      zones[i] = getZoneStr(dartScores[i]["zone"]);
    }
  }
  updateDarts(nums, zones, nDarts);

  let header = ["Player"];
  let scoreablesStr = scoreablesToStr(scoreables);
  header = header.concat(scoreablesStr);

  let players_names = Array(nPlayers);
  let players_scores = Array(nPlayers);
  let players_shots = Array(nPlayers);
  let data = Array(nPlayers);
  for (let i = 0; i < nPlayers; i++) {
    data[i] = Array(scoreables.length);
    data[i][0] = json["players"][i]["name"];
    for (let j = 0; j < scoreables.length; j++) {
      let shots = json["players"][i]["shots"]
      data[i][j+1] = getSymbol(shots[scoreables[j]]);
    }
    data[i][scoreables.length-1]
    players_names[i] = json["players"][i]["name"];
    players_scores[i] = json["players"][i]["game_score"];
    players_shots[i] = json["players"][i]["shots"];
  }

  let closedNumbers = checkClosed(players_shots);

  updateTable("cricket-table", header, data, closedNumbers);
  updateScoreCards(players_names, players_scores);
}

function scoreablesToStr(scoreables) {
  let str = [];
  for (let i = 0; i < scoreables.length; i++) {
    if (scoreables[i] == 0) {
      str.push("◎");
    } else {
      str.push(scoreables[i].toString());
    }
  }

  return str;
}

function checkClosed(shots) {
  let closedNumbers = [];
  for (let i = 0; i < shots[0].length; i++) { // 21 nums
    let opened = 0;
    for (let j = 0; j < shots.length; j++) { // 3 players
      if (shots[j][i] != 3) {
        opened = 1;
      }
    }
    if (opened == 0) {
      closedNumbers.push(getNumStr(i));
    }
  }
  return closedNumbers;
}