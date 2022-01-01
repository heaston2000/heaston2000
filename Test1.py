# Data Gathering for Arsenal (TEST)

from bs4 import BeautifulSoup
import requests

#def teamPage(url): # given url of a team, retrieves player names and calls playerStats function to get statistics for each player
url = "https://fbref.com/en/squads/18bb7c10/Arsenal-Stats"
result = requests.get(url)     # url to website to get statistics goes here 

src = result.content                    # Get the source code from the website
schedSoup = BeautifulSoup(src, 'lxml')  # Get soup object from the source code

links = schedSoup.find_all("a")         # Get all links on the page


playerLinks = []
players = []
for link in links:
    #print(str(link) + '\n')
    #print(link.text + '\n')
    
    if ("players" in str(link)): # For each players season stats
        
        # INSERT SOMETHING HERE TO PREVENT REPEATS
        if ("matchlogs" in str(link)) and ("summary" in str(link)): # Find the matchlog links in the right general format
            playerLinks.append(link)
            
            # Sort through the matchlog link urls to find the player names and add them to an array of players
            nameEnd        = str(link).find("-Match-Logs")
            nameBeginning  = str(link).find("summary/")
            playerName     = str(link)[(nameBeginning+8):nameEnd] # "8" is the amount of characters in "summary/"
            if not playerName in players:   # To prevent copies of the same player
                players.append(playerName)

playerArray = []
for i in range(len(players)):
    
    playerPage = 'httsp://fbref.com' + playerLinks[i].attrs['href']
    #playerStats(playerPage) # add playerArray.append to the beginning
    
    
    


    
    
        


