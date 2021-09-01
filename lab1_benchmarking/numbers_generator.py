import random


with open("data/data.txt", "a") as f:
    for j in range(300000):
        for i in range(10):
            f.write(f"{random.randint(-100000, 100000) + random.random()} ")
        f.write(f"\n")
