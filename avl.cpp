#include "Tree.h"
#include "Tree.cpp"
#include <iostream>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <cassert>
#include <vector>
#include <math.h>
using namespace std;

// 处理注释，空格，和空行的函数
// line，表示一行文本内容
// comment_str，表示注释前导字符串，默认设置为#，也可以用//或者%
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

int main()
{
    AVLTree<int> t;
    ifstream is("file");
    
    string line;
    while (std::getline(is, line))
    {
        line_process(line); // 把行首和行尾的多个空格, tab去掉，把注释文字也去掉
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
            while (vs[i] != "\n") {
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
            while (vs[i] != "\n") {
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
