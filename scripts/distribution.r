library(poweRlaw)
library(gsl)
library(copula)

data = read.csv('data/power_law.dat', sep=" ", header = FALSE)

pl = displ$new(ceil(res/10000))
pl = displ$new(rpldis(1000, xmin=1, alpha=1.4))
pl
plot(pl)

powerlaw = function (n,k,lambda) {
  (n^-lambda)*exp(-n/k)/polylog(exp(-1/k), lambda, n.sum = 1000)
}
polylog(exp(-1/22) , 1.4, "sum", n.sum = 1000)
powerlaw(100, 22, 0.89)
