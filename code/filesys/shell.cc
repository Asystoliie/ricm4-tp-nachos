#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include "utility.h"
#include "system.h"


const unsigned int MAX_ARGS = 128;

using namespace std;

int run_cmd(const vector<string>& tok)
{
    if (!tok.size() || tok[0] == "")
        return 0;
    else if (tok[0] == "cd")
    {

    }
}

void shell()
{
    while (!cin.eof())
    {
        cout << "$ ";
        string temp;
        getline(cin, temp);
        if (temp == "exit") break;

        vector<string> cmd;
        //Break string into separate strings on whitespace
        {
            stringstream foo(temp);
            string s;
            while (foo >> s)
            {
                if (s[0]=='~') s = getenv("HOME") + s.substr(1);
                cmd.push_back(s);
            }
        }
        run_cmd(v);
    }
    cout << "exit" << endl;
    interrupt->Halt();
}

