// Orders.h

#pragma once

#include "Map.h"
#include "LoggingObserver.h"
#include "GameEngine.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;


//Forward Declarations;
class Player;
class Territory;
class Map;
class GameEngine;

//----------------------------Order Class----------------------------//
class Order: public ILoggable, public Subject {

public:
    //-------------- Constructors --------------//
    Order();
    Order(const Order &order);
    
    //-------------- Overloads --------------//
    Order& operator =(const Order& order);
    friend std::ostream& operator<<(std::ostream& os, const Order& order);

    //-------------- Destructor --------------//
    ~Order();
    //-------------- Getters --------------//
    bool isExecuted() const;
    string getEffect() const;
    //-------------- Setters --------------//
    void setExecuted(bool);
    void setEffect(string);
    //-------------- Other Methods --------------//  
    string stringToLog();
    
    // execute method that first validates and then executes its actions
    virtual void execute() = 0;

    virtual string* getName();

private:
    bool* executed;
    string* effect;
    Observer* logger;
    
};

// There are 6 subclasses of the Order Class: deploy, advance, bomb, blockade, airlift, and negotiate
// all 6 subclasses will override the validate(), execute() and describe() methods

//----------------------------Deploy Class----------------------------//
class Deploy : public Order {
public:
    //-------------- Constructors --------------//
    Deploy();
    Deploy(Player*, Territory*, int* numToDeploy);
    Deploy(const Deploy &deploy);

    //-------------- Overloads --------------//
    Deploy& operator =(const Deploy& deploy);
    friend std::ostream& operator<<(std::ostream& os, const Deploy& deploy);

    //-------------- Destructor --------------//
    ~Deploy();

    //-------------- Getters --------------//
    Player* getPlayer();
    Territory* getTarget();
    int* getNumToDeploy();
    string* getName();

    //-------------- Other Methods --------------//
    bool validate();
    void execute() override;
    

private:
    Player* p;
    Territory* target;
    int* numToDeploy;
    string* name = new string("Deploy");

};

//Advance subclass of the Order Class
class Advance : public Order {
public:
    //-------------- Constructors --------------//
    Advance();
    Advance(Player*, Player*, Territory*, Territory*, int*,GameEngine*);
    Advance(const Advance &advance);
    
    //-------------- Overloads --------------//
    Advance& operator =(const Advance& advance);
    friend std::ostream& operator<<(std::ostream& os, const Advance& advance);

    //-------------- Destructor --------------//
    ~Advance();

    //-------------- Getters --------------//
    Player* getPlayerAttack();
    Player* getPlayerDefend();
    Territory* getSource();
    Territory* getTarget();
    int* getNumToAdvance();
    string* getName();

    //-------------- Other Methods --------------//
    bool validate();
    void execute() override;
    

private:
    Player* pAttack;
    Player* pDefend;
    Territory* source;
    Territory* target;
    Map* map;
    int* numToAdvance;
    GameEngine* game;
    string* name = new string("Advance");
};

//Bomb subclass of the Order Class
class Bomb : public Order {
public:
    //-------------- Constructors --------------//
    Bomb();
    Bomb(Player*, Territory*, Map*);
    Bomb(const Bomb &bomb);

    //-------------- Overloads --------------//
    Bomb& operator =(const Bomb& bomb);
    friend std::ostream& operator<<(std::ostream& os, const Bomb& bomb);
    //-------------- Destructor --------------//
    ~Bomb();

    //-------------- Getters --------------//
    Player* getPlayer();
    Territory* getTarget();
    Map* getMap();
    string* getName();

    //-------------- Other Methods --------------//
    bool validate();
    void execute() override;
    

private:
    Player* p;
    Territory* target;
    Map* map;
    string* name = new string("Bomb");
};

//Blockade subclass of the Order Class
class Blockade : public Order {
public:
    //-------------- Constructors --------------//
    Blockade();
    Blockade(Player*,Territory*);
    Blockade(const Blockade &blockade);

    //-------------- Overloads --------------//
    Blockade& operator =(const Blockade& blockade);
    friend std::ostream& operator<<(std::ostream& os, const Blockade& blockade);

    //-------------- Destructor --------------//
    ~Blockade();

    //-------------- Getters --------------//
    Player* getPlayer();
    Territory* getTarget();
    string* getName();

    //-------------- Other Methods --------------//
    bool validate();
    void execute() override;
    

private: 
    Player* p;
    Territory* target;
    string* name = new string("Blockade");
};

//Airlift subclass of the Order Class
class Airlift : public Order {
public:
    //-------------- Constructors --------------//
    Airlift();
    Airlift(Player*, Territory*, Territory*, int* numToDeploy);
    Airlift(const Airlift &airlift);

    //-------------- Overloads --------------//
    Airlift& operator =(const Airlift& airlift);
    friend std::ostream& operator<<(std::ostream& os, const Airlift& airlift);

    //-------------- Destructor --------------//
    ~Airlift();

    //-------------- Getters --------------//
    Player* getPlayer();
    Territory* getSource();
    Territory* getTarget();
    int* getNumToAirlift();
    string* getName();

    //-------------- Other Methods --------------// 
    bool validate();
    void execute() override;
   

private:
    Player* p;
    Territory* source;
    Territory* target;
    int* numToAirlift;
    string* name = new string("Airlift");
};

//Negotiate subclass of the Order Class
class Negotiate : public Order {
public:
    //-------------- Constructors --------------//
    Negotiate();
    Negotiate(Player*, Player*);
    Negotiate(const Negotiate &negotiate);
    friend std::ostream& operator<<(std::ostream& os, const Negotiate& negotiate);
    
    //-------------- Overloads --------------//
    Negotiate& operator =(const Negotiate& negotiate);
    //-------------- Destructor --------------//
    ~Negotiate();

    //-------------- Getters --------------//
    Player* getPlayer1();
    Player* getPlayer2();
    string* getName();

    //-------------- Other Methods --------------// 
    bool validate();
    void execute() override;
    

private:
    Player* p1;
    Player* p2;
    string* name = new string("Negotiate");
};

// OrderList Class which contains a list of Order objects.
class OrdersList : public ILoggable, public Subject {
    
private:
    std::vector<Order*> orders;
    Observer* logger;

public:
    //-------------- Constructors --------------//
    OrdersList();
    OrdersList(const OrdersList &ordersList);
    //Destructor 
    ~OrdersList();
    
    
    //-------------- Overloads --------------//
    OrdersList& operator =(const OrdersList& ordersList);
    friend std::ostream& operator<<(std::ostream&, const OrdersList&);

    //-------------- Getters --------------//
    vector<Order*> getOrdersList();

    //-------------- Other Methods --------------// 
    // addOrder method adds a pointer of the base class (order) to the vector orders
    void addOrder(Order* order);
    // remove method removes the order at position "index" from the  vector orders
    void removeOrder(Order* order);
    // move method moves the order from position "fromIndex" to position "toIndex" 
    void move(int fromIndex, int toIndex);
    // executeOrders method executes all the orders 
    void executeOrders();
    // describeOrders method outputs all the string description of all orders 
	void describeOrders();

    string stringToLog();

};

