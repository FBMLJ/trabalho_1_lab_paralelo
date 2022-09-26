
from random import randint

num_do_vetor = 1000
vetor = []
for i in range(num_do_vetor):
    vetor.append (str(randint(-10000,10000)))

input_text = " ".join(vetor)
print(input_text)
f = open("input.txt","w")
f.write(input_text)
f.close()
