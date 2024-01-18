#include "Player.h"
/*
#include "Orders.cpp"
#include "Card.cpp"
#include "Map.cpp"
*/
#include <vector>
#include <algorithm>
#include <cctype>
#include <string>

using namespace std;

//-------------------------- Player class  --------------------------//

//-------------- Constructors --------------//
Player::Player(GameEngine* game){

    this->playerID = new int(0);
    this->reinforcementPool = new int(0);
    this->unattackablePlayers = unattackablePlayers;
    //this->ownedTerritories;
    this->hand = new Hand();
    this->orders = new OrdersList();
    this->game = game;
    this->strategy = new HumanPlayerStrategy();
    strategy->setPlayer(this);
}

Player::Player(int* playerID, GameEngine* game, PlayerStrategy* strategy){

    this->playerID = playerID;
    this->reinforcementPool = new int(0);
    this->unattackablePlayers = unattackablePlayers;
    //this->ownedTerritories;
    this->hand = new Hand();
    this->orders = new OrdersList();
    this->game = game;
    this->strategy = strategy;
    strategy->setPlayer(this);
}

Player::Player(int* playerID, int* reinforcementPool, vector<Player* > unattackablePlayers, vector<Territory* > ownedTerritories, Hand* hand, OrdersList* orders, GameEngine* game,PlayerStrategy* strategy) 
{
    this->playerID = playerID;
    this->reinforcementPool = reinforcementPool;
    this->unattackablePlayers = unattackablePlayers;
    this->ownedTerritories = ownedTerritories;
    this->hand = hand;
    this->orders = orders;
    this->game = game;
    this->strategy = strategy;
    strategy->setPlayer(this);
}

Player::Player(const Player& p)
{
    this->playerID = p.playerID;
    this->reinforcementPool = p.reinforcementPool;
    this->hand = p.hand;
    this->orders = p.orders;

    for (int i = 0; i < p.ownedTerritories.size(); i++)
    {
        this->ownedTerritories.push_back(new Territory(*(p.ownedTerritories[i])));
    }

    for (int i = 0; i < p.unattackablePlayers.size(); i++)
    {
        this->unattackablePlayers.push_back(new Player(*(p.unattackablePlayers[i])));
    }
}

//-------------- Destructor --------------//
Player::~Player()
{
    delete playerID;
    delete reinforcementPool;
    unattackablePlayers.clear();
    ownedTerritories.clear();
    delete hand;
    delete orders;
}

//-------------- Overloads --------------//
std::ostream& operator<<(std::ostream& os, const Player& player) {
    os << "Player" << *player.playerID << ":" << endl;
    os << "Reinforcement pool: " << *player.reinforcementPool << endl;
    os << "Territories owned: " << endl;
    for (Territory *t : player.ownedTerritories)
    {
        os << "\t* " << *t << endl;
    }
    os << "Hand of cards: \n" <<  *player.hand << endl;

    os << "Orders List: \n" << *player.orders << std::endl;

    return os;
}

//Missing overload

//-------------- Getters --------------//
int* Player::getPlayerID()
{
    return this->playerID;
}
int* Player::getReinforcementPool()
{
    return this->reinforcementPool;
}
vector<Player *> Player::getUnattackablePlayers()
{
    return this->unattackablePlayers;
}
vector<Territory *> Player::getOwnedTerritories()
{
    return this->ownedTerritories;
}
Hand* Player::getHand()
{
    return this->hand;
}
OrdersList* Player::getOrders()
{
    return this->orders;
}

PlayerStrategy* Player::getStrategy(){
    return this->strategy;
}

//-------------- Setters --------------//
void Player::setPlayerID(int* playerID)
{
    this->playerID = playerID;
}
void Player::setReinforcementPool(int reinforcementPool)
{
    this->reinforcementPool = new int(reinforcementPool);
}

void Player::setUnattackablePlayers(std::vector<Player*> unattackablePlayers)
{
    this->unattackablePlayers = unattackablePlayers;
}

void Player::setOwnedTerritories(std::vector<Territory*> ownedTerritories)
{
    this->ownedTerritories = ownedTerritories;
}

void Player::setCards(Hand* hand)
{
    this->hand = hand;
}

void Player::setOrders(OrdersList* orders)
{
    this->orders = orders;
}

void Player::setStrategy(PlayerStrategy* strategy)
{
    this->strategy = strategy;
}

//-------------- Others--------------//
//This method prints the content of the "territory" vector to attack.
vector<Territory*> Player::toAttack() {

    return this->strategy->toAttack();
}

vector<Territory*> Player::toDefend() {
    
    return this->strategy->toDefend();
}

//This method takes a string "order" as a parameter and adds this order
//to the orderList
void Player::issueOrder()
{
	return this->strategy->issueOrder();
}

void Player::addUnattackablePlayer(Player* p){
    unattackablePlayers.push_back(p);
}

bool Player::isOwnedTerritory(string name){

	bool owned = false;
    for(Territory* ownedTerritory : ownedTerritories)
        if(*ownedTerritory->getTerritoryName() == name){
            owned = true;
			break;
		}
            
    return owned;
}

void Player::addOwnedTerritory(Territory* t){
	t -> setPlayerID(playerID);
    ownedTerritories.push_back(t);
}

GameEngine* Player::getGame(){
	return this->game;
}



