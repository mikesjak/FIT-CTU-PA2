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

private:
    // todo
    list<string> findRoute(const string& customer, const set<string> & depots) const;

    map < string, set<string>> stops;
};

CDelivery & CDelivery::addConn ( const string & from, const string & to )
{
    stops[from].insert(to);
    return *this;
}

map<string, list<string>> CDelivery::serveCustomers ( const set<string> & customers, const set<string> & depots ) const
{
    // todo
    map<string, list<string>> result;

    for ( const auto& customer : customers ) {
        list<string> res = findRoute(customer, depots);
        result.emplace(customer, res);
    }
    return result;
}

list<string> CDelivery::findRoute(const string& customer, const set<string> & depots ) const {
    list<string> path;
    int least = INT32_MAX;

    auto it = stops.find(customer);
    if ( it == stops.end() ) return path = {{customer}};

    for ( const auto & depo : depots ) {
        map <string, string> to_from;
        queue<pair<string, int>> q;

        q.emplace(depo, 0);
        to_from.emplace(depo, "");

        while ( !q.empty() ) {

            string curr = q.front().first;
            int currDist = q.front().second;
            q.pop();

            if ( currDist > least )
                break;

            if ( curr == customer ) {
                list<string> currPath;

                currPath.push_front(customer);

                auto it3 = to_from.find(curr)->second;
                while ( it3 != "" ) {
                    currPath.push_front(it3);
                    it3 = to_from[it3];
                }
                if ( currDist < least ) {
                    least = currDist;
                    path = currPath;
                }
                break;
            }

            auto it2 = stops.find(curr);
            if ( it2 == stops.end() )
                break;
            for ( const auto& c : it2->second ) {
                if ( to_from.find(c) == to_from.end() ) {
                    to_from.emplace(c, curr);
                    q.emplace(c, currDist+1);
                }
            }
        }
    }
    return path;
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
