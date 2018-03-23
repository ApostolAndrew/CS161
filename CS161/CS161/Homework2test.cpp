#include <iostream>
#include <ctime>
using namespace std;

int main()
{
    int n = 1000;
    int A[3][3];
    int B[3][3];
    int C[3][3];
    int start_s = clock();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                C[i*n+j] += A[i*n+k] * B[k*n+j];
            }
        }
    }
    int stop_s = clock();
    cout << "time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
    return 0;
}