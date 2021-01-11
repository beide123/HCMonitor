import numpy as np
import statsmodels.api as sm
import matplotlib.pyplot as plt
import sys

def cdf_plot(data, name, number):
	ecdf = sm.distributions.ECDF(data)
	x = np.linspace(min(data), max(data), number)
	y = ecdf(x)

	plt.plot(x, y, label=name)

def main():
	for f in sys.argv[1:]:
		data = open(f).readlines()
		data = map(float, data)
		cdf_plot(data, f, 100)

	plt.legend(bbox_to_anchor=(0.65, 0.3), loc=2, borderaxespad=0.)
	plt.show()

if __name__=='__main__':
	main()
