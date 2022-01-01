from bs4 import BeautifulSoup
import requests
import re
import numpy as np

#def playerStats(url):
    
url = "https://fbref.com/en/players/cd1acf9d/matchlogs/2021-2022/summary/Trent-Alexander-Arnold-Match-Logs"
src = requests.get(url).content
playerSoup = BeautifulSoup(src, 'lxml')

table_tags = playerSoup.find_all("tbody")
match_tags = playerSoup.find_all("tr")  # Data within the table

# Big for-loop meant for gathering GENERAL match statistics for a player:

# Turn 'Test' on in order to read the statistics gathered
Test = False 

# The array holds game statistics for a particular player (rows x columns)
pArray = np.empty((1, 32), dtype=np.int8) # change the number of rows if we can find a way to count the number of matches a player is in

for tag in match_tags:
    
    match = str(tag)  
    if 'data-stat="date"' in match and 'csk="' in match: # Pick out each of the "tags" that contain match statistics (middle rows in table)
       
       # Find the match date
       date_ind     = match.find("csk")
       date         = match[date_ind+5:date_ind+13]
       if Test:
           print('\nNew Match-date:', date)
       
       # Find the day of the week
       day_ind      = match.find('"dayofweek"')
       dayofweek    = match[day_ind+12:day_ind+15]
       if Test:
           print('Weekday:', dayofweek)
        
       
       # Find the league the match took place in
       league_ind1  = match.find('" data-stat="comp"')
       league_ind2  = match[league_ind1:].find("</a>")
       tempLeague   = match[league_ind1:(league_ind2+league_ind1)]
       league_ind3  = match[league_ind1:(league_ind2+league_ind1)].rindex('>')
       league       = tempLeague[league_ind3+1:league_ind2]
       if Test:
           print('League:', league)
       
       # Find the round of play the match was
       matchweek_ind        = match.find("Matchweek ")
       if matchweek_ind != -1:
           matchweek_end    = match[matchweek_ind:].find('</a>')
           rownd            = match[matchweek_ind+10:matchweek_ind+matchweek_end]
           if Test:
               print("Round:", rownd)
       
       # Find the venue (home or away the match is in)
       venue_ind    = match.find("venue")
       venue_ind2   = match[venue_ind:].find('</td>')
       venue        = match[venue_ind+7:venue_ind+venue_ind2]
       if Test:
           print('Venue:', venue)
       
       # Find the result to the match (IMPORTANT)
       result_ind1  = match.find('result')
       result_ind2  = result_ind1 + match[result_ind1:].find('>')
       result_end   = result_ind2 + match[result_ind2:].find('</td>')
       result       = match[result_ind2+1:result_end]
       if Test:
           print('Result:',result)
       
       # Find the opponent of the match
       opp_ind1 = match.find('opponent')
       opp_ind2 = opp_ind1 + match[opp_ind1:].find('href')
       opp_bgn  = opp_ind2 + match[opp_ind2:].find('>')
       opp_end  = opp_bgn + match[opp_bgn:].find('<')
       opponent = match[opp_bgn+1:opp_end]
       if Test:
           print('Opponent:',opponent)
        
       # Find out if the player started
       start_ind1   = match.find('game_started')
       start_bgn    = start_ind1 + match[start_ind1:].find('>')
       start_end    = start_bgn + match[start_bgn:].find('<')
       start        = match[start_bgn+1:start_end]
       if Test:
           print('Start?', start)
           
       # Find position player started at
       pos_ind1 = match.find('position')
       pos_bgn = pos_ind1 + match[pos_ind1:].find('>')
       pos_end = pos_bgn + match[pos_bgn:].find('<')
       position = match[pos_bgn+1:pos_end]
       if Test:
           print('Position:', position)
        
       # Find the minutes the player played
       time_ind = match.find('minutes')
       time_bgn = time_ind + match[time_ind:].find('>')
       time_end = time_bgn + match[time_bgn:].find('<')
       playTime = match[time_bgn+1:time_end]
       if Test:
           print('Playing-time:', playTime)
           
       ########## PERORMANCE #############
           
       # Find the number of goals the player scored
       goals_ind = match.find('goals')
       goals_bgn = goals_ind + match[goals_ind:].find('>')
       goals_end = goals_bgn + match[goals_bgn:].find('<')
       goals     = match[goals_bgn+1:goals_end]
       if Test:
           print('Goals:', goals)
           
       # Find the number of assists the player had
       asst_ind = match.find('assists')
       asst_bgn = asst_ind + match[asst_ind:].find('>')
       asst_end = asst_bgn + match[asst_bgn:].find('<')
       assists  = match[asst_bgn+1:asst_end]
       if Test:
           print('Assists:', assists)
           
       # PKs taken v. made should be a team statistic only!
       
       # Find the total shots taken by the player
       shts_ind = match.find('shots_total')
       shts_bgn = shts_ind + match[shts_ind:].find('>')
       shts_end = shts_bgn + match[shts_bgn:].find('<')
       shots    = match[shts_bgn+1:shts_end]
       if Test:
           print('Shots taken:', shots)
           
       # Find the amount of shots on target by the player
       sots_ind = match.find('shots_on_target')
       sots_bgn = sots_ind + match[sots_ind:].find('>')
       sots_end = sots_bgn + match[sots_bgn:].find('<')
       onTarget = match[sots_bgn+1:sots_end]
       if Test:
           print('Shots on target:', onTarget)
           
       # Find amount of yellow cards recevied
       crdY_ind = match.find('cards_yellow')
       crdY_bgn = crdY_ind + match[crdY_ind:].find('>')
       crdY_end = crdY_bgn + match[crdY_bgn:].find('<')
       yellows  = match[crdY_bgn+1:crdY_end]
       if Test:
            print('Yellow cards taken:', yellows)
            
       # Find amt of red cards received
       crdR_ind = match.find('cards_red')
       crdR_bgn = crdR_ind + match[crdR_ind:].find('>')
       crdR_end = crdR_bgn + match[crdR_bgn:].find('<')
       reds     = match[crdR_bgn+1:crdR_end]
       if Test:
           print('Red cards taken:', reds)
           
       # Find the amount of touches taken by the player
       tchs_ind = match.find('touches')
       tchs_bgn = tchs_ind + match[tchs_ind:].find('>')
       tchs_end = tchs_bgn + match[tchs_bgn:].find('<')
       touches  = match[tchs_bgn+1:tchs_end]
       if Test:
           print('Touches:', touches)
           
       # Find the amount of times the player applied pressure to the ball
       pres_ind = match.find('pressures')
       pres_bgn = pres_ind + match[pres_ind:].find('>')
       pres_end = pres_bgn + match[pres_bgn:].find('<')
       presses  = match[pres_bgn+1:pres_end]
       if Test:
           print('Applied pressure:', presses)
           
       # Find the amount of tackles given
       tckl_ind = match.find('tackles')
       tckl_bgn = tckl_ind + match[tckl_ind:].find('>')
       tckl_end = tckl_bgn + match[tckl_bgn:].find('<')
       tackles  = match[tckl_bgn+1:tckl_end]
       if Test:
           print('Tackles:', tackles)
           
       # Find the number of blocks
       blck_ind = match.find('blocks')
       blck_bgn = blck_ind + match[blck_ind:].find('>')
       blck_end = blck_bgn + match[blck_bgn:].find('<')
       blocks   = match[blck_bgn+1:blck_end]
       if Test:
           print('Blocks:', blocks)
           
       # Find the number of interceptions
       ints_ind         = match.find('interception')
       ints_bgn         = ints_ind + match[ints_ind:].find('>')
       ints_end         = ints_bgn + match[ints_bgn:].find('<')
       interceptions    = match[ints_bgn+1:ints_end]
       if Test:
           print('Interceptions:', interceptions)
           
       ############## Creating Actions #############
       
       # Find the number of shot creating actions
       SCAs_ind     = match.find('sca')
       SCAs_bgn     = SCAs_ind + match[SCAs_ind:].find('>')
       SCAs_end     = SCAs_bgn + match[SCAs_bgn:].find('<')
       shotsCreated = match[SCAs_bgn+1:SCAs_end]
       if Test:
           print('Shot-creating actions:', shotsCreated)
           
       # Find the number of goal-creating actions
       GCAs_ind     = match.find('gca')
       GCAs_bgn     = GCAs_ind + match[GCAs_ind:].find('>')
       GCAs_end     = GCAs_bgn + match[GCAs_bgn:].find('<')
       goalsCreated = match[GCAs_bgn+1:GCAs_end]
       if Test:
           print('Goal-creating actions:', goalsCreated)
           
       ########### Passing Stats #############
       
       # Find completed passes
       cmps_ind     = match.find('passes_completed')
       cmps_bgn     = cmps_ind + match[cmps_ind:].find('>')
       cmps_end     = cmps_bgn + match[cmps_bgn:].find('<')
       completions  = match[cmps_bgn+1:cmps_end]
       if Test:
           print('Passes completed:', completions)
           
       # Find attempted passes
       atts_ind = match.find('\"passes\"')
       atts_bgn = atts_ind + match[atts_ind:].find('>')
       atts_end = atts_bgn + match[atts_bgn:].find('<')
       attempts = match[atts_bgn+1:atts_end]
       if Test:
           print('Passes attempted:', attempts)
           
       # Calculate pass completion ratio
       if int(attempts) != 0:
           passRatio = int(completions)/int(attempts)
       else:
           passRatio = 0
       if Test:
           print('Completions ratio:', passRatio)
           
       # Find number of progressive passes
       prgs_ind     = match.find('progressive_passes')
       prgs_bgn     = prgs_ind + match[prgs_ind:].find('>')
       prgs_end     = prgs_bgn + match[prgs_bgn:].find('<')
       progPasses   = match[prgs_bgn+1:prgs_end]
       if Test:
           print('Progressive passes:', progPasses)
           
       ############# Dribble Statistics ############
           
       # Find the number of successful dribbles
       sDrbs_ind        = match.find('dribbles_completed')
       sDrbs_bgn        = sDrbs_ind + match[sDrbs_ind:].find('>')
       sDrbs_end        = sDrbs_bgn + match[sDrbs_bgn:].find('<')
       successDribbles  = match[sDrbs_bgn+1:sDrbs_end]
       if Test:
           print('Successful dribbles:', successDribbles)
           
       # Find the number of attempted dribbles
       aDrbs_ind        = match.find('\"dribbles\"')
       aDrbs_bgn        = aDrbs_ind + match[aDrbs_ind:].find('>')
       aDrbs_end        = aDrbs_bgn + match[aDrbs_bgn:].find('<')
       attemptDribbles  = match[aDrbs_bgn+1:aDrbs_end]
       if Test:
           print('Attempted dribbles:', attemptDribbles)
           
       # Calculate the ratio of successful dribbles
       if int(attemptDribbles) != 0:
           dribbleRatio = int(successDribbles)/int(attemptDribbles)
       else:
           dribbleRatio = 0
       if Test:
           print('Dribble success ratio:', dribbleRatio)
       
       ############ Carry Statistics ############
       
       # Find the number of total carries
       crry_ind = match.find('carries')
       crry_bgn = crry_ind + match[crry_ind:].find('>')
       crry_end = crry_bgn + match[crry_bgn:].find('<')
       carries  = match[crry_bgn+1:crry_end]
       if Test:
           print('Carries:', carries)
           
       # Find the number of progressive carries
       pcry_ind    = match.find('progressive_carries')
       pcry_bgn    = pcry_ind + match[pcry_ind:].find('>')
       pcry_end    = pcry_bgn + match[pcry_bgn:].find('<')
       progCarries = match[pcry_bgn+1:pcry_end]
       if Test:
           print('Progressive carries:', progCarries)
           
       # Caclulate the ratio of progressive carries
       if int(carries) != 0:
           carryRatio = int(progCarries)/int(carries)
       else:
           carryRatio = 0
       if Test:
           print('Progressive carry ratio:', carryRatio)
           
       
       
       
       
       
       '''
       # Possibly a simpler way, but not able to make unique how each particular statistic is gathered
       stats = tag.find_all("td")
       for tag in stats:
           print(str(tag['data-stat']), ' ')
           if "csk" in str(tag):
               print(str(tag['csk']), '\n')
       '''
        
       
       




    
    

# playerStats('https://fbref.com/en/players/2628fd2b/matchlogs/2021-2022/summary/Bernd-Leno-Match-Logs')