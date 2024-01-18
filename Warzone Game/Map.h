#pragma once
#include <iostream>
#include <vector>
using namespace std;

#include "Player.h"
#include "LoggingObserver.h"

class Player;

class Continent{
    private: 
        string* continentName;
        int* bonus;          //the name of a continent

    public:
    //-------------- Constructors --------------//
        Continent();                    
        Continent(string*); 
        Continent(string*, int*);            
        Continent(const Continent&);    

    //-------------- Destructor --------------//
    ~Continent();

    //-------------- Overloads --------------//
    Continent& operator=(const Continent&);              //assignment operator
    friend std::ostream& operator<<(std::ostream& os, const Continent& territory);    //stream insertion operator

    //-------------- Getters --------------//
        string* getContinentName();
        int* getBonus();
        
    //-------------- Setters --------------//
        void setContinentName(string*);
        void setBonus(int*);

};



class Territory {
private:
    string* territoryName;      //the name of a territory
    Continent* continent;       //the continent of a territory
    int* playerID;              //the ID of the player owning the territory
    int* armyAmount;            //the amount of army in a territory

public:
   
    //-------------- Constructors --------------//
    Territory();                                    //default constructor
    Territory(string*, Continent*);                 //constructor taking in two parameters
    Territory(string*, Continent*, int*, int*);  //constructor taking in four parameters
    Territory(const Territory&);                    //copy constructor

    //-------------- Destructor --------------//
    ~Territory();                                   

    //-------------- Overloads --------------//
    Territory& operator = (const Territory&);                                       //assignment operator
    friend std::ostream& operator<<(std::ostream& os, const Territory& territory);  //stream insertion operator


    //-------------- Getters --------------//
        string* getTerritoryName();
        Continent* getContinent();
        int* getPlayerID();
        int* getArmyAmount();
        
    //-------------- Setters --------------//
        void setTerritoryName(string*);
        void setContinent(Continent*);
        void setPlayerID(int*);
        void setArmyAmount(int);
};

class Node{
    Territory* vertex;      // territory object
    vector <Node*> edges;   // vector of edges
    
    //-------------- Constructors --------------//
    public:
        Node();                 //Default Constructor
        Node(Territory*);       //Create a node without connecting it
        Node(const Node&);      //Copy constructor

    //-------------- Destructor --------------//
        ~Node();

    //-------------- Overloads --------------//
        Node& operator=(const Node&);                       //copy constructor

    //-------------- Getters --------------//
        Territory getTerritory();      //get territory
        Territory* getTerritoryPtr();  //get territory pointer
        vector <Node*> getEdges();     //get edges

    //-------------- Setters --------------//
        void setTerritory(Territory*);

    //-------------- Other functions --------------//
        void addEdge(Node*);           //Add edge of a territory
};



class Map{
    private:
        vector<Node*> vertices;             //A vector of vertices (territories)
        vector<Continent*> continentList;   //vector of continents
        bool* valid;

    public:
        //-------------- Constructor --------------//
        Map();
        Map(const Map& original);           //Copy constructor

        //-------------- Destructor --------------//
        ~Map();
        
        //-------------- Overloads --------------//
        Map& operator=(const Map&); //assignment operator
        friend ostream& operator<<(ostream& output,const Map& obj); // stream insertion operator

        //-------------- Getters --------------//
        vector<Node*> getVertices() const;              
        vector<Continent*> getContinentList() const;
        Node* getVertex(Territory*);
        bool isValid();

        //-------------- Inserting and connecting territories/Continents --------------//

        void insertTerritory(Territory*);
        void createAndInsertContinent(string*, int*);
        void connectTwoNodes(Node* A, Node* B);
        bool areConnected(Node* A, Node* B);

        //-------------- Others --------------//
        void validate();
        bool isGraphConnected(vector<Node*>, Node*);
        Territory* getTerritoryFromMap(string);
        int calculateContinentBonus(Player*);
        vector<Territory*> getTerritoriesByContinent(Continent* );
};

class MapLoader
{

public:
    //-------------- Constructor --------------//
    //MapLoader();
    //-------------- Others --------------//
    Map* parseMapConquest(string);
    vector<string> seperateTerritoryLine(string);
    vector<string> seperateContinentLine(string);
    int findContinentIdx(vector<Continent *>, string);
    int findTerritoryIdx(vector<Node *>, string);
    vector<string> subvector(std::vector<string>, int, int);
   
    
    
};
