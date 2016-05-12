#include <iostream>
#include <cstdlib>

using namespace std;


int main(int argc, char **argv){

    int ar[6] = {0,0,0,0,0,0};

    for (int i = 1; i < argc && i < 7; ++i){
        ar[i-1] = atoi(argv[i]);
    }

    if (ar[0]>ar[3] ||
       (ar[0] == ar[3] && ( ar[1]!= 0 && ar[4] != 0  && (ar[1]%ar[4] == 0 || ar[4]%ar[1] == 0))) ||
       (ar[3]-ar[0] < ar[2]+ar[5] && ar[2] != ar[5])){
        cout << 1;
    }
    else{
        cout << 0;
    }
    return 0;
}
