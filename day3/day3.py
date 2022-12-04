f = open("inp_t1.txt")

score = 0
for l in f.readlines():
    used = []
    for c in l[: int(len(l) / 2)]:
        if c in l[int(len(l) / 2) :] and c not in used:
            used.append(c)
            if c.isupper():
                sc = ord(c) - ord("A") + 1 + 26
                print(c, "u:", sc)
                score += sc
            else:
                sc = ord(c) - ord("a") + 1
                print(c, "l:", sc)
                score += sc

print("score:", score)

f.close()
