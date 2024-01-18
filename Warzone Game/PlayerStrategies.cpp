#include "PlayerStrategies.h"
#include <vector>
#include <algorithm>



PlayerStrategy::PlayerStrategy() {
	this->player = nullptr;
}


PlayerStrategy::PlayerStrategy(const PlayerStrategy& ps) {
	this->player = ps.player;
    //player->setStrategy(this);
}

PlayerStrategy::~PlayerStrategy() = default;

PlayerStrategy& PlayerStrategy::operator = (const PlayerStrategy& ps) {
	if (&ps != this) {
		this->player = ps.player;
        //player->setStrategy(this);
	}
	return *this;
}

ostream& operator << (ostream& out, const PlayerStrategy& ps) {
	cout << ps.getStrategyName();
	return out;
}

Player* PlayerStrategy::getPlayer() {
	return player;
}

void PlayerStrategy::setPlayer(Player* p) {
	this->player = p;
}

//default constructor
NeutralPlayerStrategy::NeutralPlayerStrategy() : PlayerStrategy() {}

NeutralPlayerStrategy::~NeutralPlayerStrategy() = default;

// Copy constructor
NeutralPlayerStrategy::NeutralPlayerStrategy(const NeutralPlayerStrategy& neutralPlayerStrategy) : PlayerStrategy(neutralPlayerStrategy) {}

// Assignment operator
NeutralPlayerStrategy& NeutralPlayerStrategy::operator=(const NeutralPlayerStrategy& neutralPlayerStrategy) {
	PlayerStrategy::operator = (neutralPlayerStrategy);
	return *this;
}

// stream insertion operator
ostream& operator << (ostream& out, const NeutralPlayerStrategy& ps) {
	out << ps.getStrategyName();
	return out;
}


// neutral player doesn't issue order
void NeutralPlayerStrategy::issueOrder() {
	// Introduction message for each player
	cout << "/*------------------------------------------------------------------------------------------------*/" << endl; 
	cout << "\nPlayer " << *player->getPlayerID() << " is neutral. Therefore, player will never issue any order, nor use any cards.\n" << endl;
	
	
}

// does not do anything - neutral player doesn't attack
vector<Territory*> NeutralPlayerStrategy::toAttack() {
	 // Vector containing all the territories that can be attacked
	vector<Territory*> attackableTerritories;

    Map* m = player->getGame()->getMap();

	// Double for-loop that adds the attackable territory to the vector
	for (Territory* owned : player->getOwnedTerritories()) {

        Node* node = m->getVertex(owned);

		for (Node* adjacent : node->getEdges()) 
            if (!player->isOwnedTerritory(*adjacent->getTerritoryPtr()->getTerritoryName())) 
				attackableTerritories.push_back(adjacent->getTerritoryPtr());
	
	}	

    // Remove duplicates		
	for (Territory* t : attackableTerritories) 
		attackableTerritories.erase(unique(attackableTerritories.begin(), attackableTerritories.end()), attackableTerritories.end());

    return attackableTerritories;
}

// return sorted list of owned territories from weskest to strongest
vector<Territory*> NeutralPlayerStrategy::toDefend() {
	return player->getOwnedTerritories();
}

// strategy name getter
string NeutralPlayerStrategy::getStrategyName() const {
	return "Neutral";
}



// default constructor
CheaterPlayerStrategy::CheaterPlayerStrategy() : PlayerStrategy() {}

// copy constructor
CheaterPlayerStrategy::CheaterPlayerStrategy(const CheaterPlayerStrategy& cps) : PlayerStrategy(cps) {}

// destructor
CheaterPlayerStrategy::~CheaterPlayerStrategy() = default;

// assignment operator
CheaterPlayerStrategy& CheaterPlayerStrategy::operator=(const CheaterPlayerStrategy& cps) {
    PlayerStrategy::operator = (cps);
    return *this;
}

// stream insertion operator
ostream& operator<<(ostream& os, const CheaterPlayerStrategy& cps) {
    os << cps.getStrategyName();
    return os;
}

