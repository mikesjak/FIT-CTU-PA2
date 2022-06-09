#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CDate
{
  public:
    CDate ( int y, int m, int d ) 
    : year( y ),
      month( m ),
      day( d )
    {}

    bool operator < (const CDate & date1 ) const {
      if ( date1.year > this->year )
        return true;
      else if ( date1.year < this->year )
        return false;
      else if ( date1.month > this->month )
        return true;
      else if ( date1.month < this->month )
        return false;
      else if ( date1.day > this->day )
        return true;
      return false;
    }

    friend ostream& operator << ( ostream& os, const CDate& x ) {
        return os << x.year << '-' << x.month << '-' << x.day;
    } 

    int year = 0;
    int month = 0;
    int day = 0;
};
class CSupermarket
{
  public:
    CSupermarket() = default;

    CSupermarket& store ( const string name, const CDate expiryDate, const int count ) {
      warehouse[name][expiryDate] = warehouse[name][expiryDate] + count;
      return *this;
    }

    void sell ( list<pair<string,int>> & shoppingList ) { 

      cout << "SELLING" << '\n';
      vector<std::map<string, map<CDate, int>>::iterator> iterators;
      bool found;


      for ( auto it_in = shoppingList.begin(); it_in != shoppingList.end(); it_in++ ) {
        
        auto it = warehouse.find(it_in->first);
        int found_couter = 0;

        if ( it == warehouse.end() ) {
          cout << "Trying to find " << it_in->first << '\n';

          for ( auto it1 = warehouse.begin(); it1 != warehouse.end(); it1++ ){
            found = cmpStrings( it_in->first, it1->first );
            if ( found ) {
              found_couter++;
              cout << "Found " << it1->first << '\n';
              it = it1;
            }
          }
          if ( found_couter != 1 ){
            it = warehouse.end();
          }
        }
        iterators.push_back(it);
      }
      
      auto shIter = shoppingList.begin();
      vector<string> namesToDel;

      for ( int i = 0; i < int(iterators.size()); i++ ) {
        
        if ( iterators[i] == warehouse.end() ) {
          shIter++;
          continue;
        }
        
        auto it = iterators[i];
        cout << iterators[i]->first << '\n';
        int sum = shIter->second;
        string name;
        
        for ( auto it_in = it->second.begin(); it_in != it->second.end(); it_in ++ ){
          int stored = it_in->second;
          name = shIter->first;
          if ( sum == stored ) {
            shIter->second = 0;
            it_in->second = 0;
            break;
          }
          if ( sum > stored ) {
            shIter->second -= it_in->second;
            it_in->second = 0;
          }
          if ( sum < stored ) {
            it_in->second -= shIter->second;
            shIter->second = 0;
            break;
          }
          if (shIter->second == 0){
            break;
          }
          sum = shIter->second;
        }

        auto it_del = --it->second.end();

        if (it_del->second == 0) {
          namesToDel.push_back(name);
        }
        shIter++;
      }

      for ( auto i = 0; i < int(namesToDel.size()); i++ ) {
        warehouse.erase(namesToDel[i]);
      }

      for ( auto it =  shoppingList.begin(); it != shoppingList.end(); ){
        if ( it->second == 0 ) {
          it = shoppingList.erase(it);
        }
        else {
          ++it;
        }
      }

      cout << "LIST" << '\n';
      for (  auto it_in = shoppingList.begin(); it_in != shoppingList.end();  it_in++) {
        cout << it_in->first << " - " << it_in->second << "\n";        
      }
    }

    list<pair<string,int>> expired ( CDate date ) const {
      cout << "Searching for products expiring before " << date << '\n';
      list<pair<string,int>> array;
      multimap <int, string> map;
      int sum = 0;

      for ( auto it = warehouse.begin(); it != warehouse.end(); it++ ){
        auto itr = (it->second).lower_bound(date);
        cout << it->first << endl;
        for ( auto it_in = (it->second).begin(); it_in != itr; it_in++ ){
          cout << it_in->first << " | " << it_in->second << '\n';
          sum += it_in->second;
        }
        map.emplace( make_pair( sum, it->first) );
        sum = 0;
      }
      
      // Printing multimap
      cout << "MULTIMAP:" << endl;
      for ( auto it=map.begin(); it != map.end(); it++ ) {
        cout << (*it).first << " | " << (*it).second << '\n';
      } 

      for ( auto it = map.rbegin(); it != map.rend(); it++ ) {
        if (it->first != 0)
          array.emplace_back( it->second, it->first );
      }

      // Printing list
      cout << "LIST:" << endl;
      for ( auto it=array.begin(); it != array.end(); it++ ) {
        cout << (*it).first << " | " << (*it).second << '\n';
      }

      return array;
    }

  private:

    map < string, map < CDate, int > > warehouse;

