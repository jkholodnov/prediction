import urllib2
import sys
from Queue import *
from threading import Thread

def chunks(l, n): #cuts the work into blocks
    	return [l[i:i+n] for i in range(0, len(l), n)]

def getAllGames(): #get each team's url page, put them into one single textfile
	print "Generating gameurls on 20 threads..."
	numThreads=20
	jobs = []
	with open("textdatabases/teamurls.txt", "rU") as f:
		
		for line in f:#Read in each line(line contains the team urls)
			jobs.append(line)
			#getTeamPage(line)
	subs = chunks(jobs,4);
	queue = Queue()
	result = Queue()

	for i, item in enumerate(subs): #puts all of the subjobs into the queue. These subjobs get grabbed by the threads
		queue.put(item)

	threads = []
	for i in range(numThreads):
		t=Thread(target=getTeamPage, args=(i,queue,result))
		t.daemon = True
		t.start()
		threads.append(t)

	for t in threads:
		t.join()
	
	f = open("textdatabases/games.txt",'a')

	while not result.empty():
		x = result.get()
		for html in x:
			f.write(html)

def getTeamPage(i, q,result): #Take the url, open and store the contents in games.txt
	print "Starting Thread: " + str(i)
	def doWork(jobs,result):
		html = []
		for job in jobs:	
			input = urllib2.urlopen(str(job))
			s=input.read()
			html.append(str(s))
		result.put(html)
	while not q.empty():
		params = q.get()
		doWork(params,result)
		print "Teams in queue : " + str(q.qsize() * 4)

if __name__ == '__main__':
    getAllGames()
