with open("input.txt", "r") as file:
    res = sum(map(int, file.read().split()))

with open("output.txt", "w") as file:
    file.write(str(res))