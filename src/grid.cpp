// grid.cpp

#include <functional>
#include <cassert>
#include "simulator.h"

namespace BS {

// Allocates space for the 2D grid
void Grid::init(uint16_t sizeX, uint16_t sizeY)
{
    auto col = Column(sizeY);
    data = std::vector<Column>(sizeX, col);
}


// Finds a random unoccupied location in the grid
Coord Grid::findEmptyLocation() const {
    Coord loc;
    bool found = false;

    while (!found) {
        loc.x = randomUint(0, p.sizeX - 1);
        loc.y = randomUint(0, p.sizeY - 1);
        if (grid.isEmptyAt(loc)) {
            break;
        }
    }
    return loc;
}

void Grid::initializeFood(unsigned percentageFoodLocations, unsigned simStep)
{
   uint32_t num_cells = data.size() * grid.data[0].size();
   uint32_t num_food_cells = (uint32_t) ((percentageFoodLocations * num_cells) / 100);

   if (simStep == 0) {
       foodLocations.clear();
   }

   for (unsigned i = 0; i < num_food_cells; i++) {
       Coord loc = findEmptyLocation();
       set(loc, FOOD);
       foodLocations.push_back(loc);
   }
}

void Grid::removeFoodLocation(Coord loc)
{
    std::vector<Coord>::iterator position = std::find(foodLocations.begin(),
            foodLocations.end(), loc);
    if (position != foodLocations.end()) {
        foodLocations.erase(position);
    }
}

// This is a utility function used when inspecting a local neighborhood around
// some location. This function feeds each valid (in-bounds) location in the specified
// neighborhood to the specified function. Locations include self (center of the neighborhood).
void visitNeighborhood(Coord loc, float radius, std::function<void(Coord)> f)
{
    for (int dx = -std::min<int>(radius, loc.x); dx <= std::min<int>(radius, (p.sizeX - loc.x) - 1); ++dx) {
        int16_t x = loc.x + dx;
        assert(x >= 0 && x < p.sizeX);
        int extentY = (int)sqrt(radius * radius - dx * dx);
        for (int dy = -std::min<int>(extentY, loc.y); dy <= std::min<int>(extentY, (p.sizeY - loc.y) - 1); ++dy) {
            int16_t y = loc.y + dy;
            assert(y >= 0 && y < p.sizeY);
            f( Coord { x, y} );
        }
    }
}

} // end namespace BS