// conquers all neighbouring territories
void CheaterPlayerStrategy::issueOrder() {
	Deck* deck = player->getGame()->getDeck();

	// Introduction message for each player
	cout << "/*------------------------------------------------------------------------------------------------*/" << endl; 
	cout << "\nPlayer " << *player->getPlayerID() << " is a cheater. Player automatically conquers all territories that are adjacent " << 
	" to its own territories. " << endl;
	cout << endl;

	vector <Territory*> territories  = player->getOwnedTerritories();

	for(Territory* t : toAttack()){
		t->setPlayerID(player->getPlayerID());
		territories.push_back(t);
	}

	player->setOwnedTerritories(territories);

}

// return list of all enemy territories neighbouring player's own
vector<Territory*> CheaterPlayerStrategy::toAttack() {
	
	// Vector containing all the territories that can be attacked
	vector<Territory*> attackableTerritories;

    Map* m = player->getGame()->getMap();

	// Double for-loop that adds the attackable territory to the vector
	for (Territory* owned : player->getOwnedTerritories()) {

        Node* node = m->getVertex(owned);

		for (Node* adjacent : node->getEdges()) 
            if (!player->isOwnedTerritory(*adjacent->getTerritoryPtr()->getTerritoryName())) 
				attackableTerritories.push_back(adjacent->getTerritoryPtr());
	
	}	

    // Remove duplicates		
	for (Territory* t : attackableTerritories) 
		attackableTerritories.erase(unique(attackableTerritories.begin(), attackableTerritories.end()), attackableTerritories.end());

    return attackableTerritories;
}

// cheater does not defend
vector<Territory*> CheaterPlayerStrategy::toDefend() {
    return player->getOwnedTerritories();
}

// strategy name getter
string CheaterPlayerStrategy::getStrategyName() const {
    return "Cheater";
}





//default constructor
HumanPlayerStrategy::HumanPlayerStrategy() : PlayerStrategy() {}

HumanPlayerStrategy::~HumanPlayerStrategy() = default;

// Copy constructor
HumanPlayerStrategy::HumanPlayerStrategy(const HumanPlayerStrategy& humanPlayerStrategy) : PlayerStrategy(humanPlayerStrategy) {}

// Assignment operator
HumanPlayerStrategy& HumanPlayerStrategy::operator=(const HumanPlayerStrategy& humanPlayerStrategy) {
    PlayerStrategy::operator = (humanPlayerStrategy);
    return *this;
}

// stream insertion operator
ostream& operator << (ostream& out, const HumanPlayerStrategy& ps) {
    out << ps.getStrategyName();
    return out;
}


