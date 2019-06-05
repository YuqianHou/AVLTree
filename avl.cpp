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
void line_process(std::string &line, const std::string comment_str = "#")
{
    for (char &c : line)
    {
        // 制表符 tab，逗号，分号都当作有效的分隔符，统一转成空格
        // 为了避免错误，回车符和换行符也转为空格（否则无法处理空行）
        if (c == '\t' || c == ',' || c == ';' || c == '\r' || c == '\n')
            c = ' ';
    }
    
    line.erase(0, line.find_first_not_of(" "));//删除行首空格
    line.erase(line.find_last_not_of(" ") + 1);//删除行末空格
    
    //查找注释符所在位置，如果不存在，则得到string::npos
    std::size_t n_comment_start = line.find_first_of(comment_str);
    if (n_comment_start != std::string::npos)//这一句必须的
        line.erase(n_comment_start);         //删除注释
    
    // 处理完毕。如果这一行只有空格，制表符 tab，注释，那么处理后line为空；
    // 如果行首有多个空格(或者空格和tab交错)，行尾为注释，那么处理后字符串line的
    // 行首多个空格(和tab)和行尾注释被删掉，只保留有意义的内容。
}

void print(int totalVisited, int totalRotation, int total){
    double amortizedVisited = totalVisited / total;
    double amortizedRotation = totalRotation / total;
    cout << "Visited " << totalVisited << " (" << amortizedVisited << ") nodes and performed " << totalRotation << "(" << amortizedRotation << ") rotations" << endl;
}

int main(int argc, const char* argv[])
{
    string filename = "test.sh";
//    if (argc >= 1) {
//        filename = string(argv[1]);
//    }else{
//        cout << "Please enter correct filename." << endl;
//        return 1;
//    }
    filename = argv[optind];
    AVLTree<int> t;
    
    ifstream infile(filename);
    string line;
    while (getline(infile, line))
    {
        // Remove blank and tab at the beginning and end of this line.
        // Remove comments.
        line_process(line);
        
        if (line.empty())
            continue;
        
        istringstream iss(line);
        vector<string> vs;
        string s;
        while (iss >> s) {
            vs.push_back(s);
        }
        
        if (vs[0] == "insert") {
            int totalVisited = 0;
            int totalRotation = 0;
            int totalInserted = 0;
            int total = 0;
            int i = 1;
            while (i < vs.size()) {
                total++;
                t.insert(std::stoi(vs[i]));
                totalVisited += t.visited;
                totalRotation += t.rotation;
                totalInserted += t.inserted;
                i++;
            }
            cout << "Added " << totalInserted << " of " << total << "nodes." << endl;
            print(totalVisited, totalRotation, total);
        }else if (vs[0] == "lookup"){
            int totalVisited = 0;
            int totalRotation = 0;
            int found = 0;
            int total = 0;
            int foundList[100];
            int i = 1;
            int j = 0;
            while (i < vs.size()) {
                total++;
                if (t.contains(std::stoi(vs[i])) == true) {
                    found++;
                    totalVisited += t.visited;
                    foundList[j] = std::stoi(vs[i]);
                    j++;
                }
                i++;
            }
            cout << "Found " << found << " of " << total << " nodes: [";
            for (int j = 0; j < found - 1; j++) {
                cout << foundList[j] << ", ";
            }
            cout << foundList[found - 1] << "]" << endl;
            print(totalVisited, totalRotation, total);
        }else if (vs[0] == "print"){
            if (vs[1] == "tree") {
                t.printTree();
            }else if (vs[1] == "left-left"){
                cout << "The following inserts would cause a left-left rotation:" << endl;
                
            }else if (vs[1] == "left-right"){
                cout << "The following inserts would cause a left-right rotation:" << endl;
                
            }else if (vs[1] == "right-left"){
                cout << "The following inserts would cause a right-left rotation:" << endl;
                
            }else if (vs[1] == "right-right"){
                cout << "The following inserts would cause a right-right rotation:" << endl;
                
            }else{
                cout << "Wrong command" << endl;
            }
        }else{
            cout << "Wrong command" << endl;
        }
    }
    

    return 0;
}
