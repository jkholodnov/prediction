import urllib.request, urllib.error
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


def get_Response_Time_of_URL(num_pings,url):


    response_times = []
    for i in range(num_pings):
        starttime = time.time()
        urllib.request.urlopen("http://www.espn.go.com")
        endtime = time.time()
        response_time = endtime-starttime
        response_times.append(response_time)
    MRT = response_times[int(len(response_times)/2)]
    return MRT

def convert_Str_To_Tuple(str_format,data):
    unformatted_string = str_format.replace("?","{}")
    formatted_string = unformatted_string.format(*data)
    parameters = formatted_string.split(",")
    return tuple(parameters)

class team(object):
    def __init__(self):
        self.players = []
        self.abbreviation = None
        self.score = None
    def add_player(self,x):
        self.players.append(x)
    def length(self):
        return len(self.players)
class player(object):
    def __init__(self):
        self.name = None
        self.position = None
        self.id = None
        self.salary = None
        self.height = None
        self.weight = None
        self.teamAbbr = None
        self.game_data = []
    def add_ID(self,x):
        self.id = x
    def add_data(self,x):
        self.game_data.append(x)
    def change_name(self,name):
        self.name = name
    def height_and_weight(self,ht,wt):
        self.weight = wt
    def fix_gameData(self):
        #This function takes the raw TD data and extracts the name and position from it.
        newData = []
        for i in range(len(self.game_data)):
            position = str(self.game_data[i]).find(",")
            #fix the name and position
            if(position != -1):
                if(self.position == None):
                    pos = str(self.game_data[i])[position+1:]
                    temp_name = str(self.game_data[i])[:position]
                    temp_name = temp_name.replace("'","")
                    temp_name = temp_name.replace("-","")
                    self.name = temp_name
                    self.position = pos.replace(" ","")
            #put rest of data into dataset.
            else:
                g = self.game_data[i].find("-")
                if(g != -1 and g>0):
                    newData.append(self.game_data[i][:g])
                    newData.append(self.game_data[i][g+1:])
                else:
                    newData.append(self.game_data[i])
        self.game_data = newData


def main(): #Get the page that holds all team url pages
    con = lite.connect('predict.db', isolation_level = None)
    cur = con.cursor()

    result = Queue() #The result of the algorithm onto the queue items
    players = Queue()

    def getTeamIDs():
        baseurl = "http://espn.go.com/nba/teams"
        content = urllib.request.urlopen(baseurl).read()
        soup = BeautifulSoup(content)
        team_Abbreviations = []
        y = "/nba/teams/schedule?team="
        for link in soup.find_all('a'):
            url = str(link.get('href'))
            if(url.find(y) != -1):
                baseurl = "http://espn.go.com/" 
                teamABBR = url.replace(y,"") #drop off the useless bit.                print(teamABBR)
                team_Abbreviations.append(teamABBR)
        
        print("There are: " + str(len(team_Abbreviations)) + " teams to get games from.")
        MRT = get_Response_Time_of_URL(50,"http://espn.go.com/nba/team/_/name/mia")

        threads = []
        start_of_first_parallel = time.time()

        for teamAbbrev in team_Abbreviations:
            timestart = time.time()
            thread=Thread(target=get_gameIDs, args=(teamAbbrev,result,0,2015))
            thread.daemon = True
            thread.start()
            threads.append(thread)
            while(time.time()-timestart < MRT):
                spinlock = True

        for thread in threads:
            thread.join()
        end_of_first_parallel = time.time()
        print("Acquired gameIDs in : " + str(end_of_first_parallel-start_of_first_parallel) + " seconds.")
    getTeamIDs()
    #################################################################################################################################
    #   Now we have all of the gameIDs. We will proceed to open the url corresponding to the gameID and then:                       #
    #   -eGet the abbreviations of the 2 teams playing, store this in the games table, along with the score of the game and gameID  #
    #   -Get all of the player links. Check if the player already has an entry in the database. If no:                              #
    #       -Get the players id,teamABBR,height,weight,salary and store this in the players table.                                  #
    #   -Get all of the player-specific game data. Link this to the gameID, playerID, teamABBR.                                     #
    #################################################################################################################################
    def scrape_Games():
        retrievedGameIDs = set({})
        while not result.empty():
            gameId = result.get()
            retrievedGameIDs.add(str(gameId))
        
        cur.execute("SELECT gameID FROM games")
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

        Queries = Queue()
        web_Scraper_Threads = []
        db_updater_Threads = []

        #spawn 10 database updater threads#
        for i in range(10):
            thread = Thread(target=db_Update, args = (i,Queries))
            db_updater_Threads.append(thread)
            thread.start()
        #spawn a ton of web scraper threads#
        
        i=1
        for gameID in newGameIDs:
            starttime = time.time()
            thread=Thread(target=scrape_GameData_in_parallel, args=(i,gameID,result,players,0,Queries))
            thread.start()
            web_Scraper_Threads.append(thread)
            while(time.time() - starttime < MRT):
                pass
            i += 1
                        
        for thread in web_Scraper_Threads:
            thread.join()
        Queries.put("TERMINATE")
                        
        for thread in db_updater_Threads:
            thread.join()
    scrape_Games()

