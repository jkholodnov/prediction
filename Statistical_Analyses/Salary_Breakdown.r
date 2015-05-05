library("RSQLite")
drv = SQLite()
setwd("~/Dropbox/current_Projects/honors")
con = dbConnect(drv, dbname="2015.db")
dbListTables(con)

teams = dbGetQuery(con,"SELECT distinct teamID from players;")

get_salary_breakdown = function(teamid){
  defense_spending = 0;
  offense_spending = 0;
  centers = dbGetQuery(con,paste("SELECT position,salary FROM players WHERE teamID = '",teamid,"' and position = 'C';",sep=""))
  point_guards = dbGetQuery(con,paste("SELECT position,salary FROM players WHERE teamID = '",teamid,"' and position = 'PG';",sep=""))
  shooting_guard = dbGetQuery(con,paste("SELECT position,salary FROM players WHERE teamID = '",teamid,"' and position = 'SG';",sep=""))
  small_forward = dbGetQuery(con,paste("SELECT position,salary FROM players WHERE teamID = '",teamid,"' and position = 'SF';",sep=""))
  power_forward = dbGetQuery(con,paste("SELECT position,salary FROM players WHERE teamID = '",teamid,"' and position = 'PF';",sep=""))
  
  offense_spending = offense_spending + 1.0*sum(shooting_guard$salary)
  defense_spending = defense_spending + 0.0*sum(shooting_guard$salary)
  
  offense_spending = offense_spending + 0.8*sum(point_guards$salary)
  defense_spending = defense_spending + 0.2*sum(point_guards$salary)
  
  offense_spending = offense_spending + 0.6*sum(small_forward$salary)
  defense_spending = defense_spending + 0.4*sum(small_forward$salary)
  
  offense_spending = offense_spending + 0.2*sum(power_forward$salary)
  defense_spending = defense_spending + 0.8*sum(power_forward$salary)
  
  offense_spending = offense_spending + 0.4*sum(centers$salary)
  defense_spending = defense_spending + 0.6*sum(centers$salary)
  
  offense_percent = round(offense_spending / (offense_spending+defense_spending),2)
  defense_percent = round(defense_spending / (offense_spending+defense_spending),2)
  
  elo = dbGetQuery(con,paste("SELECT max(team1elo) FROM games WHERE team1abbr = '",teamid,"';",sep=""))
  elo
  return (cbind(offense_percent,defense_percent,elo))
}

get_salary_breakdown("mia")



teams = (unlist(teams))
overall = sapply(as.vector(teams),get_salary_breakdown)
overall = as.matrix(t(overall))
overall
colnames(overall) = c("offense","defense","max_elo")
overall = as.data.frame(overall)
overall$defense
plot(as.numeric(overall$max_elo) ~ as.numeric(overall$defense), xlab="Percentage Spent on Defense")
textxy(as.numeric(overall$defense), as.numeric(overall$max_elo), labs = rownames(overall), cex = 1.0, col=round(as.numeric(overall$max_elo)/100)+2)
title("Salary Distribution ~ Maximum Rating Achieved")
