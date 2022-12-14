import numpy as np


def compare(left, right):
    match left, right:
        case int(), int():
            return left - right
        case list(), list():
            for l, r in zip(left, right):
                if diff := compare(l, r):
                    return diff
            return len(left) - len(right)
        case int(), list():
            return compare([left], right)
        case list(), int():
            return compare(left, [right])


f = open("inp_1.txt")

lines = f.readlines()
corr = []

idx = 1
for i in range(0, len(lines), 3):
    print(i)
    l1 = eval(lines[i])
    l2 = eval(lines[i + 1])

    if compare(l1, l2) < 0:
        corr.append(idx)

    print(l1)
    print(l2)
    print()

    idx += 1

print(corr)
print(np.sum(np.array(corr)))

##
from ast import literal_eval


def cmp(l, r):
    match l, r:
        case int(), int():
            return l - r
        case list(), int():
            r = [r]
        case int(), list():
            l = [l]
    return next((c for c in map(cmp, l, r) if c), len(l) - len(r))


with open("inp_1.txt") as f:
    pkts = list(map(literal_eval, [l for l in f if l.strip()]))

print(sum(i for i, p in enumerate(zip(*[iter(pkts)] * 2), 1) if cmp(*p) <= 0))
print(
    sum((1 for p in pkts if cmp(p, [[2]]) < 0), 1)
    * sum((1 for p in pkts if cmp(p, [[6]]) < 0), 2)
)