// prompts user for deploy and card orders parameters, then for a choice of attack/defend advance orders or do nothing
void HumanPlayerStrategy::issueOrder() {
    Deck* deck = player->getGame()->getDeck();

	cout << "/*------------------------------------------------------------------------------------------------*/" << endl; 

	// Introduction message for each player
	cout << "\nIssuing orders for Player " << *player->getPlayerID() << ":" << endl;
	

	cout << "#--- Deploying Phase ---#" << endl;

	// List of territories to defend
	cout << "\nTerritories to defend:" << endl;
	for (Territory* t : toDefend()) {
		cout << " Territory: " + *t->getTerritoryName() << " , Armies: " << *t->getArmyAmount() << endl;
	}
	cout << endl;

	int currentReinforcementPool = *player->getReinforcementPool();

	// Deploy all to strongest territory
	while (currentReinforcementPool > 0) {

		// Announce how big the reinforcement pool is
		cout << "Player " << *player->getPlayerID() << "'s number of armies left in the reinforcement pool: " << currentReinforcementPool << endl << endl;

		// Prompt the user to choose index of territory to defend
		cout << "Input the name of the defending territory you want to deploy armies to: ";
		string tName;
		cin >> tName;
		cout << endl;

		

		// Prompt the user to input number of armies to deploy
		cout << "Input how many armies you wish to deploy to that territory: ";
		int deployNum;
		cin >> deployNum;
		cout << endl;

			
		// If the user enters a number that is less than the total number of armies in the reinforcement pool 
		if (deployNum <= currentReinforcementPool) {

			// Negative input
			if (deployNum <= 0) {
				cout << "Cannot input a negative number!" << endl;
			}

			// Non-negative input that is less or equal to number of armies available
			else {
                    
                Territory* t = player->getGame()->getMap()->getTerritoryFromMap(tName);

				// Add a deploy order to the player's list of orders
				player->getOrders()->addOrder(new Deploy(this->player, t, new int(deployNum)));
					
					// Sets the reinforcement pool with whatever is left after the user's input
				currentReinforcementPool = currentReinforcementPool - deployNum;
				cout << deployNum << " armies will be deployed!" << endl;
			}
		}

		// If the user's input is bigger than the reinforcement pool
		else {
			cout << "Insufficient number of armies available in reinforcement pool, you can deploy at most " << *player->getReinforcementPool()<< " armies!" << endl;
		}
		
	}

	cout << "\n#--- Deploying Phase OVER ---#" << endl;
	

	// All deploy orders have been issued at this point!

	// Now issuing advance orders
	cout << "\n#--- Advancing Phase ---#" << endl;

	// Advance orders are optional, ask user whether they want to issue any
	cout << "Do you wish to issue any advance orders?" << endl
		<< "Reply with \"yes\" if you wish to do so, any other input will be assumed to be a no : ";

	string isAdvancing;
	cin >> isAdvancing;
	cout << endl;

	// "y" as input for yes
	while (isAdvancing == "yes") {

		// Two options to choose from
		cout << "Do you wish to :" << endl;
		cout << "1. (DEFENSE) : Move armies from their own territories to defend another?" << endl;
		cout << "OR" << endl;
		cout << "2. (ATTACK) : Move armies from their own territories to a neighboring enemy territory to attack?" << endl;
		cout << "Reply with either number 1 or 2 : ";
		int optionAdvance;
		cin >> optionAdvance;
		cout << endl;

		// Option 1 (DEFENSE)
		if (optionAdvance == 1) {

			// List of territories to defend
			cout << "\nTerritories to defend: " << endl;
			for (Territory* t : toDefend()) {
				cout <<  "Territory: " + *t->getTerritoryName() << " , Armies: " << *t->getArmyAmount() << endl;
			}
			cout << endl;

			// Accept input for source territory's index (where armies originate)
			cout << "Input the souce territory's name: ";
			string sourceName;
			cin >> sourceName;
			cout << endl;

			// Accept input for target territory's index (where armies go to)
			cout << "Input the target territory's name: ";
			string targetName;
			cin >> targetName;
			cout << endl;

			// Accept input for number of armies to move
			cout << "Finally, input the number of armies you wish to move: ";
			int army;
			cin >> army;
			cout << endl;

			// Convert the territory index's to territory pointers
			Territory* source = player->getGame()->getMap()->getTerritoryFromMap(sourceName);
			Territory* target = player->getGame()->getMap()->getTerritoryFromMap(targetName);

			// Adds the advance order to the player's list of order
			player->getOrders()->addOrder(new Advance(this->player, this->player, source, target, new int(army), player->getGame()));	
		}

		// Option 2 (ATTACK)
		else if (optionAdvance == 2) {
			// List of territories to defend
			cout << "\nTerritories to defend:" << endl;
            for (Territory* t : toDefend()) {
				cout <<  "Territory: " + *t->getTerritoryName() << " , Armies: " << *t->getArmyAmount() << endl;
			}
			cout << endl;

			// List of territories to attack
			cout << "\nTerritories to attack: " << endl;
			for (Territory* t : toAttack()) {
				cout <<  "Territory: " + *t->getTerritoryName() << " , Armies: " << *t->getArmyAmount() << endl;
			}
			cout << endl;

			// Accept input for source territory's index (where armies originate)
			cout << "Input the souce territory's name: ";
			string sourceName;
			cin >> sourceName;
			cout << endl;

			
			// Accept input for target territory's index (where armies go to)
			cout << "Input the target territory's name: ";
			string targetName;
			cin >> targetName;
			cout << endl;

			// Convert source and targert territories from index to a territory pointer
			Territory* source = player->getGame()->getMap()->getTerritoryFromMap(sourceName);
			Territory* target = player->getGame()->getMap()->getTerritoryFromMap(targetName);

                
			// Accept input for number of armies to move
			cout << "Finally, input the number of armies you wish to move: ";
			int army;
			cin >> army;
			cout << endl;

            //Find target player
            Player* targetPlayer = nullptr;
        
            for(Player* p : player->getGame()->getPlayers())
                if(*p->getPlayerID() == *target->getPlayerID())
                    targetPlayer = p;

			//Takes care of what happens if diplomacy was used last turn
			player->getOrders()->addOrder(new Advance(this->player, targetPlayer, source, target, new int(army), player->getGame()));
					
		}

		// Wrong input (not 1 or 2)
		else {
			cout << "Wrong input, try again (Input only 1 or 2)" << endl;
		}

		// Issue another advance order
		cout << "\nDo you wish to issue another advance order? " << endl;
		cout << "Reply with \"yes\" if you wish to do so, any other input will be assumed to be a no : ";
		cin >> isAdvancing;
		cout << endl;
	}

	cout << "\n#--- Advancing Phase OVER ---#" << endl;
	

	// All advance orders have been issued at this point!

	// Clearing the cannotAttack vector for Negotiate order
	player->getUnattackablePlayers().clear();

	// Now playing a card, Player plays one card per turn
	cout << "\n#--- Card Playing Phase ---#" << endl << endl;

	// Check if player has any cards in hand
	if (player->getHand()->size() > 0) {
		// Print the player's hand
		cout << *player->getHand() << endl;

		// Ask input for the index of the card to play
		cout << "Select the index of the card you want to use : ";

		int cardIndex;
		cin >> cardIndex;
		cout << endl;

		// If input index is wrong, ask for input again
		while (cardIndex < 0 || cardIndex >= player->getHand()->size()) {
			cout << "Wrong index selected, please input another index : ";
			cin >> cardIndex;
			cout << endl;
		}

		// Convert input index to a card pointer
		Card* card = player->getHand()->getCard(cardIndex);

		Card::ctype type = card->getCardType();

		// Cases for each type of card to be played and its required input
		if (type == Card::ctype::bomb) {
			cout << "Bomb card selected:" << endl;

			cout << "Which territory should be bombed? Input a name from the list: " << endl;

			// List of territories to attack
			cout << "\nTerritories to attack: " << endl;
			for (Territory* t : toAttack()) {
				cout <<  "Territory: " + *t->getTerritoryName() << " , Armies: " << *t->getArmyAmount() << endl;
			}
			cout << endl;

			// Input target territory na,e
			string targetName;
			cin >> targetName;
			cout << endl;

			// Convert territory index to territory pointer
			Territory* target = player->getGame()->getMap()->getTerritoryFromMap(targetName);

			card->play(card,new int(0), this->player, nullptr, nullptr, target, player->getGame());

			cout << "Bomb order will be issued!";
		}
		else if (type == Card::ctype::reinforcement) {
			cout << "Reinforcement card selected --> Will be played immediately below: " << endl;
			card->play(card, new int(0), this->player, nullptr, nullptr, nullptr, player->getGame());
		}
		else if (type == Card::ctype::blockade) {
			cout << "Blockade card selected:" << endl;

			cout << "Which territory should receive a blockade? Input the name from the list: " << endl;

			// List of territories to defend
			cout << "\nTerritories to defend:" << endl;
            for (Territory* t : toDefend()) {
                cout << " Territory: " + *t->getTerritoryName() << " , Armies: " << *t->getArmyAmount() << endl;
            }
            cout << endl;

			// Input enemy territory index
			string tName;
			cin >> tName;
			cout << endl;

			// Convert territory index to territory pointer
			Territory* target = player->getGame()->getMap()->getTerritoryFromMap(tName);

			card->play(card, new int(0), this->player, nullptr, nullptr, target, player->getGame());

			cout << "Blockade order will be issued!";
		}
		else if (type == Card::ctype::airlift) {
			cout << "Airlift card selected:" << endl;

			cout << "Where should the airlift start? Input the name from the list: " << endl;

			// List of territories to defend
			cout << "\nTerritories to defend:" << endl;
            for (Territory* t : toDefend()) {
                cout << " Territory: " + *t->getTerritoryName() << " , Armies: " << *t->getArmyAmount() << endl;
            }
            cout << endl;

			// Input enemy territory index
			string tName;
			cin >> tName;
			cout << endl;

			// Convert territory index to territory pointer
			Territory* t1 = player->getGame()->getMap()->getTerritoryFromMap(tName);

			cout << "Where should the airlift head to? Input the name from the list: " << endl;

			// List of territories to defend
			cout << "\nTerritories to defend:" << endl;
            for (Territory* t : toDefend()) {
                cout << " Territory: " + *t->getTerritoryName() << " , Armies: " << *t->getArmyAmount() << endl;
            }
            cout << endl;

			// Input enemy territory name
			string tName2;
			cin >> tName2;
			cout << endl;

			cout << "Input no of armies: " << endl;
			int army; cin >> army;
			cout << endl;

			// Convert territory index to territory pointer
			Territory* t2 = player->getGame()->getMap()->getTerritoryFromMap(tName2);

			card->play(card, new int(army), this->player, nullptr, t1, t2, player->getGame());

			cout << "Airlift order will be issued!";
		}
		else if (type == Card::ctype::diplomacy) {
			cout << "Diplomacy card selected:" << endl;

			cout << "Who should Diplomacy be used on? Input the playerID from the list: " << endl;

			// List of players in the game
			cout << "\nList of players in the game: (Index : Name)" << endl;
			for (int i = 0; i < player->getGame()->players.size(); i++) {
				cout << player->getGame()->players.at(i)->getPlayerID() << endl;
			}
			cout << endl;

			// Input other player's index
			int id;
			cin >> id;
			cout << endl;

            // If input index is wrong, ask for input again
		    while (id < 0 || id >= player->getGame()->players.size()) {
			    cout << "Wrong index selected, please input another index : ";
			    cin >> id;
			    cout << endl;
		    }

			Player* otherP = player->getGame()->players.at(id - 1);

			card->play(card, 0, this->player, otherP, nullptr, nullptr, player->getGame());

			cout << "Diplomacy order will be issued!";
		}
	}

	// Player has no cards in their hand
	else {
		cout << "Player " << *player->getPlayerID() << " has no cards in their hand, skipping the card playing phase!" << endl;
	}

	cout << "\n#--- Card Playing Phase OVER ---#" << endl;
	
}

