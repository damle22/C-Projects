// Orders.cpp

#include "Orders.h"
#include "Player.h"

#include <type_traits>
#include <iostream>
#include <string>
#include <algorithm>
using namespace std; 

//----------------------------Order Class----------------------------//

//-------------- Constructors --------------//
Order::Order(){
    this->executed = new bool(false);
    this->effect = new string("Order Not Executed");

    this->logger = new LogObserver();
    this->Attach(logger);
}
Order::Order(const Order &order){
    this->executed = order.executed;
    this->effect = order.effect;
}

//-------------- Overloads --------------//
Order& Order::operator =(const Order &order){
    this->executed = order.executed;
    this->effect = order.effect;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Order& order) {
    return os << *order.effect;
}

//-------------- destructors --------------//
Order::~Order(){
    delete executed;
    delete effect;
    this->Detach(logger);
}

//-------------- Getters --------------//
bool Order::isExecuted() const{
    return *this->executed;
} 

string Order::getEffect() const{
    return *this->effect;
}

string* Order::getName() {return new string{""};}

//-------------- Setters --------------//
void Order::setExecuted(bool value){
    this->executed = new bool(value);
} 

void Order::setEffect(string effect){
    this->effect = new string(effect);
}

string Order::stringToLog()
{
    return getEffect();
}

//----------------------------Deploy Class----------------------------//

//-------------- Constructors --------------//
Deploy::Deploy(){
    this->p = nullptr;
    this->target = nullptr;
    this->numToDeploy = new int(0);

}

Deploy::Deploy(Player* player, Territory* territory, int* numArmies) : Order()
{
    this->p = player;
    this->target = territory;
    this->numToDeploy = numArmies;

 
}

Deploy::Deploy(const Deploy &deploy){
    this->p = deploy.p;
    this->target = deploy.target;
    this->numToDeploy = deploy.numToDeploy;

    setEffect(deploy.getEffect());

}

//-------------- Overloads --------------//
Deploy& Deploy::operator =(const Deploy &deploy){
    this->p = deploy.p;
    this->target = deploy.target;
    this->numToDeploy = deploy.numToDeploy;

    setEffect(deploy.getEffect());
    return *this;
}    
//-------------- destructors --------------//
Deploy::~Deploy(){
    delete p;
    delete target;
    delete numToDeploy;
}

//-------------- Getters --------------//
Player* Deploy::getPlayer()
{
    return this->p;
}

Territory* Deploy::getTarget()
{
    return this->target;
}

int* Deploy::getNumToDeploy()
{
    return this->numToDeploy;
}

string* Deploy::getName(){
    return this->name;
}

//-------------- Other Methods --------------//
bool Deploy::validate() {
    
    if(target == nullptr){
        cout << "Territory does not exist." << endl;
        return false;
    }


    bool validTarget = false;

    for (Territory* t : p->getOwnedTerritories())
    {
        if (t == target)
        {
            validTarget = true;
            break;
        }
    }    
        
    if(validTarget == false) {
        cout << "Player " << *p->getPlayerID() << " does not own Territory " << *target->getTerritoryName() << ". "<< endl;
        return false;
    }

    if(*numToDeploy <= 0){
        cout << "Deploy amount from Player " << *p->getPlayerID() << " is " << *numToDeploy << " which is less than or equal to 0."<< endl;
        return false;
    }

    if(*numToDeploy > *p->getReinforcementPool()){
        cout << "Deploy amount from Player " << *p->getPlayerID() << " is " << *numToDeploy << " which is over the reinforcement pool's amount of " << *p->getReinforcementPool() << ". "<< endl;
        return false;
    }
        

    return true;
}
void Deploy::execute() {

    if(validate()){

        cout << "Player " << *p->getPlayerID() << " is deploying " << *getNumToDeploy() << " armies to territory " << *target->getTerritoryName() << "." << endl;

        int oldNumArmies = *getTarget()->getArmyAmount();
        int newNumArmies = *target->getArmyAmount() + *numToDeploy;

        target->setArmyAmount(*target->getArmyAmount() + *numToDeploy);
        p->setReinforcementPool(*p->getReinforcementPool() - *numToDeploy);

        setExecuted(true);
        setEffect("Deploy order executed: Player " + to_string(*p->getPlayerID()) + " has deployed " + to_string(*getNumToDeploy()) +
        " armies to territory " + *target->getTerritoryName() +". Old army count = " + to_string(oldNumArmies) + ". New army count = " + to_string(newNumArmies) + ".\n");
        
        cout << "Deploy executed\n" << endl;
    }
    else{
        setExecuted(false);
        setEffect("Deploy order did not execute. Deploy order puts a certain number of army units on a target territory.\n");
        cout << "Invalid Deploy Order\n" << endl;
    }
    
    Notify(this);
}