###############################################################################################################################
###############################################################################################################################
#############################                                                              ####################################
#############################                    BEGIN  PARALLEL FUNCTIONS                 ####################################
#############################                                                              ####################################
###############################################################################################################################
###############################################################################################################################

def get_gameIDs(teamABBR,the_Game_IDs,attempt,the_year):
    try:
        print(str(teamABBR) + " " + str(the_year))
        baseurl = "http://espn.go.com/nba/team/_/name/"
        ################################################
        #               Scrape pre-season              #
        ################################################
        # the_year must be a 4 digit integer
        toscrape = "http://espn.go.com/nba/team/schedule/_/name/"+ teamABBR + "/year/ " + str(the_year) + "/"
        soup = BeautifulSoup(urllib.request.urlopen(toscrape, timeout = 150).read())
        for link in soup.find_all('a'):
            thelink = str(link.get('href'))
            y = "/nba/recap?id="
            x = thelink.find(y)
            if(x!= -1):
                gameID = thelink[-9:]
                the_Game_IDs.put(gameID)

        ################################################
        #           Scrape regular season              #
        ################################################
        regular_season = toscrape + "seasontype/2/"
        soup = BeautifulSoup(urllib.request.urlopen(regular_season, timeout = 150).read())
        for link in soup.find_all('a'):
            thelink = str(link.get('href'))
            y = "/nba/recap?id="
            x = thelink.find(y)
            if(x!= -1):
                gameID = thelink[-9:]
                the_Game_IDs.put(gameID)
        endtime = time.time()
        ################################################
        #               Scrape POST season             #
        ################################################
        post_season = toscrape + "seasontype/3/"
        soup = BeautifulSoup(urllib.request.urlopen(post_season, timeout = 150).read())
        for link in soup.find_all('a'):
            thelink = str(link.get('href'))
            y = "/nba/recap?id="
            x = thelink.find(y)
            if(x!= -1):
                gameID = thelink[-9:]
                the_Game_IDs.put(gameID)
    #THREAD EXCEPTION HANDLING
    except timeout:
        print("URL timeout. Restarting.")
        get_gameIDs(teamABBR,the_Game_IDs,attempt+1, the_year)
    except urllib.error.URLError as e:
        print("URL Timeout Error. Restarting")
        get_gameIDs(teamABBR,the_Game_IDs,attempt+1, the_year)
    except ConnectionResetError as e:
        print("Connection got reset. Calling back.")
        get_gameIDs(teamABBR,the_Game_IDs,attempt+1, the_year)

    except OSError as e:
        if(e.errno == errno.EHOSTUNREACH):
            print("Host Unreachable. Trying again.")
            Get_GameIDs_In_Parallel(teamABBR,the_Game_IDs,attempt+1)
        if(e.errno == errno.ECONNRESET):
            print("Host hung up. Calling back.")
            Get_GameIDs_In_Parallel(teamABBR,the_Game_IDs,attempt+1)
        else:
            print(e.args)
            print(e[0])
    except Exception as e:
        print("idk what just happened.")
        print(e.arg)


