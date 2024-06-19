
function getSymbol(num) {
  switch(num) {
  case 1:
    return "𝈺";
  case 2:
    return "⨉";
  case 3:
    return "⦻";
  default:
    return "";
  }
}

function getNumStr(num) {
  if (num == 0) {
    return "◎";
  } else if (num == -1) {
    return "-";
  } else {
    return num.toString();
  }
}

function getZoneStr(zone_id) {  
  if (zone_id == -1) {
    return "--";
  } else if (zone_id == 0) {
    return "Triple";
  } else if (zone_id == 1) {
    return "Double";
  } else {
    return "Single";
  }
}

function getZoneColor(zone) {
  if (zone == "Triple") {
    return "rgb(218,165,32)";
  } else if (zone == "Double") {
    return "rgb(128,128,128)";
  } else {
    return "white";
  }
}

function getZoneShadow(zone) {
  if (zone == "Triple") {
    return "#FC0 0 0 10px";
  } else if (zone == "Double") {
    return "#AAA 0 0 10px";
  } else {
    return "";
  }
}
