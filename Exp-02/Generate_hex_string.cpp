#include<iostream>

using namespace std;

int main(){
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    
    char a, b;
    for(int i = 0; i < 8; i++){
        cin >> a >> b;
        cin >> a >> b;
        cout << a << b;
        if(i != 7) cin >> a;
    }
    return 0;
}