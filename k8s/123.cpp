#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

string longestStrOfStr(string s) {
    int* mark = (int*)malloc(sizeof(int) * 255);
    memset(mark, -1, sizeof(int) * 255);
    int start, end, max = 0;
    if(s.empty()) {
        return "";
    }
    for(int i = 0; i < s.length(); i++) {
        if(mark[s[i]] == -1) {
            mark[s[i]] = i;
            end = i;
        } else {
            if(mark[s[i]] < start) {
                mark[s[i]] = i;
                end = i;
            } else {
                start = mark[s[i]] + 1;
                mark[s[i]] = i;
                end = i;
            }
        }
        if(end - start + 1 > max) {
            max = end - start + 1;
        }
    }
    if(s.length() <= max) {
        return s;
    }
    return s.substr(start, max);
}

int main(){
    string str;
    getline(cin, str);
    cout << longestStrOfStr(str) << endl;
    return 0;
}