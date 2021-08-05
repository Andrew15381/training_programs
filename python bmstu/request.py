import requests as r
from bs4 import BeautifulSoup as bs

def __search_page_to_urls__(content):
    '''Collects one search page links to get full vacancy info'''
    soup = bs(content, 'html.parser')
    links = soup.find_all('a', attrs={'data-qa': 'vacancy-serp__vacancy-title'})
    #First link is payed and does not match filter
    urls = [link['href'] for link in links[1:]]
    return urls

def __search_pages_to_urls__(session, search_text):
    '''Generates links to all vacancies that match filter (per page)'''
    base_url = 'https://hh.ru/search/vacancy'
    params = {
        'text': search_text,
        'search_field': 'name',
        'currency_code': 'RUR',
        'only_with_salary': 'true',
        'experience': 'doesNotMatter',
        'order_by': 'publication_time',
        'search_period': '30',
        'items_on_page': '100',
        'no_magic': 'true'
    }

    page_number = 0
    while True:
        params['page'] = str(page_number)
        request = session.get(base_url, params=params)
        if request.status_code == 200:
            links = __search_page_to_urls__(request.content)
            #No more vacancies
            if len(links) == 0: break
            yield links
        else: break
        page_number += 1

def vacancies(search_text):
    '''Generates vacancies ''' 
    headers = {'accept': '*/*', 'user-agent': 'Mozilla/5.0 (Windows NT 6.3; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safary/537.36'}
    session = r.Session()
    session.headers.update(headers)
    for search_page_urls in __search_pages_to_urls__(session, search_text):
        for url in search_page_urls:
            request = session.get(url)
            if request.status_code == 200:
                soup = bs(request.content, 'html.parser')
                title = soup.find('h1', attrs={'data-qa': 'vacancy-title'}).text

                min_salary_meta = soup.find('meta', attrs={'itemprop': 'minValue'})
                max_salary_meta = soup.find('meta', attrs={'itemprop': 'maxValue'})
                salary_meta = soup.find('meta', attrs={'itemprop': 'value'})
                if min_salary_meta != None and max_salary_meta != None:
                    min_salary = int(min_salary_meta['content'])
                    max_salary = int(max_salary_meta['content'])
                    salary = (min_salary + max_salary) / 2
                elif max_salary_meta != None:
                    salary = int(max_salary_meta['content'])
                elif min_salary_meta != None:
                    salary = int(min_salary_meta['content'])
                else:
                    salary = int(salary_meta['content'])
                                                    
                firm = soup.find('meta', attrs={'itemprop': 'name'})['content']
                
                skill_spans = soup.find_all('span', attrs={'data-qa':'skills-element'})
                skills = []
                for skill_span in skill_spans:
                    skills.append(skill_span['data-tag-id'])

                yield firm, title, salary, skills
            else: break

            
