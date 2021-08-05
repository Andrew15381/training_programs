import request as req
from db import DB
import json
import statistics as st
import argparse as ap

parser = ap.ArgumentParser()
parser.add_argument('-s', '--stats',
                    action='store_true',
                    help='Get stats for a specific vacancy from local db')
parser.add_argument('-u', '--update',
                    action='store_true',
                    help='Update local avcancies db')
parser.add_argument('text',
                    help='Vacancy title to serach for')
args = parser.parse_args()

text = args.text
DB = DB(name='hh_vacancies', commit=10)
try:
    if args.update:
        for firm, title, salary, skills in req.vacancies(text):
            DB.insert(firm, title, salary, skills)
        DB.force_commit()
    if args.stats:
        salaries = DB.salaries(text)
        salaries = list(map(lambda x: float(x[0]), salaries))
        print(st.seq_mean(salaries))
        print(st.seq_mid(salaries))
        print(st.seq_range(salaries))
    
        skills = DB.skills(text)
        skills = list(map(lambda x: json.loads(x[0]), salaries))
        skills = set([skill for vacancy_skills in skills for skill in vacancy_skills])
        print(skills)
        print(st.seq_top(skills, top=5))
    DB.close()
except: DB.close()