//----------------------------Advance Class----------------------------//

//-------------- Constructors --------------//
Advance::Advance(){
    this->pAttack = nullptr;
    this->pDefend = nullptr;
    this->source= nullptr;
    this->target = nullptr;
    this->map = nullptr;
    this->numToAdvance = new int(0);
    this->game = nullptr;

    
}

Advance::Advance(Player* playerAttack, Player* playerDefend, Territory* source, Territory* target, int* numArmies,GameEngine* game)
{
    this->pAttack = playerAttack;
    this->pDefend = playerDefend;
    this->source= source;
    this->target = target;
    this->numToAdvance = numArmies;
    this->game = game;
    this->map = game->getMap();
}

Advance::Advance(const Advance &advance){
    this->pAttack = advance.pAttack;
    this->pDefend = advance.pDefend;
    this->source= advance.source;
    this->target = advance.target;
    this->map = advance.map;
    this->numToAdvance = advance.numToAdvance;
    this->game = advance.game;

}

//-------------- Overloads --------------//
Advance& Advance::operator =(const Advance &advance){
    this->pAttack = advance.pAttack;
    this->pDefend = advance.pDefend;
    this->source= advance.source;
    this->target = advance.target;
    this->map = advance.map;
    this->numToAdvance = advance.numToAdvance;
    this->game = advance.game;
    return *this;
}    

//-------------- destructors --------------//
Advance::~Advance(){
    delete pAttack;
    delete pDefend;
    delete source;
    delete target;
    delete map;
    delete numToAdvance;
}

//-------------- Getters --------------//
Player* Advance::getPlayerAttack()
{
    return this->pAttack;
}

Player* Advance::getPlayerDefend()
{
    return this->pDefend;
}
Territory* Advance::getSource()
{
    return this->source;
}
Territory* Advance::getTarget()
{
    return this->target;
}

int* Advance::getNumToAdvance()
{
    return this->numToAdvance;
}

string* Advance::getName(){
    return this->name;
}

//-------------- Other Methods --------------//
bool Advance::validate() {

    if(source == nullptr){
        cout << "Source territory does not exist" << endl;
        return false;
    }
    
    if(target == nullptr){
        cout << "Target territory does not exist." << endl;
        return false;
    }
    

    bool validSource = false;

    for (Territory* t : pAttack->getOwnedTerritories())
    {
        if (t == source)
        {
            validSource = true;
        }
    }

    if(validSource == false) {
        cout << "Player " << *pAttack->getPlayerID() << " does not own Territory " << *source->getTerritoryName() << ". "<< endl;
        return false;
    }


    if (map->areConnected(map->getVertex(target), map->getVertex(source)) ==  false){
        cout << *source->getTerritoryName() << " and " << *target->getTerritoryName() << " are not adjacent territories. " << endl;
        return false;
    }


    bool attackable = true;

    if (pAttack->getUnattackablePlayers().size() > 0)
    {
        for (int i = 0; i < pAttack->getUnattackablePlayers().size(); i++)
        {
            if (pAttack->getUnattackablePlayers()[i] == pDefend)
            {
                attackable = false;
                break;
            }
        }
    }

    if(attackable == false) {
        cout << "Player " << *pAttack->getPlayerID() << " cannot attack Player " << *pDefend->getPlayerID() << " for this turn. "<< endl;
        return false;
    }

    
    if(*numToAdvance <= 0){
        cout << "Advance amount from Player " << *pAttack->getPlayerID() << " is " << *numToAdvance << " which is less than or equal to 0."<< endl;
        return false;
    }

    if(*numToAdvance > *source->getArmyAmount()){
        cout << "Advance amount from Player " << *pAttack->getPlayerID() << " is " << *numToAdvance << " which is over the army amount of " << *source->getArmyAmount() << ". "<< endl;
        return false;
    }

    return true;
}

