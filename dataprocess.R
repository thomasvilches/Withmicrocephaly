setwd("/home/thomas/Doutorado/PDSEproject/prog/WithMicrocephaly2/R19/teste6/")

x1=read.table("NoM0.dat",h=F)
x2=read.table("NoM1.dat",h=F)
x3=read.table("NoM2.dat",h=F)
x4=read.table("NoM3.dat",h=F)


x=rbind(x1,x2,x3,x4)

mean(x$V1)

x=x[x$V1+x$V2+x$V3>0,]
mean(x$V1)


y=read.table("/home/thomas/Doutorado/PDSEproject/prog/WithMicrocephaly2/symp0.dat",h=F)

y=as.matrix(y)


sum=c()

for(j in 1:ncol(y)){
sum[j]=0;
  for(i in 1:42){
  sum[j]=sum[j]+y[i,j]
  }

}

R0=1.9
y=y[,sum>R0+R0^2+R0^3]
y2=c()
for(i in 1:nrow(y))
y2[i]=sum(y[i,])
y2=y2/ncol(y)

a=seq(1,nrow(y),1)
a=a/7
limit=c(1:20)
plot(a,y[,1],type="l",col="blue",ylim=c(1,20))
for(i in 2:ncol(y)){
  
  lines(a,y[,i],col="blue")
  
}
lines(a,y2,col="black",lwd=3)
