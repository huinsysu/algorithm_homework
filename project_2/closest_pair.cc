#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>

using namespace std;

#define PI 3.1415926535

typedef struct closest_pair{

    int obj1, obj2;
    double distance;

}CP;

typedef struct projection_vector {

    int index;
    double value;

}PV;

int object_num;
int dimension;
int projection_num = 100;

int **object_list;
PV** random_projections;

void get_random_projection() {

    srand(time(NULL));
    double** vector_list = new double*[projection_num];
    for (int i = 0; i < projection_num; i++) {
        vector_list[i] = new double[dimension];
        for (int j = 0 ; j < dimension; j++) {
            double random1 = (rand() * 1.0) / (RAND_MAX * 1.0);
            double random2 = (rand() * 1.0) / (RAND_MAX * 1.0);
            if (j % 2 == 0) {
                vector_list[i][j] = sqrt((-2) * log(random1)) * cos(2 * PI * random2);
            } else {
                vector_list[i][j] = sqrt((-2) * log(random1)) * sin(2 * PI * random2);
            }
        }
    }
    for (int i = 0; i < object_num; i++) {
        for (int j = 0; j < projection_num; j++) {
            PV projectionItem;
            projectionItem.index = i;
            double value = 0;
            for (int k = 0; k < dimension; k++) {
                value += double(object_list[i][k]) * vector_list[j][k];
            }
            projectionItem.value = value;
            random_projections[j][i] = projectionItem;
        }
    }

    for (int i = 0; i < projection_num; i++) delete [] vector_list[i];
    delete [] vector_list;

}

void swap_projection(PV* projection, int first, int second) {

    PV temp = projection[first];
    projection[first] = projection[second];
    projection[second] = temp;

}

int get_median_index(PV* projection, int low, int high, int excepted_num) {

    int last_small = low;
    int swap_index = rand() % (high - low + 1) + low;
    PV pivot = projection[swap_index];
    swap_projection(projection, low, swap_index);
    
    for (int i = low + 1; i <= high; i++) {
        if (projection[i].value < pivot.value) {
            last_small++;
            swap_projection(projection, last_small, i);
        }
    }
    swap_projection(projection, low, last_small);

    int return_index;
    if (last_small - low == excepted_num - 1) {
        return_index = last_small;
    } else if (last_small - low >= excepted_num) {
        return_index = get_median_index(projection, low, last_small - 1, excepted_num);
    } else {
        return_index = get_median_index(projection, last_small + 1, high, excepted_num - 1 - (last_small - low));
    }
    return return_index;

}

CP get_closest_pair_median(PV* projection, int low, int high) {

    CP cur_result;
    if (low == high) {
        cur_result.distance = 1000000;
    } else if (high == low + 1) {
        if (projection[low].value > projection[high].value) {
            swap_projection(projection, low, high);
        }
        cur_result.obj1 = projection[low].index;
        cur_result.obj2 = projection[high].index;
        cur_result.distance = projection[high].value - projection[low].value;
    } else {
        CP low_result, middle_result, high_result;
        int median_index = get_median_index(projection, low, high, (high - low) / 2 + 1);
        low_result = get_closest_pair_median(projection, low, median_index);
        high_result = get_closest_pair_median(projection, median_index + 1, high);
        middle_result.obj1 = projection[median_index].index;
        middle_result.obj2 = projection[median_index + 1].index;
        middle_result.distance = projection[median_index + 1].value - projection[median_index].value;

        cur_result.distance = 1000000;
        if (low_result.distance < cur_result.distance) {
            cur_result = low_result;
        }
        if (middle_result.distance < cur_result.distance) {
            cur_result = middle_result;
        }
        if (high_result.distance < cur_result.distance) {
            cur_result = high_result;
        }
    }
    return cur_result;

}