    static bool cmpStrings ( string a, string b ) {

      int miss = 0;
      cout << "Comparing " << a << " - " << b << '\n';
      if ( a.size() != b.size() ) {
        cout << "Not found \n";
        return false;
      }
      else {
        for ( auto i = 0; i <= int(a.size()); i++ ) {
          if ( a[i] != b[i] )
            miss++;
        }
        cout << "Missed found " << miss << '\n';
        if (miss == 1)
          return true;
        else {
          cout << "Not found \n";
          return false;
        }
      }
      cout << "Not found \n";
      return false;
    }
};
#ifndef __PROGTEST__
int main ( void )
{
  CSupermarket s;
  s . store ( "bread", CDate ( 2016, 4, 30 ), 100 )
    . store ( "butter", CDate ( 2016, 5, 10 ), 10 )
    . store ( "beer", CDate ( 2016, 8, 10 ), 50 )
    . store ( "bread", CDate ( 2016, 4, 25 ), 100 )
    . store ( "okey", CDate ( 2016, 7, 18 ), 5 );

  list<pair<string,int> > l0 = s . expired ( CDate ( 2018, 4, 30 ) );
  assert ( l0 . size () == 4 );
  assert ( ( l0 == list<pair<string,int> > { { "bread", 200 }, { "beer", 50 }, { "butter", 10 }, { "okey", 5 } } ) );

  list<pair<string,int> > l1 { { "bread", 2 }, { "Coke", 5 }, { "butter", 20 } };
  s . sell ( l1 );
  assert ( l1 . size () == 2 );
  assert ( ( l1 == list<pair<string,int> > { { "Coke", 5 }, { "butter", 10 } } ) );

  list<pair<string,int> > l2 = s . expired ( CDate ( 2016, 4, 30 ) );
  assert ( l2 . size () == 1 );
  assert ( ( l2 == list<pair<string,int> > { { "bread", 98 } } ) );

  list<pair<string,int> > l3 = s . expired ( CDate ( 2016, 5, 20 ) );
  assert ( l3 . size () == 1 );
  assert ( ( l3 == list<pair<string,int> > { { "bread", 198 } } ) );

  list<pair<string,int> > l4 { { "bread", 105 } };
  s . sell ( l4 );
  assert ( l4 . size () == 0 );
  assert ( ( l4 == list<pair<string,int> > {  } ) );

  list<pair<string,int> > l5 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l5 . size () == 3 );
  assert ( ( l5 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 } } ) );

  s . store ( "Coke", CDate ( 2016, 12, 31 ), 10 );

  list<pair<string,int> > l6 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 }, { "Cokes", 1 } };
  s . sell ( l6 );
  assert ( l6 . size () == 3 );
  assert ( ( l6 == list<pair<string,int> > { { "cake", 1 }, { "cuke", 1 }, { "Cokes", 1 } } ) );

  list<pair<string,int> > l7 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l7 . size () == 4 );
  assert ( ( l7 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 7 }, { "okey", 5 } } ) );

  s . store ( "cake", CDate ( 2016, 11, 1 ), 5 );

  list<pair<string,int> > l8 { { "Cake", 1 }, { "Coke", 1 }, { "cake", 1 }, { "coke", 1 }, { "cuke", 1 } };
  s . sell ( l8 );
  assert ( l8 . size () == 2 );
  assert ( ( l8 == list<pair<string,int> > { { "Cake", 1 }, { "coke", 1 } } ) );

  list<pair<string,int> > l9 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l9 . size () == 5 );
  assert ( ( l9 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 }, { "cake", 3 } } ) );

  list<pair<string,int> > l10 { { "cake", 15 }, { "Cake", 2 } };
  s . sell ( l10 );
  assert ( l10 . size () == 2 );
  assert ( ( l10 == list<pair<string,int> > { { "cake", 12 }, { "Cake", 2 } } ) );

  list<pair<string,int> > l11 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l11 . size () == 4 );
  assert ( ( l11 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "Coke", 6 }, { "okey", 5 } } ) );

  list<pair<string,int> > l12 { { "Cake", 4 } };
  s . sell ( l12 );
  assert ( l12 . size () == 0 );
  assert ( ( l12 == list<pair<string,int> > {  } ) );

  list<pair<string,int> > l13 = s . expired ( CDate ( 2017, 1, 1 ) );
  assert ( l13 . size () == 4 );
  assert ( ( l13 == list<pair<string,int> > { { "bread", 93 }, { "beer", 50 }, { "okey", 5 }, { "Coke", 2 } } ) );

  list<pair<string,int> > l14 { { "Beer", 20 }, { "Coke", 1 }, { "bear", 25 }, { "beer", 10 } };
  s . sell ( l14 );
  assert ( l14 . size () == 1 );
  assert ( ( l14 == list<pair<string,int> > { { "beer", 5 } } ) );

  s . store ( "ccccb", CDate ( 2019, 3, 11 ), 100 )
    . store ( "ccccd", CDate ( 2019, 6, 9 ), 100 )
    . store ( "dcccc", CDate ( 2019, 2, 14 ), 100 );

  list<pair<string,int> > l15 { { "ccccc", 10 } };
  s . sell ( l15 );
  assert ( l15 . size () == 1 );
  assert ( ( l15 == list<pair<string,int> > { { "ccccc", 10 } } ) );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
