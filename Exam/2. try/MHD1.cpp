#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <list>
#include <queue>
#include <cassert>
using namespace std;

class CMHD
{
public:
    void Add ( istringstream & is ) {

        string st1, st2;
        getline( is, st1 );
        while( getline(is, st2) ) {
            if ( lines[st1].size() >= 1 )
                transits.insert(st1);
            lines[st1].push_back(currLane);
            stationsInLine[currLane].insert(st1);
            st1 = st2;
        }
        if ( lines[st1].size() >= 1 )
            transits.insert(st1);
        lines[st1].push_back(currLane);
        stationsInLine[currLane].insert(st1);
        currLane++;
    }

    set < string > Dest ( const string & from, int maxCost ) const {
        set < string > result;
        queue<pair<string, int>> q;
        set < int > visitedLines;
        set <string> visitedStations;

        auto it = lines.find(from);
        if ( it == lines.end() )
            return result = {{from}};
        for ( const auto& l : it->second ) {
            visitedLines.insert(l);
            for ( const auto& st : stationsInLine.find(l)->second ) {
                result.insert(st);
                if ( transits.find(st) != transits.end() )
                    q.emplace(st, 0);
                visitedStations.insert(st);
            }
        }

        while ( !q.empty() ) {

            string curr = q.front().first;
            int cost = q.front().second;
            q.pop();
            visitedStations.insert(curr);

            if ( cost > maxCost )
                return result;
            result.insert(curr);

            for ( const auto& l : lines.find(curr)->second ) {
                if ( visitedLines.count(l) == 1 )
                    continue;
                for ( const auto& st : stationsInLine.find(l)->second ) {
                    if ( visitedStations.count(st) != 1 )
                        q.emplace(st, cost+1);
                }
            }
        }
        return result;
    }
    //===============================================================================
private:
    int currLane = 0;
    map < string, vector <int> > lines;
    map < int, set <string> > stationsInLine;
    set < string > transits;
};

int main ( void ) {
    CMHD city;
    istringstream iss;
    iss.clear();

    iss . str ( "A\nB\nC\nD\nE\n" );
    city . Add ( iss );
    iss . clear();

    iss . str ( "B\nC\nF\nH\n" );
    city . Add ( iss );
    iss . clear();

    iss . str ( "F\nG\nI\nJ\nK\nN\n" );
    city . Add ( iss );
    iss . clear();

    iss . str ( "H\nL\n" );
    city . Add ( iss );
    iss . clear();

    iss . str ( "L\nM\nN\nO\n" );
    city . Add ( iss );
    iss . clear();

    iss . str ( "P\nQ\nR\nN\nS" );
    city . Add ( iss );
    iss . clear();

    assert ( city . Dest ( "S", 0 ) == set < string > ( {"S", "N", "R", "Q", "P"} ) );

    auto s1 = city . Dest ( "S", 1 );
    assert ( s1 == set < string > ( { "S", "N", "R", "Q", "P",
                                                          "O", "M", "L",
                                                          "K", "J", "I", "G", "F" } ) );

    assert ( city . Dest ( "N", 0 ) == set < string > ( { "S", "N", "R", "Q", "P",
                                                          "O", "M", "L",
                                                          "K", "J", "I", "G", "F" } ) );

    assert ( city . Dest ( "N", 1 ) == set < string > ( { "S", "N", "R", "Q", "P",
                                                          "O", "M", "L",
                                                          "K", "J", "I", "G", "F",
                                                          "H", "F", "C", "B" } ) );

    assert ( city . Dest ( "N", 2 ) == set < string > ( { "S", "N", "R", "Q", "P",
                                                          "O", "M", "L",
                                                          "K", "J", "I", "G", "F",
                                                          "H", "F", "C", "B",
                                                          "A", "D", "E" } ) );

    assert ( city . Dest ( "unknown", 0 ) == set < string > ( { "unknown" } ) );
    assert ( city . Dest ( "unknown", 1 ) == set < string > ( { "unknown" } ) );
    assert ( city . Dest ( "unknown", 2 ) == set < string > ( { "unknown" }) );

    return 0;
}
