#include <iostream> // Voor Console input en output
#include <fstream>  // Voor het schrijven naar disk en dus bestanden
#include <sstream>  // Voor stringstream
#include <stdlib.h> // Voor de 'rand()' functie, random number generator
#include <vector>   // Voor de 'vector' template
#include <string>   // Voor de 'string' datatype
#include <algorithm> // Voor de replace functie voor de CSV bestanden

using namespace std;

class GameInstance // De GameInstance class representeerd 1 spel van D aantal kaarten in het deck
{
public:
  int initialDeckSize = 0;        // Moet een even getal zijn! Anders moet je beslissen of er 1 meer van rood of zwart moet zijn (bijvoorbeeld 3 kaarten, 2 zwart 1 rood of 1 zwart 2 rood?)
  vector<int> simulatedDeck = {}; // Een 'fysieke' representatie van het deck in de vorm van een lijst met integers, gebruikt om willekeurig een kaart te kiezen
  int amountRed = 0;   // De hoeveelheid rode kaarten in het deck of spel
  int amountBlack = 0; // De hoeveelheid zwarte kaarten in het deck of spel
  int round = 1;     // We weten dat we altijd op ronde 1 beginnen
  int lastDrawnCard; // 0 is zwart, 1 is rood
  int chosenIndex;   // Nodig om buiten functies om bij te houden welke index is gekozen
  int cheaterPick;   // Welke kaart de valsspeler deze ronde zal kiezen op basis van zijn winstkans, 0 of 1
  int legitPick;     // Welke kaart de legitieme speler deze ronde zal kiezen op basis van zijn winstkans, 0 of 1
  bool cheaterWon;
  bool legitWon;

  GameInstance(int D) // Deze functie wordt geroepen als een nieuwe GameInstance wordt gemaakt, om het spel te initializeren
  {
    initialDeckSize = D;
    amountBlack = amountRed = initialDeckSize / 2;

    for (int i = 0; i < amountBlack; i++) // Stop een rode en zwarte kaart in het deck
    {
      simulatedDeck.push_back(0);
      simulatedDeck.push_back(1);
    };
  };

  double calculateCheaterChance() // Functie om de kans voor de ronde te berekenen voor de valsspeler
  {
    int highestAmount = max(amountRed, amountBlack);                        // Kijk welke van de 2 kleuren er het meest van is en zet dat aantal in "highestAmount". Dit kan omdat de valsspeler altijd voor het groosts aantal van een kleur kiest
    double winChance = highestAmount / double(initialDeckSize - round + 1); // De berekening van de kans: P(winst voor valsspeler met highestAmount)
    return winChance;                                                       // Return de waarde uit de functie om er in de code mee te werken
  };

  void chooseCards() // Laat de spelers hun kaarten kiezen op basis van hun winstkansen
  {

    if (round == 1) // Als er nog geen vorige kaart was, kies random tussen 0 en 1 voor beiden
    {
      legitPick = rand() % 2;
      cheaterPick = rand() % 2;
    }
    else
    {
      cheaterPick = (amountBlack > amountRed) ? 0 : 1; // Als er meer zwart dan rood aanwezig is, kies zwart. Anders, kies rood
      legitPick = (lastDrawnCard == 1) ? 0 : 1;        // Als vorige kaart rood was, kies zwart en vice versa
    };
  };

  double calculateLegitChance() // Functie om de kans voor de ronde te berekenen voor de legitieme speler
  {

    if (round == 1) // Kijk of het ronde 1 is of niet. Zo ja, dan zal de kans altijd 0,5 zijn omdat er evenveel zwart en rood nog aanwezig zijn in het spel, dus maakt de keuze dan niet uit
    {
      return 0.5;
    };
    int chosenAmount = (legitPick == 1) ? amountRed : amountBlack;         // Als 'lastDrawnCard' == zwart, 'chosenAmount' = rood, else 'chosenAmount' = zwart
    double winChance = chosenAmount / double(initialDeckSize - round + 1); // De berekening van de kans: P(winst voor legitieme speler met chosenAmount)
    return winChance;                                                      // Return de waarde uit de functie om er in de code mee te werken
  };

  void doTurn() // Functie een nieuwe kaart te trekken
  {
    chosenIndex = rand() % simulatedDeck.size();
    lastDrawnCard = simulatedDeck[chosenIndex];
  };

  void checkIfWon() // Kijk of er iemand goed gegokt heeft en dus die ronde heeft gewonnen
  {
    cheaterWon = (cheaterPick == lastDrawnCard) ? true : false;
    legitWon = (legitPick == lastDrawnCard) ? true : false;
  };

  string getRoundDataString() // Wordt gebruikt om de data per ronde op een geformatteerde wijze te laten zien in de console, niet cruciaal
  {
    stringstream roundData;
    roundData << round << " | ";
    roundData << "Drawn card: " << lastDrawnCard << " | ";
    roundData << "Amount red: " << amountRed << " | ";
    roundData << "Amount black: " << amountBlack;
    roundData << endl;
    // kan eruit zien als "3 | Drawn card: 0 | Amount red: 2 | Amount black: 2"

    roundData << "Cheater = ";
    roundData << "Pick: " << cheaterPick << " | ";
    roundData << "Won: " << cheaterWon << " | ";
    roundData << "Win chance: " << calculateCheaterChance();
    roundData << endl;
    // Kan eruit zien als "Cheater = Pick: 1 | Won: 0 | Win chance: 0,5

    roundData << "Legit   = ";
    roundData << "Pick: " << legitPick << " | ";
    roundData << "Won: " << legitWon << " | ";
    roundData << "Win chance: " << calculateLegitChance();
    roundData << endl;
    // Kan eruit zien als "Legit = Pick: 1 | Won: 0 | Win chance: 0,5

    roundData << endl;
    // Gecombineerd kan de console output er dus als volgt uitzien voor een ronde:
    // 3 | Drawn card: 0 | Amount red: 2 | Amount black: 2
    // Cheater = Pick: 1 | Won: 0        | Win chance: 0,5
    // Legit   = Pick: 1 | Won: 0        | Win chance: 0,5

    return roundData.str();
  }

