#include <iostream>
#include <cassert>
#include <functional>
#include <sstream>
using namespace std;


class CHash
{
public:
    CHash ( size_t size ) : m_Size(size)
    {
        allocateTable(size);
    }

    CHash ( const CHash  & src ) {
        allocateTable(src.m_Size);
        copyMap(src);
    }

    ~CHash ( void ) {
        deleteMap();
    }

    CHash & operator = ( const CHash  & src ) {
        if ( this == &src )
            return *this;
        deleteMap();
        allocateTable(src.m_Size);
        copyMap(src);

        return *this;
    }

    bool Ins ( const string & key, const string & val ) {
        TItem * itemToAdd = new TItem(key, val);
        int hash = hashFn(key, m_Size);

        if ( m_FirstOrder == nullptr ) {
            m_FirstOrder = itemToAdd;
            m_LastOrder = itemToAdd;
            m_Table[hash] = itemToAdd;
            return true;
        }

        if ( m_Table[hash] == nullptr ) {
            m_Table[hash] = itemToAdd;
            m_LastOrder->m_NextOrder = itemToAdd;
            itemToAdd->m_PrevOrder = m_LastOrder;
            m_LastOrder = itemToAdd;
            return true;
        }

        auto it = m_Table[hash];
        if ( it->m_Key == key ) {
            delete itemToAdd;
            return false;
        }
        while ( it->m_NextHash != nullptr ) {
            if ( it->m_Key == key ) {
                delete itemToAdd;
                return false;
            }
            it = it->m_NextHash;
        }
        if ( it->m_Key == key ) {
            delete itemToAdd;
            return false;
        }
        itemToAdd->m_NextHash = m_Table[hash];
        m_Table[hash] = itemToAdd;
        m_LastOrder->m_NextOrder = itemToAdd;
        itemToAdd->m_PrevOrder = m_LastOrder;
        m_LastOrder = itemToAdd;
        return true;
    }

    bool Del ( const string & key ) {
        int hash = hashFn(key, m_Size);

       if ( m_Table[hash] == nullptr )
           return false;

       auto it = m_Table[hash];
       auto prev = it;
       while ( it != nullptr ) {
           if ( it->m_Key == key )
               break;
           prev = it;
           it = it->m_NextHash;
       }
       if ( it == nullptr ) return false;

       if ( it == m_FirstOrder )
           m_FirstOrder = m_FirstOrder->m_NextOrder;
       if ( it == m_LastOrder )
           m_LastOrder = m_LastOrder->m_PrevOrder;
       if ( it->m_PrevOrder )
           it->m_PrevOrder->m_NextOrder = it->m_NextOrder;
       if ( it->m_NextOrder )
           it->m_NextOrder->m_PrevOrder = it->m_PrevOrder;
       if ( it->m_NextHash && prev!=it )
           prev->m_NextHash = it->m_NextHash;
       if ( it == m_Table[hash] )
           m_Table[hash] = it->m_NextHash;
       delete it;
       return true;
    }

    bool IsSet ( const string & key ) const {
        int hash = hashFn(key, m_Size);

        if ( m_Table[hash] == nullptr ) return false;

        auto it = m_Table[hash];
        while ( it != nullptr ) {
            if ( it->m_Key == key )
                return true;
            it = it->m_NextHash;
        }
        return false;
    }

    template<typename func>
    void ForEach ( func f ) const {
        TItem * it = m_FirstOrder;
        while ( it != nullptr ) {
            f(it->m_Val);
            it = it->m_NextOrder;
        }
    }

private:
    struct TItem
    {
        TItem ( const string & key, const string & val )
                : m_Key ( key ),
                  m_Val ( val ),
                  m_NextHash ( NULL ),
                  m_NextOrder ( NULL ),
                  m_PrevOrder ( NULL )
        {}

        string m_Key;
        string m_Val;
        TItem * m_NextHash;
        TItem * m_NextOrder;
        TItem * m_PrevOrder;
    };

    TItem ** m_Table;
    size_t m_Size;
    TItem * m_FirstOrder;
    TItem * m_LastOrder;

    friend int main ( void );

    void copyMap ( const CHash& src ) {
        m_FirstOrder = m_LastOrder = nullptr;
        m_Size = src.m_Size;
        auto it = src.m_FirstOrder;
        while ( it != nullptr ) {
            Ins(it->m_Key, it->m_Val);
            it = it->m_NextOrder;
        }
    }
    void deleteMap () {
        auto it = m_FirstOrder;
        while ( it != nullptr ) {
            auto tmp = it;
            it = it->m_NextOrder;
            delete tmp;
        }
        delete[] m_Table;
    }
    void allocateTable ( const size_t size ) {
        m_Table = new TItem *[size];
        for ( size_t i = 0; i < size; i++ )
            m_Table[i] = nullptr;
    }

