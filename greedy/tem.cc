#include<iostream>
#include<algorithm>
using namespace std;

struct node{
    int a;
    int b;
};

bool comp(struct node fir, struct node sec) {
    return fir.a < sec.a;
}

int main() {
    struct node arr[2];

    arr[0].a = 10;
    arr[0].b = 1;
    arr[1].a = 7;
    arr[1].b = 2;

    sort(arr, arr+2, comp);

    cout << arr[0].a << arr[1].a << endl;
    return 0;
}
