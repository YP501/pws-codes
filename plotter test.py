import matplotlib.pyplot as plt
from random import randint

deckSize = 6

plt.ylim(0, 1)
plt.xlim(1, deckSize)
plt.yticks([0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0])

for i in range(1, 10):
    plt.axhline(y=i / 10, color="lightgrey")
for i in range(1, deckSize):
    plt.axvline(x=i, color="lightgrey")

xRound = list(range(1, deckSize + 1))
yChances1 = []
yChances2 = []

# Run legit
black = int(deckSize / 2)
red = int(deckSize / 2)
cards = []
for j in range(black):
    cards.append("black")
for j in range(red):
    cards.append("red")
previous = None

for card in range(len(cards)):
    # print(f"{black} Black | {red} Red | {black + red} Total | {previous} Previous")
    if not previous:
        print("Legit chance: 0.5")
        yChances1.append(0.5)
    elif previous == "red":
        blackChance = black / (black + red)
        yChances1.append(blackChance)
        print(f"Legit chance: {blackChance}")
    else:
        redChance = red / (red + black)
        yChances1.append(redChance)
        print(f"Legit chance: {redChance}")

    drawnCard = cards.pop(randint(0, len(cards) - 1))
    if drawnCard == "black":
        black -= 1
    else:
        red -= 1
    previous = drawnCard
print("\n")

# Run cheater
black = int(deckSize / 2)
red = int(deckSize / 2)
cards = []
for j in range(black):
    cards.append("black")
for j in range(red):
    cards.append("red")
previous = None

for card in range(len(cards)):
    highest = max(black, red)
    winChance = highest / (black + red)
    yChances2.append(winChance)
    print(f"Cheater chance: {winChance}")

    drawnCard = cards.pop(randint(0, len(cards) - 1))
    if drawnCard == "black":
        black -= 1
    else:
        red -= 1
    previous = drawnCard

# Visualize results
plt.plot(xRound, yChances1, label="Legit Player")
plt.plot(xRound, yChances2, label="Cheater")
plt.xlabel("Amount of rounds")
plt.ylabel("Chance of correct guess")
plt.legend()
plt.show()
