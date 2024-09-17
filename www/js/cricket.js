
function cricketCreateCanvas(gameCanvas, nPlayers, options) {
  gameCanvas.replaceChildren();
  let titleStr = "Cricket";
  if (options & (1 << 0)) {
    titleStr += "";
  } else if (options & (1 << 1)) {
    titleStr += " no score";
  } else if (options & (1 << 2)) {
    titleStr += " cut throat";
  } else if (options & (1 << 3)) {
    titleStr += " wild";
  } else if (options & (1 << 4)) {
    titleStr += " crazy";
  } else if (options & (1 << 5)) {
    titleStr += " wild and crazy";
  } else {
    titleStr += "ERROR";
    console.log("Error in Cricket options")
  }
  titleStr += ` (ID=${gameId})`;
  let title = createGameHeader(titleStr);
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
  let enabled = json["enabled"];
  let currPlayerIdx = json["current_player"];
  let currPlayer = json["players"][currPlayerIdx];

  let roundStr = `Round: ${json["round"]}/${json["max_rounds"]}`;
  let scoreStr = `Round score: ${currPlayer["round_score"]}`
  let mprStr = `MPR: ${currPlayer["mpr"] / 100}`
  updateCardInfo(currPlayer["name"], [roundStr, scoreStr, mprStr]);
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
  let targetsStr = targetsToStr(enabled);
  header = header.concat(targetsStr);

  let players_names = Array(nPlayers);
  let players_scores = Array(nPlayers);
  let players_shots = Array(nPlayers);
  let data = Array(nPlayers);
  for (let i = 0; i < nPlayers; i++) {
    data[i] = Array(enabled.length);
    data[i][0] = json["players"][i]["name"];
    for (let j = 0; j < enabled.length; j++) {
      let shots = json["players"][i]["shots"]
      data[i][j+1] = getSymbol(shots[j]);
    }
    players_names[i] = json["players"][i]["name"];
    players_scores[i] = json["players"][i]["game_score"];
    players_shots[i] = json["players"][i]["shots"];
  }

  let closedNumbers = getClosed(enabled, players_shots);

  updateTable("cricket-table", header, data, closedNumbers);
  updateScoreCards(players_names, players_scores);
}

function targetsToStr(targets) {
  let str = [];
  for (let i = 0; i < targets.length; i++) {
    if (targets[i] == 0) {
      str.push("◎");
    } else {
      str.push(targets[i].toString());
    }
  }

  return str;
}

function getClosed(enabled, shots) {
  let closedNumbers = [];
  for (let i = 0; i < shots[0].length; i++) { // 7 nums
    let opened = 0;
    for (let j = 0; j < shots.length; j++) { // 3 players
      if (shots[j][i] != 3) {
        opened = 1;
      }
    }
    if (opened == 0) {
      closedNumbers.push(getNumStr(enabled[i]));
    }
  }
  return closedNumbers;
}