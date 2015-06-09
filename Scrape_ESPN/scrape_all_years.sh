#!/bin/bash
years=('2003' '2004' '2005' '2006' '2007' '2008' '2009' '2010' '2011' '2012' '2013' '2014' '2015');
for i in "${years[@]}"
do
	:

	if [ -e "predict.db" ];
	then
		rm predict.db
	fi

	python3 create_tables_nba.py
	echo Beginning to scrape season $i
	python3 NBA_Game_Updater.py $i > /dev/null
	mv predict.db ../databases/$i.db
	#mpg123 ../alarm.mp3 > /dev/null
done