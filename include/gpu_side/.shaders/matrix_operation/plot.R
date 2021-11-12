library(ggplot2)

data = read.csv("/home/clem/Workspace/crisp/include/gpu_side/.shaders/matrix_operation/matrix_bckp.csv", comment.char = "#")

plot = function()
{
  alpha = 0.4;
  
  out = ggplot(data = data, mapping = aes(x=size, y=eigen, color="Eigen"))
  out = out + geom_smooth(se = FALSE, size = 1.5) + geom_line(size=1, alpha=alpha)
  out = out + geom_smooth(aes(x=size, y=cv, color="OpenCV"), formula=y ~ poly(x, 1), se = FALSE, size = 1) + geom_line(aes(x=size, y=cv, color="OpenCV"), size=1, alpha=alpha)
  out = out + geom_smooth(aes(x=size, y=crisp, color="crisp"), formula=y ~ poly(x, 1), se = FALSE, size = 1) + geom_line(aes(x=size, y=crisp, color="crisp"), size=1, alpha=alpha)

  out = out + coord_cartesian(ylim=c(0, max(data$cv) * 2))

  out = out + scale_x_continuous(name="sqrt(# matrix elements)", breaks=c(seq(0, 100, 10), seq(100, max(data$size), 25)))
  out = out + scale_y_continuous(name="runtime (μs)", breaks=seq(0, max(data$eigen), 500))

  out = out + ggtitle("Matrix Multiplication: Runtime ~ Matrix Size")
  out = out + theme(text=element_text(family="roboto", size=20), legend.title = element_blank(), legend.text = element_text(size=20), title=element_text(size=25))
  out = out + scale_color_manual(values = c("OpenCV" = "deepskyblue", "crisp" = "deeppink", "Eigen" = "springgreen3"))
  return(out)
}


plot()


