#!/bin/bash
cp 2015.db Scrape_ESPN/predict.db
cd Scrape_ESPN
echo "Beginning to scrape required games."
python3 NBA_Game_Updater.py >/dev/null
echo "Completed game scraping."
cp predict.db ../2015.db
cd ../Team_Elo && make && ./Team_Elo_Executable 1 0 1
