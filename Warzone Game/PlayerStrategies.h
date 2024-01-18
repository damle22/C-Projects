#pragma once
#include <string>
#include <vector>
#include "GameEngine.h"
#include "Card.h"
#include "Player.h"


using std::vector;
using std::string;
using std::ostream;

class Territory;
class Player;

class PlayerStrategy {

public:
    //-------------- Constructors --------------//
    PlayerStrategy(); 
    PlayerStrategy(const PlayerStrategy& ps); 

    //-------------- Overloads --------------//
    PlayerStrategy& operator=(const PlayerStrategy& ps); // Assignment operator
    friend ostream& operator << (ostream& out, const PlayerStrategy& ps); // stream insertion operator

    //-------------- Destructor --------------//
    ~PlayerStrategy();

    //-------------- Getter --------------//
    Player* getPlayer(); // Player getter
    //-------------- Setter --------------//
    void setPlayer(Player*); // Player setter

    //-------------- Other Methods --------------//
    virtual void issueOrder() = 0;
    virtual vector<Territory*> toAttack() = 0;
    virtual vector<Territory*> toDefend() = 0;
    virtual string getStrategyName() const = 0;
   

protected:
    Player* player;
};


class NeutralPlayerStrategy : public PlayerStrategy {

public:
    //-------------- Constructors --------------//
    NeutralPlayerStrategy(); 
    NeutralPlayerStrategy(const NeutralPlayerStrategy& neutralPlayerStrategy);

    //-------------- Destructor --------------// 
    ~NeutralPlayerStrategy();

    //-------------- Overloads --------------//
    NeutralPlayerStrategy& operator=(const NeutralPlayerStrategy& neutralPlayerStrategy); // Assignment operator
    friend ostream& operator << (ostream& out, const NeutralPlayerStrategy& nps); // stream insertion operator

    //-------------- Other Methods --------------//  
    void issueOrder() override;
    vector<Territory*> toAttack() override;
    vector<Territory*> toDefend() override;
    string getStrategyName() const override;
    
};


class CheaterPlayerStrategy : public PlayerStrategy {

public:
    //-------------- Constructors --------------//
    CheaterPlayerStrategy(); // default constructor
    CheaterPlayerStrategy(const CheaterPlayerStrategy& cps); // Copy constructor

    //-------------- Overloads --------------//
    CheaterPlayerStrategy& operator=(const CheaterPlayerStrategy& cps); // Assignment operator
    friend ostream& operator << (ostream& os, const CheaterPlayerStrategy& cps); // stream insertion operator
    
    //-------------- Destructor --------------// 
    ~CheaterPlayerStrategy();

    //-------------- Other Methods --------------//  
    void issueOrder() override;
    vector<Territory*> toAttack() override;
    vector<Territory*> toDefend() override;
    string getStrategyName() const override;
};


class HumanPlayerStrategy : public PlayerStrategy {

public:
    //-------------- Constructors --------------//
    HumanPlayerStrategy(); // default constructor
    HumanPlayerStrategy(const HumanPlayerStrategy& humanPlayerStrategy); // Copy constructor

    //-------------- Destructor --------------// 
    ~HumanPlayerStrategy();

    //-------------- Overloads --------------//
    HumanPlayerStrategy& operator=(const HumanPlayerStrategy& humanPlayerStrategy); // Assignment operator
    friend ostream& operator << (ostream& out, const HumanPlayerStrategy& nps); // stream insertion operator

    //-------------- Other Methods --------------// 
    void issueOrder() override;
    vector<Territory*> toAttack() override;
    vector<Territory*> toDefend() override;
    string getStrategyName() const override;
    
};


class AggressivePlayerStrategy : public PlayerStrategy {

public:
    //-------------- Constructors --------------//
    AggressivePlayerStrategy(); // default constructor
    AggressivePlayerStrategy(const AggressivePlayerStrategy& aps); // copy constructor

    //-------------- Overloads --------------//
    AggressivePlayerStrategy& operator=(const AggressivePlayerStrategy& aps); // assignment operator
    friend ostream& operator << (ostream& out, const AggressivePlayerStrategy& aps); // stream insertion operator
    
    //-------------- Destructor --------------//
    ~AggressivePlayerStrategy();
    
    //-------------- Other Methods --------------// 
    void issueOrder() override;
    vector<Territory*> toAttack() override;
    vector<Territory*> toDefend() override;
    string getStrategyName() const override;
    Territory* strongestTerritory(vector<Territory*>);
    Territory* weakestTerritory(vector<Territory*>);
   
    
};


class BenevolentPlayerStrategy : public PlayerStrategy {

public:
    //-------------- Constructors --------------//
    BenevolentPlayerStrategy(); // default constructor
   
    //-------------- Destructor --------------//
    ~BenevolentPlayerStrategy(); // destructor
    
    //-------------- Overloads --------------//
    BenevolentPlayerStrategy& operator=(const BenevolentPlayerStrategy& bps); // assignment operator
    friend ostream& operator<<(ostream& out, const BenevolentPlayerStrategy& bps); // stream insertion operator
    
    //-------------- Other Methods --------------// 
    void issueOrder() override;
    vector<Territory*> toAttack() override;
    vector<Territory*> toDefend() override;
    string getStrategyName() const override;
    Territory* strongestTerritory(vector<Territory*>);
    Territory* weakestTerritory(vector<Territory*>);
    Player* strongestPlayer();
    
};
