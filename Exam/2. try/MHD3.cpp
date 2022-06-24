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
#include <cassert>

using namespace std;

class CTrain
{
public:
    void Add(istringstream &is);
    int Count(void) const;

private:
    map < string, set<string>> stations;
};

void CTrain::Add(istringstream &is)
{
    string st1, st2;
    getline(is, st1);
    if ( st1 == "" ) return;
    stations[st1];
    while ( getline(is,st2) ) {
        stations[st1].insert(st2);
        stations[st2].insert(st1);
        st1 = st2;
    }
}

int CTrain::Count(void) const
{
    set <string> visited;
    int result = 0;

    for ( const auto& st : stations ) {
        queue <string> q;
        if ( visited.count(st.first) == 1 || st.first == "")
            continue;
        q.emplace(st.first);

        while ( !q.empty() ) {

            string curr = q.front();
            q.pop();

            visited.insert(curr);

            for ( const auto& st2 : stations.find(curr)->second ) {
                if ( visited.count(st2) == 1 )
                    continue;
                q.emplace(st2);
            }
        }
        result++;
    }
    return result;
}

int main()
{
    {
        CTrain t0;
        istringstream iss;
        assert(t0.Count() == 0);

        iss.clear();
        iss.str("Newton\n");
        t0.Add(iss);
        assert(t0.Count() == 1);

        iss.clear();
        iss.str("Newton\n");
        t0.Add(iss);
        assert(t0.Count() == 1);

        iss.clear();
        iss.str("\n");
        t0.Add(iss);
        assert(t0.Count() == 1);

        iss.clear();
        iss.str("\n\nNewton\n\nNewton\n\nNewton\n\n");
        t0.Add(iss);
        assert(t0.Count() == 1);

        iss.clear();
        iss.str("Black Hill\n");
        t0.Add(iss);
        assert(t0.Count() == 2);

        iss.clear();
        iss.str("Wood Side\n");
        t0.Add(iss);
        assert(t0.Count() == 3);

        iss.clear();
        iss.str("Green Hill\n");
        t0.Add(iss);
        assert(t0.Count() == 4);

        iss.clear();
        iss.str("Lakeside\n");
        t0.Add(iss);
        assert(t0.Count() == 5);

        iss.clear();
        iss.str("Newton\nBlack Hill\nWood Side\nGreen Hill\nLakeside\n");
        t0.Add(iss);
        assert(t0.Count() == 1);

        iss.clear();
        iss.str("Little Newton\nLittle Burnside\nCastle Hill\nNewton Crossroad\nLakeside Central\n");
        t0.Add(iss);
        assert(t0.Count() == 2);

        iss.clear();
        iss.str("Waterton West\nWaterton Central\nWaterton East\nWaterton Woods\nLittle Waterton\nWaterton West\n");
        t0.Add(iss);
        assert(t0.Count() == 3);

        iss.clear();
        iss.str("Little Waterton\nLakeside Central\n");
        t0.Add(iss);
        assert(t0.Count() == 2);

        iss.clear();
        iss.str("Great Newton\nLittle Burnside\nGreen Hill\nWood Side\n");
        t0.Add(iss);
        assert(t0.Count() == 1);

        iss.clear();
        iss.str("Sodor Ironworks\nSodor Steamworks\nKnapford\nMaron\n");
        t0.Add(iss);
        assert(t0.Count() == 2);

        iss.clear();
        iss.str("Tidmouth\nGordons Hill\nSuderry\nKnapford\nGreat Waterton\nBrendam Docks\n");
        t0.Add(iss);
        assert(t0.Count() == 2);

        iss.clear();
        iss.str("Newton\nNewton Crossroad\nTidmouth\nBrendam Docks\n");
        t0.Add(iss);
        assert(t0.Count() == 1);
    }
    {
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
    }
    return 0;
}