// returns list of enemy territories neighbouring player's own
vector<Territory*> HumanPlayerStrategy::toAttack() {
    // Vector containing all the territories that can be attacked
	vector<Territory*> attackableTerritories;

    Map* m = player->getGame()->getMap();

	// Double for-loop that adds the attackable territory to the vector
	for (Territory* owned : player->getOwnedTerritories()) {

        Node* node = m->getVertex(owned);

		for (Node* adjacent : node->getEdges()) 
            if (!player->isOwnedTerritory(*adjacent->getTerritoryPtr()->getTerritoryName())) 
				attackableTerritories.push_back(adjacent->getTerritoryPtr());
	
	}	

    // Remove duplicates		
	for (Territory* t : attackableTerritories) 
		attackableTerritories.erase(unique(attackableTerritories.begin(), attackableTerritories.end()), attackableTerritories.end());

    return attackableTerritories;
}

// return own territories sorted from weakest to strongest
vector<Territory*> HumanPlayerStrategy::toDefend() {
   return player->getOwnedTerritories();
}

string HumanPlayerStrategy::getStrategyName() const {
    return "Human";
}

// default constructor
AggressivePlayerStrategy::AggressivePlayerStrategy(){
    
}

AggressivePlayerStrategy::~AggressivePlayerStrategy() = default;