void Advance::execute() {
	if(validate())
    {

        //this means the target is not in the player issuing the order's owned territories
        if (pAttack != pDefend)
        {
            cout << *source->getTerritoryName() << " owned by Player " << *pAttack->getPlayerID() << " is attacking " << *target->getTerritoryName() << " owned by Player " << *pDefend->getPlayerID() << " with an army amount of " << *numToAdvance << "!" << endl;

            if(pDefend->getStrategy()->getStrategyName() == "Neutral")
                pDefend->setStrategy(new AggressivePlayerStrategy());

            int attackersKilled = *target->getArmyAmount() * 0.7;
            int defendersKilled = *numToAdvance * 0.6;

            //Attackers win and take control of the territory
            if ((*numToAdvance - attackersKilled) > (*target->getArmyAmount() - defendersKilled))
            {
                cout << "Territory Conquered!\n";
                target->setArmyAmount(*numToAdvance - attackersKilled);
                source->setArmyAmount(*source->getArmyAmount() - *numToAdvance);

                //Change ownership id of the territory
                
                //Remove the territory from defenders list
                vector<Territory *> loserTerritories = pDefend->getOwnedTerritories();
                
                for(int i = 0; i < loserTerritories.size(); i++)
                {
                    if(target == loserTerritories[i]){
                        loserTerritories.erase(loserTerritories.begin() + i);
                        break;
                    }
                }
                
                pDefend->setOwnedTerritories(loserTerritories);

                //Add the territory to the attackers list
                pAttack -> addOwnedTerritory(target);
                
                string messageCard = "";
                 
                if(!pAttack->receivedCardThisTurn){
                    game->getDeck()->draw(pAttack->getHand());
                    pAttack->receivedCardThisTurn = true;
                    messageCard = "Player has drawn a card";
                }
                else
                    messageCard = "Player has not drawn a card"; 

                
                
                setExecuted(true);
                setEffect("Advance Order executed: Player " + to_string(*pAttack->getPlayerID()) + " won battle against Player "
                    + to_string(*pDefend->getPlayerID()) + " and takes the territory " + *target->getTerritoryName() + ". " + messageCard + ".\n");

            }
            //Defenders win and keep control of the territory
            else
            {
                cout << "Territory was successfully defended!\n";
                target->setArmyAmount(*target->getArmyAmount() - defendersKilled);
                if ((*source->getArmyAmount() - attackersKilled) >= 0)
                {
                    source->setArmyAmount(*source->getArmyAmount() - attackersKilled);
                }
                else
                {
                    source->setArmyAmount(0);
                }

                setExecuted(true);
                setEffect("Advance Order executed: Player " + to_string(*pAttack->getPlayerID()) + " attacks Player "
                    + to_string(*pDefend->getPlayerID()) + " 's territory " + *target->getTerritoryName() + " and lost.\n");
            }

            
        }
        else
        {
            
            int oldSourceAmount = *source->getArmyAmount();
            int oldTargetAmount = *target->getArmyAmount();

            target->setArmyAmount(*target->getArmyAmount() + *numToAdvance);
            source->setArmyAmount(*source->getArmyAmount() - *numToAdvance);

            setExecuted(true);

            setEffect("Advance Order executed: Player " + to_string(*pAttack->getPlayerID()) + " has moved " + to_string(*getNumToAdvance()) +
        " armies from territory " + *source->getTerritoryName() + " with " + to_string(oldSourceAmount) +" of army units to territory " + *target->getTerritoryName() + " with " + to_string(oldTargetAmount) + " of army units. Now, Territory " + *source->getTerritoryName() + " have " + to_string(*source->getArmyAmount())+ " of army units and Territory " + *source->getTerritoryName() + " have " + to_string(*target->getArmyAmount()) + " of army units.\n" );
        }
        
        cout << "Advance executed\n" << endl;
    }
    else{

        setExecuted(false);
        setEffect("Advance order did not execute. Advance order moves a certain number of army units from one territory to another territory.\n");
        cout << "Invalid Advance Order\n" << endl;
    }
        
    Notify(this);
}


