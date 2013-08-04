#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <cassert>
#include <iomanip>
using namespace std;

map<string, string> plural;

void init() {
    plural["mile"] = "miles";
    plural["yard"] = "yards";
    plural["inch"] = "inches";
    plural["foot"] = "feet";
    plural["fath"] = "faths";
    plural["furlong"] = "furlongs";
}

string trim(const string &s) {
    string t;
    for ( int i = 0; i < s.size(); ++i )
        if ( s[i]!=' ' )
            t += s[i];
    return t;
}

pair<double, string> parse(const string &s) {
    double val;
    string unit;
    stringstream sm(s);

    sm >> val;
    getline(sm, unit);

    return make_pair( val, trim(unit) );
}


map<string, double> rate; // 1 inch = rate["inch"] meter

int main() {
	ifstream fin("input.txt", ios::in);
	ofstream fout("output.txt", ios::out);
	fout.setf(ios::fixed, ios::floatfield); 
    init();
	fout.precision(2);
    fout << "sbihero@gmail.com" << endl;
    fout << endl;
    for ( string cmd; getline(fin, cmd); ) {
        if ( trim(cmd) == "" ) continue;
        int eq = cmd.find('=');
        if ( eq != string::npos ) {
            pair<double, string> f = parse( cmd.substr(0, eq)  );
            pair<double, string> t = parse( cmd.substr(eq + 1) );
            if ( t.second == "m" ) {
                rate[f.second] = t.first / f.first;
                if ( plural.find(f.second) != plural.end() ) {
                    rate[plural[f.second]] = t.first / f.first;
                }
            } else {
                assert(0);
            }
        } else {
            double val = 0;
            for ( int last = 0, cur, sign = 1;; ) {
                cur = cmd.find_first_of("+-", last);
                string slice = (cur == string::npos ?
                                cmd.substr(last) :
                                cmd.substr(last, cur - last));
                pair<double, string> term = parse( slice );
                if ( rate.find(term.second) == rate.end() ) {
                    cerr << "Unrecognized unit: " << term.second << endl;
                }
                val += sign * term.first * rate[term.second];
//                cerr << term.first << "(" << term.second <<")" << endl;

                if ( cur == string::npos ) break;
                sign = (cmd[cur] == '+' ? 1 : -1);
                last = cur + 1;
            }
            fout<<val<<" m"<<endl;
        }
    }
}