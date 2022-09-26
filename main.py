from random import randint
tamanho = 1000
with  open("input.txt","w") as f:
    f.write(" ".join([ (str(randint(-10000,10000))) for i in range(tamanho)]))