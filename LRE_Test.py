import LRE
lre = LRE.Evaluator(2, # Type. 0=DLREF 1=DLREG 2=DLREP
                    0.0, # xMin
                    1.0, # xMax
                    0.01, # intSize
                    0.05, # error
                    0.1, # preFirst
                    1E-2, # gMin
                    0, # forceRMinusAOk
                    100000, # maxNrv
                    0) #skipInterval

file = open("input.txt","r")
for line in file:
    value = float(line)
    lre.put(value)

lre.printResult()
