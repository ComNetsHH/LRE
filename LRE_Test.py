import LRE
lre = LRE.Evaluator(2, 0.0, 1.0, 0.01, 0.05, 0.1, 1E-2, 0, 100000, 0)

file = open("input.txt","r")
for line in file:
    value = float(line)
    lre.put(value)

lre.printResult()
