#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Map.h"
#include "LoggingObserver.h"
#include "CommandProcessing.h"
#include "Player.h"
#include "Orders.h"
#include "Card.h"
using namespace std;

class Map;
class Deck;
class Player;
class CommandProcessor;

// A State holds a string
class State
{
public:
	// State's name
	string stateName;

	// Default constructor
	State();

	// Parametrized constructor
	State(string name);

	// Copy constructor
	State(const State& state);

	// Assignment operator
	State& operator=(const State& state);

	// ostream conversion
	friend ostream& operator<<(ostream& out, const State& state);
};

// Contains pointers from one state to another as well as its required command
class Transition
{
public:
	// Current state
	State* current;

	// Next state
	State* next;

	// Required command to perform transition
	string command;

	// Default Constructor
	Transition();
    
	// Parametrized Constructor
	Transition(State* current, State* next, string command);

	// Copy constructor
	Transition(const Transition& transition);

	// Assignment operator
	Transition& operator=(const Transition& transition);

	// ostream conversion
	friend ostream& operator<<(ostream& out, const Transition& transition);
};

// The ruleset for the game using transitions and states
class GameEngine : public ILoggable, public Subject
{
public:
	// Vector of pointers to transitions
	vector<Transition*> transitions;

	// Vector of pointers to states
	vector<State*> states;

	// Current State
	State* currentState;
	State* cmd_currentState;

	Observer* _observer;

	Deck* deck;

	Map* map;

	vector <Player*> players; 

	// Default Constructor
	GameEngine();

		GameEngine(Observer*);

	// Destructor
	~GameEngine();

	// Copy constructor
	GameEngine(const GameEngine& gameEngine);

	// Assignment operator
	GameEngine& operator=(const GameEngine& gameEngine);

	// ostream conversion
	friend ostream& operator<<(ostream& out, const GameEngine& gameEngine);

	Map* getMap();

	void setMap(Map*);

	Deck* getDeck();

	void setDeck(Deck*);

 vector <Player*> getPlayers();  

	void addPlayer(Player*);

	// Getter for map
	bool checkState(string command);

	// Checks and Changes the states
	bool changeState(string command);

	// Creates a new state
	State* newState(string state);

	// Creates a new Transition
	void newTransition(State* current, State* next, string command);

	string stringToLog();
 
	void startupPhase(CommandProcessor*);

	void mainGameLoop();

	// Ends the game and deletes game object
	void end();

	// Reset GameEngine's context to start new games
	void resetContext();

	// Reinforcement Phase
	void reinforcementPhase();

	// Issue Orders Phase
	void issueOrdersPhase();

	// Execute Orders Phase
	void executeOrdersPhase();

};