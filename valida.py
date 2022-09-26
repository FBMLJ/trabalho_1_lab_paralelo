with open("output.txt","r") as f:
    
    valores = list( map(int, f.readline().split("  ")[:-1]))
    # print(sorted(valores))
    temp = valores[0]
    for i in valores[1:]:
        
        if temp > i:
            print("Não ordenada")
            # break
            exit()
        temp = i
print("Ordenada")
