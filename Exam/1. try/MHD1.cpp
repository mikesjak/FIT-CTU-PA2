#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <stack>
#include <cassert>
#include <iostream>
using namespace std;

class CMHD
{
public:
    void Add ( istringstream & is ) {
        vector<string> stInLine;
        string st;
        while( getline(is, st) )
            stInLine.push_back(st);
        for ( auto &it1 : stInLine )
            for ( auto &it2 : stInLine )
                stations[it1].insert(it2);
    }

    //===============================================================================

    set < string > Dest ( const string & from, int maxCost ) const {
        set <string> result;

        if ( stations.count(from) == 0 )
            return result = {from};

        queue< pair <string, int> > queue1;
        queue1.push({ from, 0 });

        while ( !queue1.empty() ) {

            string st = queue1.front().first;
            int cost = queue1.front().second;
            queue1.pop();

            if ( result.count(st) == 1 ) continue;
            result.insert(st);
            if ( cost > maxCost ) continue;

            for ( auto& it : stations.at(st) )
                queue1.push( { it, cost+1} );
        }
        for ( auto& c : result )
            cout << c << endl;
        return result;
    }

    //===============================================================================
private:
    map< string, set <string> > stations;
};

int main ( void ) {
    CMHD city;
    istringstream iss;
    iss.clear();

    iss.str("A\nB\nC\nD\nE\n");
    city.Add(iss);
    iss.clear();

    iss.str("B\nC\nF\nH\n");
    city.Add(iss);
    iss.clear();

    iss.str("F\nG\nI\nJ\nK\nN\n");
    city.Add(iss);
    iss.clear();

    iss.str("H\nL\n");
    city.Add(iss);
    iss.clear();

    iss.str("L\nM\nN\nO\n");
    city.Add(iss);
    iss.clear();

    iss.str("P\nQ\nR\nN\nS");
    city.Add(iss);
    iss.clear();

    assert (city.Dest("S", 0) == set<string>({"S", "N", "R", "Q", "P"}));

    assert (city.Dest("S", 1) == set<string>({"S", "N", "R", "Q", "P",
                                              "O", "M", "L",
                                              "K", "J", "I", "G", "F"}));

    assert (city.Dest("N", 0) == set<string>({"S", "N", "R", "Q", "P",
                                              "O", "M", "L",
                                              "K", "J", "I", "G", "F"}));

    assert (city.Dest("N", 1) == set<string>({"S", "N", "R", "Q", "P",
                                              "O", "M", "L",
                                              "K", "J", "I", "G", "F",
                                              "H", "F", "C", "B"}));

    assert (city.Dest("N", 2) == set<string>({"S", "N", "R", "Q", "P",
                                              "O", "M", "L",
                                              "K", "J", "I", "G", "F",
                                              "H", "F", "C", "B",
                                              "A", "D", "E"}));

    assert (city.Dest("unknown", 0) == set<string>({"unknown"}));
    assert (city.Dest("unknown", 1) == set<string>({"unknown"}));
    assert (city.Dest("unknown", 2) == set<string>({"unknown"}));

    return 0;
}