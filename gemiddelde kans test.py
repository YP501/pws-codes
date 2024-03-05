from random import randint


def runLegit(total, iterations):
    totalChance = 0
    for _ in range(iterations):
        black = int(total / 2)
        red = int(total / 2)

        cards = []

        for __ in range(black):
            cards.append("black")
        for __ in range(red):
            cards.append("red")

        previous = None
        for ___ in range(len(cards)):
            if not previous:
                totalChance += 0.5
            elif previous == "red":
                blackChance = black / (black + red)
                totalChance += blackChance
            else:
                redChance = red / (red + black)
                totalChance += redChance

            drawnCard = cards.pop(randint(0, len(cards) - 1))
            if drawnCard == "black":
                black -= 1
            else:
                red -= 1
            previous = drawnCard

    print(f"Average Legit correct card chance: {totalChance / (iterations * total)}")


def runCheat(total, iterations):
    totalChance = 0
    for _ in range(iterations):
        black = int(total / 2)
        red = int(total / 2)
        cards = []

        for __ in range(black):
            cards.append("black")
        for __ in range(red):
            cards.append("red")
        previous = None

        for ___ in range(len(cards)):
            highest = max(black, red)
            winChance = highest / (black + red)
            totalChance += winChance

            drawnCard = cards.pop(randint(0, len(cards) - 1))
            if drawnCard == "black":
                black -= 1
            else:
                red -= 1
            previous = drawnCard

    print(f"Average Cheater correct card chance: {totalChance / (iterations * total)}")


runLegit(52, 9999)
runCheat(52, 9999)
