import torch
x = torch.tensor([[ [[1, 2, 3], [4, 5, 6],[13,14,15]],[[7,8,9],[10,11,12],[16,17,18]] ], [[[19,20,21],[22,23,24],[25,26,27]],[[28,29,30],[31,32,33],[34,35,36]]] ])
print(x.size())
x=torch.reshape(x,[3,3,2,2])

print(x.size())
print(x)
x=torch.flatten(x)
print(x)