//----------------------------Airlift Class----------------------------//

//-------------- Constructors --------------//
Airlift::Airlift(){
    this->p = nullptr;
    this->source = nullptr;
    this->target = nullptr;
    this->numToAirlift = new int(0);
}

Airlift::Airlift(Player* player, Territory* sourceTerritory, Territory* targetTerritory, int* numArmies)
{
    this->p = player;
    this->source = sourceTerritory;
    this->target = targetTerritory;
    this->numToAirlift = numArmies;
    
}
Airlift::Airlift(const Airlift &airlift){
    this->p = airlift.p;
    this->source = airlift.source;
    this->target = airlift.target;
    this->numToAirlift = airlift.numToAirlift;

}

//-------------- Overloads --------------//
Airlift& Airlift::operator =(const Airlift &airlift){
    this->p = airlift.p;
    this->source = airlift.source;
    this->target = airlift.target;
    this->numToAirlift = airlift.numToAirlift;

    return *this;
}

//-------------- destructors --------------//
Airlift::~Airlift(){
    delete p;
    delete source;
    delete target;
    delete numToAirlift;
}

//-------------- Getters --------------//
Player* Airlift::getPlayer()
{
    return this->p;
}

Territory* Airlift::getSource()
{
    return this->source;
}

Territory* Airlift::getTarget()
{
    return this->target;
}

int* Airlift::getNumToAirlift()
{
    return this->numToAirlift;
}
string* Airlift::getName(){
    return this->name;
}

//-------------- Other Methods --------------//
bool Airlift::validate() {
    
    if(source == nullptr){
        cout << "Source territory does not exist." << endl;
        return false;
    }
    
    if(target == nullptr){
        cout << "Target territory does not exist." << endl;
        return false;
    }

    bool validTarget = false;
    bool validSource = false;

    for (Territory* t : p->getOwnedTerritories())
    {
        if (t == target)
            validTarget = true;
        if (t == source)
            validSource = true;
    }

    if(validSource == false) {
        cout << "Player " << *p->getPlayerID() << " does not own Territory " << *source->getTerritoryName() << ". "<< endl;
        return false;
    }

    if(validTarget == false) {
        cout << "Player " << *p->getPlayerID() << " does not own Territory " << *target->getTerritoryName() << ". "<< endl;
        return false;
    }

    if(*numToAirlift <= 0){
        cout << "Airlift amount from Player " << *p->getPlayerID() << " is " << *numToAirlift << " which is less than or equal to 0."<< endl;
        return false;
    }

    if(*numToAirlift > *source->getArmyAmount()){
        cout << "Airlift amount from Player " << *p->getPlayerID() << " is " << *numToAirlift << " which is over the army amount of " << *source->getArmyAmount() << ". "<< endl;
        return false;
    }

    return true;
}
void Airlift::execute() {
    if(validate()){
        cout << "Player " << *p->getPlayerID() << " is moving " << *getNumToAirlift() << " armies from territory " << *source->getTerritoryName() << " to territory " + *target->getTerritoryName() << " with airlift. " << endl;

        int oldSourceAmount = *source->getArmyAmount();
        int oldTargetAmount = *target->getArmyAmount();

        source->setArmyAmount(*source->getArmyAmount() - *numToAirlift);
        target->setArmyAmount(*target->getArmyAmount() + *numToAirlift);
        setExecuted(true);

        setEffect("Airlift Order executed: Player " + to_string(*p->getPlayerID()) + " has succesfully moved " + to_string(*getNumToAirlift()) +
        " armies from territory " + *source->getTerritoryName() + " with " + to_string(oldSourceAmount) +" of army units to territory " + *target->getTerritoryName() + " with " + to_string(oldTargetAmount) + " of army units via airlift. Now, Territory " + *source->getTerritoryName() + " have " + to_string(*source->getArmyAmount())+ " of army units and Territory " + *source->getTerritoryName() + " have " + to_string(*target->getArmyAmount()) + " of army units.\n" );

        cout << "Airlift executed\n" << endl;
    }
    else{
        setExecuted(false);
        setEffect("Airlift Order did not execute. Airlift Order advances a certain number of army units from one from one territory to another territory.\n");
        cout << "Invalid Airlift Order\n" << endl;
    }

    Notify(this);  
}

