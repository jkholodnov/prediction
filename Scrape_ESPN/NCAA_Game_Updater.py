import urllib.request, urllib.error, urllib.parse
import sys
from queue import *
from threading import Thread
import bs4
from bs4 import BeautifulSoup
import sqlite3 as lite
from math import *
import time
import re
from socket import timeout
import errno

class ostream:
	def __init__(self, file):
		self.file = file
		
	def __lshift__(self, obj):
		self.file.write(str(obj));
		return self
cout = ostream(sys.stdout)
cerr = ostream(sys.stderr)
endl = '\n'

def get_Response_Time_of_URL(num_pings,url):
	response_times = []
	for i in range(num_pings):
		starttime = time.time()
		urllib.request.urlopen("http://www.espn.go.com")
		endtime = time.time()
		response_time = endtime-starttime
		response_times.append(response_time)
	MRT = response_times[int(len(response_times)/2)]
	print("Acquired the response time. Proceeding with function.")
	return MRT

class Player(object):
	def __init__(self):
		self.PlayerData = []
		self.playerName = "undefined"
		self.playerposition = "undefined"
		self.playerheight = "undefined"
		self.playerweight = "undefined"
		self.playerclass_year = "undefined"

	def updateName(self,x):
		y = x.replace("'","")
		self.playerName = y
	def updatePosition(self,x):
		self.playerposition = x
	def updateHeight(self,x):
		self.playerheight = x
	def updateWeight(self,x):
		self.playerweight = x
	def updateClass_Year(self,x):
		self.playerclass_year = x
	def fixHeightToInches(self):
		x = str(self.playerheight)
		y = x[0]
		z = x[2:]
		inches = 12*int(y) + int(z)
		self.playerheight = inches
		

				###############################################################################################################################
				###############################################################################################################################
				#############################                                                              ####################################
				#############################                   START OF PARALLEL FUNCTIONS                ####################################
				#############################                                                              ####################################
				###############################################################################################################################
				###############################################################################################################################

						############################################################################################################
						#####              This function browses through all team's webpages, saving their rosters             #####
						#####              as well as retrieving the ID's of games that they played in. These                  #####
						#####              values are put into "thePlayers" and "result" respectively                          #####
						############################################################################################################
def parallel_teampage_scraper(i,url,result,MRT,attempt):
	if(attempt > 0):
		print("Thread #: " + str(i) + " restarted for the : " + str(attempt) + " time.")
	starttime = time.time()
	try:
		baseurl = "http://espn.go.com/mens-college-basketball/team/_/id/"
		uniques = url.replace(baseurl,"") #drop off the useless bit.
		breakpoint = uniques.find("/")
		teamID = uniques[:breakpoint]
		url = "http://espn.go.com/mens-college-basketball/team/schedule/_/id/"+ teamID + "/year/2014/"
		soup = BeautifulSoup(urllib.request.urlopen(url, timeout = 150).read())
		for link in soup.find_all('a'):
			url = link.get('href')
			y = "/ncb/recap?gameId="
			x = str(url).find(y)
			if(x!= -1):
				gameID = url[-9:]
				result.put(gameID)
		endtime = time.time()
		print("Thread: " + str(i) + " finished scraping in: " + str(endtime-starttime) + " seconds.")
	#THREAD EXCEPTION HANDLING
	except timeout:
		print("URL timeout. Restarting.")
		parallel_teampage_scraper(i,url,result,MRT,attempt+1)
	except urllib.error.URLError as e:
		print("URL Timeout Error. Restarting")
		parallel_teampage_scraper(i,url,result,MRT,attempt+1)
	except ConnectionResetError as e:
		print("Connection got reset. Calling back.")
		parallel_teampage_scraper(i,url,result,MRT,attempt+1)

	except OSError as e:
		if(e.errno == errno.EHOSTUNREACH):
			print("Host Unreachable. Trying again.")
			parallel_teampage_scraper(i,url,result,MRT,attempt+1)
		if(e.errno == errno.ECONNRESET):
			print("Host hung up. Calling back.")
			parallel_teampage_scraper(i,url,result,MRT,attempt+1)
		else:
			print(e.args)
			print(e[0])
	except Exception as e:
		print("idk what just happened.")
		print(e.arg)
	
						############################################################################################################
						#####              This function takes in a queue of game ID's, and parses through the                 #####
						#####              corresponding webpage, retrieving the game Recap, as well as play-by-play.          #####
						#####              This data iss then stored into a the "gameData" table                                #####
						############################################################################################################

