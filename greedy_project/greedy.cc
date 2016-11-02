#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <queue>
using namespace std;

int nodes[100];  //存放没有被实例化的view,数组的下标表示view的id，值为-1代表已经被实例化。
int nodeIndex[100];  //存放每一个view的下标值。例如view id 的是第i个节点。设该数组的原因是view的id与其下标值不一定相同。
int object[100];  //顺序存放将被实例化的view。
int BvS[100];  //临时存放每一个view的代价的改善值。
int min_cost[100];  //存放每一次实例化新view前每个view的代价。
int lattice[100][100];  //记录lattice中view的连通关系。
int number[] = {-1, -1};  //number[0]是view的数目。

void getFromInput(char* filename) {  //解析输入文件。

    int index = 1;  //view的下标从1开始。
    char buffer[100];  //临时存放文件输入的每一行字符串。
    ifstream fin(filename);
    
    while (fin.getline(buffer, 100)) {
        int blank_flag = 0;  //用来区分某个字符串代表输入的上半部分还是下半部分。
        int j = 0;  //j是临时字符串数组的下标。
        int id;  //id是每一个view的标识符。
        int first, second;  //表示存在first->second的关系。
        char str[20];
        for (int i = 0; i <= strlen(buffer); i++) {
            if (buffer[i] == '\0' && number[1] == -1) {
                if (blank_flag == 1) {  //表示view及其代价那一行中的代价。
                    str[j] = '\0';
                    nodes[id] = atoi(str);
                    nodeIndex[index++] = id;
                } else {  //表示view或者边的数目。
                    str[j] = '\0';
                    if (number[0] > 0) {
                        number[1] = atoi(str);
                    } else {
                        number[0] = atoi(str);
                    }
                }
            } else if (buffer[i] == '\0' && number[1] > 0) {  //表示两连通view的第二个view。
                str[j] = '\0';
                second = atoi(str);
                lattice[first][second] = 1;  //view之间连通使用id代表view，而不是用view的下标。
            } else if (buffer[i] == ' ' && number[1] == -1) {  //表示view及其代价那一行中的view。
                blank_flag = 1;
                str[j] = '\0';
                id = atoi(str);
                j = 0;
            } else if (buffer[i] == ' ' && number[1] > 0) {  //表示两连通view的第一个view。
                blank_flag = 1;
                str[j] = '\0';
                first = atoi(str);
                j = 0;
            } else {
                str[j++] = buffer[i];
            }
        }
    }
    fin.close();
}

void init() {  //初始化各个数组。

    for (int i = 0;  i < 100; i++) {
        nodes[i] = object[i]  = min_cost[i] = -1;
        BvS[i] = 0;
        for (int j = 0; j < 100; j++)
            lattice[i][j] = -1;
    }
}

void findSelections(int k) {  //计算出将要实例化的view。

    queue<int> que;  //用来遍历一个view的所有连通view。

    int rootIndex;  //根view的下标。

    for (int j = 1; j <= number[0]; j++) {  //找出根view的下标。
        int i;
        for (i = 1; i <= number[0]; i++) {
            if (lattice[nodeIndex[i]][nodeIndex[j]] > 0)
                break;
        }
        if (i == number[0] + 1) {  //根view没有其他view指向它。
            rootIndex = j;
            break;
        }
    }

    for (int i = 0; i <= number[0]; i++)  //一开始每个view的代价都是根view的代价。
        min_cost[i] = nodes[nodeIndex[rootIndex]];
 
    object[0] = nodeIndex[rootIndex];  //根view一定是第一个实例化的view。
    nodes[nodeIndex[rootIndex]] = -1;  //已经实例化的view在nodes数组中值为-1。
    
    for (int i = 1; i <= k; i++) {
        for (int j = 1; j <= number[0]; j++) {  //计算各个view所能减少的代价，存放在BvS数组中。
            if (nodes[nodeIndex[j]] > 0) {
                int decrease = min_cost[j] - nodes[nodeIndex[j]];
                BvS[j] = decrease > 0 ? decrease : BvS[j];
                que.push(nodeIndex[j]);
                while (!que.empty()) {
                    int top = que.front();
                    for (int l = 1; l <= number[0]; l++) {
                        if (lattice[top][nodeIndex[l]] > 0) {
                            int decrease = min_cost[l] - nodes[nodeIndex[j]];
                            if (decrease > 0) BvS[j] += decrease;
                            que.push(nodeIndex[l]);
                        }
                    }
                    que.pop();
                }
            }
        }

        int max = 0;
        int max_index;
        for (int j = 1; j <= number[0]; j++) {  //找出此次搜索到的目标view，即BvS数组中值最大对应的那一项。
            if (BvS[j] > max) {
                max = BvS[j];
                max_index = j;
            }
        }
        if (max == 0) {  //可能存在没有实例化的所有view都不能在改善现有的开销。
            for (int j = 1; j <= number[0]; j++)
                if (nodes[nodeIndex[j]] > 0)
                    max_index = j;
        }
        object[i] = nodeIndex[max_index];
        min_cost[max_index] = nodes[nodeIndex[max_index]];  //某view的代价一定小于或等于其所有父view的代价。

        que.push(nodeIndex[max_index]);
        while (!que.empty()) {  //更新min_cost数组。
            int top = que.front();
            for (int l = 1; l <= number[0]; l++) {
                if (lattice[top][nodeIndex[l]] > 0) {
                    int decrease = min_cost[l] - nodes[nodeIndex[max_index]];
                    min_cost[l] = decrease > 0 ? nodes[nodeIndex[max_index]] : min_cost[l];
                    que.push(nodeIndex[l]);
                }
            }
            que.pop();
        }
        nodes[nodeIndex[max_index]] = -1;

        for(int j = 0; j <= number[0]; j++)
            BvS[j] = 0;
    }
}

void writeToFile(int k) {

    for (int i = 0; i <= k; i++) {
        if (i != k) {
            cout << object[i] << " -> ";
        } else {
            cout << object[i] << endl;
        }
    }
}

int main(int argc, char* argv[]) {
    
    init();
    getFromInput(argv[2]);

    int k = atoi(argv[4]);  //要实例化的view数。
    findSelections(k);

    writeToFile(k);

    return 0;
}

