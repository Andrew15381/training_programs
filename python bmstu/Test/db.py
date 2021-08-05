import sqlite3
import json

class DB:
    '''Примитивная обертка над sqlite3 для hh'''
    def __init__(self, name, commit=10):
        '''Открывает соединение, создает таблицу и инициализирует счетчик изменений'''
        self.connection = sqlite3.connect('%s.sqlite' % name)
        self.cursor = self.connection.cursor()
        self.cursor.execute('CREATE TABLE IF NOT EXISTS Vacancies (firm TEXT, title TEXT, salary TEXT, skills TEXT)')
        #Необходим для более редкого коммита
        self.modification_counter = 0
        self.commit = commit

    def insert(self, firm, title, salary, skills):
        '''Вставляет вакансию и коммитит по счетчику'''
        skills = json.dumps(skills)
        self.cursor.execute('INSERT INTO Vacancies VALUES(?, ?, ?, ?)',
                            (firm, title, salary, skills))
        #коммитим по счетчику
        self.modification_counter += 1
        if self.modification_counter == self.commit:
            self.modification_counter = 0
            self.connection.commit()

    def force_commit(self):
        '''Форсит коммит'''
        self.connection.commit()
        self.modification_count = 0

    def salaries(self, text):
        '''Возвращает зарплаты по названию вакансии'''
        self.cursor.execute('SELECT salary FROM Vacancies WHERE INSTR(Vacancies.title, ?)',
                            (text,))
        return self.cursor.fetchall()

    def skills(self, text):
        self.cursor.execute('SELECT skills FROM Vacancies WHERE INSTR(Vacancies.title, ?)',
                            (text,))
        return self.cursor.fetchall()

    def close(self):
        '''Коммитит и закрывает соединение'''
        self.connection.commit()
        
        self.cursor.close()
        self.connection.close()

    
