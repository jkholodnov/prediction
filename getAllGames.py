import urllib2
import sys
from Queue import *
from threading import Thread

def chunks(l, n): #cuts the work into blocks
    	return [l[i:i+n] for i in range(0, len(l), n)]


def getAllGamesHtml():
	print "Generating gameurls on 30 threads..."
	numThreads=30
	jobs = []
	with open("textdatabases/gameUrlsParsed.txt", "rU") as f:
		for line in f:
			jobs.append(line)

	subs = chunks(jobs,4);
	queue = Queue()
	result = Queue()

	for i, item in enumerate(subs): #puts all of the subjobs into the queue. These subjobs get grabbed by the threads
		queue.put(item)

	threads = []
	for i in range(numThreads):
		t=Thread(target=getSingleGame, args=(i,queue,result))
		t.daemon = True
		t.start()
		threads.append(t)

	for t in threads:
		t.join()
	
	f = open("textdatabases/websitedata1.txt",'a')

	while not result.empty():
		x = result.get()
		for html in x:
			f.write(html)


def getSingleGame(i, q,result):
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
		print "Games left to be scraped : " + str(q.qsize() * 4)


# you can put all of this into a class, but either way use this

# call this with:
# std::string command;
# command = "python some/path/getData.py " + arg1 + " " + arg2; std::system (command.c_str());

# when cpp calls the function above this is what is executed

if __name__ == '__main__':
    getAllGamesHtml()
