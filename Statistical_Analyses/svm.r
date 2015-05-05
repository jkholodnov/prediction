library("RSQLite")
drv = SQLite()
setwd("~/Dropbox/current_Projects/honors")
con = dbConnect(drv, dbname="2015.db")
dbListTables(con)

games = dbGetQuery(con,"SELECT distinct gameID FROM games;")
tail(games)
games = as.vector(unlist(games))

get_data = function(gameid){
  print(gameid)
  query = paste("SELECT sum(a.minutes), sum(a.fgm),sum(a.fga),sum(a.tpm),sum(a.tpa),sum(a.ftm),sum(a.fta),sum(a.oreb),sum(a.dreb),sum(a.assist),sum(a.steal),sum(a.block),sum(a.turnover),sum(a.fouls),sum(a.plus_minus), sum(a.pir),sum(a.npr) FROM gamedata a, games b where a.gameid = b.gameid and a.teamid = b.team1Abbr and b.gameid =",gameid,";")
  query
  gamedata = dbGetQuery(con, query)  
  gamedata2 = dbGetQuery(con, paste("SELECT sum(a.minutes), sum(a.fgm),sum(a.fga),sum(a.tpm),sum(a.tpa),sum(a.ftm),sum(a.fta),sum(a.oreb),sum(a.dreb),sum(a.assist),sum(a.steal),sum(a.block),sum(a.turnover),sum(a.fouls),sum(a.plus_minus), sum(a.pir),sum(a.npr) FROM gamedata a, games b where a.gameid = b.gameid and a.teamid = b.team2Abbr and b.gameid =",gameid,";"))
  
  colnames(gamedata) = c("minutes","fgm","fga","tpm","tpa","ftm","fta","oreb","dreb","assist","steal","block","turnover","fouls","plus_minus","pir","npr")
  gamedata = as.data.frame(gamedata)
  gamedata
  if(gamedata$plus_minus > 0){
    gamedata = cbind(gamedata,1)
    gamedata2 = cbind(gamedata2,0)
  }else{
    gamedata = cbind(gamedata,0)
    gamedata2 = cbind(gamedata2,1)
  }
  gamedata$plus_minus
  colnames(gamedata) = c("minutes","fgm","fga","tpm","tpa","ftm","fta","oreb","dreb","assist","steal","block","turnover","fouls","plus_minus","pir","npr","Win")
  colnames(gamedata2) = c("minutes","fgm","fga","tpm","tpa","ftm","fta","oreb","dreb","assist","steal","block","turnover","fouls","plus_minus","pir","npr","Win")
  
  output = cbind(gamedata,gamedata2)
  return(output)
}

all_games = dbGetQuery(con, paste("SELECT distinct(gameID) FROM games where day > '2014-10-28' ORDER BY day ASC", sep=""))

gamesmatrix = (unlist(all_games))
overall = sapply(as.vector(gamesmatrix),get_data)
overall = t(overall)
overall = rbind(overall2[,c(1:18)],overall2[,c(19:36)])
rownames(overall) = 1:nrow(overall)
colnames = colnames(overall)
overall = matrix(unlist(overall),ncol=18)
colnames(overall) = colnames
overall = as.data.frame(overall)
head(overall)
dependent = overall[,18]
independent = overall[,-c(15,18)]
head(overall)
dim(overall)
model = svm(Win~minutes +fga+tpa+fta+oreb+dreb+assist+steal+block+turnover+fouls+npr, data = overall)
pred = predict(model, independent, type = "class")
table(pred = round(pred), true=dependent)

# 
model
head(round(pred))

dependent= as.numeric(unlist(dependent))
independent = matrix(unlist(independent),ncol=16)

model = svm(dependent~independent)
pred = predict(model,independent)
head(overall)

overall$Win = as.numeric(overall$Win)
head(overall)