##############################################################################################
#                                                                                            #
#                       Begin Scraping the Games for their Content.                          #
#               We have 15 db updater threads and numGameIDs scraper threads                 #
#                                                                                            #
##############################################################################################
def db_Update(i,Queries):
    print("Database Updater thread " + str(i) + " has began.")
    con = lite.connect('predict.db', isolation_level = None)
    cur = con.cursor()
    while(True):
        if(Queries.qsize == 0):
            pass
        else:
            data = Queries.get()
            if(data == "TERMINATE"):
                Queries.put(data)
                print("Database Updater thread " + str(i) + " has terminated.")
                break
            try:
                for input_set in data:
                    if(input_set[0] == "Players"):
                        cur.executemany("INSERT OR IGNORE INTO players VALUES(?,?,?,?,?,?)",input_set[1])
                    if(input_set[0] == "GameData"):
                        cur.executemany("INSERT OR IGNORE INTO gameData VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,'NULL','NULL')",input_set[1])
                    if(input_set[0] == "Games"):
                        gameid = input_set[1][0]
                        cur.execute("INSERT INTO games VALUES(?,?,?,?,?,?, 'NULL','NULL')",input_set[1])
                con.commit()
                print("Inserted Game with ID : " + str(gameid))
            except Exception as e:
                Queries.put(data)
                #print("Error handling database update. " + str(e) + " " + repr(data))

