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

// Process comments, blanks and blank lines
// line: a line of commend
// comment_str: when the line start with #, it is a comment and should be ignored
// Likewise, empty lines should be ignored.
//void line_process(std::string &line, const std::string comment_str = "#")
//{
//    for (char &c : line)
//    {
//        // 制表符 tab，逗号，分号都当作有效的分隔符，统一转成空格
//        // 为了避免错误，回车符和换行符也转为空格（否则无法处理空行）
//        if (c == '\t' || c == ',' || c == ';' || c == '\r' || c == '\n')
//            c = ' ';
//    }
//
//    line.erase(0, line.find_first_not_of(" "));//删除行首空格
//    line.erase(line.find_last_not_of(" ") + 1);//删除行末空格
//
//    //查找注释符所在位置，如果不存在，则得到string::npos
//    std::size_t n_comment_start = line.find_first_of(comment_str);
//    if (n_comment_start != std::string::npos)//这一句必须的
//        line.erase(n_comment_start);         //删除注释
//
//    // 处理完毕。如果这一行只有空格，制表符 tab，注释，那么处理后line为空；
//    // 如果行首有多个空格(或者空格和tab交错)，行尾为注释，那么处理后字符串line的
//    // 行首多个空格(和tab)和行尾注释被删掉，只保留有意义的内容。
//}

void print(int totalVisited, int totalRotation, int total){
    double amortizedVisited = totalVisited / total;
    double amortizedRotation = totalRotation / total;
    cout << "Visited " << totalVisited << " (" << amortizedVisited << ") nodes and performed " << totalRotation << "(" << amortizedRotation << ") rotations" << endl;
}

int main(int argc, char* argv[])
{
    AVLTree<int> t;
//    istringstream input;
//    const char* storefile;
//    storefile = argv[optind];
//    stringstream buffer;
//    string line;
//    string command;
//    string value;
//    // Insert lines from storefile
//    std::ifstream store(storefile);
//    if(store.fail()) {
//        std::cerr << "Unable to open file: " << storefile << '\n';
//        exit(1);
//    }
    
//    while(std::getline(store, line)) {
//        if(v > 1) std::cout << ADDL << line << '\n';
//        hashset.insert(line);
//        filter.insert(line);
//
//        nstores += 1;
//        if(nstores == n) {
//            break;
//        }
//    }
    
//    if(v > 1) std::cout << '\n';
//    store.close();

    //string filename = "test.sh";
//    if (argc >= 1) {
//        filename = string(argv[1]);
//    }else{
//        cout << "Please enter correct filename." << endl;
//        return 1;
//    }

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
        // Remove comments.
        //line_process(line);
        
//        if (line.empty())
//            continue;
        
//        istringstream iss(line);
//        vector<string> vs;
//        string s;
//        while (iss >> s) {
//            vs.push_back(s);
//        }
        
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
                t.inserted = 0;
                t.rotation = 0;
                t.visited = 0;
                total++;
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
                t.rotation = 0;
                t.visited = 0;
                total++;
                if (t.contains(std::stoi(command)) == true) {
                    found++;
                    totalVisited += t.visited;
                    foundList[j] = std::stoi(command);
                    j++;
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
    }

    return 0;
}
