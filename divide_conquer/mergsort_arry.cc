#include <iostream>
using namespace std;

void combine(int* seq, int first, int middle, int last) {

    int lengtha = middle - first + 1;
    int lengthb = last - middle;
    int tempa[100], tempb[100];

    for (int i = first, j = 0; i <= middle; i++, j++)
        tempa[j] = seq[i];
    for (int i = middle + 1, j = 0; i <= last; i++, j++)
        tempb[j] = seq[i];

    int count = 0, i = 0, j = 0;
    while (i < lengtha && j < lengthb) {
        if (tempa[i] <= tempb[j])
            seq[first + count] = tempa[i++];
        else seq[first + count] = tempb[j++];
        count++;
    }
    if (i < lengtha) {
        for ( ; i < lengtha; i++) {
            seq[first + count] = tempa[i];
            count++;
        }
    }
    if (j < lengthb) {
        for ( ; j < lengthb; j++) {
            seq[first + count] = tempb[j];
            count++;
        }
    }

}

void merge_sort(int* seq, int first, int last) {

    if (last > first) {
        int middle = (first + last) / 2;
        merge_sort(seq, first, middle);
        merge_sort(seq, middle + 1, last);
        combine(seq, first, middle, last);
    }

}

int main() {

    int seq[10];
    for (int i = 0; i < 10; i++)
        cin >> seq[i];

    merge_sort(seq, 0, 9);

    for (int i = 0; i < 10; i++)
        cout << seq[i] << " ";
    cout << endl;

}