def getGameData_in_parallel(i,gameid,result):
	conn = lite.connect("predict.db", isolation_level = None)
	cur = conn.cursor()

	try:
		class Game(object):
			def __init__(self):
				self.Teams = []
			def addTeam(self,team):
				self.Teams.append(team)

			def updateGameData(self):
				for team in self.Teams:
					for player in team.Players:
						if(player.name != "Undefined"):
							player.updatePlayersTable()
							player.getPlayerID()
							g = []
							g.append(gameid)
							g.append(team.teamID)
							g.append(player.playerID)
							g.append("'" + str(player.name) + "'")
							for data in player.PlayerData:
								g.append(data)
							try:
								cur.execute("INSERT INTO gameData VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)",g)
								conn.commit()
							except Exception as e:
								print(e)
								print(e.args)
			def updatePlayerTotals(self):
				for team in self.Teams:
					for player in team.Players:
						if(player.name != "Undefined"):
							theid = []
							theid.append(player.playerID)
							cur.execute("SELECT * FROM playerTotals WHERE Id = ?",theid)
							playerTotals = cur.fetchall()
							already_in_db = len(playerTotals)                       
							if(already_in_db == 0):
								#initialize totals to 0 filled list
								totals = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
								numGames = 1
								query = []
								query.append(numGames)
								query.append(player.playerID)
								#Update the totals value by the current games values
								if(len(totals) != len(player.PlayerData)):
									print(player.name)
									print(len(player.PlayerData))
									print(len(totals))
									print(gameid)
								for i in range(len(totals)):
									totals[i] = totals[i] + int(player.PlayerData[i])
								for total in totals:
									query.append(total)

								try:
									cur.execute("INSERT INTO playerTotals VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)",query)
									conn.commit()
								except Exception as e:
									print(e)
									print(e.args)
							else:
								numGasmes = playerTotals[0][0]
								numGames = numGames + 1
								playerTotals = playerTotals[0]

								totals = []
								for data in playerTotals:
									totals.append(data)

								totals = totals[2:]
								try:
									for i in range(len(totals)):
										totals[i] = totals[i] + int(player.PlayerData[i])
								except:
									if(len(player.PlayerData) != len(totals)):
										f = open('Error Log','a')
										f.write("Something is wrong with the recap data vs the database data" + str(gameid) + "\n")
										f.close
								try:
									cur.execute("UPDATE playerTotals SET minutes = ?, fgm = ?, fga = ?, tpm = ?, tpa = ?, ftm = ?, fta = ?, oreb = ?, dreb = ?, reb = ?, assist = ?, steal = ?, block = ?, turnover = ?, fouls = ?, points = ? where Id = " + str(player.playerID), totals)
									conn.commit()
								except Exception as e:
									print(e)
									print(e.args)

		class Team(object):
			def __init__(self):
				self.teamName = "undefined"
				self.Players = []
				self.gameStats = []
				self.teamID = "undefined"
			def changeName(self,name):
				self.name = name
			def addPlayer(self , player):
				self.Players.append(player)
			def addGameStats(self,stats):
				self.gameStats.append(stats)

		class Player(object):
			def __init__(self):
				self.PlayerData = []
				self.name = "Undefined"
				self.link = "undefined"
				self.height = "undefined"
				self.weight = "undefined"
				self.teamID = "undefined"
			def add_Height_and_Weight_NBA(self,ht,wt):
				self.weight = int(wt)
				y = ht.find("' ")
				feet = ht[:y]
				inches = ht[y+2:]
				height_in_inches = int(feet)*12 + int(inches)
				print(height_in_inches)
				self.height = int(height_in_inches)
			def add_Height_notnba(self,x):
				g = x.find("-")
				tempint = int(x[:g]) * 12
				tempint = tempint + int(x[g+1:])
				self.height = tempint
			def add_Weight_notnba(self,x):
				g = x.find(" lbs")
				self.weight = int(x[:g])
			def changeName(self,x):
				y = x.replace("'","")
				self.name = y
			def addPlayerData(self , data):
				x = str(data).find("-")
				if x!= -1:
					self.PlayerData.append(data[0:x])
					self.PlayerData.append(data[x+1:])
				else:
					self.PlayerData.append(data)
			def updatePlayersTable(self):
				namerino = []
				namerino.append(self.name)
				cur.execute("SELECT COUNT(*) FROM players WHERE Name = ?",namerino)
				already_in_db = cur.fetchone()[0]
				if(int(already_in_db) != 0):
					if(int(already_in_db)>1):
						print(int(already_in_db))
				else:
					playersname = []
					playersname.append(self.name)
					cur.execute("INSERT INTO players VALUES(NULL,NULL,?,NULL,NULL)", playersname)
					conn.commit()

					content = urllib.request.urlopen(self.link)
					soup = BeautifulSoup(content)
					meta_table = soup.find('ul', {"class": "player-metadata"})
					lis = meta_table.find_all('li')
					if(len(lis) < 4):
						general = soup.find('ul', {"class": "general-info"})
						listy = general.find_all('li')
						for li in listy:
							x = li.getText().find("lbs")
							if(x != -1):
								thevar = li.getText()
								y = thevar.find("""", """)
								z = thevar.find(" lbs")
								ht = thevar[:y]
								wt = thevar[y+3:z]
								self.add_Height_and_Weight_NBA(ht,wt)
					for li in lis:
						thespan = li.find('span')
						if(thespan.getText() == "Height"):
							text = li.getText()
							self.add_Height_notnba(text[len("Height"):])
						elif(thespan.getText() == "Weight"):
							text = li.getText()
							self.add_Weight_notnba(text[len("Weight"):])
					values_into_players = []
					values_into_players.append(self.teamID)
					values_into_players.append(self.height)
					values_into_players.append(self.weight)
					cur.execute("UPDATE players SET teamID = ?, height = ?, weight = ? WHERE Name = " + "'" + str(self.name) + "'", values_into_players)
					conn.commit()
			def getPlayerID(self):
				namerino = []
				namerino.append(self.name)
				cur.execute("SELECT Id FROM players WHERE Name = ?", namerino)
				theid = cur.fetchone()
				if theid != None: 
					self.playerID = theid[0]

		def updateGamesTable():
			teams = soup.find('div',{"class" : "matchup"})
			links = teams.find_all('a')
			for link in links:
				k = str(link.get('href'))
				m = "http://espn.go.com/mens-college-basketball/team/_/id/"
				try:
					x = k.find(m)
				except Exception as e:
					pass
					
				if(x != -1):
					teamUniques = k[x + len(m):]
					l = teamUniques.find("/")
					teamID = teamUniques[0:l]
					if team1.teamID == "undefined":
						team1.teamID = int(teamID)
					else:
						team2.teamID = int(teamID)
			
			query = []
			query.append(gameid)
			query.append(team1.teamID)
			query.append(team2.teamID)
			thegameid = []
			thegameid.append(gameid)
			cur.execute("SELECT * FROM games WHERE Id = ?",thegameid)
			x = cur.fetchall()
			if len(x) == 0:
				try:
					cur.execute("INSERT INTO games VALUES(?,?,?)", query)
					conn.commit()
				except Exception as e:
					print(e)

		def getGameRecap():
			table = soup.find('table', {"class" : "mod-data"})
			rows = table.find_all('tr')
			found1 = 0
			found2 = 0
			for row in rows:
				tds = row.find_all('td')
				if len(tds) == 0:
					found1= found1+1
				
				if (found1 > 1) & (found1 < 4):
					tempPlayer = Player()
					for td in tds:
						try:
							name = td.find('a')
							tempPlayer.link = name.get('href')
							playerName = name.getText()
							tempPlayer.changeName(playerName)
						except:
							tempPlayer.addPlayerData(td.getText())
					tempPlayer.teamID = team1.teamID
					team1.addPlayer(tempPlayer)
				if (found1 > 5) & (found1 < 8):
					tempPlayer = Player()
					for td in tds:
						try:
							name = td.find('a')
							tempPlayer.link = name.get('href')
							playerName = name.getText()
							tempPlayer.changeName(playerName)
						except:
							tempPlayer.addPlayerData(td.getText())
					tempPlayer.teamID = team1.teamID
					team2.addPlayer(tempPlayer)

				if len(tds) == 13:
					found2 = found2+1
					if(found2 == 1):
						for td in tds:
							team1.addGameStats(td.getText())
					if(found2 == 2):
						for td in tds:
							team2.addGameStats(td.getText())

		theGame = Game()
		team1 = Team()
		team2 = Team()
		base = "http://espn.go.com/ncb/boxscore?gameId="
		content = urllib.request.urlopen(base + gameid).read()
		soup = BeautifulSoup(content)

		updateGamesTable()
		getGameRecap()
		theGame.addTeam(team1)
		theGame.addTeam(team2)
		theGame.updateGameData()
		theGame.updatePlayerTotals()
		conn.close()
	except Exception as e:
		print(e)
		print(e.args)


					###############################################################################################################################
					###############################################################################################################################
					#############################                                                              ####################################
					#############################                   START OF MAIN FUNCTION                     ####################################
					#############################                                                              ####################################
					###############################################################################################################################
					###############################################################################################################################
