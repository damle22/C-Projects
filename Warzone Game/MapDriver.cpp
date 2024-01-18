#include "Map.h"
#include <iomanip>
#include <vector>
#include <string>

#include <iostream>



void testLoadMaps(){
    MapLoader* ml = new MapLoader();

    Map* map = ml->parseMapConquest("Africa.map");


}

int main() {
    
    testLoadMaps();
    return 0;
}
