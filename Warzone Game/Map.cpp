#include "Map.h"
#include <stack>
#include <vector>
#include <string>
#include <fstream>



//-------------------------- Continent class  --------------------------//

//-------------- Constructor --------------//
Continent::Continent() {
    this->continentName = new string("No name");
    this->bonus = new int(0);
}

Continent::Continent(string* name) {
    this->continentName = name;
    this->bonus = new int(0);
}

Continent::Continent(string* name, int* bonus) {
    this->continentName = name;
    this->bonus = bonus;
}

Continent::Continent(const Continent& continent) {
    this->continentName = continent.continentName;
    this->bonus = continent.bonus;
}

//-------------- Destructor --------------//
Continent::~Continent() {
    delete continentName;
    delete bonus;
}


//-------------- Overloads --------------//
Continent& Continent::operator=(const Continent& continent)
{
    this->continentName = continent.continentName;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Continent& continent) {
    return os << "The continent's name is " << *continent.continentName <<  " with bonus of "  << *continent.bonus << " army units." ;
}

 //-------------- Getters --------------//
string* Continent::getContinentName(){
    return this->continentName;
}

int* Continent::getBonus(){
    return this->bonus;
}

//-------------- Setters --------------//
void Continent::setContinentName(string* name){
    this->continentName = name;
}

void Continent::setBonus(int* bonus){
    this->bonus = bonus;
}

//-------------------------- Territory class  --------------------------//

//-------------- Constructor --------------//
Territory::Territory() {
    this->territoryName = new string("default territory");
    this->continent = new Continent();
    this->playerID = new int(0);
    this->armyAmount = new int(0);
}

Territory::Territory(const Territory& newTerritory) {
    this->territoryName = newTerritory.territoryName;
    this->continent = newTerritory.continent;
    this->playerID = newTerritory.playerID;
    this->armyAmount = newTerritory.armyAmount;
}


Territory::Territory(string* tname, Continent* continent, int* playerID, int* army) {
    this->territoryName = tname;
    this->continent = continent;
    this->playerID = playerID;
    this->armyAmount = army;
}

Territory::Territory(string* tname, Continent* continent) {
    this->territoryName = tname;
    this->continent = continent;
    this->playerID = new int(0);
    this->armyAmount = new int(0);
}

//-------------- Destructor --------------//
Territory::~Territory() {
    delete territoryName;
    delete [] continent;
    delete playerID;
    delete armyAmount;
}

//-------------- Overloads --------------//
Territory& Territory::operator=(const Territory& newTerritory)
{
    this->territoryName = newTerritory.territoryName;
    this->continent = newTerritory.continent;
    this->playerID = newTerritory.playerID;
    this->armyAmount = newTerritory.armyAmount;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Territory& t) {
    return os << "The territory " << *t.territoryName << " on continent " << *t.continent->getContinentName() << " is owned by playerID "<< *t.playerID << " and has an army amount of " << *t.armyAmount << "." ;
}

 //-------------- Getters --------------//
string* Territory::getTerritoryName(){
    return this->territoryName;
}

Continent* Territory::getContinent(){
    return this->continent;
}

int* Territory::getPlayerID(){
    return this->playerID;
}

int* Territory::getArmyAmount(){
    return this->armyAmount;
}

//-------------- Setters --------------//
void Territory::setTerritoryName(string* name){
    this->territoryName = name;
}

void Territory::setContinent(Continent* continent){
    this->continent = continent;
}

void Territory::setPlayerID(int* id){
    this->playerID = id;
}

void Territory::setArmyAmount(int amount){
    this->armyAmount = new int(amount);
}


//-------------------------- Node class  --------------------------//

//-------------- Constructor --------------//
Node::Node() {}

Node::Node(Territory* newVertex)
{
    this->vertex = newVertex;
}

Node::Node(const Node &original)
{ //Copy constructor
    this->vertex = original.vertex;
    this->edges = original.edges;
}

//-------------- Destructor --------------//
Node::~Node()
{
    delete vertex;
    vertex = nullptr;
}

//-------------- Overloads --------------//
//overload assignment operator
Node &Node::operator=(const Node &rhs)
{
    if (this != &rhs)
    {
        *vertex = *rhs.vertex;
        edges= rhs.edges;
    }

    return *this;
}


//-------------- Getters --------------//
Territory Node::getTerritory()
{
    return *vertex;
}

Territory* Node::getTerritoryPtr()
{
    return vertex;
}

vector <Node*> Node::getEdges()
{
    return edges;
}


//-------------- Setters --------------//
void Node::setTerritory(Territory *newVertex)
{
    this->vertex = newVertex;
}


//-------------- Other functions --------------//
void Node::addEdge(Node* edge)
{
    edges.push_back(edge);
}


//-------------------------- Map class  --------------------------//

//-------------- Constructor --------------//
Map::Map() {
    this->valid = new bool(false);
}

Map::Map(const Map &original)
{ 
    //create a deep copy of V
    for (auto i = 0; i < original.getVertices().size(); i++)
    {
        vertices.push_back(new Node(*original.vertices[i]));
    }
    this->valid = original.valid;
}

