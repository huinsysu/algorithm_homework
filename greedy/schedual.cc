/*给定一系列的工作序列，每个工作都有开始时间和结束时间，
不同工作在时间上可能会重叠。找出一个合适的调度，使得工作
完成的总数达到最大。
*/

#include<iostream>
#include<algorithm>
using namespace std;

struct Job {
    int start;
    int finish;
};

bool comp(struct Job fir, struct Job sec) {
    return fir.finish < sec.finish;
}

int main(){
    int n;
    struct Job job_arr[100];
    struct Job answer[100];
    cout << "请输入工作的数量:";
    cin >> n;
    cout << "请输入每个工作的开始和结束时间：" << endl;

    for (int i = 0;  i < n; i++)
        cin >> job_arr[i].start >> job_arr[i].finish;

    sort(job_arr, job_arr+n, comp);

    answer[0].start = job_arr[0].start;
    answer[0].finish = job_arr[0].finish;

    int index = 0;

    for (int i = 1; i < n; i++)
        if (job_arr[i].start >= answer[index].finish) {
            answer[++index].start = job_arr[i].start;
            answer[index].finish = job_arr[i].finish;
        }

    cout << "合理的调度为：" << endl;

    for (int i = 0; i <= index; i++)
        cout << answer[i].start << ' ' << answer[i].finish << endl;
   
    return 0;
}

