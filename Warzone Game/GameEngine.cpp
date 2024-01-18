#include "GameEngine.h"
#include <iostream>
#include <math.h>
#include <random> 
#include <algorithm>
#include <regex>

using namespace std;

// Members of State Class

State::State() : stateName("none") {}
State::State(string name) : stateName(name) {}
State::State(const State& state) : stateName(state.stateName) {}

State& State::operator=(const State& state)
{
	this->stateName = state.stateName;
	return *this;
}

ostream& operator<<(ostream& out, const State& state)
{
	out << "Current State: " << state.stateName << endl;
	return out;
}

// Members of Transition Class

Transition::Transition() : current(nullptr), next(nullptr), command("none") {}
Transition::Transition(State* current, State* next, string command) : current(current), next(next), command(command) {}
Transition::Transition(const Transition& transition) : current(transition.current), next(transition.next), command(transition.command) {}

Transition& Transition::operator=(const Transition& transition)
{
	// Intentionally making shallow copies to refer to the same states
	this->current = transition.current;
	this->next = transition.next;
	this->command = transition.command;
	return *this;
}

ostream& operator<<(ostream& out, const Transition& transition)
{
	out << "(From: " << transition.current->stateName << ", To: " << transition.next->stateName << ")" << endl;
	return out;
}

// Members of GameEngine class

GameEngine::GameEngine() : currentState(nullptr), cmd_currentState(nullptr) {}

GameEngine::GameEngine(Observer* _obs) : currentState(nullptr), _observer(_obs), cmd_currentState(nullptr) {
    this->Attach(_obs);
				this->deck = new Deck();
}

GameEngine::~GameEngine()
{
	// use for loop first to delete pointer contents inside of vector
	for (int i = 0; i < transitions.size(); i++)
	{
		delete transitions.at(i);
		transitions.at(i) = NULL;
	}
	// clear vector of pointers
	transitions.clear();

	// same thing but for states
	for (int j = 0; j < states.size(); j++)
	{
		delete states.at(j);
		states.at(j) = NULL;
	}
	// clear vector of pointers
	states.clear();

	// Calls delete on every element inside of the vector
	currentState = nullptr;

	cmd_currentState = nullptr;

	delete deck;
}

GameEngine::GameEngine(const GameEngine& gameEngine)
{
	this->states = gameEngine.states;
	this->transitions = gameEngine.transitions;
	//this->players = gameEngine.players;
	//this->map = new Map(*gameEngine.map);
	//this->deck = new Deck(*gameEngine.deck);
}


GameEngine& GameEngine::operator=(const GameEngine& gameEngine)
{
	this->states = gameEngine.states;
	this->transitions = gameEngine.transitions;
	//this->players = gameEngine.players;
	//this->map = new Map(*gameEngine.map);
	//this->deck = new Deck(*gameEngine.deck);
	return *this;
}

ostream& operator<<(ostream& out, const GameEngine& gameEngine)
{
	for (int i = 0; i < gameEngine.transitions.size(); i++)
	{
		out << *gameEngine.transitions.at(i) << "\n";
	}
	return out;
}

Map* GameEngine::getMap(){
	return this->map;
}

void GameEngine::setMap(Map* map){
	this->map = map; 

}

Deck* GameEngine::getDeck(){
	return this->deck;
}

void GameEngine::setDeck(Deck* deck){
	this->deck = deck; 

}

void GameEngine::addPlayer(Player* p){
	players.push_back(p);
}

vector <Player*> GameEngine::getPlayers(){
	return this->players;
}

bool GameEngine::checkState(string command)
{
	for (int i = 0; i < transitions.size(); i++)
	{
		if (cmd_currentState->stateName == transitions[i]->current->stateName && command == transitions[i]->command)
		{
			cmd_currentState = transitions[i]->next;
			//cout << *cmd_currentState;
			Notify(this);
			return true;
		}
		else if (cmd_currentState->stateName == "win" && command == "quit") return true;
	}
	Notify(this);
	return false;
}

bool GameEngine::changeState(string command)
{
	for (int i = 0; i < transitions.size(); i++)
	{
		if (currentState->stateName == transitions[i]->current->stateName && command == transitions[i]->command)
		{
			currentState = transitions[i]->next;
			//cout << *currentState;
			Notify(this);
			return true;
		}
		else if (currentState->stateName == "win" && command == "end") return true;
	}
	Notify(this);
	return false;
}

