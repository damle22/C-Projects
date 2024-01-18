// OrdersDriver.cpp
#include "Orders.h"
#include <iostream>
#include <algorithm>
using namespace std; 

// free function testOrdersList() which will create all 6 orders and add them on the list.
// Tests also the remove and move method that manipulates the list 
void testOrdersLists() {

     Observer* _observer = new LogObserver();
    
    //Create Map
    Map* map = new Map();
    
    //Create Continent
    map->createAndInsertContinent(new string("Canada"), new int(3));
    
    //Save Continent as canada
    Continent* canada =  map->getContinentList()[0];
    
    //Create Territories
    Territory* qc = new Territory(new string("Quebec"),canada,new int(1),new int(60));
    Territory* ont = new Territory(new string("Ontario"),canada,new int(1),new int(20));
    Territory* mba = new Territory(new string("Manitoba"),canada,new int(2),new int(20));
    Territory* alb = new Territory(new string("Alberta"),canada,new int(2),new int(60));

    //Insert Territories in map
    map->insertTerritory(qc);
    map->insertTerritory(ont);
    map->insertTerritory(mba);
    map->insertTerritory(alb);

    //Connect the Nodes
    map->connectTwoNodes(map->getVertex(qc),map->getVertex(ont));
    map->connectTwoNodes(map->getVertex(qc),map->getVertex(mba));

    map->connectTwoNodes(map->getVertex(ont),map->getVertex(qc));

    map->connectTwoNodes(map->getVertex(mba),map->getVertex(qc));
    map->connectTwoNodes(map->getVertex(mba),map->getVertex(alb));

    map->connectTwoNodes(map->getVertex(alb),map->getVertex(mba));

    //Array of Territories for Player 1
    vector<Territory*> tarray1;
    tarray1.push_back(qc);
    tarray1.push_back(ont);

    //Array of Territories for Player 2
    vector<Territory*> tarray2;
    tarray2.push_back(mba);
    tarray2.push_back(alb);

    //Array of Players to not Attack
    vector<Player*> noAttack1;
    vector<Player*> noAttack2;

    //Two hands objects
    Hand* hand1 = new Hand();
    Hand* hand2 = new Hand();


    GameEngine* game = new GameEngine(_observer);
    game->map = map;

    //Two OrderLists
    OrdersList* orderList1 = new OrdersList();
    OrdersList* orderList2 = new OrdersList();

    //Creating Players
    Player* p1 = new Player(new int(1),new int(50),noAttack1,tarray1,hand1,orderList1,game, new HumanPlayerStrategy());
    Player* p2 = new Player(new int(2),new int(40),noAttack2,tarray2,hand2,orderList2,game, new HumanPlayerStrategy());
    
    game->addPlayer(p1); game->addPlayer(p2);

    //Execute the orders
    
    
    Order* order1 = new Advance(p1,p2,qc,mba,new int(40),game);
    order1->execute(); 
    cout << *order1 << endl;
    /*
    Order* order11 = new Advance(p1,p1,qc,ont,new int(5),game);
    order11->execute();  
    cout << *order11 << endl;
    
    Order* order2 = new Deploy(p1,qc,new int(20));
    order2->execute();
    cout << *order2 << endl;
    
    Order* order3 = new Bomb(p1,alb,map);
    order3->execute();
    cout << *order3 << endl; 
*/



/*
    Order* order4 = new Blockade(p1,ont);
    order4->execute();
    cout << *order4 << endl; 

   
    Order* order5 = new Airlift(p1,qc,mba,new int(10));
    order5->execute();
    cout << *order5 << endl; 
  
    Order* order6 = new Negotiate(p1,p2);
    order6->execute();
    cout << *order6 << endl; 

    Order* order7 = new Advance(p1,p2,qc,alb,new int(5),game);
    order7->execute(); 
    cout << *order7 << endl; 
    

    orderList1->addOrder(order1);
    orderList1->addOrder(order2);
    orderList1->addOrder(order3);
    */

    // Clean up
    delete map;
    delete canada;
    delete qc;
    delete ont;
    delete mba;
    delete alb;
    delete hand1;
    delete hand2;
    delete orderList1;
    delete orderList2;
    
    delete order1;
	/*
    delete order11;
    delete order2;
    delete order3;
    delete order4;
    delete order5;
    delete order6;
    delete order7;
    */
    

 
}

// main function to test the free function
int main() {

    testOrdersLists();
    return 0;
}
