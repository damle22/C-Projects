#include <iostream>
#include "Orders.cpp"
#include "Player.cpp"
#include "Card.cpp"
#include "map.cpp"
using namespace std;

void testPlayers(){

    

    Hand* hand = new Hand();

    OrdersList* orderList = new OrdersList();

    Order* order1 = new Deploy();
    Order* order2 = new Advance();
    Order* order3 = new Bomb();
 

    // Add orders to the list
    orderList->addOrder(order1);
    orderList->addOrder(order2);
    orderList->addOrder(order3);
   
    
    // Create Continent
    Continent* canada =  new Continent(new string("Canada"));
    
    //Create Territories
    Territory* qc = new Territory(new string("Quebec"),canada,new int(1),new int(50));
    Territory* ont = new Territory(new string("Ontario"),canada,new int(1),new int(20));
    Territory* mba = new Territory(new string("Manitoba"),canada,new int(1),new int(30));

    vector<Territory *> tarray;
    tarray.push_back(qc );
    tarray.push_back(ont);
    tarray.push_back(mba);
   
    vector<Player*> noAttack1;

    //Creating the player
    Player* p1 = new Player(new int(1),new int(50),noAttack1,tarray,hand,orderList);

    cout << *p1 << endl;

    //p1->issueOrder("negotiate");

    
    //p1->printOrders();

}


int main() {

    testPlayers();
    return 0;
    
    }


