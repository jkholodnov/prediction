#!/usr/bin/python
#test program

import urllib2

def getAllTeams(): #Get the page that holds all team url pages
	input = urllib2.urlopen('http://espn.go.com/mens-college-basketball/standings')
	f = open("textdatabases/teams.txt",'a')
	print f
	s=input.read()
	f.write(str(s))

if __name__ == '__main__':
    getAllTeams()
