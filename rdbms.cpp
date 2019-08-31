#include <iostream>
#include "headers.cpp"
using namespace std;

int main()
{
    rdbms obj;
    while (1)
    {
        string input = "";
        cout << ">";
        getline(cin, input);
        parseSyntax(obj, input);
    }

    return 0;
}