// copy constructor
AggressivePlayerStrategy::AggressivePlayerStrategy(const AggressivePlayerStrategy& aps) : PlayerStrategy(aps) {
   
};

// assignment operator
AggressivePlayerStrategy& AggressivePlayerStrategy::operator=(const AggressivePlayerStrategy& aps) {
    PlayerStrategy::operator = (aps);
    return *this;
}

// stream insertion operator
ostream& operator << (ostream& out, const AggressivePlayerStrategy& aps) {
    out << aps.getStrategyName();
    return out;
}

// issue order method - either issue 1 deploy order, 1 card order, 1 advance defend, 1 advance attack or no order
void AggressivePlayerStrategy::issueOrder() {
 Deck* deck = player->getGame()->getDeck();

	// Introduction message for each player
	cout << "/*------------------------------------------------------------------------------------------------*/" << endl; 
	cout << "\nIssuing orders for Player " << *player->getPlayerID() << " that is aggressive:" << endl;
	cout << endl;

	cout << "#--- Deploying Phase ---#\n" << endl;

	
	player->getOrders()->addOrder(new Deploy(this->player, strongestTerritory(toDefend()), player->getReinforcementPool()));
	cout << "Deploy order of " << *player->getReinforcementPool() << " armies will be issued by Player " << *player->getPlayerID() <<"!" << endl;

	cout << "\n#--- Deploying Phase OVER ---#\n" << endl;
	

	// All deploy orders have been issued at this point!

	// Now issuing advance orders
	cout << "#--- Advancing Phase ---#\n" << endl;

	Territory* source = strongestTerritory(toDefend());
	Territory* target = strongestTerritory(toAttack());

	Player* targetPlayer = nullptr;
        
    for(Player* p : player->getGame()->getPlayers())
        if(*p->getPlayerID() == *target->getPlayerID())
            targetPlayer = p;
	
	cout << "Advance order will be issued by Player " << *player->getPlayerID() << " to attack Player " << *targetPlayer->getPlayerID()<< "!" << endl;

	player->getOrders()->addOrder(new Advance(this->player, targetPlayer, source, target, source->getArmyAmount(), player->getGame()));

	cout << "\n#--- Advancing Phase OVER ---#\n" << endl;
	

	// All advance orders have been issued at this point!

	// Clearing the cannotAttack vector for Negotiate order
	player->getUnattackablePlayers().clear();

	// Now playing a card, Player plays one card per turn
	cout << "#--- Card Playing Phase ---#" << endl << endl;

	// Check if player has any cards in hand
	if (player->getHand()->size() > 0) {
		
		// Convert input index to a card pointer
		Card* card = player->getHand()->getCard(0);

		Card::ctype type = card->getCardType();

		// Cases for each type of card to be played and its required input
		if (type == Card::ctype::bomb) {
			cout << "Bomb card is selected!" << endl;

			card->play(card,new int(0), this->player, nullptr, nullptr, strongestTerritory(toAttack()), player->getGame());

			cout << "Bomb order will be issued!" << endl;
		}
		else if (type == Card::ctype::reinforcement) {
			cout << "Reinforcement card is selected!" << endl;
			card->play(card, new int(0), this->player, nullptr, nullptr, nullptr, player->getGame());
		}
		else if (type == Card::ctype::blockade) {
			cout << "Blockade card is selected!" << endl;

			card->play(card, new int(0), this->player, nullptr, nullptr, weakestTerritory(toDefend()), player->getGame());

			cout << "Blockade order will be issued!" << endl;
		}
		else if (type == Card::ctype::airlift) {
			cout << "Airlift card is selected!" << endl;
			if(toDefend().size() >= 2) {

				if(weakestTerritory(toDefend()) != strongestTerritory(toDefend()))
					card->play(card, weakestTerritory(toDefend())->getArmyAmount(), this->player, nullptr, weakestTerritory(toDefend()), strongestTerritory(toDefend()), player->getGame());
				else
					card->play(card, toDefend()[1]->getArmyAmount(), this->player, nullptr, toDefend()[1], toDefend()[0], player->getGame());

				cout << "Airlift order will be issued!" << endl;
			}
			else
				cout << "Airlift order is not issued since Player " << *player->getPlayerID() << " owns 1 territory." << endl;
		}
		else if (type == Card::ctype::diplomacy) {
			cout << "Diplomacy card selected:" << endl;

			cout << "Diplomacy order will not be issued since Aggressive Player wants to always advance to enemy territories!" << endl;
		}
	}

	// Player has no cards in their hand
	else {
		cout << "Player " << *player->getPlayerID() << " has no cards in their hand, skipping the card playing phase!" << endl;
	}

	cout << "\n#--- Card Playing Phase OVER ---#\n" << endl;  
}