State* GameEngine::newState(string stateName)
{
	State* state = new State(stateName);
	states.push_back(state);
	return state;
}

void GameEngine::newTransition(State* current, State* next, string command)
{
	transitions.push_back(new Transition(current, next, command));
}
void GameEngine::startupPhase(CommandProcessor* cp){
	// Reset the state of the game every time we start a new one.

	//resetContext();


	// Use the command list to get the commands, make sure to change states in between commands
	for (Command* c : cp->getValidCommandList()) {
		// Display the current state
		cout << endl;
		cout << "Current state: " << currentState->stateName << endl << endl;

		string command = c->getCommand();

		if (command.find("loadmap") != std::string::npos) {
			// Load the map )
			string fileName = c->getToAdd();
			MapLoader* ml  = new MapLoader();
	
			map = ml->parseMapConquest(fileName);
			
				
			// Transition to 'validatemap' state, Handle failure
			if (!changeState("loadmap")) {
				cout << "ERROR: Could not transition to 'loadmap' from current state " << currentState->stateName << endl;
			}
			
		}
		

		// Do I have to check if we are in a valid state before executing the command?
		else if (command == "validatemap") {
			// Validate the map
			map->validate();

			if (map->isValid()) {
				// Transition to 'addplayer' state, handle failure
				if (!changeState("validatemap")) {
					cout << "ERROR: Could not to transition to 'validatemap' from current state " << currentState->stateName << endl;
				}
			}

			else {
				// Does the state go back to loadmap, or do we just fail and exit?
				cout << "An invalid map has been loaded." << endl;
				exit(1);
			}
		}

		else if (command.find("addplayer") != std::string::npos) {
			// Add player (This part should loop so as to ensure that we have 2-6 players in the game.)
			int* num = new int(stoi(c->getToAdd()));

			// Check to see if we have 2-6 players in the game
			if (players.size() < 6) 
				addPlayer(new Player(num,this, new HumanPlayerStrategy()));
				
			else {
				// Let the user know that they cannot add anymore players
				cout << "Limit of players in game has been reached, no new players may be added." << endl;
			}


			if (players.size() >= 2) {
				// Switch states

				// Check if state changed successfully, otherwise handle failure.
				if (!changeState("addplayer")) {
					cout << "ERROR: Could not transition to 'addplayer' from current state " << currentState->stateName << endl;
				}
			}

			else {
				// Let the user know that there needs to be more players for the game to start
				cout << "There must be at least 2 players before the game can start." << endl;
			}
		}

		else if (command == "gamestart") {
			// Initialize random seed
			srand(time(NULL));

			/* Gamestart command does the following:
			*  a) Evenly pass out territories to players (remainder goes to the neutral player).

			*  b) Determine the order in which players get their turns (Randomly rearrange this class's Players list)

			*  c) Give 50 armies to each player, which are placed in their respective reinforcement pool (new Player field)

			*  d) Let each player draw 2 cards from the deck using Deck's draw() method

			*  e) Switch the game to the "play" state. (Call mainGameLoop())
			*/
			
			// Assign territories - Come up with a way to fairly distribute all countries between players
			
			int distrbutedTerritories = map->getVertices().size();

			// Randomize the territories and then pass them out to each player
			// ===============================================================

			// Define the number of territories each player will get (remove from denominator, since the neutral player shouldn't get its own territories yet.)
			int territoriesPerPlayer = distrbutedTerritories / (players.size());

			// Keep track of which territories we've already handed out
			vector<int> taken;

			// For each player, pass this number of territories out.
			for (Player* p : players) {
					int i = 0;
					while (i < territoriesPerPlayer) {
						// Generate the index of a random territory
						int choose = rand() % map->getVertices().size();

						// If it hasn't already been taken, then give it to the player.
						if (!std::count(taken.begin(), taken.end(), choose)) {
							p->addOwnedTerritory(map->getVertices().at(choose)->getTerritoryPtr());
							taken.push_back(choose);
							i++;
						}
					}
			}


			// Determine the turn order randomly - Re-arrange the players in the vector
			std::shuffle(players.begin(), players.end(), std::random_device()); // This is not total randomization, given 'n' players this will produce the same shuffle order of those 'n' players.

			// Give each player 50 armies to begin with and let them draw 2 cards from the deck
			for (Player* p : players) {
				// Give 50 armies
				p->setReinforcementPool(50);

				// Draw 2 cards for the player
				
				deck->draw(p->getHand());
				deck->draw(p->getHand());
				
			}

			// Switch the game to the play phase
			
			// Command processor setter for bool
			//cmd_game pointer points to win state

			// Only start the main game loop if the appropriate state can be changed to.
			if (!changeState("gamestart")) {
				cout << "ERROR: Could not transition to 'gamestart' from current state " << currentState->stateName << endl;
			}

			else {
				//mainGameLoop();
			}
		}            	
	}
}