//----------------------------Bomb Class----------------------------//

//-------------- Constructors --------------//
Bomb::Bomb(){
    this->p = nullptr;
    this->target = nullptr;
    this->map = nullptr;
}
Bomb::Bomb(Player* player, Territory* targetTerritory, Map* map)
{
    this->p = player;
    this->target = targetTerritory;
    this->map = map;
    
}
Bomb::Bomb(const Bomb &bomb){
    this->p = bomb.p;
    this->target = bomb.target;
    this->map = bomb.map;

}

//-------------- Overloads --------------//
Bomb& Bomb::operator =(const Bomb &bomb){
    this->p = bomb.p;
    this->target = bomb.target;
    this->map = bomb.map;
    return *this;
}


//-------------- destructors --------------//
Bomb::~Bomb(){
    delete p;
    delete target;
    delete map;
}

//-------------- Getters --------------//
Player* Bomb::getPlayer(){
    return this->p;
}
Territory* Bomb::getTarget(){
    return this->target;
}
Map* Bomb::getMap(){
    return this->map;
}
string* Bomb::getName(){
    return this->name;
}
//-------------- Other Methods --------------//
bool Bomb::validate() {
    if(target == nullptr){
        cout << "Territory does not exist." << endl;
        return false;
    }
    bool validTarget = true;
    
    for (Territory* t : p->getOwnedTerritories())
    {
        if (t == target)
        {
            validTarget = false;
            break;
        }
    }

    if(validTarget == false) {
        cout << "Player " << *p->getPlayerID() << " does not own Territory " << *target->getTerritoryName() << ". "<< endl;
        return false;
    }

    bool adjacent = false;

    for (Territory* t : p->getOwnedTerritories())
    {
        if (map->areConnected(map->getVertex(t), map->getVertex(target)))
        {
            adjacent = true;
            break;
        }
    }

    if (adjacent ==  false){
        cout <<  *target->getTerritoryName() << " is not adjacent to any of the territories owned by Player " << *p->getPlayerID() << "."<< endl;
        return false;
    }

    return true;
}

void Bomb::execute() {
	if(validate()){

        cout << *target->getTerritoryName() + " is getting bombed and its army units is halved."  << endl;

        int oldTargetArmy = *target->getArmyAmount();
        target->setArmyAmount(*target->getArmyAmount() / 2);

        cout << "Bomb executed\n" << endl;
        setExecuted(true);
        setEffect("Bomb Order executed: Successfully bombed " + *target->getTerritoryName() + " territory that and its army units went from " + to_string(oldTargetArmy) + " to " + to_string(*target->getArmyAmount()) +".\n");

    }
    else{
        setExecuted(false);
        setEffect("Bomb Order did not execute. Bomb order destroys half of the army units located on a target territory.\n");
        cout << "Invalid Bomb Order\n" << endl;
    }
        
    Notify(this);	
}



//----------------------------Blockade Class----------------------------//

//-------------- Constructors --------------//
Blockade::Blockade(){
    this->p = nullptr;
    this->target = nullptr;
}

