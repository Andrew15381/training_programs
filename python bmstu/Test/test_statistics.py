import statistics as st
import unittest

class Test_statistics(unittest.TestCase):
    def test_seq_mean(self):
        self.assertEqual(st.seq_mean([10, 20, 30]), 20)
        self.assertEqual(st.seq_mean((0, 0, 0)), 0)
        self.assertEqual(st.seq_mean([20, 10, 30]), 20)
        self.assertEqual(st.seq_mean([20, 40, 30]), 30)
        self.assertEqual(st.seq_mean([20]), 20)
        self.assertEqual(st.seq_mean([20, 10, 30, 40, 50]), 30)
        self.assertEqual(st.seq_mean([]), None)

    def test_seq_mid(self):
        self.assertEqual(st.seq_mid([20, 10, 30]), 20)
        self.assertEqual(st.seq_mid([20, 10, 31]), 20)
        self.assertEqual(st.seq_mid([29, 11, 31]), 29)
        self.assertEqual(st.seq_mid([20, 10, 30, 0, 50]), 20)
        self.assertEqual(st.seq_mid([0]), 0)
        self.assertEqual(st.seq_mid([]), None)

    def test_seq_range(self):
        self.assertEqual(st.seq_range([20, 10, 30]), (10, 30))
        self.assertEqual(st.seq_range([20, 10, 30, 50, 1]), (1, 50))
        self.assertEqual(st.seq_range([20, 10]), (10, 20))
        self.assertEqual(st.seq_range([20, 20, 20]), (20, 20))
        self.assertEqual(st.seq_range([10]), (10, 10))
        self.assertEqual(st.seq_range([]), None)

    def test_seq_top(self):
        self.assertEqual(st.seq_top([1, 2, 3], 4), (1, 2, 3))
        self.assertEqual(st.seq_top([2, 2, 3]), (2,))
        self.assertEqual(st.seq_top([0, 0, 0, 0], 2), (0,))
        self.assertEqual(st.seq_top([1, 1, 2, 2], 2), (1, 2))
        self.assertEqual(st.seq_top([1, 1, 2, 2, 2]), (2,))
        self.assertEqual(st.seq_top([]), None)

if __name__ == '__main__':
    unittest.main()