void GameEngine::mainGameLoop() {
	/*This loop shall continue until only one of the players owns all the territories in the map, at which point a winner is
	announced and the game ends.*/

	bool onePlayerOwnsAllTerritories = false;


	// Get the total number of territories, 0 if there is no map
	int numTotalTerritories = 0;
	if (map != nullptr) {
		numTotalTerritories = map->getVertices().size();
	}

	do {
	    for (Player* p : players) {

			// If player owns no territories, remove from game
	        if (p->toDefend().empty()) {
				cout << "Player " << *p->getPlayerID() << " has no more territories! Removing Player " << *p->getPlayerID() << " from the game!" << endl;
				players.erase(remove(players.begin(), players.end(), p), players.end());
	        }

			if (players.size() == 1) {
				
				//onePlayerOwnsAllTerritories = true;

				cout << "\n/*----------------------------------------------------------------*/" << endl;
				cout << "Only 1 Player Remaining! " << endl;
				cout << "The winner is Player " << *players[0]->getPlayerID() << "!" << endl;
				cout << "/*----------------------------------------------------------------*/" << endl;

				changeState("win");

				return;
	        }

			// If player owns all the territories, they win
	        if (p->toDefend().size() >= numTotalTerritories) {

				// Used to break the loop
				onePlayerOwnsAllTerritories = true;
				cout << "\n/*----------------------------------------------------------------*/" << endl;
				cout << " Player " << *p->getPlayerID() << " owns all the territories!" << endl;
				// Announce this player as winner
				cout << "The winner is Player " << *p->getPlayerID() << "!" << endl;
				cout << "/*----------------------------------------------------------------*/" << endl;

                //Storing the results in a vector
                //results.push_back(p->getName());
				
				changeState("win");

				return;
	        }
	    }

		if (!onePlayerOwnsAllTerritories) {
			// Reinforcement Phase
			changeState("endexecorders");	
			reinforcementPhase();
			//cout<< "Reinforcement Orders!" <<endl;	

			// Issue Orders Phase
			changeState("issueorder");
			issueOrdersPhase();
			//cout<< "Issue Orders!" <<endl;	
			
			// Execute Orders Phase
			changeState("issueordersend");

			cout << "/*------------------------------------------------------------------------------------------------*/" << endl; 
			cout<< "Execution Orders:\n" <<endl;
			executeOrdersPhase();
			
			
		}
	


	} while (!onePlayerOwnsAllTerritories);
}

