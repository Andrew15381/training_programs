from db import DB
import os
import unittest as ut

class Test_db(ut.TestCase):
    def setUp(self):
        self.db = DB('test')
        self.db.cursor.execute('INSERT INTO Vacancies VALUES(?, ?, ?, ?)',
                               ('abc', 'Python', 100, '[\'cde\', \'def\']'))
        self.db.cursor.execute('INSERT INTO Vacancies VALUES(?, ?, ?, ?)',
                               ('tyy', 'Python', 10, '[\'e4e\', \'njf\']'))
        self.db.cursor.execute('INSERT INTO Vacancies VALUES(?, ?, ?, ?)',
                               ('qwd', 'C++', 1000, '[\'hgj\', \'vbn\']'))
        self.db.force_commit()

    def test_salaries(self):
        self.assertEqual(self.db.salaries('Python'), [('100',), ('10',)])
        self.assertEqual(self.db.salaries('C++'), [('1000',)])
        self.assertEqual(self.db.salaries('Cython'), [])

    def test_skills(self):
        self.assertEqual(self.db.skills('Python'), [('[\'cde\', \'def\']',), ('[\'e4e\', \'njf\']',)])
        self.assertEqual(self.db.skills('C++'), [('[\'hgj\', \'vbn\']',)])
        self.assertEqual(self.db.skills('Cython'), [])

    def tearDown(self):
        self.db.close()
        os.remove('test.sqlite')

if __name__ == '__main__':
    ut.main()
        