Blockade::Blockade(Player* player, Territory* targetTerritory)
{
    this->p = player;
    this->target = targetTerritory;
    
}
Blockade::Blockade(const Blockade &blockade){
    this->p = blockade.p;
    this->target = blockade.target;
}

//-------------- Overloads --------------//
Blockade& Blockade::operator =(const Blockade &blockade){
    this->p = blockade.p;
    this->target = blockade.target;
    return *this;
}


//-------------- destructors --------------//
Blockade::~Blockade(){
    delete p;
    delete target;
}

//-------------- Getters --------------//
Player* Blockade::getPlayer(){
    return this->p;
}
Territory* Blockade::getTarget(){
    return this->target;
}
string* Blockade::getName(){
    return this->name;
}

//-------------- Other Methods --------------//
bool Blockade::validate() {

    
    if(target == nullptr){
        cout << "Territory does not exist." << endl;
        return false;
    }

    bool validTarget = false;

    for (Territory* t : p->getOwnedTerritories())
    {
        if (t == target)
        {
            validTarget = true;
            break;
        }
    }

    if(validTarget == false) {
        cout << "Player " << *p->getPlayerID() << " does not own Territory " << *target->getTerritoryName() << ". "<< endl;
        return false;
    }

    return true;
}

void Blockade::execute() {
    if(validate()){

        cout << *target->getTerritoryName() + "'s army is doubled and its ownership is changed to the neutral player." << endl;

        //Double the number of armies on the target territory
        int oldTargetArmy = *target->getArmyAmount();

        target->setArmyAmount(oldTargetArmy * 2);

        //Transfer ownership of the target territory to the neutral player
        
        
        
        vector<Territory *> newOwnedTerritories = p->getOwnedTerritories();
        
        for(int i = 0; i < newOwnedTerritories.size(); i++){
            if(target == newOwnedTerritories[i]){
                newOwnedTerritories.erase(newOwnedTerritories.begin() + i);
                break;
            }
        }

        p->setOwnedTerritories(newOwnedTerritories);

        bool neutralExist = false;
        Player* neutralPlayer = nullptr;
        for (Player* pNeutral : p->getGame()->getPlayers()){
            if(pNeutral->getStrategy()->getStrategyName() == "Neutral"){
                neutralExist = true;
                neutralPlayer = pNeutral;
                break;
            } 
        }

        if(neutralExist){
            target->setPlayerID(neutralPlayer->getPlayerID());

            vector <Territory*> list = neutralPlayer->getOwnedTerritories();
            list.push_back(target);
            neutralPlayer->setOwnedTerritories(list);
        }
        else{
                
            neutralPlayer = (new Player(new int(0),p->getGame(),new NeutralPlayerStrategy()));

            vector <Territory*> newList = neutralPlayer->getOwnedTerritories();
            newList.push_back(target);
            neutralPlayer->setOwnedTerritories(newList);

            p->getGame()->addPlayer(neutralPlayer);
        }

        setExecuted(true);
        setEffect("Blockade order executed: Successfully doubled the number of armies in " + *target->getTerritoryName() + " territory" +
        " from " +  to_string(oldTargetArmy) + " to " + to_string(*target->getArmyAmount()) + " army units and ownership changed to neutral player.\n");
        cout << "Blockade executed\n" << endl;
    }

    else
    {
        setExecuted(false);
        setEffect("Blockade order did not execute: Blockade order triples the number of army units on a target territory and make it a neutral territory.\n");
        cout << "Invalid Blockade Order\n" << endl;
    }

    Notify(this);
}


//----------------------------Negotiate Class----------------------------//

//-------------- Constructors --------------//
Negotiate::Negotiate(){
    this->p1 = nullptr;
    this->p2 = nullptr;
}
Negotiate::Negotiate(Player* player1, Player* player2)
{
    this->p1 = player1;
    this->p2 = player2;
    
}
Negotiate::Negotiate(const Negotiate &negotiate){
    this->p1 = negotiate.p1;
    this->p2 = negotiate.p2;
}