// returns enemy territories from weakest to strongest
vector<Territory*> AggressivePlayerStrategy::toAttack() {
   // Vector containing all the territories that can be attacked
	vector<Territory*> attackableTerritories;

    Map* m = player->getGame()->getMap();

	// Double for-loop that adds the attackable territory to the vector
	for (Territory* owned : player->getOwnedTerritories()) {

        Node* node = m->getVertex(owned);

		for (Node* adjacent : node->getEdges()) 
            if (!player->isOwnedTerritory(*adjacent->getTerritoryPtr()->getTerritoryName())) 
				attackableTerritories.push_back(adjacent->getTerritoryPtr());
	
	}	

    // Remove duplicates		
	for (Territory* t : attackableTerritories) 
		attackableTerritories.erase(unique(attackableTerritories.begin(), attackableTerritories.end()), attackableTerritories.end());

    return attackableTerritories;
}

// returns own territories from weakest to strongest
vector<Territory*> AggressivePlayerStrategy::toDefend() {
	return player->getOwnedTerritories();
}

Territory* AggressivePlayerStrategy::strongestTerritory(vector<Territory*> territories){
	
	if(territories.size() == 0)
		return nullptr;
	
	Territory* strongest = territories[0];
	for(Territory* t : territories){
		if(*t->getArmyAmount() > *strongest->getArmyAmount())
			strongest = t;
	}

	return strongest;
}