def main(): #Get the page that holds all team url pages
	f = open('Error Log','a')
	f.write("########################\n")
	f.write("####Parse New Games\t####\n")
	f.write(time.strftime("####\t%m/%d/%Y\t####\n"))
	f.write(time.strftime("####\t%H:%M:%S\t####\n"))
	f.write("########################\n")
	f.close

	con = lite.connect('predict.db')
	cur = con.cursor()

	numThreads = 40
	queue = Queue() #The queue of jobs that are required to be done
	result = Queue() #The result of the algorithm onto the queue items

	def getTeamIDs():
		baseurl = "http://espn.go.com/mens-college-basketball/standings"
		content = urllib.request.urlopen(baseurl).read()
		soup = BeautifulSoup(content)
		teamurls = []
		teamurlsparsed=[]
		y = "http://espn.go.com/mens-college-basketball/team/_/id/"
		for link in soup.find_all('a'):
			url = str(link.get('href'))
			if(url.find(y) != -1):
				queue.put(url)
		
		print("There are: " + str(queue.qsize()) + " teams to get games from.")
		print ("Pinging ESPN to check MRT........")
		MRT = get_Response_Time_of_URL(50,"http://espn.go.com/mens-college-basketball/team/_/id/399/albany-great-danes")
		print ("MRT = " + str(MRT))

		threads = []
		start_of_first_parallel = time.time()
		print("Cranking up the batmobile...")
		for i in range(queue.qsize()):
			timestart = time.time()
			t=Thread(target=parallel_teampage_scraper, args=(i,queue.get(),result,MRT,0))
			t.daemon = True
			t.start()
			threads.append(t)
			while(time.time()-timestart < MRT):
				spinlock = True
			

		for t in threads:
			t.join()
		end_of_first_parallel = time.time()
		print("Acquired targets in : " + str(end_of_first_parallel-start_of_first_parallel) + " seconds.")
	getTeamIDs()
	print("There's going to be some serious shit when this hits 88mph........")

	def getRecap():
		retrievedGameIDs = set({})
		while not result.empty():
			gameId = result.get()
			retrievedGameIDs.add(str(gameId))
		
		cur.execute("SELECT gameID FROM gameData")
		rows = cur.fetchall()
		databaseGameIDs = set({})
		for row in rows:
			theid = str(row[0])
			databaseGameIDs.add(theid)

		print("Number of ids taken from parse: " + str(len(retrievedGameIDs)))
		print("Number of ids in db: " + str(len(databaseGameIDs)))

		newGameIDs = retrievedGameIDs.difference(databaseGameIDs)

		print("The number of games to scrape is: " + str(len(newGameIDs)))


		print ("Pinging ESPN to check MRT........")
		MRT = get_Response_Time_of_URL(50,"http://espn.go.com/ncb/boxscore?gameId=400498718")
		print ("MRT = " + str(MRT))

		threads = []
		i=1
		for gameID in newGameIDs:
			starttime = time.time()
			t=Thread(target=getGameData_in_parallel, args=(i,gameID,result))
			t.daemon = True
			t.start()
			threads.append(t)
			while(time.time() - starttime < MRT):
				spinlock = True
			i += 1

		for t in threads:
			t.join()

		con.commit()
		numThreads = 40

	getRecap()

if __name__ == '__main__':
	main()