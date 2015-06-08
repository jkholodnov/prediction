import urllib.request
import urllib.error
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


Queries = Queue()  # The result of the algorithm onto the queue items


def scrape_GameData_in_parallel(gameID, attempt, Queries):
    print(gameID)
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

        def add_ID(self, x):
            self.id = x

        def add_data(self, x):
            self.game_data.append(x)

        def change_name(self, name):
            self.name = name

        def height_and_weight(self, ht, wt):
            self.weight = wt

        def sanitize_data(self):
            # This function takes the raw TD data and extracts the name and
            # position from it.
            newData = []
            for i in range(len(self.game_data)):
                position = str(self.game_data[i]).find(",")
                # fix the name and position
                if(position != -1):
                    if(self.position == None):
                        pos = str(self.game_data[i])[position + 1:]
                        temp_name = str(self.game_data[i])[:position]
                        temp_name = temp_name.replace("'", "")
                        temp_name = temp_name.replace("-", "")
                        self.name = temp_name
                        self.position = pos.replace(" ", "")
                # put rest of data into dataset.
                else:
                    g = self.game_data[i].find("-")
                    if(g != -1 and g > 0):
                        newData.append(self.game_data[i][:g])
                        newData.append(self.game_data[i][g + 1:])
                    else:
                        newData.append(self.game_data[i])
            self.game_data = newData

    def convert_Str_To_Tuple(str_format, data):
        unformatted_string = str_format.replace("?", "{}")
        formatted_string = unformatted_string.format(*data)
        parameters = formatted_string.split(",")
        return tuple(parameters)

    gameData_Insert_Queries = []
    games_Insert_Queries = []

    if(attempt == 4):
        print("Maximum attempts reached for game id: " + str(gameID))
        return 0

    try:
        url = "http://espn.go.com/nba/boxscore?gameId=" + str(gameID)
        soup = BeautifulSoup(urllib.request.urlopen(url, timeout=250).read())
        # print(soup.getText())

        matchup = soup.find('div', {"class": "matchup"})
        if(not matchup):
            matchup = soup.find('div', {"class": "matchup "})

        firstteam = matchup.find('div', {"class": "team away"})
        secondteam = matchup.find('div', {"class": "team home"})

        team1link = firstteam.find('a')
        team2link = secondteam.find('a')

        firstinfo = firstteam.find('div', {"class": "team-info"})
        secondinfo = secondteam.find('div', {"class": "team-info"})

        team1scorespan = firstinfo.find('span')
        team2scorespan = secondinfo.find('span')

        team1score = team1scorespan.getText()
        team2score = team2scorespan.getText()

        months = {'January': 1, 'February': 2, 'March': 3, 'April': 4, 'May': 5, 'June': 6,
                  'July': 7, 'August': 8, 'September': 9, 'October': 10, 'November': 11, 'December': 12}
        date_div = soup.find('div', {"class": "game-time-location"})
        date = (date_div.find('p')).getText()
        x = date.find(",")
        if(x != -1):
            date = date[x + 2:]
        x = date.find(" ")
        if(x != -1):
            month = date[:x]
            day_year = date[x + 1:]
        else:
            print(date)
        x = day_year.find(",")
        if(x != -1):
            day = day_year[:x]
            year = day_year[x + 2:]
        else:
            print(day_year)

        if(len(str(day)) == 1):
            day = "0" + str(day)
        if(len(str(months[month])) == 1):
            themonth = "0" + str(months[month])
        else:
            themonth = str(months[month])

        date_of_game = str(year) + "-" + themonth + "-" + day

        team1abbr = ""
        team2abbr = ""
        g = "http://espn.go.com/nba/team/_/name/"
        if not team1link:
            team1Name = (firstinfo.find('h3')).getText()
            if team1Name.find("Bobcats") != -1:
                team1abbr = "cha"
            elif team1Name.find("Nets") != -1:
                team1abbr = "bkn"
            elif team1Name.find("Hornets") != -1:
                team1abbr = "no"
        else:
            team1href = team1link.get('href')
            team1href = team1href.replace(g, "")
            x = team1href.find("/")
            team1abbr = team1href[:x]


        if not team2link:
            team2Name = (secondinfo.find('h3')).getText()
            if team2Name.find("Bobcats") != -1:
                team2abbr = "cha"
            elif team2Name.find("Nets") != -1:
                team2abbr = "bkn"
            elif team2Name.find("Hornets") != -1:
                team2abbr = "no"
        else:
            team2href = team2link.get('href')
            team2href = team2href.replace(g, "")
            y = team2href.find("/")
            team2abbr = team2href[:y]
           

        tbodies = soup.find_all('tbody')

        if(len(tbodies) == 6):
            # print(gameID)
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
                if playerLink is not None:
                    playerLink = str(playerLink.get('href'))
                    base_href = "http://espn.go.com/nba/player/_/id/"
                    uniques = playerLink.replace(base_href, "")
                    breakpoint = uniques.find("/")
                    playerID = uniques[:breakpoint]
                    tds = tr.find_all('td')
                    temporary_player = player()
                    temporary_player.add_ID(playerID)
                    for td in tds:
                        temporary_player.add_data(td.getText())
                    temporary_player.sanitize_data()
                    # we are on the first team
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
                        sqlupdate = sqlupdate = convert_Str_To_Tuple(
                            sqlformat, query)
                        gameData_Insert_Queries.append(sqlupdate)

                    else:
                        for data in temporary_player.game_data:
                            query.append(data)

                        if(len(query) == 20):
                            sqlformat = "?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,NULL,?,NULL"
                            sqlupdate = convert_Str_To_Tuple(sqlformat, query)
                            gameData_Insert_Queries.append(sqlupdate)

                        elif(len(query) == 21):
                            sqlformat = "?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,NULL"
                            sqlupdate = convert_Str_To_Tuple(sqlformat, query)
                            gameData_Insert_Queries.append(sqlupdate)

            body_count += 1

        #UPDATE THE GAMES TABLE IN DATABASE#
        to_insert = []
        to_insert.append(gameID)
        to_insert.append(date_of_game)
        to_insert.append(team1abbr)
        to_insert.append(team2abbr)
        to_insert.append(team1score)
        to_insert.append(team2score)
        gameData_Insert_Queries = tuple(gameData_Insert_Queries)

        Queries.put(['GameData', gameData_Insert_Queries])
        Queries.put(['Games', to_insert])

    # Thread Exception Handling
    except timeout:
        print("Timeout.")
        scrape_GameData_in_parallel(gameID, attempt + 1, Queries)

    except urllib.error.URLError as e:
        print(e.reason)
        #print("URL Timeout." + str())
        scrape_GameData_in_parallel(gameID, attempt + 1, Queries)

    except ConnectionResetError as e:
        print("Connection Reset by Host.")
        scrape_GameData_in_parallel(gameID, attempt + 1, Queries)

    except OSError as e:
        if(e.errno == errno.EHOSTUNREACH):
            print("Host Unreachable. Trying again.")
            scrape_GameData_in_parallel(gameID, attempt + 1, Queries)

        if(e.errno == errno.ECONNRESET):
            print("Host hung up. Calling back.")
            scrape_GameData_in_parallel(gameID, attempt + 1, Queries)
    except IndexError as e:
        print(str(e) + " " + str(gameID))

scrape_GameData_in_parallel(240223005,0,Queries)

while(Queries.qsize() != 0):
    data = Queries.get()
    print(data)