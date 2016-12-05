#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

int num, degree;
int **matric;
//int num, degree;

void set() {
    
    FILE *file = fopen("train-images.idx3-ubyte", "rb");
    if (file == NULL) {
        cout << "The file is empty!" << endl;
        abort();
    }

    int magic, numImages, rows, cols;
    fread(&magic, sizeof(int), 1, file);
    fread(&numImages, sizeof(int), 1, file);
    fread(&rows, sizeof(int), 1, file);
    fread(&cols, sizeof(int), 1, file);

    for (int i = 0 ; i < 60000; i++) {
        for (int j = 0 ; j < 784; j++) {
            unsigned char temp;
            fread(&temp, sizeof(unsigned char), 1, file);
            matric[i][j] = int(temp);
        }
    }
    fclose(file);

}

void hah() {

    for (int i = 0; i < 60000; i ++) {
        for (int j = 0; j < 784; j++)
            matric[i][j] = i+j;
    }

}

int main() {

    num = 60000;
    degree = 784;
    matric = new int*[num];
    for (int i = 0 ; i < num; i++) {
        matric[i] = new int [degree];
    }

    cout << num << " " << degree << endl;
    set();

    for (int i = 0; i < 2; i++) {
        delete [] matric[i];
    }
    delete [] matric;
    return 0;
}
