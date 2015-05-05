library("RSQLite")
drv = SQLite()
setwd("~/Dropbox/current_Projects/honors")
con = dbConnect(drv, dbname="2015.db")
dbListTables(con)

games = dbGetQuery(con,"SELECT distinct gameID FROM games;")
teams = dbGetQuery(con,"SELECT distinct teamID from players;")
teams

get_salary_of_team = function(teamid){
  
  salaries = dbGetQuery(con,paste("SELECT sum(salary) FROM players WHERE teamID = '",teamid,"';",sep=""))
  elo = dbGetQuery(con,paste("SELECT max(team1elo) FROM games WHERE team1abbr = '",teamid,"';",sep=""))
  elo
  return (cbind(salaries,elo))
}
get_salary_of_team("mia")
gamesmatrix = (unlist(teams))
overall = sapply(as.vector(gamesmatrix),get_salary_of_team)
head(t(overall))
cbind(teams,t(overall))
overall = as.matrix(t(overall))
overall
colnames(overall) = c("Salary","currentELO")
overall = as.data.frame(overall)
overall$Salary = as.numeric(overall$Salary)
teamnames = rownames(overall)

plot(overall)
textxy(overall$Salary, overall$currentELO, labs = teamnames, cex = 1.0, col=round(overall$Salary/10000000)+5)
title("Total Salary Spending of Teams ~ Maximum Rating Achieved")

