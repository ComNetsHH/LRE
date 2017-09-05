import LRE
lre = LRE.Evaluator(1, 		# Type. 0=DLREF (CDF) 1=DLREG (CCDF) 2=DLREP (PDF)
                    0.0, 	# Minimum value on x-axis
                    500.0, 	# Maximum value on x-axis
                    1.0, 	# Size of each bin on x-axis
                    0.05, 	# Maximum allowed relative error
                    0.0, 	# Initial value (does not have any influence)
                    1E-5, 	# Minimum value on y-axis determined with less than the maximum allowed relative error (intended reliability)
                    False, 	# Apply Large Sample Condition requiring certain minimum number of values per bin (min. of 10 transitions will always be reuired)
                    100000000, 	# Maximum number of samples to be considered (after this evaluation will stop and each bin will be evaluated if result is confident) 
                    0) 		# Evaluating if result is confident for each bin is expensive, if >0 evaluation will only be done every n samples

file = open("input.txt","r")
for line in file:
    value = float(line)
    lre.put(value)

lre.printResult()


# This example applies LRE to the test file input.txt with 40 minutes of Ping RTT measurements from Germany to New Zeeland. The CCDF is calculated (DLREG) and results larger
# 341 ms are considered not confident since the relative error (column 3) exceeds 5 %. Y-axis value is 9% at this point so it is far away from intended 1E-5 (99.999%) minimum value
#
# In this example the algorithm terminates when the whole file is processed (~20,000 samples). Alternatively the algorithm can continue until either enough samples to assess
# 99.999% are collected or maximum number of samples is reached (1E8). The second condition is used to prevent infinite runtime 
#
# To visualize result: 
# python LRE_Test.py > out
# gnuplot
# plot 'out' using 2:1                  will plot the CCDF as far as result considered confident
# plot 'out' using 2:3                  will plot the relative error up to maximum allowed one (default 5%)
#
# You can also drag the out file into MATLAB
#
# Check "RESTART/LRE Simulation: The Reliability Issue" by Kai Below, Lorenzo Battaglia, Ulrich Killat for more information on the Large Sample Condition

