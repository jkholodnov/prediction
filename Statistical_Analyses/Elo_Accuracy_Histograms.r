library("RSQLite")
library("ggplot2")
drv = SQLite()
setwd("~/Dropbox/current_Projects/honors")
con = dbConnect(drv, dbname="2015.db")
dbListTables(con)

failed_games = "SELECT gameid FROM games WHERE (team1elo != 1500 and team2elo != 1500) and (team1ELO > team2ELO and team1score < team2score) or (team1elo < team2elo and team1score > team2score);"
games = dbGetQuery(con, failed_games)
correct_games = "SELECT gameid FROM games WHERE (team1elo != 1500 and team2elo != 1500) and ((team1elo > team2elo and team1score > team2score) or (team1elo < team2elo and team1score < team2score));"
all_games = dbGetQuery(con, correct_games)
get_elo_spreads = function(gameid){
  query = paste("SELECT team1ELO, team2ELO FROM games where gameid = '",gameid,"';", sep="")
  elos = dbGetQuery(con,query)
  elos = as.data.frame(elos)
  elo_spread = abs(elos$Team1ELO - elos$Team2ELO)
  return(elo_spread)
}
all_games_spreads = sapply(as.vector(unlist(all_games)),get_elo_spreads)
all_games = as.data.frame(all_games_spreads)
colnames(all_games) = "Elo_Spread"
head(all_games)

elo_spreads = sapply(as.vector(unlist(games)),get_elo_spreads)
elo_spreads = as.data.frame(elo_spreads)
#hist(as.matrix(elo_spreads),breaks=15,main="Games where ELO did not predict the winner", xlim=c(-500,500), col="green")
failed_games = as.data.frame(elo_spreads)
colnames(failed_games) = "Elo_Spread"
# as.vector(elo_spreads)

#and combine into your new data frame vegLengths
All_ELO_Spreads <- rbind( failed_games,all_games)
all_games
failed_games
dim(failed_games)
dim(all_games)
#now make your lovely plot
qplot(Elo_Spread, data=All_ELO_Spreads, geom = "density", fill = tag, position="stack", alpha = .6)
dim(all_games)
dim(failed_games)
tail(vegLengths)

hist(as.numeric(unlist(all_games)), breaks =  20, xlab= "Elo Differential of Teams", col="green", main="Elo Used as a Predictive Statistic")
hist(as.numeric(unlist(failed_games)), breaks=20, col="red", alpha = .2, add=T)
legend(325,52, c("Correct Prediction","Incorrect Prediction"), box.lty = par("lty"), lwd=c(2.5,2.5),col=c("green","red"))
box()

dim(all_games)
dim(failed_games)
