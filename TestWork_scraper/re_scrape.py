from bs4 import BeautifulSoup
import urllib3
import time


url_pages = ["https://www.realtor.com/realestateandhomes-search/New-York_NY/"]
prmsls = ['span.data-price', 'li[data-label~=property-meta-sqft] > span','span.data-value.meta-beds', 'li[data-label~=property-meta-baths] > span']
row = 0
scrapemat = []
urllib3.disable_warnings()


#This function selects the number out of
#the string of the parameter without repeatedly writing the loop. It is faster
# and I don't repeat myself over time.
def selectparam(location, data):
    loc_str = ''
    for letter in data.select(location)[0].string:
        if letter.isdigit():
            loc_str = loc_str + letter
    return int(loc_str)

def con_sqft_to_sqm():
    for i in range(0, len(scrapemat) - 1):
        scrapemat[i][1] = int(scrapemat[i][1] / 10.764)

#Function to quickly scrape the real estates I need.   
def totally_scrape(chosen_url):
    global row
    datana = BeautifulSoup(urllib3.PoolManager().request('GET',chosen_url).data, "lxml")
    reidcard = datana.select('div.detail-wrap')
    for i in range(0,len(reidcard)-1):
        if reidcard[i].select(prmsls[0]) != [] and reidcard[i].select(prmsls[1]) != [] and reidcard[i].select(prmsls[2]) != [] and reidcard[i].select(prmsls[3]) != []:
            scrapemat.append([])
            for l in range(0, 4):
                scrapemat[row].append(selectparam(prmsls[l], reidcard[i]))
            row += 1
    time.sleep(20)   
    
def KeepData():
    with open('re_nys.txt', 'w') as database:
        database.write('4:' + str(len(scrapemat)) + '\n')
        for i in range(0, len(scrapemat)):
            b = i+1
            for j in range(0,4):
                database.write(str(scrapemat[i][j]) + ',')
            database.write('#' + str(b) + '\n')
    
def _init_():
    print('Scraper - Scraping page ' + str(1) +'...')
    totally_scrape(url_pages[0])
    print('Scraper - Scraping page ' + str(1) + ' successful')
    for i in range(2, 207):  #Just commenting this section out to test out the program.
        url_pages.append(str("https://www.realtor.com/realestateandhomes-search/New-York_NY/pg-"+str(i)))
        print('Scraper - Scraping page ' + str(i) +'...')
        totally_scrape(url_pages[i-1])
        print('Scraper - Scraping page ' + str(i) + ' successful.')
    con_sqft_to_sqm()
    print(scrapemat)
    KeepData()
    print('Scraping Done !')
    
_init_()
