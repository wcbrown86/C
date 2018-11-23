#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <ctime>

#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif

int main(int argc, char *argv[]){

    if (argc < 6)
    {
        std::cerr << "usage: " << argv[0] << " <functionid> <a> <b> <n> <intensity>" << std::endl;
        return -1;
    }

    //Global variables.
    int start = clock();
    int functionid = atoi(argv[1]);
    float a = atoi(argv[2]);
    float b = atoi(argv[3]);
    float n = atoi(argv[4]);
    int intensity = atoi(argv[5]);
    float total = 0.0f;

    for (int i = 0; i < n; i++){
        float x = (a + (i + 0.5) * ((b - a) / n));
        float f = 0.0f;

        switch (functionid){
        case 1:
            f = f1(x, intensity) * ((b - a) / n);
            break;
        case 2:
            f = f2(x, intensity) * ((b - a) / n);
            break;
        case 3:
            f = f3(x, intensity) * ((b - a) / n);
            break;
        case 4:
            f = f4(x, intensity) * ((b - a) / n);
            break;
        }

        total += f;
    }

    std::cout << total << std::endl;

    int stop = clock();
    int time = stop - start;
    std::cerr << time << std::endl;
    return 0;
}
