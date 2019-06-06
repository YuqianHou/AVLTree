#include "Tree.h"
#include "Tree.cpp"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <cassert>
#include <vector>
#include <math.h>
#include <unistd.h>
using namespace std;

void print(int totalVisited, int totalRotation, int total){
    double amortizedVisited = (double)totalVisited / total;
    double amortizedRotation = (double)totalRotation / total;
    cout << "Visited " << totalVisited << " (" << amortizedVisited << ") nodes and performed " << totalRotation << " (" << amortizedRotation << ") rotations." << endl;
}



int main(int argc, char* argv[])
{
    AVLTree<int> t;

    string filename;
    if (argc > 1) filename = string(argv[1]);
    else {
        cout << "Please enter the filename in correct format." << endl;
        return 1;
    }
    
    ifstream infile(filename);
    string line;
    
    while (getline(infile, line))
    {
        // Remove comments and blank lines
        if (line == "" || line[0] == '#') {
            continue;
        }
        
        istringstream input(line);
        string command;
        getline(input, command, ' ');
        if (command == "insert") {
            int totalVisited = 0;
            int totalRotation = 0;
            int totalInserted = 0;
            int total = 0;
            while (getline(input, command, ' ')) {
                total++;
                t.inserted = 0;
                t.rotation = 0;
                t.visited = 0;
                t.insert(std::stoi(command));
                totalVisited += t.visited;
                totalRotation += t.rotation;
                totalInserted += t.inserted;
            }
            cout << "Added " << totalInserted << " of " << total << " nodes." << endl;
            print(totalVisited, totalRotation, total);
        }else if (command == "lookup"){
            int totalVisited = 0;
            int totalRotation = 0;
            int found = 0;
            int total = 0;
            int foundList[100];
            int j = 0;
            while (getline(input, command, ' ')) {
                total++;
                t.rotation = 0;
                t.visited = 0;
                if (t.contains(std::stoi(command)) == true) {
                    found++;
                    totalVisited += t.visited;
                    foundList[j] = std::stoi(command);
                    j++;
                }else{
                    totalVisited += t.visited;
                }
            }
            cout << "Found " << found << " of " << total << " nodes: [";
            for (int j = 0; j < found - 1; j++) {
                cout << foundList[j] << ", ";
            }
            cout << foundList[found - 1] << "]" << endl;
            print(totalVisited, totalRotation, total);
        }else if (command == "print"){
            while (getline(input, command, ' ')) {
                if (command == "tree") {
                    t.printTree();
                }else if (command == "left-left"){
                    cout << "The following inserts would cause a left-left rotation:" << endl;
                    
                }else if (command == "left-right"){
                    cout << "The following inserts would cause a left-right rotation:" << endl;
                    
                }else if (command == "right-left"){
                    cout << "The following inserts would cause a right-left rotation:" << endl;
                    
                }else if (command == "right-right"){
                    cout << "The following inserts would cause a right-right rotation:" << endl;
                    
                }else{
                    cout << "Wrong command" << endl;
                }
            }
            
        }else{
            cout << "Wrong command" << endl;
        }
        cout << endl;
    }

    return 0;
}
