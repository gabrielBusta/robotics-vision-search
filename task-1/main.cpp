#include <iostream>
#include "AStar/AStar.hpp"

int main()
{
    AStar::Generator generator;
    generator.setWorldSize({10, 10});
    generator.setHeuristic(AStar::Heuristic::euclidean);
    generator.setDiagonalMovement(true);
    
    generator.addCollision({4, 2});
    generator.addCollision({5, 2});
    generator.addCollision({5, 3});
    generator.addCollision({5, 4});
    generator.addCollision({4, 4});
    generator.addCollision({4, 5});
    generator.addCollision({4, 6});
    generator.addCollision({3, 6});
    generator.addCollision({3, 7});
    generator.addCollision({2, 7});
    
    std::cout << "Generate path ... \n";
    auto path = generator.findPath({0, 0}, {9, 9});
    
    for (auto& coordinate : path) {
        std::cout << coordinate.x << " " << coordinate.y << "\n";
    }
}
