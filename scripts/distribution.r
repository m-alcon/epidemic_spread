library(poweRlaw)
library(gsl)
library(copula)
setwd('/home/malcon/GitHub/epidemic_spread')

data = scan('data/power_law.dat', sep=" ")
hist_data = hist(data, breaks=50, plot=FALSE)
plot(hist_data$counts, log="xy")