//-------------- Destructor --------------//
Map::~Map()
{
    vertices.clear();
    continentList.clear();
    delete valid;

}

//-------------- Getters --------------//
vector<Node*> Map::getVertices() const
{
    return this->vertices;
}
vector<Continent*> Map::getContinentList() const
{
    return this->continentList;
}
Node* Map::getVertex(Territory* t){

    Node* result = new Node();

    for (Node* node : vertices){
        if(node->getTerritoryPtr() == t)
            result = node;
    }

    return result;
}

bool Map::isValid(){
    return *this->valid;
}
//-------------- Inserting and connecting territories --------------//

void Map::insertTerritory(Territory* newTerritory)
{
    vertices.push_back(new Node(newTerritory));
}

void Map::createAndInsertContinent(string* name, int* bonus)
{
    Continent* c = new Continent(name,bonus);
    continentList.push_back(c);
}

void Map::connectTwoNodes(Node* A, Node* B)
{
    A->addEdge(B);
}

bool Map::areConnected(Node* A, Node* B)
{
    bool result = false;

    for(Node* node: A->getEdges()){
        if (*node->getTerritoryPtr()->getTerritoryName() == *B->getTerritoryPtr()->getTerritoryName()){
            result = true;
            break;
        }
    }

   return result; 
}

//-------------- Others --------------//
void Map::validate()
{
    bool isNotValid= false;

    //check that the map is a connected graph
    try
    {
        if (!isGraphConnected(getVertices(), getVertices()[0]))
            throw std::logic_error("The map is not a connected graph.");
    }
    catch (const std::exception &e)
    {
        isNotValid = true;
        std::cerr << e.what() << endl;
    }

    //check if all continents are connected subgraphs
    try
    {
        for (Continent* continent : getContinentList())
        {
            vector<Node *> territoriesInContinent;
            //getting all territories with the same continent in a single vector
            for (Node *node : getVertices())
            {
                if (node->getTerritoryPtr()->getContinent() == continent)
                    territoriesInContinent.push_back(node);
            }

            if (!isGraphConnected(territoriesInContinent, territoriesInContinent[0]))
                throw std::logic_error(*continent->getContinentName() + " continent is not a connected subgraph.");

            territoriesInContinent.clear(); //earse the vector contentss
        }
    }
    catch (const std::exception &e)
    {
        isNotValid = true;
        std::cerr << e.what() << endl;
    }

    //check if each country belongs to one and only one continent
    try
    {
        for (Node *node : getVertices())
        {
            int countNumOfMatches = 0; //count how many continents associate with each territory
            for (Continent *Continent : getContinentList())
            {
                if (node->getTerritoryPtr()->getContinent() == Continent)
                    countNumOfMatches++;
            }

            if (countNumOfMatches != 1)
                throw std::logic_error(*node->getTerritoryPtr()->getTerritoryName() + " country belongs to more than one continent.");
        }
    }
    catch (const std::exception &e)
    {
        isNotValid = true;
        std::cerr << e.what() << endl;
    }

    //will terminate the program if an error was thrown
    if (isNotValid)
        exit(EXIT_FAILURE);

    cout << "The Map is valid" << endl;
    this->valid = new bool(true);
}



stack<Node *> adjList;
vector<Node *> visited;

bool Map::isGraphConnected(vector<Node *> nodeGraph, Node *startNode)
{
    bool isConnected = false;
    //remove visited element
    for (int i = 0; i < nodeGraph.size(); i++)
    {
        if (startNode == nodeGraph[i])
            nodeGraph.erase(nodeGraph.begin() + i);
    }

    visited.push_back(startNode);
    for (Node* edge : visited.end()[-1]->getEdges())
    {
        for (Node *node : nodeGraph)
        {
            if (node == edge)
            {
                adjList.push(node);
            }
        }
    }
    
    if (!adjList.empty())
    { //base condition is for adjList to be empty
        Node *temp = adjList.top();
        adjList.pop();
        return isGraphConnected(nodeGraph, temp);
    }
    else
    {
        visited.clear(); //to be ready for the next use
        if (nodeGraph.size() == 0)
            isConnected = true; //connected Map
    }
    return isConnected;
}

Territory* Map::getTerritoryFromMap(string name){

    for(Node* n : vertices){
        if(*n->getTerritoryPtr()->getTerritoryName() == name)
            return n->getTerritoryPtr();
    }

    return nullptr;
}


int Map::calculateContinentBonus(Player* p)
{
	int continentBonus = 0;
	for (Continent* c : continentList) {
		bool all = true;
		for (Territory* t : getTerritoriesByContinent(c)) {
			if (!p->isOwnedTerritory(*t->getTerritoryName())) {
				all = false;
			}
		}
		if (all) {
			continentBonus = continentBonus + *c->getBonus();
		}
	}
	return continentBonus;
}

vector<Territory*> Map::getTerritoriesByContinent(Continent* c)
{
	vector<Territory*> result;
		
	for (Node* node : vertices) {
		if (node->getTerritoryPtr()->getContinent() == c) {
			result.push_back(node->getTerritoryPtr());
		}
	}

	return result;
}