CP get_closest_pair_pivot(PV* projection, int low, int high) {

    CP cur_result;
    if (low == high) {
        cur_result.distance = 1000000;
    } else if (high == low + 1) {
        if (projection[low].value > projection[high].value) {
            swap_projection(projection, low, high);
        }
        cur_result.obj1 = projection[low].index;
        cur_result.obj2 = projection[high].index;
        cur_result.distance = projection[high].value - projection[low].value;
    } else {
        int last_small = low;
        int swap_index = rand() % (high - low + 1) + low;
        PV pivot = projection[swap_index];
        swap_projection(projection, low, swap_index);

        for (int i = low + 1; i <= high; i++) {
            if (projection[i].value < pivot.value) {
                last_small++;
                swap_projection(projection, last_small, i);
            }
        }
        swap_projection(projection, low, last_small);

        CP low_result, middle_result, high_result;
        if (last_small == high) {
            low_result = get_closest_pair_pivot(projection, low, last_small - 1);
            high_result = get_closest_pair_pivot(projection, last_small, high);
            middle_result.obj1 = projection[last_small - 1].index;
            middle_result.obj2= projection[last_small].index;
            middle_result.distance = projection[last_small].value - projection[last_small - 1].value;
        } else {
            low_result = get_closest_pair_pivot(projection, low, last_small);
            high_result = get_closest_pair_pivot(projection, last_small + 1, high);
            middle_result.obj1 = projection[last_small].index;
            middle_result.obj2= projection[last_small + 1].index;
            middle_result.distance = projection[last_small + 1].value - projection[last_small].value;
        }
        cur_result.distance = 1000000;
        if (low_result.distance < cur_result.distance) {
            cur_result = low_result;
        }
        if (middle_result.distance < cur_result.distance) {
            cur_result = middle_result;
        }
        if (high_result.distance < cur_result.distance) {
            cur_result = high_result;
        }
    }
    return cur_result;

}

CP get_line_closest_pair(PV* projection) {

    CP result;
    result = get_closest_pair_pivot(projection, 0, object_num - 1);
    //result = get_closest_pair_median(projection, 0, object_num - 1);
    return result;
}

double calculate_distance(CP closest_pair) {

    double dist = 0;
    int* obj1 = object_list[closest_pair.obj1];
    int* obj2 = object_list[closest_pair.obj2];
    for (int i = 0; i < dimension; i++) {
        dist += pow(obj1[i] - obj2[i], 2);
    }
    return sqrt(dist);

}

CP get_closest_pair() {

    random_projections = new PV*[projection_num];
    for (int i = 0; i < projection_num; i++) {
        random_projections[i] = new PV[object_num];
    }

    get_random_projection();

    double min = 1000000.0;
    CP result;

    for (int i = 0; i < projection_num; i++) {
        CP temp;
        double temp_dist;
        temp = get_line_closest_pair(random_projections[i]);
        temp_dist = calculate_distance(temp);
        if (temp_dist < min) {
            min = temp_dist;
            result = temp;
            result.distance = temp_dist;
        }
    }
    for (int i = 0; i < projection_num; i++) {
        delete [] random_projections[i];
    }
    delete [] random_projections;
    return result;

}

void read_from_file(char* filename) {

    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        cout << "The file is empty!" << endl;
        abort();
    }

    int magic, numImages, rows, cols;
    fread(&magic, sizeof(int), 1, file);
    fread(&numImages, sizeof(int), 1, file);
    fread(&rows, sizeof(int), 1, file);
    fread(&cols, sizeof(int), 1, file);

    for (int i = 0 ; i < object_num; i++) {
        for (int j = 0 ; j < dimension; j++) {
            unsigned char temp;
            fread(&temp, sizeof(unsigned char), 1, file);
            object_list[i][j] = int(temp);
        }
    }
    fclose(file);
}

int main(int argc, char* argv[]) {

    object_num = atoi(argv[2]);
    dimension = atoi(argv[4]);
    char* filename = argv[6];
    clock_t start, finish;

    object_list = new int*[object_num];
    for (int i = 0; i < object_num; i++) {
        object_list[i] = new int [dimension];
    }

    read_from_file(filename);

    start = clock();
    CP result = get_closest_pair();
    finish = clock();

    cout << "使用算法1找最近对需要的时间为：" << double(finish - start) << endl;
    cout << result.obj1 << " " << result.obj2 << endl;
    for (int i = 0; i < dimension; i++) {
        if (i != 0 && i % 28 == 0) cout << endl;
        if (object_list[result.obj1][i] > 0) cout << " ";
        else cout << "*";
    }
    cout << endl << endl;
    for (int i = 0; i < dimension; i++) {
        if (i != 0 && i % 28 == 0) cout << endl;
        if (object_list[result.obj2][i] > 0) cout << " ";
        else cout << "*"; 
    }
    cout << endl;

    for (int i = 0; i < object_num; i++) {
        delete [] object_list[i];
    }
    delete [] object_list;
    return 0;

}
