#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <stack>

using namespace std;

//--------------------------------------------------------------------------------------

class CTrain {
public:

    void Add(istringstream & is) {
        string st1, st2;
        getline(is, st1);
        stations[st1];
        while (getline(is, st2)) {
            stations[st1].insert(st2);
            stations[st2].insert(st1);
            st1 = st2;
        }
    }
/*
    int Count(void) {
        visited.clear();
        int count = 0;
        for ( const auto& st : stations )
            count += recurse(st.first);
        return count;
    }

    int recurse(string st) {
        // already visited -> has to be on already checked island
        if ( visited[st] ) return 0;
        // new island -> need to add 1;
        visited[st] = true;
        // needs to mark all stations within this island
        for ( const auto& it : stations[st] )
            recurse(it);
        return 1;
    }
*/

    int Count(void) {
        int count = 0;
        visited.clear();
        for ( const auto& st: stations ) {
            if ( visited[st.first] )
                continue;
            queue<string> q;
            q.push(st.first);
            while ( !q.empty() ) {
                string station = q.front();
                q.pop();
                visited[station] = true;
                for ( const auto& c : stations[station] ) {
                    if ( !visited[c] )
                        q.emplace(c);
                }
            }
            count++;
        }
        return count;
    }

private:
    map <string, bool> visited;
    map< string, set<string> > stations;
};

//--------------------------------------------------------------------------------------

int main() {
    CTrain t0;
    istringstream iss;
    iss . clear();
    iss . str("Newton\nBlack Hill\nWood Side\nGreen Hill\nLakeside\n");
    t0 . Add(iss);
    cout << "res : " << t0.Count() << "  ref : 1" << endl;
    iss . clear();
    iss . str("Little Newton\nLittle Burnside\nCastle Hill\nNewton Crossroad\nLakeside Central\n");
    t0 . Add(iss);
    cout << "res : " << t0.Count() << "  ref : 2" << endl;
    iss . clear();
    iss . str("Waterton West\nWaterton Central\nWaterton East\nWaterton Woods\nLittle Waterton\nWaterton West\n");
    t0 . Add(iss);
    cout << "res : " << t0.Count() << "  ref : 3" << endl;
    iss . clear();
    iss . str("Little Waterton\nLakeside Central\n");
    t0 . Add(iss);
    cout << "res : " << t0.Count() << "  ref : 2" << endl;
    iss . clear();
    iss . str("Great Newton\nLittle Burnside\nGreen Hill\nWood Side\n");
    t0 . Add(iss);
    cout << "res : " << t0.Count() << "  ref : 1" << endl;
    iss . clear();
    iss . str("Sodor Ironworks\nSodor Steamworks\nKnapford\nMaron\n");
    t0 . Add(iss);
    cout << "res : " << t0.Count() << "  ref : 2" << endl;
    iss . clear();
    iss . str("Tidmouth\nGordons Hill\nSuderry\nKnapford\nGreat Waterton\nBrendam Docks\n");
    t0 . Add(iss);
    cout << "res : " << t0.Count() << "  ref : 2" << endl;
    iss . clear();
    iss . str("Newton\nNewton Crossroad\nTidmouth\nBrendam Docks\n");
    t0 . Add(iss);
    cout << "res : " << t0.Count() << "  ref : 1" << endl;
    return 0;
}