//-------------------------- MapLoader class  --------------------------//

//-------------- Constructor --------------//
//MapLoader::MapLoader() {}

//-------------- Others --------------//
vector<string> MapLoader::seperateTerritoryLine(string line)
{
    vector<string> result;
    string word = "";

    for (int i = 0; i < line.length(); ++i)
    {
        if (i == line.length() - 1 && isalpha(line.at(i)))
            word = word + line.at(i);

        if (i == line.length() - 1)
            result.push_back(word);
            
        if (line.at(i) == ','){
            result.push_back(word);
            word = "";
        } 
        else
            word = word + line.at(i);
    }
    return result;
}

vector<string> MapLoader::seperateContinentLine(string line)
{
    vector<string> result;
    string word = "";

    for (int i = 0; i < line.length(); ++i)
    {
        if (i == line.length() - 1 && isdigit(line.at(i)))
            word = word + line.at(i);
        
        if (i == line.length() - 1)
            result.push_back(word);
        
        if (line.at(i) == '='){
            result.push_back(word);
            word = "";
        }
        else
            word = word + line.at(i);
        
    }
    return result;
}

int MapLoader::findContinentIdx(vector<Continent*> continents, string name)
{
    int idx = -1;

    for(int i=0; i < continents.size(); i++){
        if(*continents[i]->getContinentName() == name)
            idx = i;
    }

    return idx;
}

int MapLoader::findTerritoryIdx(vector<Node*> territories, string name)
{
    int idx = -1;
    
    for(int i=0; i < territories.size(); i++){
        if(*territories[i]->getTerritoryPtr()->getTerritoryName() == name)
            idx = i;
    }

    return idx;
}

vector<string> MapLoader::subvector(vector<string> list, int start, int end)
{
    auto first = list.cbegin() + start;
    auto last = list.cbegin() + end + 1;

    vector<string> v(first, last);
    return v;
}


Map* MapLoader::parseMapConquest(string map)
{
    cout << "-----"
         << "the map is " << map << "----" << endl;
    string line;
    vector<vector<string>> edges;
    Map* myGraph = new Map();
    vector<Continent *> continentts;
    vector<Territory *> teritories;
    // Read from the text file
    ifstream MyReadFile("./conquestMaps/" + map);

    while (getline(MyReadFile, line))
    {
        //skip line if it is empty
        if (line.length() == 0 || line.length() == 1)
        {
            continue;
        }
        //I first store all the continents in a vector list.
        if (line.find("[Continents]") != std::string::npos)
        {
            string continentLine;
            while (getline(MyReadFile, continentLine))
            {   
                //Once arrived in Territories, break out of while loop while changing value of contentline to line
                if (!(continentLine.find("[Territories]") == -1))
                {
                    line = continentLine;
                    break;
                }
                if (continentLine.length() == 0)
                    continue;
                
                vector<string> continents = seperateContinentLine(continentLine);
                string continentName = continents[0];
                string continentBonus = continents[1];
                
                if (continentName.length() > 0)
                {
                    myGraph->createAndInsertContinent(new string(continentName),new int(stoi(continentBonus)));
                }

                
            }
        }
        
        //        then I store all the territories in a vector list.
        if (line.find("[Territories]") != std::string::npos)
        {
            string territoryLine;
            while (getline(MyReadFile, territoryLine))
            {
                if (territoryLine.length() == 0)
                {
                    continue;
                }
                
                vector<string> territories = seperateTerritoryLine(territoryLine);
                
                string territoryName = territories[0];
                string continentName = territories[3];
                
                edges.push_back(subvector(territories, 4, territories.size() - 1));
                
                int continentIndex = findContinentIdx(myGraph->getContinentList(), continentName);

                
                Territory* newTerritory = new Territory(new string(territoryName), myGraph->getContinentList()[continentIndex]);
                myGraph->insertTerritory(newTerritory);

                
            }
        }
    }

    //    after all the territories and continents are stored, I loop through the map and connect all territories together.
    
     for(int i = 0; i < edges.size(); i++) 
    { 
        for (int j = 0; j < edges[i].size(); j++) 
        {
            int edgeIdx = findTerritoryIdx(myGraph->getVertices(), edges[i][j]);

            if (edgeIdx == -1)
            {
                cout << "file is invalid";
                exit(EXIT_FAILURE); //stops the application
            }
            
            myGraph->connectTwoNodes(myGraph->getVertices()[i], myGraph->getVertices()[edgeIdx]);  
        }
    }

    
    
    //print returned map
    for (Node *territory : myGraph->getVertices())
    {
        cout << *territory->getTerritoryPtr()->getTerritoryName() + " belongs to " + *territory->getTerritoryPtr()->getContinent()->getContinentName() + " has the following edges: " << endl;
        for (Node* edge : territory->getEdges())
        {
            cout << "- " << *edge->getTerritoryPtr()->getTerritoryName() << "\n";
        }
        cout << endl;
    }

    
    // Close the file
    MyReadFile.close();
    cout << "++++end+++" << endl;

    return myGraph;
}
