# scores = {"A": 1, "B": 2, "C": 3}
value = {"X": 1, "Y": 2, "Z": 3}
o_dic = {"A": "R", "B": "P", "C": "S"}
y_dic = {"X": "R", "Y": "P", "Z": "S"}
win_combs = ["C X", "A Y", "B Z"]

f = open("inp_1.txt")

## Part 1
score = 0
for l in f.readlines():
    l = l.strip()
    sl = l.split(" ")
    opp = o_dic[sl[0]]
    you = y_dic[sl[1]]
    score += value[sl[1]]
    if opp == you:
        score += 3
    elif l in win_combs:
        score += 6

print("Score: ", score)

## Part 2
f.seek(0)
win_dic = {"A": "Y", "B": "Z", "C": "X"}
dra_dic = {"A": "X", "B": "Y", "C": "Z"}
los_dic = {"A": "Z", "B": "X", "C": "Y"}

score = 0
for l in f.readlines():
    l = l.strip()
    sl = l.split(" ")
    opp = o_dic[sl[0]]
    you = y_dic[sl[1]]
    if sl[1] == "Y":
        score += 3
        score += value[dra_dic[sl[0]]]
    elif sl[1] == "Z":
        score += 6
        score += value[win_dic[sl[0]]]
    elif sl[1] == "X":
        score += value[los_dic[sl[0]]]

print("Score: ", score)
print("Done.")
f.close()
