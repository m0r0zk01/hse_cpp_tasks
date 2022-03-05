#include "robot.h"

#include <algorithm>

bool PathExists(Topology& topology, const Point& finish) {
    Point start = topology.GetPosition();
    std::vector<Point> points_stack{start};
    PointSet points_used{start};
    PointEqual eq;
    while (!points_stack.empty()) {
        topology.Move(points_stack.back());
        points_stack.pop_back();
        if (eq(topology.GetPosition(), finish)) {
            topology.Move(start);
            return true;
        }
        std::vector<Point> to_be_visited;
        for (const auto& to : topology.Lookup()) {
            if (points_used.find(to) == points_used.end()) {
                points_used.insert(to);
                to_be_visited.push_back(to);
            }
        }
        std::sort(to_be_visited.begin(), to_be_visited.end(), [&topology, &finish](const Point& a, const Point& b) {
            return topology.MeasureDistance(a, finish) > topology.MeasureDistance(b, finish);
        });
        points_stack.insert(points_stack.end(), to_be_visited.begin(), to_be_visited.end());
    }
    topology.Move(start);
    return false;
}