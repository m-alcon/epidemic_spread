setwd('/home/malcon/GitHub/epidemic_spread')

distr.to.str = function (s) {
  aux = c()
  if (substr(s,1,1) == '0')
    aux = c('Poisson')
  if (substr(s,1,1) == '1')
    aux = c('power law')
  if (substr(s,1,1) == '2')
    aux = c('delta')
  if (substr(s,2,2) == '0')
    aux = c(aux,'Poisson')
  if (substr(s,2,2) == '1')
    aux = c(aux,'power law')
  if (substr(s,2,2) == '2')
    aux = c(aux,'delta')
  return(aux)
}

for (epidemic in c('sir','sis')) {
  main.path = paste('data',epidemic,sep='/')
  for (distr.path in list.files(main.path)) {
    file.list = list.files(paste(main.path,distr.path, sep='/'))
    mean = read.csv(paste(main.path,distr.path, file.list[1], sep='/'), sep=' ', header=TRUE)
    max = mean
    min = mean
    for (file in file.list[2:length(file.list)]) {
      full.path = paste(main.path,distr.path, file, sep='/')
      data = read.csv(full.path, sep=' ', header=TRUE)
      mean = mean + data
      if (max(data$infected) > max(max$infected))
        max = data
      if (min(data$infected) < max(min$infected))
        min = data
    }
    mean = mean/length(file.list)
    names = distr.to.str(distr.path)
    title=paste(bquote(P[k]),names[1],'\n', expression(P[l]), names[2], sep=' ')
    title=bquote(P[k] ~ .(paste(names[1],' &',sep='')) ~ P[l] ~ .(names[2]))
    pdf(paste('img/',epidemic,'_',distr.path,'.pdf',sep=''), width=3*3, height=2.5*3)
    plot(mean$t, mean$infected, ylim=c(0,10000), type='l', lwd=2, xlab='', ylab='')
    title(title, line=-2)
    lines(max$infected, col='red', lty=2, lwd=2)
    lines(min$infected, col='blue', lty=2, lwd=2)
    lines(mean$recovered, col='green', lwd=2)
    dev.off()
  }
}


