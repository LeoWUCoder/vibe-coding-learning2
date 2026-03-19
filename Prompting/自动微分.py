import torch
x=torch.arange(4.0)# 创建一个包含4个元素的张量，元素值为0.0、1.0、2.0和3.0
# torch.arange(start（起始，默认0.0）, end, step（步长，默认1.0）, dtype(数据类型)=None, device（cpu/cuda）=None)
x.requires_grad_(True)# 设置张量x的requires_grad属性为True，表示需要计算梯度，理解成（开关）开启自动求导功能
y = 2 * torch.dot(x, x)
# torch.dot(input, other)计算两个一维张量的点积，结果是一个标量。这里计算的是2乘以x和x的点积，即2乘以x中每个元素的平方和
y.backward()
# 反向传播，计算y关于x的梯度。由于y是一个标量，backward()函数会自动计算y对x的梯度，y对x的四个元素点求导
print(x.grad)
# 输出x的梯度，即y对x的导数。根据y=2*x^2，y对x的导数为4*x，因此输出的梯度应该是[0.0, 4.0, 8.0, 12.0]，对应于x中每个元素的导数值。