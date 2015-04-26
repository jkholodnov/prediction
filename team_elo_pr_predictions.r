library("RSQLite")
drv = SQLite()
setwd("~/Dropbox/current_Projects/predict")
con = dbConnect(drv, dbname="predict.db")
dbListTables(con)


gameid = 400558916
get_team_elo_and_performance = function(gameid, teamid){
  print(teamid)
  teamid = "mia"
  teams = dbGetQuery(con, paste("SELECT Team1Abbr, Team1ELO, Team1Score, Team2Abbr, Team2ELO, Team2Score FROM games WHERE gameID = ", gameid, ";", sep="") )

  team1query = paste("select performance_rating from gamedata where gameid = ", gameid, " and teamid = '", teams$Team1Abbr ,"' order by performance_rating desc LIMIT 7;", sep="")
  team2query = paste("select performance_rating from gamedata where gameid = ", gameid, " and teamid = '", teams$Team2Abbr ,"' order by performance_rating desc LIMIT 7;", sep="")
  
  team1performances = dbGetQuery(con, team1query)
  team2performances = dbGetQuery(con, team2query)
  teams
  team1PR = sum(team1performances)
  team2PR = sum(team2performances)
  
  team1info = cbind(teams$Team1ELO, team1PR, teams$Team1Score)
  team2info = cbind(teams$Team2ELO, team2PR, teams$Team2Score)
  
  output = cbind(team1info,team2info)
  
  if(teams$Team1Abbr == teamid){
    output = cbind(output, team1PR)
  } else {
    output = cbind(output, team2PR)
  }
  
  colnames(output) = c("T1ELO","T1PR","T1Score","T2ELO","T2PR","T2Score","PR")
  #output = rbind(output, cbind(team2info))
  
  return(output)
}

Get_Team_Performances_To_Date = function(teamid){
  gameids = "SELECT a.performance_rating from gamedata a, games b, where a.gameid = b.gameid ORDER BY b.day ASC"
  return(output)
}

game = 400559378
x = get_team_elo_and_performance(400558916,"mia")
x

TeamId = "okc"
games = dbGetQuery(con, paste("SELECT distinct(gameID) FROM games where team1abbr = '",TeamId,"' or team2abbr = '",TeamId,"' and day > '2013-11-10' ORDER BY day DESC LIMIT 10;", sep=""))
dim(games)
gamesmatrix = (unlist(games))
overall = sapply(as.vector(gamesmatrix),get_team_elo_and_performance, teamid = "mia")
overall = t(overall)
colnames(overall) = c("Team1ELO","Team2ELO","Team1PR","Team2PR","Team1Score","Team2Score")

overall = as.data.frame(overall)
overall
