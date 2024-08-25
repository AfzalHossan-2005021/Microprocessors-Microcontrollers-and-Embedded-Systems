#include<iostream>

using namespace std;

int main(){
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    char a, b;
    for(int i = 0; i < 4; i++){
        cout << "{";
        for(int j = 0; j < 8; j++){
            cin >> a >> b;
            cout << "0x" << a << b;
            if(j != 7) cout << ", ";
        }
        cout << "}";
        if(i != 3) cout << "," << endl;
    }
    return 0;
}