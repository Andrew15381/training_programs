from collections import Counter

def seq_mean(seq):
    '''Возвращает среднее арифметическое'''
    if len(seq) == 0:
        return None
    return sum(seq) / len(seq)

def seq_mid(seq):
    '''Возвращает медиану'''
    if len(seq) == 0:
        return None
    seq.sort()
    if len(seq) % 2 == 0:
        return (seq[len(seq) // 2] + seq[len(seq) // 2 - 1]) / 2
    else:
        return seq[len(seq) // 2]

def seq_range(seq):
    '''Возвращает разброс'''
    if len(seq) == 0:
        return None
    return min(seq), max(seq)

def seq_top(seq, top=1):
    if len(seq) == 0:
        return None
    counts = Counter(seq)
    return tuple(dict(counts.most_common(top)).keys())

