// TODO: highlight row!
// TODO: darts!

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
    let noAudio = createAudio("cricket_no_audio", "audio/no.oga");
    let tripleAudio = createAudio("cricket_triple_audio", "audio/triple.oga");
    let doubleAudio = createAudio("cricket_double_audio", "audio/double.oga");
    let simpleAudio = createAudio("cricket_simple_audio", "audio/simple.oga");
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
    let table = createCricketTable(nPlayers);
    
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
    let darts = json["darts"];
    let dartScores = json["dart_scores"];
    let currPlayerIdx = json["current_player"];
    let currPlayer = json["players"][currPlayerIdx];
  
    let roundStr = `Round: ${json["round"]}/${json["max_rounds"]}`;
    let scoreStr = `Round score: ${currPlayer["round_score"]}`
    updateCardInfo(currPlayer["name"], [roundStr, scoreStr]);
  
    let header = ["Player"];
    let scoreables = ["20", "19", "18", "17", "16", "15", "BULL"];
    header = header.concat(scoreables);
  
    let players_names = Array(nPlayers);
    let players_scores = Array(nPlayers);
    let data = Array(nPlayers);
    for (let i = 0; i < nPlayers; i++) {
      data[i] = Array(scoreables.length);
      data[i][0] = json["players"][i]["name"];
      for (let j = 0; j < scoreables.length; j++) {
        data[i][j+1] = getSymbol(json["players"][i]["shots"][20-j]); //TODO: fix this
      }
      players_names[i] = json["players"][i]["name"];
      players_scores[i] = json["players"][i]["game_score"];
    }
    updateCricketTable(header, data);
    updateScoreCards(players_names, players_scores);
  }