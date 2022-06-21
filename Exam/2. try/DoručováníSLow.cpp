#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <memory>
#include <cassert>
using namespace std;

class CDelivery
{
public:
    // ctor
    // dtor

    // add connection from -> to, fluent interface
    CDelivery & addConn ( const string & from, const string & to );

    // returns a map containing shortest path to each customer from any of the depots
    // map key is the customer name, value is a list of nodes on the shortest path (depot -> customer)
    // if there is no path, the value is an empty list
    map<string, list<string>> serveCustomers ( const set<string> & customers, const set<string> & depots ) const;
    pair<string, list<string>> findShortestPath( const string customer, const set<string> & depots ) const;
private:
    // todo
    map <string, set<string>> depos;

};

CDelivery & CDelivery::addConn ( const string & from, const string & to )
{
    depos[from].insert(to);
    return *this;
}

map<string, list<string>> CDelivery::serveCustomers ( const set<string> & customers, const set<string> & depots ) const
{
    map < string, list<string>> result;

    for ( const auto& customer : customers ) {
        auto tmp = findShortestPath(customer, depots);
        result.emplace(customer, tmp.second);
    }
    return result;
}

pair<string, list<string>> CDelivery::findShortestPath( const string customer, const set<string> & depots ) const {

    string nearest;
    int lowest = INT32_MAX;
    list<string> currentPath;

    for ( const auto& depot : depots ) {
        queue< pair<string, int> > q;
        map <string, string> to_from;
        q.emplace(depot, 0);
        to_from.emplace(depot, "");

        while ( !q.empty() ) {
            string curr = q.front().first;
            int dist = q.front().second;
            q.pop();

            if ( dist > lowest )
                break;

            if ( curr == customer ) {
                list<string> road;

                auto it = to_from.find(curr)->second;
                while ( it != "" ) {
                    road.push_front(it);
                    it = to_from[it];
                }
                road.push_back(customer);
                if ( dist < lowest ) {
                    lowest = dist;
                    currentPath = road;
                    nearest = curr;
                }
                break;
            }
            auto it = depos.find(curr);
            if ( it == depos.end() ) break;
            for ( const auto& c : it->second ) {
                auto tmp = to_from.find(c);
                if ( tmp != to_from.end() )
                    continue;
                else {
                    q.emplace(c, dist +1 );
                    to_from.emplace(c, curr);
                }
            }
        }
    }
    return make_pair(nearest, currentPath);
}

int main ()
{
    CDelivery t0;
    map<string, list<string>> r;
    t0.addConn("Austin", "Berlin");
    t0.addConn("Chicago", "Berlin");
    t0.addConn("Berlin", "Dallas");
    t0.addConn("Dallas", "Essen");
    t0.addConn("Essen", "Austin");
    t0.addConn("Frankfurt", "Gyor");
    t0.addConn("Gyor", "Helsinki");
    t0.addConn("Helsinki", "Frankfurt");

    r = t0.serveCustomers(set<string>{"Berlin", "Gyor"}, set<string>{"Essen", "Helsinki"});
    assert ( r == (map<string, list<string>>{ {"Berlin", {"Essen", "Austin", "Berlin"}}, {"Gyor", {"Helsinki", "Frankfurt", "Gyor"}} }) );

    r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
    assert ( r == (map<string, list<string>>{ {"Austin", {"Austin"}}, {"Chicago", { }}, {"Gyor", { }} }) );
    t0.addConn("Chicago", "Helsinki");

    r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
    assert ( r == (map<string, list<string>>{ {"Austin", {"Austin"}}, {"Chicago", { }}, {"Gyor", { }} }) );
    t0.addConn("Berlin", "Chicago");

    r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
    assert ( r == (map<string, list<string>>{ {"Austin", {"Austin"}}, {"Chicago", {"Austin", "Berlin", "Chicago"}}, {"Gyor", {"Austin", "Berlin", "Chicago", "Helsinki", "Frankfurt", "Gyor"}} }) );
    t0.addConn("Essen", "Frankfurt");

    r = t0.serveCustomers(set<string>{"Austin", "Gyor", "Chicago"}, set<string>{"Austin", "Dallas"});
    assert ( r == (map<string, list<string>>{ {"Austin", {"Austin"}}, {"Chicago", {"Austin", "Berlin", "Chicago"}}, {"Gyor", {"Dallas", "Essen", "Frankfurt", "Gyor"}} }) );

    return EXIT_SUCCESS;
}