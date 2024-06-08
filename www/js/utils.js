
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
    return "Bull";
  } else {
    return num;
  }
}

function getZoneStr(zone_id) {
  if (zone_id == 0) {
    return "Triple";
  } else if (zone_id == 1) {
    return "Double";
  } else {
    return "Single";
  }
}

function getZoneColor(zone_id) {
  if (zone_id == 0) {
    return "rgb(218,165,32)";
  } else if (zone_id == 1) {
    return "rgb(128,128,128)";
  } else {
    return "white";
  }
}