    size_t hashFn ( const string & x, size_t modulus ) const {
        size_t h = 5381;
        for ( string::size_type c : x )
            h += ( h << 5 ) + c;
        return h % modulus;
    }
};


int main ( void )
{
    CHash x ( 100 );

    assert ( sizeof ( CHash ) == sizeof ( CHash::TItem * ) * 3 + sizeof ( size_t ) );
    assert ( sizeof ( CHash::TItem ) == sizeof ( CHash::TItem * ) * 3 + sizeof ( string ) * 2 );
    assert ( x . Ins ( "peg", "foo" )
             && x . m_Table[1] -> m_Key == "peg"
             && x . m_Table[1] -> m_Val == "foo"
             && x . m_FirstOrder == x . m_Table[1]
             && x . m_FirstOrder -> m_NextOrder == NULL
             && x . m_LastOrder == x . m_Table[1]
             && x . m_LastOrder -> m_PrevOrder == NULL );
    assert ( x . Ins ( "bwc", "bar" )
             && x . m_Table[45] -> m_Key == "bwc"
             && x . m_Table[45] -> m_Val == "bar"
             && x . m_FirstOrder == x . m_Table[1]
             && x . m_FirstOrder -> m_NextOrder == x . m_Table[45]
             && x . m_FirstOrder -> m_NextOrder -> m_NextOrder == NULL
             && x . m_LastOrder == x . m_Table[45]
             && x . m_LastOrder -> m_PrevOrder == x . m_Table[1]
             && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder == NULL );
    assert ( x . Ins ( "jdr", "test" )
             && x . m_Table[45] -> m_Key == "jdr"
             && x . m_Table[45] -> m_Val == "test"
             && x . m_Table[45] -> m_NextHash -> m_Key == "bwc"
             && x . m_Table[45] -> m_NextHash -> m_Val == "bar"
             && x . m_FirstOrder == x . m_Table[1]
             && x . m_FirstOrder -> m_NextOrder == x . m_Table[45] -> m_NextHash
             && x . m_FirstOrder -> m_NextOrder -> m_NextOrder == x . m_Table[45]
             && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == NULL
             && x . m_LastOrder == x . m_Table[45]
             && x . m_LastOrder -> m_PrevOrder == x . m_Table[45] -> m_NextHash
             && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[1]
             && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == NULL );
    assert ( x . Ins ( "peg", "something" ) == false );
    assert ( x . Ins ( "peg", "foo" ) == false );
    assert ( x . Ins ( "bwc", "test" ) == false );
    assert ( x . Ins ( "jdr", "mudr" ) == false );
    assert ( x . Ins ( "asa", "butterfield" )
             && x . m_Table[22] -> m_Key == "asa"
             && x . m_Table[22] -> m_Val == "butterfield"
             && x . m_FirstOrder == x . m_Table[1]
             && x . m_FirstOrder -> m_NextOrder == x . m_Table[45] -> m_NextHash
             && x . m_FirstOrder -> m_NextOrder -> m_NextOrder == x . m_Table[45]
             && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22]
             && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == NULL
             && x . m_LastOrder == x . m_Table[22]
             && x . m_LastOrder -> m_PrevOrder == x . m_Table[45]
             && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[45] -> m_NextHash
             && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[1]
             && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == NULL );
    assert ( x . IsSet ( "peg" ) );
    assert ( x . IsSet ( "asa" ) );
    assert ( x . IsSet ( "bwc" ) );
    assert ( x . IsSet ( "jdr" ) );
    assert ( x . IsSet ( "whf" ) == false );
    assert ( x . IsSet ( "xns" ) == false );
    assert ( x . IsSet ( "abc" ) == false );
    assert ( x . IsSet ( "ooj" ) == false );
    ostringstream oss;

    x . ForEach ( [&oss] ( const string & val ){
        oss << val << endl;
    } );

    for ( int i = 0; i < 100; i++ ) {
        auto it = x.m_Table[i];
        if ( it == nullptr ) continue;
        while ( it != nullptr ) {
            cout << "[" << i << "] " << it->m_Key << ", " << it->m_Val << endl;
            it = it->m_NextHash;
        }
    }

    assert ( oss . str () == "foo\nbar\ntest\nbutterfield\n" );
    CHash n ( x );
    assert ( n . m_Table[1] -> m_Key == "peg"
             && n . m_Table[1] -> m_Val == "foo"
             && n . m_Table[45] -> m_Key == "jdr"
             && n . m_Table[45] -> m_Val == "test"
             && n . m_Table[45] -> m_NextHash -> m_Key == "bwc"
             && n . m_Table[45] -> m_NextHash -> m_Val == "bar"
             && n . m_Table[22] -> m_Key == "asa"
             && n . m_Table[22] -> m_Val == "butterfield"
             && n . m_FirstOrder == n . m_Table[1]
             && n . m_FirstOrder -> m_NextOrder == n . m_Table[45] -> m_NextHash
             && n . m_FirstOrder -> m_NextOrder -> m_NextOrder == n . m_Table[45]
             && n . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == n . m_Table[22]
             && n . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == NULL
             && n . m_LastOrder == n . m_Table[22]
             && n . m_LastOrder -> m_PrevOrder == n . m_Table[45]
             && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder == n . m_Table[45] -> m_NextHash
             && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == n . m_Table[1]
             && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == NULL );
    assert ( n . Del ( "jdr" )
             && n . m_Table[45] -> m_Key == "bwc"
             && n . m_Table[45] -> m_Val == "bar"
             && n . m_FirstOrder == n . m_Table[1]
             && n . m_FirstOrder -> m_NextOrder == n . m_Table[45]
             && n . m_FirstOrder -> m_NextOrder -> m_NextOrder == n . m_Table[22]
             && n . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == NULL
             && n . m_LastOrder == n . m_Table[22]
             && n . m_LastOrder -> m_PrevOrder == n . m_Table[45]
             && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder == n . m_Table[1]
             && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == NULL );
    assert ( n . IsSet ( "jdr" ) == false );
    assert ( x . m_Table[1] -> m_Key == "peg"
             && x . m_Table[1] -> m_Val == "foo"
             && x . m_Table[45] -> m_Key == "jdr"
             && x . m_Table[45] -> m_Val == "test"
             && x . m_Table[45] -> m_NextHash -> m_Key == "bwc"
             && x . m_Table[45] -> m_NextHash -> m_Val == "bar"
             && x . m_Table[22] -> m_Key == "asa"
             && x . m_Table[22] -> m_Val == "butterfield"
             && x . m_FirstOrder == x . m_Table[1]
             && x . m_FirstOrder -> m_NextOrder == x . m_Table[45] -> m_NextHash
             && x . m_FirstOrder -> m_NextOrder -> m_NextOrder == x . m_Table[45]
             && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22]
             && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == NULL
             && x . m_LastOrder == x . m_Table[22]
             && x . m_LastOrder -> m_PrevOrder == x . m_Table[45]
             && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[45] -> m_NextHash
             && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[1]
             && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == NULL );
    assert ( n . Del ( "404" ) == false );
    assert ( n . Del ( "system32" ) == false );
    assert ( n . Del ( "root" ) == false );
    n = n;
    assert ( n . Ins ( "xns", "test" )
             && n . m_Table[22] -> m_Key == "xns"
             && n . m_Table[22] -> m_Val == "test"
             && n . m_Table[22] -> m_NextHash -> m_Key == "asa"
             && n . m_Table[22] -> m_NextHash -> m_Val == "butterfield"
             && n . m_FirstOrder == n . m_Table[1]
             && n . m_FirstOrder -> m_NextOrder == n . m_Table[45]
             && n . m_FirstOrder -> m_NextOrder -> m_NextOrder == n . m_Table[22] -> m_NextHash
             && n . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == n . m_Table[22]
             && n . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == NULL
             && n . m_LastOrder == n . m_Table[22]
             && n . m_LastOrder -> m_PrevOrder == n . m_Table[22] -> m_NextHash
             && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder == n . m_Table[45]
             && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == n . m_Table[1]
             && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == NULL );
    x = n;
    assert ( x . m_Table[22] -> m_Key == "xns"
             && x . m_Table[22] -> m_Val == "test"
             && x . m_Table[22] -> m_NextHash -> m_Key == "asa"
             && x . m_Table[22] -> m_NextHash -> m_Val == "butterfield"
             && x . m_FirstOrder == x . m_Table[1]
             && x . m_FirstOrder -> m_NextOrder == x . m_Table[45]
             && x . m_FirstOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22] -> m_NextHash
             && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22]
             && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == NULL
             && x . m_LastOrder == x . m_Table[22]
             && x . m_LastOrder -> m_PrevOrder == x . m_Table[22] -> m_NextHash
             && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[45]
             && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[1]
             && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == NULL );
    assert ( x . Ins ( "whf", "FIT" )
             && x . m_Table[22] -> m_Key == "whf"
             && x . m_Table[22] -> m_Val == "FIT"
             && x . m_Table[22] -> m_NextHash -> m_Key == "xns"
             && x . m_Table[22] -> m_NextHash -> m_Val == "test"
             && x . m_Table[22] -> m_NextHash -> m_NextHash -> m_Key == "asa"
             && x . m_Table[22] -> m_NextHash -> m_NextHash -> m_Val == "butterfield"
             && x . m_FirstOrder == x . m_Table[1]
             && x . m_FirstOrder -> m_NextOrder == x . m_Table[45]
             && x . m_FirstOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22] -> m_NextHash -> m_NextHash
             && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22] -> m_NextHash
             && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22]
             && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == NULL
             && x . m_LastOrder == x . m_Table[22]
             && x . m_LastOrder -> m_PrevOrder == x . m_Table[22] -> m_NextHash
             && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[22] -> m_NextHash -> m_NextHash
             && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[45]
             && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[1]
             && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == NULL );
    assert ( n . m_Table[22] -> m_Key == "xns"
             && n . m_Table[22] -> m_Val == "test"
             && n . m_Table[22] -> m_NextHash -> m_Key == "asa"
             && n . m_Table[22] -> m_NextHash -> m_Val == "butterfield"
             && n . m_FirstOrder == n . m_Table[1]
             && n . m_FirstOrder -> m_NextOrder == n . m_Table[45]
             && n . m_FirstOrder -> m_NextOrder -> m_NextOrder == n . m_Table[22] -> m_NextHash
             && n . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == n . m_Table[22]
             && n . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == NULL
             && n . m_LastOrder == n . m_Table[22]
             && n . m_LastOrder -> m_PrevOrder == n . m_Table[22] -> m_NextHash
             && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder == n . m_Table[45]
             && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == n . m_Table[1]
             && n . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == NULL );
    assert ( x . Del ( "xns" )
             && x . m_Table[22] -> m_Key == "whf"
             && x . m_Table[22] -> m_Val == "FIT"
             && x . m_Table[22] -> m_NextHash -> m_Key == "asa"
             && x . m_Table[22] -> m_NextHash -> m_Val == "butterfield"
             && x . m_FirstOrder == x . m_Table[1]
             && x . m_FirstOrder -> m_NextOrder == x . m_Table[45]
             && x . m_FirstOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22] -> m_NextHash
             && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22]
             && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == NULL
             && x . m_LastOrder == x . m_Table[22]
             && x . m_LastOrder -> m_PrevOrder == x . m_Table[22] -> m_NextHash
             && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[45]
             && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[1]
             && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == NULL );
    assert ( x . Del ( "whf" )
             && x . m_Table[22] -> m_Key == "asa"
             && x . m_Table[22] -> m_Val == "butterfield"
             && x . m_FirstOrder == x . m_Table[1]
             && x . m_FirstOrder -> m_NextOrder == x . m_Table[45]
             && x . m_FirstOrder -> m_NextOrder -> m_NextOrder == x . m_Table[22]
             && x . m_FirstOrder -> m_NextOrder -> m_NextOrder -> m_NextOrder == NULL
             && x . m_LastOrder == x . m_Table[22]
             && x . m_LastOrder -> m_PrevOrder == x . m_Table[45]
             && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder == x . m_Table[1]
             && x . m_LastOrder -> m_PrevOrder -> m_PrevOrder -> m_PrevOrder == NULL );
    assert ( x . IsSet ( "asa" ) );
    assert ( x . IsSet ( "xns" ) == false );
    assert ( x . IsSet ( "whf" ) == false );
    assert ( x . Del ( "asa" )
             && x . m_Table[22] == NULL );
    assert ( x . Del ( "peg" )
             && x . m_Table[1] == NULL );
    assert ( x . Del ( "bwc" )
             && x . m_Table[45] == NULL );
    assert ( x . m_FirstOrder == NULL
             && x . m_LastOrder == NULL );
    return 0;
}