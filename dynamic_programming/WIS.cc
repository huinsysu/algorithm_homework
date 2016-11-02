#include <iostream>
#include <algorithm>

using namespace std;

typedef struct weighted_interval {

    int start;
    int finish;
    int value;

}interval;

int p[100];
int M[100];
interval interval_arr[100];

bool comp(interval a, interval b) {

    return a.finish < b.finish;

}

int max(int a, int b) {

    return a > b ? a : b;

}

int find_p(int first, int second, int start) {

    if (first == second)
        if (first == 1)
            return 0;
        else
            return first;

    int result;
    int mid = (first + second) / 2;

    if (interval_arr[mid].finish > start) 
        result = find_p(first, mid, start);
    else if (interval_arr[mid].finish < start)
        if (interval_arr[mid + 1].finish <= start)
            result = find_p(mid + 1, second, start);
        else 
            return mid;
    else
        result = mid;

    return result;

}

int compute_opt(int cur) {

    if (M[cur] == -1) {
        M[cur] = max(interval_arr[cur].value + compute_opt(p[cur]), compute_opt(cur-1));
    }
    return M[cur];

}

void find_solution(int index) {

    if (index == 0)
        return;
    else if (M[p[index]] + interval_arr[index].value > M[index-1]) {
        cout << "( " << interval_arr[index].start << ", " << interval_arr[index].finish << " )" << endl;
        find_solution(p[index]);
    } else
        find_solution(index - 1);

}

int main () {

    int interval_num;
    int optimal;

    cout << "区间总数为：" << endl;
    cin >> interval_num;

    cout << "依次输出区间的开始时间，结束时间以及权重：" << endl;

    for (int i = 1; i <= interval_num; i++)
        cin >> interval_arr[i].start >> interval_arr[i].finish >> interval_arr[i].value;

    sort(interval_arr+1, interval_arr + interval_num + 1, comp);
    for (int i = 1; i <= interval_num; i++) {
        p[i] = find_p(1, i, interval_arr[i].start);
    }

    for (int i = 0; i <= interval_num; i++)
        M[i] = -1;
    M[0]  = 0;

    optimal = compute_opt(interval_num);

    cout << "最佳的结果为：" << optimal << endl;
    cout << "调度的序列为：" << endl;

    find_solution(interval_num);

    return 0;

}