Territory* AggressivePlayerStrategy::weakestTerritory(vector<Territory*> territories){
	
	if(territories.size() == 0)
		return nullptr;
	
	Territory* weakest = territories[0];
	for(Territory* t : territories){
		if(*t->getArmyAmount() < *weakest->getArmyAmount())
			weakest = t;
	}

	return weakest;
}

// strategy name getter
string AggressivePlayerStrategy::getStrategyName() const {
    return "Aggressive";
}


// Default constructor
BenevolentPlayerStrategy::BenevolentPlayerStrategy() : PlayerStrategy() {
}

// Destructor
BenevolentPlayerStrategy::~BenevolentPlayerStrategy() = default;


// Assignment operator
BenevolentPlayerStrategy& BenevolentPlayerStrategy::operator=(const BenevolentPlayerStrategy& bps) {
    PlayerStrategy::operator = (bps);
    return *this;
}

// Stream insertion operator
ostream& operator<<(ostream& out, const BenevolentPlayerStrategy& bps) {
    out << bps.getStrategyName();
    return out;
}

// Benevolent player issueOrder method
void BenevolentPlayerStrategy::issueOrder() {
	Deck* deck = player->getGame()->getDeck();

	// Introduction message for each player
	cout << "/*------------------------------------------------------------------------------------------------*/" << endl; 
	cout << "\nIssuing orders for Player " << *player->getPlayerID() << " that is Benevolent:" << endl;
	cout << endl;

	cout << "#--- Deploying Phase ---#\n" << endl;

	player->getOrders()->addOrder(new Deploy(this->player, weakestTerritory(toDefend()), player->getReinforcementPool()));

	cout << "- Deploy order of " << *player->getReinforcementPool() << " armies will be issued by Player " << *player->getPlayerID() <<"!" << endl;

	cout << "\n#--- Deploying Phase OVER ---#\n" << endl;
	

	// All deploy orders have been issued at this point!

	// Now issuing advance orders
	cout << "#--- Advancing Phase ---#\n" << endl;

	cout << "- Benevolent Player will never advance to enemy territories!" << endl;

	cout << "\n#--- Advancing Phase OVER ---#\n" << endl;
	

	// All advance orders have been issued at this point!

	// Clearing the cannotAttack vector for Negotiate order
	player->getUnattackablePlayers().clear();

	// Now playing a card, Player plays one card per turn
	cout << "#--- Card Playing Phase ---#" << endl << endl;

	// Check if player has any cards in hand
	if (player->getHand()->size() > 0) {
		
		// Convert input index to a card pointer
		Card* card = player->getHand()->getCard(0);

		Card::ctype type = card->getCardType();

		// Cases for each type of card to be played and its required input
		if (type == Card::ctype::bomb) {
			cout << "Bomb card is selected!" << endl;

			cout << "Bomb order will not be issued because it will harm someone since it is a Benevolent Player !" << endl;
		}
		else if (type == Card::ctype::reinforcement) {
			cout << "Reinforcement card is selected! " << endl;
			card->play(card, new int(0), this->player, nullptr, nullptr, nullptr, player->getGame());
		}
		else if (type == Card::ctype::blockade) {
			cout << "Blockade card is selected!" << endl;

			card->play(card, new int(0), this->player, nullptr, nullptr, weakestTerritory(toDefend()), player->getGame());

			cout << "Blockade order will be issued!" << endl;
		}
		else if (type == Card::ctype::airlift) {
			cout << "Airlift card is selected!" << endl;
			if(toDefend().size() >= 2) {

				if(weakestTerritory(toDefend()) != strongestTerritory(toDefend()))
					card->play(card, strongestTerritory(toDefend())->getArmyAmount(), this->player, nullptr, strongestTerritory(toDefend()), weakestTerritory(toDefend()), player->getGame());
				else
					card->play(card, toDefend()[1]->getArmyAmount(), this->player, nullptr, toDefend()[1], toDefend()[0], player->getGame());

				cout << "Airlift order will be issued!" << endl;
			}
			else
				cout << "Airlift order is not issued since Player " << *player->getPlayerID() << " owns 1 territory." << endl;
		}
		else if (type == Card::ctype::diplomacy) {
			cout << "Diplomacy card selected:" << endl;
			card->play(card, 0, this->player, strongestPlayer(), nullptr, nullptr, player->getGame());
			cout << "Diplomacy order will  be issued!" << endl;
		}
	}

	// Player has no cards in their hand
	else {
		cout << "Player " << *player->getPlayerID() << " has no cards in their hand, skipping the card playing phase!" << endl;
	}

	cout << "\n#--- Card Playing Phase OVER ---#\n" << endl;
	
}

