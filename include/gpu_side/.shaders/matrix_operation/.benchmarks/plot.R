library(ggplot2)
library(ggdark)

data_original = read.csv("/home/clem/Workspace/crisp/include/gpu_side/.shaders/matrix_operation/.benchmarks/matrix_multiplication.csv", comment.char = "#")

plot_to_1000 = function()
{
  data = data_original;

  out = ggplot(data = data, mapping = aes(x=size, y=eigen, color="Eigen"))
  out = out + dark_theme_gray()

  out = out + geom_smooth(se = FALSE, size = 1.5)
  out = out + geom_smooth(aes(x=size, y=cv, color="OpenCV"), formula=y ~ poly(x, 1), se = FALSE, size = 1)
  out = out + geom_smooth(aes(x=size, y=crisp, color="crisp"), formula=y ~ poly(x, 1), se = FALSE, size = 1)

  out = out + coord_cartesian(ylim=c(0, max(data$crisp) * 10))

  out = out + scale_y_continuous(name="runtime (μs)", breaks=seq(0, max(data$cv), 10000))
  out = out + scale_x_continuous(name="sqrt(# matrix elements)", breaks=seq(0, max(data$size), 100))

  out = out + ggtitle("Matrix Multiplication: Performance")
  out = out + theme(text=element_text(family="roboto", size=20), legend.title = element_blank(), legend.text = element_text(size=20), title=element_text(size=25))
  out = out + scale_color_manual(values = c("OpenCV" = "deepskyblue", "crisp" = "deeppink", "Eigen" = "springgreen3"))

  return(out)
}

plot_to_150 = function()
{
  alpha = 0.4;

  data = data_original[data$size <= 150,]

  out = ggplot(data = data, mapping = aes(x=size, y=eigen, color="Eigen"))
  out = out + dark_theme_gray()

  out = out + geom_smooth(se = FALSE, size = 1.5)
  out = out + geom_line(size=1, alpha=alpha)

  out = out + geom_smooth(aes(x=size, y=cv, color="OpenCV"), formula=y ~ poly(x, 1), se = FALSE, size = 1)
  out = out + geom_line(aes(x=size, y=cv, color="OpenCV"), size=1, alpha=alpha)

  out = out + geom_smooth(aes(x=size, y=crisp, color="crisp"), formula=y ~ poly(x, 1), se = FALSE, size = 1)
  out = out + geom_line(aes(x=size, y=crisp, color="crisp"), size=1, alpha=alpha)

  out = out + coord_cartesian(ylim=c(0, max(data$cv) * 2))

  out = out + scale_x_continuous(name="sqrt(# matrix elements)", breaks=c(seq(0, 100, 10), seq(100, 150, 20)))
  out = out + ylim(0, data[150, "cv"] * 2) +  scale_y_continuous(name="runtime (μs)", breaks=seq(0, 3000, 500))

  out = out + ggtitle("Matrix Multiplication: Performance")
  out = out + theme(text=element_text(family="roboto", size=20), legend.title = element_blank(), legend.text = element_text(size=20), title=element_text(size=25))
  out = out + scale_color_manual(values = c("OpenCV" = "deepskyblue", "crisp" = "deeppink", "Eigen" = "springgreen3"))

  return(out)
}

path = "/home/clem/Workspace/crisp/include/gpu_side/.shaders/matrix_operation/.benchmarks/";

temp = plot_to_150();
ggsave(paste0("/home/clem/Workspace/crisp/include/gpu_side/.shaders/matrix_operation/.benchmarks/", "to_150.png"), plot=temp)

temp = plot_to_1000();
ggsave(paste0("/home/clem/Workspace/crisp/include/gpu_side/.shaders/matrix_operation/.benchmarks/", "to_1000.png"), plot=temp)

print(paste0("wrote plots to ", path));


