f = open("inp_1.txt")

score = 0
lines = f.readlines()
for idx in range(0, len(lines), 3):
    print(idx)
    for c in lines[idx][:-1]:
        if c in lines[idx + 1] and c in lines[idx + 2]:
            if c.isupper():
                sc = ord(c) - ord("A") + 1 + 26
                print(c, "u:", sc)
                score += sc
            else:
                sc = ord(c) - ord("a") + 1
                print(c, "l:", sc)
                score += sc
            break

print("score:", score)

f.close()