// Benevolent player toAttack method
vector<Territory*> BenevolentPlayerStrategy::toAttack() {
    // Vector containing all the territories that can be attacked
	vector<Territory*> attackableTerritories;

    Map* m = player->getGame()->getMap();

	// Double for-loop that adds the attackable territory to the vector
	for (Territory* owned : player->getOwnedTerritories()) {

        Node* node = m->getVertex(owned);

		for (Node* adjacent : node->getEdges()) 
            if (!player->isOwnedTerritory(*adjacent->getTerritoryPtr()->getTerritoryName())) 
				attackableTerritories.push_back(adjacent->getTerritoryPtr());
	
	}	

    // Remove duplicates		
	for (Territory* t : attackableTerritories) 
		attackableTerritories.erase(unique(attackableTerritories.begin(), attackableTerritories.end()), attackableTerritories.end());

    return attackableTerritories;
}

// Benevolent player toDefend method
vector<Territory*> BenevolentPlayerStrategy::toDefend() {
	return player->getOwnedTerritories();
}

// getStrategyName method
string BenevolentPlayerStrategy::getStrategyName() const {
    return "Benevolent";
}

Territory* BenevolentPlayerStrategy::strongestTerritory(vector<Territory*> territories){
	
	if(territories.size() == 0)
		return nullptr;
	
	Territory* strongest = territories[0];
	for(Territory* t : territories){
		if(*t->getArmyAmount() > *strongest->getArmyAmount())
			strongest = t;
	}

	return strongest;
}

Territory* BenevolentPlayerStrategy::weakestTerritory(vector<Territory*> territories){
	
	if(territories.size() == 0)
		return nullptr;
	
	Territory* weakest = territories[0];
	for(Territory* t : territories){
		if(*t->getArmyAmount() < *weakest->getArmyAmount())
			weakest = t;
	}

	return weakest;
}

Player* BenevolentPlayerStrategy::strongestPlayer(){

	vector <Territory*> enemyterritories;

	for(Node* node : player->getGame()->getMap()->getVertices())
		if(*node->getTerritoryPtr()->getPlayerID() != *player->getPlayerID())
			enemyterritories.push_back(node->getTerritoryPtr());

	int* strongestID = strongestTerritory(enemyterritories)->getPlayerID();

	Player* strongest = nullptr;

	for(Player* p : player->getGame()->getPlayers())
		if(p->getPlayerID() == strongestID){
			strongest = p;
			break;
		}

	return strongest;
}