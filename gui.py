#!/usr/bin/python
from Tkinter import *
import Tkinter as tk
import ttk
import subprocess
import sys
import os

# include c++ cout syntax
class ostream:
    def __init__(self, file):
        self.file = file
        
    def __lshift__(self, obj):
        self.file.write(str(obj));
        return self
cout = ostream(sys.stdout)
cerr = ostream(sys.stderr)
endl = '\n'

def frame(root, side):
    w = Frame(root)
    w.pack(side=side, expand=YES, fill=BOTH)
    return w

def button(root, side, text, command=None):
	w = Button(root, text=text, command=command)
	w.pack(side=side, expand=YES, fill=BOTH)
	return w
#main class for gui
class Predictor(Frame):
	def __init__(self):
		Frame.__init__(self)
		self.pack(expand=YES, fill=BOTH)
		
		self.master.title("Basketball Predictor")
		self.master.iconname("Basketball Predictor")
        #self is a master frame

        #START OF FRAME F
		f=Frame(self, height=500,width=700)
		f.pack_propagate(0)
		l1 = Label(f,text="Select Teams").grid(row=0,column=0)
		l2 = Label(f,text="Winner").grid(row=4,column=0)
		l3 = Label(f,text="Team Value").grid(row=0,column=1)
		l4 = Label(f,text="Difference").grid(row=4,column=1)


		team1=StringVar()
		box = ttk.Combobox(f, textvariable=team1, state='normal')
		box['values'] = ("abilene_christian","air_force","akron","alabama_a&m","alabama","alabama_state", "albany","alcorn_state","alice_lloyd_college","allen","american_u","anderson_ind","appalachian_st","arizona","arizona_state","arkansas","arkansas_state","ar-little_rock","army","ar-pine_bluff","asbury","aub.-montgomery","auburn","austin_peay","ball_state","barber-scotia","baruch_college","baylor","belhaven","belmont","bethel_(tn)","bethune-cookman","binghamton","black_hills_st","boise_state","boston_college","boston_u","bowling_green","bradley","brescia","brewton","brown")
		box.current(0)
		box.grid(column=0, row=1)

		team2=StringVar()
		box = ttk.Combobox(f, textvariable=team2, state='normal')
		box['values'] = ("abilene_christian","air_force","akron","alabama_a&m","alabama","alabama_state", "albany","alcorn_state","alice_lloyd_college","allen","american_u","anderson_ind","appalachian_st","arizona","arizona_state","arkansas","arkansas_state","ar-little_rock","army","ar-pine_bluff","asbury","aub.-montgomery","auburn","austin_peay","ball_state","barber-scotia","baruch_college","baylor","belhaven","belmont","bethel_(tn)","bethune-cookman","binghamton","black_hills_st","boise_state","boston_college","boston_u","bowling_green","bradley","brescia","brewton","brown")
		box.current(0)
		box.grid(column=0, row=3)
		winner=StringVar()
		t1value=StringVar()
		t2value=StringVar()
		difference = StringVar()

		Message(f, relief=SUNKEN, textvariable=winner, width=300).grid(row=5,column=0)
		Message(f, relief=SUNKEN, textvariable=difference, width=150).grid(row=5,column=1)
		Message(f, relief = SUNKEN, textvariable = t1value, width=150).grid(row=1, column=1)
		Message(f, relief = SUNKEN, textvariable = t2value, width=150).grid(row=3, column=1)

		##DONE WITH FRAME F

		#START OF FRAME G
		g=Frame(self,height=300,width=700)
		

		checkvar1 = StringVar()
		checkvar2 = StringVar()
		checkvar3 = StringVar()
		checkvar4 = StringVar()
		checkvar5 = StringVar()
		checkvar6 = StringVar()
		l5 = Label(g,text="Select how much to parse.").grid(row=0,column=0)
		c1 = Checkbutton(g, text="Get Team Urls", variable = checkvar1, onvalue = "1", offvalue = "0", height=0, width=10)
		c2 = Checkbutton(g, text="Get Game Urls", variable = checkvar2, onvalue = "1", offvalue = "0", height=0, width=10)
		c3 = Checkbutton(g, text="Get BPI", variable = checkvar3, onvalue = "1", offvalue = "0", height=0, width=10)
		c4 = Checkbutton(g, text="Get Game Data", variable = checkvar4, onvalue = "1", offvalue = "0", height=0, width=10)
		c5 = Checkbutton(g, text="Get Team Games", variable = checkvar5, onvalue = "1", offvalue = "0", height=0, width=10)
		c6 = Checkbutton(g, text="Get Team Avgs", variable = checkvar6, onvalue = "1", offvalue = "0", height=0, width=10)
		c1.deselect()
		c2.deselect()
		c3.deselect()
		c4.deselect()
		c5.deselect()
		c6.deselect()
		c1.grid(row=1,column=0,sticky=W)
		c2.grid(row=2,column=0,sticky=W)
		c3.grid(row=3,column=0,sticky=W)
		c4.grid(row=4,column=0,sticky=W)
		c5.grid(row=5,column=0,sticky=W)
		c6.grid(row=6,column=0,sticky=W)
		g.grid(row=0, column =0,sticky=W)
		f.grid(row=0, column =1,sticky=W)
		h = Frame(self,height=1000, width=1200)
		j = Frame(self,height=1000, width=1200)
		#buttonsF = frame(self, TOP)

		b2=button(h,LEFT,'Parse Data')
		b2.bind('<ButtonRelease-1>',
				lambda e, s=self:s.parse(checkvar1,checkvar2,checkvar3,checkvar4,checkvar5,checkvar6))

		b1=button(j,LEFT,'Predict')
		b1.bind('<ButtonRelease-1>',
				lambda e, s=self:s.predict(team1,team2,winner,t1value,t2value,difference))
		h.grid(row=1, column =0)
		j.grid(row=1, column =1)
	def predict(self,team1,team2,winner,t1value,t2value,difference):
		if(len(team1.get())>0):
			if(len(team2.get())>0):
				args = team1.get()+" "+team2.get()
				#cout << args << endl
				process.stdin.write(args+'\n')
				t1v=process.stdout.readline()
				t2v=process.stdout.readline()
				winningteam=process.stdout.readline()
				t1value.set(t1v.rstrip())
				t2value.set(t2v.rstrip())
				x=t1v.rstrip()
				y=t2v.rstrip()
				x = float(x)
				y = float(y)

				if(x>y):
					v = x-y
					format(v,'.2f')
					difference.set(v)
				else:
					v = y-x
					format(v,'.2f')
					difference.set(v)
				winner.set(winningteam.rstrip())

	def parse(self,checkvar1,checkvar2,checkvar3,checkvar4,checkvar5,checkvar6):
		args = "./parse " + checkvar1.get() + " " + checkvar2.get() + " " +checkvar3.get() + " " + checkvar4.get() + " " + checkvar5.get() + " " +checkvar6.get()
		cout << args << endl
		os.system(args)
if __name__ == '__main__':
	process = subprocess.Popen('./compare',shell=True,
							stdin=subprocess.PIPE,
							stdout=subprocess.PIPE,
							stderr=subprocess.PIPE)
	Predictor().mainloop()