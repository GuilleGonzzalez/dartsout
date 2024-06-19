
function x01CreateCanvas(gameCanvas, nPlayers, maxScore, options) {
  let titleStr = maxScore;
  if (options & (1<<0)) {
    titleStr += " - Double In";
  }
  if (options & (1<<1)) {
    titleStr += " - Double Out";
  }
  let title = createHeader(titleStr);
  gameCanvas.appendChild(title);
  let spacer = createSpacer(30);
  gameCanvas.appendChild(spacer);
  let grid = x01CreateGrid(nPlayers);
  gameCanvas.appendChild(grid);
  let spacer2 = createSpacer(30);
  gameCanvas.appendChild(spacer2);

  let winnerAudio = createAudio("winner_audio", "audio/winner.oga");
  let nextPlayerAudio = createAudio("next_player_audio", "audio/next_player.oga");
  let tripleAudio = createAudio("triple_audio", "audio/triple.oga");
  let doubleAudio = createAudio("double_audio", "audio/double.oga");
  let simpleAudio = createAudio("simple_audio", "audio/simple.oga");
  let noAudio = createAudio("no_audio", "audio/no.oga");
  gameCanvas.appendChild(winnerAudio);
  gameCanvas.appendChild(nextPlayerAudio);
  gameCanvas.appendChild(tripleAudio);
  gameCanvas.appendChild(doubleAudio);
  gameCanvas.appendChild(simpleAudio);
  gameCanvas.appendChild(noAudio);

  gameCanvas.appendChild(spacer2);
}

function x01CreateGrid(nPlayers) {
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
  
  colLeft.appendChild(info);
  colRight.appendChild(cards);
  
  row.appendChild(colLeft);
  row.appendChild(colRight);
  grid.appendChild(row);

  return grid;
}

function x01Proccess(json) {
  let nPlayers = json["n_players"];
  let nDarts = json["darts"];
  let dartScores = json["dart_scores"];
  let currPlayerIdx = json["current_player"];
  let currPlayer = json["players"][currPlayerIdx];

  let roundStr = `Round: ${json["round"]}/${json["max_rounds"]}`;
  let scoreStr = `Round score: ${currPlayer["round_score"]}`
  updateCardInfo(currPlayer["name"], [roundStr, scoreStr]);

  let nums = Array(dartScores.length).fill("");
  let zones = Array(dartScores.length).fill("");
  for (let i = 0; i < dartScores.length; i++) {
    nums[i] = getNumStr(dartScores[i]["num"]);
    zones[i] = getZoneStr(dartScores[i]["zone"]);
  }
  updateDarts(nums, zones, nDarts);

  let players_names = Array(nPlayers);
  let players_scores = Array(nPlayers);
  for (let i = 0; i < nPlayers; i++) {
    players_names[i] = json["players"][i]["name"];
    players_scores[i] = json["players"][i]["game_score"];
  }

  updateScoreCards(players_names, players_scores);
  highlightScoreCard(currPlayerIdx);
}