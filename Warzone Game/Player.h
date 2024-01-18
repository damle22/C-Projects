#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Map.h"
#include "Card.h"
#include "Orders.h"
#include "GameEngine.h"
#include "PlayerStrategies.h"
using namespace std;

class Territory;
class OrdersList;
class GameEngine;
class Hand;
class PlayerStrategy;
class HumanPlayerStrategy;

class Player {

public:
    //-------------- Constructors --------------//
    Player(GameEngine*);       
    Player(int*, GameEngine*, PlayerStrategy*);
    Player(int*, int*, vector<Player *>, vector<Territory *>, Hand*, OrdersList*, GameEngine*, PlayerStrategy*);
    Player(const Player &p); 
    
    //-------------- Overloads --------------//
    friend std::ostream& operator<<(std::ostream& os, const Player& player);
    Player& operator = (const Player& ); 

    //-------------- Destructor --------------//
    ~Player(); 

    //-------------- Getters --------------//
    int* getPlayerID();
    int* getReinforcementPool();
    vector<Player *> getUnattackablePlayers();
    vector<Territory *> getOwnedTerritories();
    Hand* getHand();
    OrdersList* getOrders();
    GameEngine* getGame();
    PlayerStrategy* getStrategy();

    //-------------- Setters --------------//
    void setPlayerID(int*);
    void setReinforcementPool(int);
    void setUnattackablePlayers(vector <Player*>);
    void setOwnedTerritories(vector <Territory*>);
    void setCards(Hand*);
    void setOrders(OrdersList*);
    void setStrategy (PlayerStrategy*) ;
    

    //-------------- others --------------//
    vector<Territory*> toAttack();
    vector<Territory*> toDefend();
    void issueOrder();
    void addUnattackablePlayer(Player*);
    bool isOwnedTerritory(string);
    void addOwnedTerritory(Territory*);
   

    

    bool receivedCardThisTurn = false;

private:
    int* playerID;
    int* reinforcementPool;
    vector<Player *> unattackablePlayers;
    vector<Territory*> ownedTerritories;
    Hand* hand;
    OrdersList* orders;
    GameEngine* game;
    PlayerStrategy* strategy;
};
