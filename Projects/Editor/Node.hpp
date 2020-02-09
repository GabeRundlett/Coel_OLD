#pragma once

#include <Math.hpp>
#include <vector>

#include <algorithm>
#include <iostream>

struct Node {
    Math::Vec2 pos;
};

static auto getClosestNode(const Math::Vec2 &pos, std::vector<Node> &nodes) {
    float closestDist = 400;
    std::vector<Node>::iterator it = nodes.begin();
    int index = -1;

    for (unsigned int i = 0; it != nodes.end(); ++it, ++i) {
        float dist = (pos - it->pos).lengthSq();
        if (dist < closestDist) closestDist = dist, index = i;
    }

    return index;
}