def scrape_GameData_in_parallel(i, gameID, result, players, attempt, Queries):
    print(str(i) + " " + str(gameID) + " " + repr(result) + " " + repr(players) + " " + str(attempt) + " " + repr(Queries))
    gameData_Insert_Queries = []
    games_Insert_Queries = []
    players_Insert_Queries = []

    if(attempt == 4):
        print("Tried 4 times. It ain't working bro." + str(gameID))
        return 0
    try:
        url = "http://espn.go.com/nba/boxscore?gameId=" + str(gameID)
        soup = BeautifulSoup(urllib.request.urlopen(url, timeout = 250).read())
        #print(soup.getText())
        base_href = "http://espn.go.com/nba/player/_/id/"
        links = soup.find_all('a')
        for link in links:
            url = str(link.get('href'))
            #############################################################
            #   Put player URL into output, create players table entry  #
            #############################################################
            if(url.find(base_href) != -1):
                players.put(url)
                uniques = url.replace(base_href,"")
                breakpoint = uniques.find("/")
                playerID = uniques[:breakpoint]
                player_Name = uniques[breakpoint+1:]
                player_Name = player_Name.replace("-"," ")
                player_Name = player_Name.replace("'","")
                playerIDarray = []
                playerIDarray.append(playerID)

                sqlformat = "?,NULL,NULL,NULL,NULL,NULL"
                sqlupdate = sqlupdate = convert_Str_To_Tuple(sqlformat,playerIDarray)
                players_Insert_Queries.append(sqlupdate)
        
        team1 = team()
        team2 = team()  

        matchup = soup.find('div', {"class" : "matchup"})
        if(not matchup):
            matchup = soup.find('div', {"class" : "matchup "})
    
        firstteam = matchup.find('div', {"class" : "team away"})
        secondteam = matchup.find('div', {"class" : "team home"})

        team1link = firstteam.find('a')
        team2link = secondteam.find('a')

        firstinfo = firstteam.find('div', { "class" : "team-info"})
        secondinfo = secondteam.find('div', { "class" : "team-info"})

        team1scorespan = firstinfo.find('span')
        team2scorespan = secondinfo.find('span')

        team1score = team1scorespan.getText()
        team2score = team2scorespan.getText()


        months = {'January' : 1, 'February' : 2, 'March' : 3, 'April' : 4, 'May' : 5, 'June' : 6, 'July' : 7, 'August' : 8, 'September' : 9, 'October' : 10, 'November' : 11, 'December' : 12} 
        date_div = soup.find('div', { "class" : "game-time-location" })
        date = (date_div.find('p')).getText()
        x = date.find(",")
        if(x!= -1):
            date = date[x+2:]
        x = date.find(" ")
        if(x!= -1):
            month = date[:x]
            day_year = date[x+1:]
        else:
            print(date)
        x = day_year.find(",")
        if(x!= -1):
            day = day_year[:x]
            year = day_year[x+2:]
        else:
            print(day_year)

        if(len(str(day)) == 1):
            day = "0" + str(day)
        if(len(str(months[month])) == 1):
            themonth = "0" + str(months[month])
        else:
            themonth = str(months[month])

        date_of_game = str(year) + "-" + themonth + "-" + day

        g = "http://espn.go.com/nba/team/_/name/"
        if not team1link:
            team1Name = (firstinfo.find('h3')).getText()
            if(team1Name.find("Bobcats") != -1):
                team1abbr = "cha"
        else:
            team1href = team1link.get('href')
            team1href = team1href.replace(g,"")
            x = team1href.find("/")
            team1abbr = team1href[:x]
        if not team2link:
            team2Name = (secondinfo.find('h3')).getText()
            if(team2Name.find("Bobcats") != -1):
                team2abbr = "cha"
        else:
            team2href = team2link.get('href')       
            team2href = team2href.replace(g,"")
            y = team2href.find("/")
            team2abbr = team2href[:y]

        tbodies = soup.find_all('tbody')

        if(len(tbodies) == 6):
            #print(gameID)
            bodies = []
            bodies.extend(tbodies[:2])
            bodies.extend(tbodies[3:5])
        else:
            bodies = []
            bodies.extend(tbodies[:])
        
        body_count = 0
        for body in bodies:
            trs = body.find_all('tr')
            for tr in trs:
                playerLink = tr.find('a')
                playerLink = str(playerLink.get('href'))
                uniques = playerLink.replace(base_href,"")
                breakpoint = uniques.find("/")
                playerID = uniques[:breakpoint]
                tds = tr.find_all('td')
                temporary_player = player()
                temporary_player.add_ID(playerID)
                for td in tds:
                    temporary_player.add_data(td.getText())
                temporary_player.fix_gameData()
                ##we are on the first team
                if(body_count < 2):
                    temporary_player.teamAbbr = team1abbr
                else:
                    temporary_player.teamAbbr = team2abbr

                query = []
                query.append(gameID)
                query.append(temporary_player.teamAbbr)
                query.append(temporary_player.id)
                query.append(temporary_player.name)

                player_key = []
                player_key.append(gameID)
                player_key.append(temporary_player.id)

                if(len(temporary_player.game_data) > 0 and len(temporary_player.game_data) < 5):
                    query.append(temporary_player.game_data[0])
                    
                    sqlformat = "?,?,?,?,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,?"
                    sqlupdate = sqlupdate = convert_Str_To_Tuple(sqlformat,query)
                    gameData_Insert_Queries.append(sqlupdate)

                else:
                    for data in temporary_player.game_data:
                        query.append(data)
                    
                    if(len(query) == 20):
                        sqlformat = "?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,NULL,?,NULL"
                        sqlupdate = convert_Str_To_Tuple(sqlformat,query)
                        gameData_Insert_Queries.append(sqlupdate)
                    
                    elif(len(query) == 21):
                        sqlformat = "?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,NULL"
                        sqlupdate = sqlupdate = convert_Str_To_Tuple(sqlformat,query)
                        gameData_Insert_Queries.append(sqlupdate)
                        
            body_count +=1
        
        #UPDATE THE GAMES TABLE IN DATABASE#
        to_insert = []
        to_insert.append(gameID)
        to_insert.append(date_of_game)
        to_insert.append(team1abbr)
        to_insert.append(team2abbr)
        to_insert.append(team1score)
        to_insert.append(team2score)
        players_Insert_Queries = tuple(players_Insert_Queries)
        gameData_Insert_Queries = tuple(gameData_Insert_Queries)
        Encapsulated_Game_Data = []
        Encapsulated_Game_Data.append(['GameData',gameData_Insert_Queries])
        Encapsulated_Game_Data.append(['Players',players_Insert_Queries])
        Encapsulated_Game_Data.append(['Games',to_insert])
        Queries.put(Encapsulated_Game_Data)
    #Thread Exception Handling
    except timeout:
        print("Timeout.")
        scrape_GameData_in_parallel(i, gameID, result, players, attempt + 1, Queries)
    except urllib.error.URLError as e:
        print("URL Timeout." + str())
        scrape_GameData_in_parallel(i, gameID, result, players, attempt + 1, Queries)
    except ConnectionResetError as e:
        print("Connection Reset by Host.")
        scrape_GameData_in_parallel(i, gameID, result, players, attempt + 1, Queries)
    except OSError as e:
        if(e.errno == errno.EHOSTUNREACH):
            print("Host Unreachable. Trying again.")
            scrape_GameData_in_parallel(i, gameID, result, players, attempt + 1, Queries)
        if(e.errno == errno.ECONNRESET):
            print("Host hung up. Calling back.")
            scrape_GameData_in_parallel(i, gameID, result, players, attempt + 1, Queries)
    except IndexError as e:
        print(str(e) + " " + str(gameID))

##############################################################################################
#                                                                                            #
#                       Scrape Player Webpages for Weight+Height                             #
#                                                                                            #
##############################################################################################
if __name__ == '__main__':
    main()