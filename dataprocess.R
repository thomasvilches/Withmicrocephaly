setwd("/home/thomas/Doutorado/PDSEproject/prog/WithMicrocephaly2/R19/teste6/")

x1=read.table("NoM0.dat",h=F)
x2=read.table("NoM1.dat",h=F)
x3=read.table("NoM2.dat",h=F)
x4=read.table("NoM3.dat",h=F)


x=rbind(x1,x2,x3,x4)

mean(x$V1)