void GameEngine::mainGameLoopForTournament() {
	/*This loop shall continue until only one of the players owns all the territories in the map, at which point a winner is
	announced and the game ends.*/

	bool onePlayerOwnsAllTerritories = false;


	// Get the total number of territories, 0 if there is no map
	int numTotalTerritories = 0;
	if (map != nullptr) {
		numTotalTerritories = map->getVertices().size();
	}

	int maxNumOfturns = 5;
	int turn = 0;
	do {
	    for (Player* p : players) {

			// If player owns no territories, remove from game
	        if (p->toDefend().empty()) {
				cout << "Player " << *p->getPlayerID() << " has no more territories! Removing Player " << *p->getPlayerID() << " from the game!" << endl;
				players.erase(remove(players.begin(), players.end(), p), players.end());
	        }

			if (players.size() == 1) {
				
				//onePlayerOwnsAllTerritories = true;

				cout << "\n/*----------------------------------------------------------------*/" << endl;
				cout << "Only 1 Player Remaining! " << endl;
				cout << "The winner is Player " << *players[0]->getPlayerID() << "!" << endl;
				cout << "/*----------------------------------------------------------------*/" << endl;

				changeState("win");

				return;
	        }

			// If player owns all the territories, they win
	        if (p->toDefend().size() >= numTotalTerritories) {

				// Used to break the loop
				onePlayerOwnsAllTerritories = true;
				cout << "\n/*----------------------------------------------------------------*/" << endl;
				cout << " Player " << *p->getPlayerID() << " owns all the territories!" << endl;
				// Announce this player as winner
				cout << "The winner is Player " << *p->getPlayerID() << "!" << endl;
				cout << "/*----------------------------------------------------------------*/" << endl;

                //Storing the results in a vector
                //results.push_back(p->getName());
				
				changeState("win");

				return;
	        }
	    }

		if(turn == maxNumOfturns){
			cout << "\n/*----------------------------------------------------------------*/" << endl;
			cout << "The game has reached its maximum number of turns of " << maxNumOfturns << "." << endl;
			cout << "The remaining players are: " << endl;

			for (Player* p : players) {
				cout << "- Player " << *p->getPlayerID() << " with strategy " << p->getStrategy()->getStrategyName() << endl;
			}

			cout << "It is a draw!" << endl;
			cout << "/*----------------------------------------------------------------*/" << endl;
			changeState("win");

			return;
		}

		cout << "\nRound " << turn << ":" << endl;

		if (!onePlayerOwnsAllTerritories) {
			// Reinforcement Phase
			changeState("endexecorders");	
			reinforcementPhase();
			//cout<< "Reinforcement Orders!" <<endl;	

			// Issue Orders Phase
			changeState("issueorder");
			issueOrdersPhase();
			//cout<< "Issue Orders!" <<endl;	
			
			// Execute Orders Phase
			changeState("issueordersend");

			cout << "/*------------------------------------------------------------------------------------------------*/" << endl; 
			cout<< "Execution Orders:\n" <<endl;
			executeOrdersPhase();
			
			
		}
	
	turn++;

	} while (!onePlayerOwnsAllTerritories);
}


void GameEngine::reinforcementPhase() {
	for (Player* p : players) {
	    // Armies = # of territories owned divided by 3, rounded down : or 3 minimum
	    int armies = floor(p->toDefend().size() / 3);
	    // Bonus for owning all of a continent's territories gets added to the armies
	    // Place armies in player's reinforcement pool
		// Minimum 3 armies, or armies + continent bonus
	    p->setReinforcementPool(max(*p->getReinforcementPool() + armies + map->calculateContinentBonus(p), 3));
	}
}

void GameEngine::issueOrdersPhase() {
	// Issue orders for each player in the players list
	for (Player* p : players) 
            p->issueOrder();
        	
}

void GameEngine::executeOrdersPhase() {
	for (Player* p : players) {
        //Executing deploys first
      	for(Order* o: p->getOrders()->getOrdersList()){
                if(*o->getName() == "Deploy"){
					
                    o->execute();
                    p->getOrders()->removeOrder(o);
                }
            }
        
	}

	// Find the longest list length
	int longestList = 0;
	for (Player* p : players) 
		if (p->getOrders()->getOrdersList().size() > longestList) 
				longestList = p->getOrders()->getOrdersList().size();
			
		
	// Round-robin execution of the other orders
	for (int i = 0; i < longestList; i++) {
		for (Player* p : players) {
			if (!p->getOrders()->getOrdersList().empty()) {
				Order* o = p->getOrders()->getOrdersList().at(0);
				
				o->execute();
				p->getOrders()->removeOrder(o);
			}
			
		}
	}

    //used for Advance order in Orders.cpp
    for(auto it:players){
        it->receivedCardThisTurn = false;
    }
	
}

// Used when the player enters 'quit'
void GameEngine::end()
{
	cout << "Thank you for playing!" << endl;
    delete this;
}

void GameEngine::resetContext() {
	// Clear map
	if (this->map != NULL) {
		delete this->map;
		this->map = NULL;
	}

	// Clear players
	for (int i = 0; i < this->players.size(); i++) {
		delete this->players.at(i);
		this->players.at(i) = NULL;
	}
	this->players.clear();
	

	// Reset Deck
	delete this->deck;
	this->deck = NULL;
	this->deck = new Deck(52);

	// Go back to start state
	this->currentState = this->states.at(0);
}

string GameEngine::stringToLog() {
    return "Current GameEngine State: " + cmd_currentState->stateName;
}

