#include <iostream>

using namespace std;

int main()
{
    string name[3];
    cin >> name[2] >> name[1] >> name[0];
    for (int i = 0; i < 3; i++)
        cout << "Hello, " << name [i] << ".\nWelcome to Codecker." << endl;
    return (0);
}