//-------------- Overloads --------------//
Negotiate& Negotiate::operator =(const Negotiate &negotiate){
    this->p1 = negotiate.p1;
    this->p2 = negotiate.p2;
    return *this;
}


//-------------- destructors --------------//
Negotiate::~Negotiate(){
    delete p1;
    delete p2;
}

//-------------- Getters --------------//
Player* Negotiate::getPlayer1(){
    return this->p1;
}
Player* Negotiate::getPlayer2(){
    return this->p2;
}
string* Negotiate::getName(){
    return this->name;
}

//-------------- Other Methods --------------//
bool Negotiate::validate() {
    
    if (p1 == p2){
        cout << "The two players are the same player."<< endl;
        return false;
    }

    return true;    
}

void Negotiate::execute() {
    if(validate())
	{
        cout << "Player " << *p1->getPlayerID() << " and Player " << *p2->getPlayerID() << " are prevented to attack each other until the end of the turn.\n" << endl;

        p1->addUnattackablePlayer(p2);
        p2->addUnattackablePlayer(p1);

        setExecuted(true);
        setEffect("Negotiate order executed: Attacking between Player " + to_string(*p1->getPlayerID()) + " and Player " + to_string(*p2->getPlayerID()) + " has been prevented until the end of the turn.\n");

        cout << "Negotiate executed\n" << endl;
    }
    else{
        setExecuted(false);
        setEffect("Negotiate order did not execute. Negotiate Order prevents attacks between the current player and another target player until the end of the turn.\n");
        cout << "Invalid Negotiate Order\n" << endl;
    }
        
    Notify(this);
}



//----------------------------OrdersList Class----------------------------//

//-------------- Constructors --------------//

OrdersList::OrdersList(){

    this->logger = new LogObserver();
    this->Attach(logger);
}
OrdersList::OrdersList(const OrdersList &ordersList) : orders(ordersList.orders){}

//-------------- Destructor --------------//
OrdersList::~OrdersList(){
    orders.clear();
    this->Detach(logger);
}


OrdersList& OrdersList::operator =(const OrdersList &ordersList){
    this->orders = ordersList.orders;
    return *this;}

std::ostream& operator<<(std::ostream& os, const OrdersList& ordersList) {
    for (size_t i = 0; i < ordersList.orders.size(); ++i) {
        os << "Order " << i << ": " << *(ordersList.orders[i]) << std::endl;
    }

    return os;
}

// Use push_back to add the order to the vector orders
void OrdersList::addOrder(Order* order) {
    orders.push_back(order);
    Notify(this);
}


void OrdersList::removeOrder(Order* o) {
    orders.erase(remove(orders.begin(), orders.end(), o), orders.end());
}

void OrdersList::move(int fromIndex, int toIndex) {
     // must check if both indexes are part of the range of the vector orders
    if (fromIndex >= 0 && fromIndex < orders.size() && toIndex >= 0 && toIndex <= orders.size()) {
        // Save the order of position fromIndex
        Order* order = orders[fromIndex];
        // remove from the vector orders
        orders.erase(orders.begin() + fromIndex);
        // Add the order at position toIndex
        orders.insert(orders.begin() + toIndex, order);
    }
    else{
        std::cout << "Indexes do not respect the range of the list." << std::endl;
    }
}

// Use execute method for all orders in the vector
void OrdersList::executeOrders() {
    for (Order* order : orders) {
            order->execute();
    }
}

//  Use the stream insertion operator to print description of each order
void OrdersList::describeOrders() {
    for (size_t i = 0; i < orders.size(); ++i) {
        std::cout << "Order " << i << ": " << *(orders[i]) << std::endl;
    }
}

vector<Order*> OrdersList::getOrdersList(){
    return this->orders;
}

string OrdersList::stringToLog()
{
    string out = "Inserted ";
    Order* o = orders.back();
    out += "Order: " + *o->getName() + "";
    out += " into the list.";
    return out;
}