  string getRoundDataRaw() // Wordt gebruikt om de "ruwe" data per ronde in een CSV bestand te stoppen, wel cruciaal
  {
    stringstream roundData;
    roundData << round << ';';
    roundData << lastDrawnCard << ';';
    roundData << amountBlack << ';';
    roundData << amountRed << ';';
    roundData << cheaterPick << ';';
    roundData << calculateCheaterChance() << ';';
    roundData << cheaterWon << ';';
    roundData << legitPick << ';';
    roundData << calculateLegitChance() << ';';
    roundData << legitWon << endl;

    // Werkt heel erg hetzelfde als getRoundDataString() alleen dan wordt tussen elke waarde een punt-komma gezet zodat Excel weet waar de kolommen eindigen
    
    string modifiedData = roundData.str();
    replace(modifiedData.begin(), modifiedData.end(), '.', ','); // Vervang de decimale punten met komma's zodat excel weet welk datatype het is

    return modifiedData;
  }

  void endRound() // Functie om de ronde te beëindigen en de waarder bij te werken
  {
    simulatedDeck.erase(simulatedDeck.begin() + chosenIndex); // Verwijder de gekozen kaart van het deck

    if (lastDrawnCard == 0) // Zet de nieuwe waardes voor amountRed en amountBlack. Als zwart is gekozen, maak amountBlack 1 waarde kleiner. Hetzelfde doe je voor rood als die is gekozen
    {
      amountBlack--;
    }
    else
    {
      amountRed--;
    };

    round++; // Zet ronde 1 verder
  }
};

int main()
{
  // Vraag om input van de gebruiker voor de simulatie(s)
  int cardsPerGame;
  int totalGames;

  cout << "How many cards should each game have: ";
  cin >> cardsPerGame;
  cout << "How many games should there be: ";
  cin >> totalGames;

  vector<double> cheaterRoundWinChances(cardsPerGame, 0.0); // Een lijst die de opgetelde kans van elke ronde bijhoudt van de valsspeler over alle games gespreid
  vector<double> legitRoundWinChances(cardsPerGame, 0.0); // Een lijst die de opgetelde kans van elke ronde bijhoudt van de legitieme speler over alle games gespreid

  // Deze for loop runt voor elke game die we hebben opgegeven. Dus 6 keer als je 6 games hebt opgegeven
  for (int gamesPlayed = 1; gamesPlayed < totalGames + 1; gamesPlayed++)
  {
    // Logging
    cout << "Simulating game #" + to_string(gamesPlayed) + " with " + to_string(cardsPerGame) + " cards..." + '\n';

    GameInstance cardGame(cardsPerGame); // Initialiseer een game met de GameInstance class
    for (int round = 0; round < cardGame.initialDeckSize; round++)
    {
      // Zet een seed voor de random number generator waardoor deze altijd anders zal zijn
      srand(time(0) + gamesPlayed * round);

      cardGame.chooseCards();
      cardGame.doTurn();
      cardGame.checkIfWon();

      // Voeg voor bijde lijsten de kans voor deze ronde toe
      cheaterRoundWinChances[round] += cardGame.calculateCheaterChance();
      legitRoundWinChances[round] += cardGame.calculateLegitChance();

      cardGame.endRound();
    };
  }

  // Open het CSV bestand
  ofstream outputCsv("./CSVs/data.csv");

  // Eerste regel voor het CSV bestand met de kolom-headers
  outputCsv << "Round;Average cheater win chance;Average legit win chance" << endl;

  // Een loop die runt voor elke ronde, als er 52 kaarten in het spel zitten, zijn er 52 rondes dus loopt de loop 52 keer om alle opgetelde waardes van elke ronde te bemachtigen
  for (int round = 0; round < cardsPerGame; round++)
  {
    double averageCheaterChance = cheaterRoundWinChances[round] / totalGames; // Deel de opgetelde kans voor een ronde door het totaal aantal rondes voor een gemiddelde
    double averageLegitChance = legitRoundWinChances[round] / totalGames; // Deel de opgetelde kans voor een ronde door het totaal aantal rondes voor een gemiddelde

    stringstream roundLine;
    roundLine << round + 1 << ';' << averageCheaterChance << ';' << averageLegitChance << endl; // Een regel in het CSV bestand met de informatie van één ronde
    string modifiedData = roundLine.str();
    replace(modifiedData.begin(), modifiedData.end(), '.', ','); // Vervang weer decimalen met komma's voor excel

    outputCsv << modifiedData; // Stop de regel in het CSV bestand
  }

  outputCsv.close(); // Sluit het CSV bestand af en sla het op

  cout << "FINISHED SIMULATION" << endl;

  // Dit is nodig zodat het scherm niet meteen weggaat want we willen wel de resultaten kunnen zien
  system("pause");
  return 0;
};
