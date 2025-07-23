// HUSEYIN EREN YILDIZ - 31047 - HOMEWORK2


#include "Quadtree.h"
#include <queue>

Quadtree::Quadtree(pointNode * Root)
    :root(Root){}


Quadtree::~Quadtree() {
    delete root;
}


void Quadtree::insert(const coordinate_points & points)  {
    // If the tree is empty, create the root node
    if (root == nullptr) {
        // Creating a new root node with given point
        root = new pointNode(points, nullptr, nullptr, nullptr, nullptr);
        return;
    }

    // Using a vector to simulate a queue for level-order traversal
    vector<pointNode*> queue;
    queue.push_back(root);    // Add the root node to the queue

    // Index to simulate queue's front
    int index = 0;

    // Navigate through the tree to locate the appropriate position for the new point
    while (index < queue.size()) {
        pointNode * current = queue[index];     // Get the current node from the queue
        index++;   // Move to the next "front" element

        // Prevent the insertion of duplicate coordinates
        if (points.x == current->points.x && points.y == current->points.y)
            return;   // Terminate the operation if the point is already present in the tree

        // Identify the appropriate quadrant and place the point accordingly
        if (points.x >= current->points.x) {  // Right side of the current node
            if (points.y >= current->points.y) {    // Northeast quadrant

                if (current->NE == nullptr) {  //  Insert the point if the NE child is empty
                    current->NE = new pointNode(points, nullptr, nullptr, nullptr, nullptr);
                    return;

                } else {  // Add the NE child to the queue to continue exploring
                    queue.push_back(current->NE);
                }
            } else {  // Southeast
                if (current->SE == nullptr) {  // Place the point in the SE child if it’s unoccupied
                    current->SE = new pointNode(points, nullptr, nullptr, nullptr, nullptr);
                    return;
                } else {    // Add the SE child to the queue to continue exploring
                    queue.push_back(current->SE);
                }
            }
        } else {
            if (points.y >= current->points.y) { // Northwest

                if (current->NW == nullptr) {    // Place the point in the NW child if it’s unoccupied
                    current->NW = new pointNode(points, nullptr, nullptr, nullptr, nullptr);
                    return;
                } else {     // Add the NW child to the queue to continue exploring
                    queue.push_back(current->NW);
                }
            } else {  // Southwest
                if (current->SW == nullptr) {   // Place the point in the SW child if it’s unoccupied.
                    current->SW = new pointNode(points, nullptr, nullptr, nullptr, nullptr);
                    return;
                } else {       // Add the SW child to the queue to continue exploring
                    queue.push_back(current->SW);
                }
            }
        }
    }
}


void Quadtree::search_cities(int x, int y, int r) {
// This function find and list all cities within a circular region

    if (root == nullptr)  // If the tree is empty, exit the function
        return;

    // Clearing previous results
    path.clear();    // Clear the path traversed during the search
    city_name.clear();  // Clear the list of cities found within the radius

    // a stack for iterative traversal of the quadtree
    vector <const pointNode*> stack;
    stack.push_back(root);   // start traversal from the root node

    while (!stack.empty()) {
        const pointNode * activeNode = stack.back();   // Retrieve the current node from the stack
        stack.pop_back();     // Remove the node from the stack

        if (activeNode  == nullptr)   // Skip processing if the current node is null
            continue;

        // Record the current node's city name in the path
        path.push_back(activeNode->points.nameOfCities);

        // Check if the current node is within the radius
        double distance = sqrt(pow(activeNode->points.x - x, 2) + pow(activeNode->points.y - y, 2));

        if (distance <= r) {    // If the current node is within the radius, add its city name to the result
            city_name.push_back(activeNode->points.nameOfCities);
        }

        // Check each quadrant for potential overlap with the circular region and add to stack
        if (activeNode->NW != nullptr && isCircleInQuadrant(x, y, r, activeNode->points.x, activeNode->points.y, "NW"))
            stack.push_back(activeNode->NW);
        if (activeNode->NE != nullptr && isCircleInQuadrant(x, y, r, activeNode->points.x, activeNode->points.y, "NE"))
            stack.push_back(activeNode->NE);
        if (activeNode->SW != nullptr && isCircleInQuadrant(x, y, r, activeNode->points.x, activeNode->points.y, "SW"))
            stack.push_back(activeNode->SW);
        if (activeNode->SE != nullptr && isCircleInQuadrant(x, y, r, activeNode->points.x, activeNode->points.y, "SE"))
            stack.push_back(activeNode->SE);
    }
}


bool Quadtree::isCircleInQuadrant(int x, int y, int r, int x_node, int y_node, const string& quadrant) const {
// This function checks if a given circle (its center: (x, y) and radius: r) overlaps a specific quadrant of the quadtree
// Each quadrant is defined by its relationship to a nodeX and nodeY point

    int left, right, top, bottom;   // boundaries for the quadrant

    // Define the boundaries based on the specified quadrant
    if (quadrant == "SE") {   // Southeast quadrant: right and below x_node, y_node
        left = x_node;        // Left boundary starts at x_node
        right = INT_MAX;      // There is no right limit
        top = y_node;         // Top boundary starts at y_node
        bottom = INT_MIN;     // There is no bottom limit
    }

    else if (quadrant == "SW") {  // Southwest quadrant: left and below x_node, y_node
        left = INT_MIN;       // No left limit
        right = x_node;
        top = y_node;
        bottom = INT_MIN;     // No bottom limit
    }

    else if (quadrant == "NE") {  // Northeast quadrant: right and above x_node, y_node
        left = x_node;
        right = INT_MAX;      // No right limit
        top = INT_MAX;        // No top limit
        bottom = y_node;
    }

    else if (quadrant == "NW") {  // Northwest quadrant: left and above x_node, y_node
        left = INT_MIN;       // No left limit
        right = x_node;
        top = INT_MAX;        // No top limit
        bottom = y_node;
    }

    else {
        return false; // invalid quadrant input
    }

    // Check if the circle overlaps the quadrant's bounding box
    bool bounding_overlap = !(x + r < left || x - r > right || y + r < bottom || y - r > top);

    if (!bounding_overlap) {  // No overlap in the bounding boxes --> the circle cannot overlap the quadrant
        return false;
    }

    // Manual closest point calculation
    int closestX = x;
    int closestY = y;

    if (x < left) {
        closestX = left;       // Circle is left of the quadrant
    }
    else if (x > right) {
        closestX = right;     // Circle is right of the quadrant
    }

    if (y < bottom) {
        closestY = bottom;    // Circle is below the quadrant
    }
    else if (y > top) {
        closestY = top;      // Circle is above the quadrant
    }

    // Calculate the distance from this closest point to the circle's center
    int dis_x = closestX - x;
    int dis_y = closestY - y;

    return (dis_x * dis_x + dis_y * dis_y) <= r * r;
}


void Quadtree::pretty_print(const pointNode * tree) {
// This function performs a recursive traversal of a quadtree
// and prints the nameOfCities property of each node in a specific order: SE, SW, NE, NW

    if (tree != NULL){   // Check if the current tree is not empty

        cout << tree->points.nameOfCities <<endl;   // Print the name of the city stored in the current node
        pretty_print(tree->SE);   // Recursively print the SE subtree
        pretty_print(tree->SW);   // Recursively print the SW subtree
        pretty_print(tree->NE);   // Recursively print the NE subtree
        pretty_print(tree->NW);   // Recursively print the NW subtree
    }
}


void Quadtree::pretty_print() {
    // Start the recursive pretty_print function from the root of the quadtree
    pretty_print(root);
    cout << endl;
}