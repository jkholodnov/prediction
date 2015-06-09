#!/bin/bash

if [ -e "2015.db" ];
then
	cp 2015.db Scrape_ESPN/predict.db
	cd Scrape_ESPN
else
	cd Scrape_ESPN
	python3 create_tables_nba.py
fi

echo "Beginning to scrape required games."
python3 NBA_Game_Updater.py 2015 #> /dev/null
echo "Completed game scraping."
cp predict.db ../2015.db
cd ../Team_Elo && make && ./Team_Elo_Executable 1 0 1
