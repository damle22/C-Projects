#include "GameEngine.h"
#include <iostream>
using namespace std; 

void testGameStates()
{
    const int CARDS_IN_DECK = 52;

	// Instantiate Observer object
	Observer* _observer = new LogObserver();

	CommandProcessor* commandprocessor = new CommandProcessor(_observer);

	GameEngine* game = new GameEngine(_observer);
	game->setDeck(new Deck(CARDS_IN_DECK));

	State* start = game->newState("start");
	State* map_loaded = game->newState("map_loaded");
	State* map_validated = game->newState("map_validated");
	State* players_added = game->newState("players_added");
	State* assign_reinforcement = game->newState("assign_reinforcement");
	State* win = game->newState("win");
	State* end_game = game->newState("end_game");


	game->newTransition(start, map_loaded, "loadmap");
	game->newTransition(map_loaded, map_loaded, "loadmap");
	game->newTransition(map_loaded, map_validated, "validatemap");
	game->newTransition(map_validated, players_added, "addplayer");
	game->newTransition(players_added, players_added, "addplayer");
	game->newTransition(players_added, assign_reinforcement, "gamestart");
	game->newTransition(assign_reinforcement, win, "gameend");	//For testing purposes
	game->newTransition(win, start, "replay");
	game->newTransition(win, end_game, "quit");

	game->cmd_currentState = start;
	game->currentState = start;

	string answer;
	string file;

	cout << "Here, we read the commands that will be executed by startupPhase()" << endl;
	cout << "==================================================================" << endl << endl;
	cout << "Do you wish to accept commands from the console or read them from a file? " << '\n';

	cin >> answer;

	if (answer == "console") {
		cout << "Please enter a command" << '\n';
		while (!(commandprocessor->getExitProgram())) {
			commandprocessor->getCommand(game, commandprocessor);
		}
	}
	else if (answer == "file") {
		string fileName;
		cout << "\nWhich file do you wish to open? " << '\n';

		cin >> fileName;

		cout << '\n';

		FileLineReader* fileReader = new FileLineReader();
		fileReader->readLineFromFile(fileName);

		FileCommandProcessorAdapter* fileCP = new FileCommandProcessorAdapter(fileReader,_observer);

		while (!(commandprocessor->getExitProgram())) {
			commandprocessor->getCommand(game,fileCP);
		}
		

	}

	
	// TESTING OF PART 2 BEGINS HERE
	game->startupPhase(commandprocessor);

	// Show effect of 'gamestart' command
	// Show turn order
	cout << endl;
	cout << "TURN ORDER (Shows that 'gamestart' command randomizes the turn order of players and the effect of the 'addplayer' command)" << endl;
	cout << "==========================================================================================================================" << endl;

	int i = 0;
	for (Player* p : game->players) {
			i++;
			cout << i << ") Player" << *p->getPlayerID() << endl;
		
	}

    /*
    /**/
	// Show distribution of territories
	cout << endl;
	cout << "TERRITORY DISTRIBUTION" << endl;
	cout << "======================" << endl;

	for (Node* t : game->map->getVertices()) {
			for (Player* p : game->players) {
				if (p->isOwnedTerritory(*t->getTerritoryPtr()->getTerritoryName())) {
					cout << "Territory " << *t->getTerritoryPtr()->getTerritoryName() << " is owned by Player " << *p->getPlayerID() << endl;
				}
			}
		
	}

	cout << endl;

	// Show number of cards and armies for each player
	cout << "This next section shows that each player gets 50 armies and 2 cards from the deck." << endl;
	cout << "==================================================================================" << endl;

	for (Player* p : game->players) {
		cout << endl;
		cout << "Player " << *p->getPlayerID() << " has " << p->getHand()->getCards().size() << " cards in their hand and " << *p->getReinforcementPool() << " armies" << endl;
		
	}

	cout << endl;

	cout << "At the end of the startup phase, the game switches to first state in the play phase: assign_reinforcements." << endl;
	cout << "===========================================================================================================" << endl;
	cout << "Current state: " << game->currentState->stateName << endl;

	/// /////////////////
	delete game;
	delete commandprocessor;
	delete _observer;
	game = NULL;
	commandprocessor = NULL;
	_observer = NULL;
}

int main(){
    testGameStates();
    return 0;
}