import random

size = raw_input("Digite o valor de N: ")

n = int(size)
l = []
for i in range(n):
    l.append(i + 1)

s = ""
j = 0
size = n

signals = ["-","+"]

for i in range(n):
    s += signals[random.randrange(0,2)]
    j = random.randrange(0,size)
    s += str(l[j])
    s += " "
    del l[j]
    size = size - 1

s = s[:-1]

f = open("input.txt", "w")
f.write(s)
f.close()
