#include <string>
#include <iostream>
#include <cassert>

using namespace std;

struct TItem
{
    TItem(string key, string val, TItem *nextHash, TItem *nextOrd, TItem *prevOrd)
            : m_Key(key), m_Val(val), m_NextHash(nextHash), m_NextOrder(nextOrd), m_PrevOrder(prevOrd) {}

    string m_Key, m_Val;
    TItem *m_NextHash, *m_NextOrder, *m_PrevOrder;
};

class CHash
{
public:
    CHash(int n) : m_Table(NULL), m_Size(n), m_FirstOrder(NULL), m_LastOrder(NULL)
    {
        m_Table = new TItem * [n];
        for ( int i = 0; i < n; i++ )
            m_Table[i] = nullptr;
    }
    ~CHash()
    {
        auto it = m_FirstOrder;
        while ( it ) {
            auto tmp = it;
            it = it->m_NextOrder;
            delete tmp;
        }
        delete []m_Table;
    }
    CHash(CHash &) = delete;
    CHash &operator=(CHash &) = delete;

    bool Ins(string key, string val) {
        int hash = hashFn(key);
        TItem * itemToAdd = new TItem(key, val, nullptr, nullptr, nullptr);

        if ( m_FirstOrder == nullptr ) {
            m_FirstOrder = itemToAdd;
            m_LastOrder = itemToAdd;
            m_Table[hash] = itemToAdd;
            return true;
        }

        if ( m_Table[hash] == nullptr ) {
            m_LastOrder->m_NextOrder = itemToAdd;
            itemToAdd->m_PrevOrder = m_LastOrder;
            m_LastOrder = itemToAdd;
            m_Table[hash] = itemToAdd;
            return true;
        }

        auto it = m_Table[hash];

        while ( it->m_NextHash ) {
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
        m_LastOrder->m_NextOrder = itemToAdd;
        itemToAdd->m_PrevOrder = m_LastOrder;
        m_LastOrder = itemToAdd;
        m_Table[hash] = itemToAdd;\
        return true;
    }
    bool Del(string key) {
        int hash = hashFn(key);

        if ( m_Table[hash] == nullptr )
            return false;

        TItem * it = m_Table[hash];
        auto prev = it;

        while ( it ) {
            if ( it->m_Key == key )
                break;
            prev = it;
            it = it->m_NextHash;
        }

        if ( it == nullptr )
            return false;

        if ( it == m_FirstOrder )
            m_FirstOrder = m_FirstOrder->m_NextOrder;
        if ( it == m_LastOrder )
            m_LastOrder = m_LastOrder->m_PrevOrder;
        if ( it == m_Table[hash] )
            m_Table[hash] = it->m_NextHash;
        if ( prev != it )
            prev->m_NextHash = it->m_NextHash;
        if ( it->m_PrevOrder )
            it->m_PrevOrder->m_NextOrder = it->m_NextOrder;
        if ( it->m_NextOrder )
            it->m_NextOrder->m_PrevOrder = it->m_PrevOrder;

        it->m_NextHash = it->m_PrevOrder = it->m_NextOrder = nullptr;
        delete it;
        return true;
    }
    bool IsSet(string key) {
        int hash = hashFn(key);

        if ( m_Table[hash] == nullptr )
            return false;

        auto it = m_Table[hash];

        while ( it != nullptr ) {
            if ( it->m_Key == key )
                return true;
            it = it->m_NextHash;
        }
        return false;
    }
    void rehash(size_t newSize) {

        if ( newSize == m_Size )
            return;
        // erasing table
        for ( int i = 0; i < m_Size; i++ ) {

            auto it = m_Table[i];
            while ( it ) {
                it->m_NextHash = nullptr;
                it = it->m_NextOrder;
            }
            m_Table[i] = nullptr;
        }

        delete [] m_Table;
        m_Table = new TItem * [newSize];
        m_Size = newSize;

        for ( int i = 0; i < m_Size; i++ ) {
            m_Table[i] = nullptr;
        }

        auto it = m_FirstOrder;

        while ( it ) {
            int hash = hashFn(it->m_Key);

            if ( m_Table[hash] == nullptr ) {
                m_Table[hash] = it;
                it = it->m_NextOrder;
                continue;
            }
            auto it2 = m_Table[hash];
            while (it2->m_NextHash) {
                it2 = it2->m_NextHash;
            }
            it2->m_NextHash = it;
            it = it->m_NextOrder;
        }
    }

    template <typename func>
    void ForEach(func f)
    {
        TItem * it = m_FirstOrder;
        while( it ) {
            f(it);
            it = it->m_NextOrder;
        }
    }
    void print() {
        for ( int i = 0; i < m_Size; i++ ) {
            auto it = m_Table[i];
            if ( it == nullptr ) continue;
            while ( it != nullptr ) {
                cout << "[" << i << "] " << it->m_Key << ", " << it->m_Val << endl;
                it = it->m_NextHash;
            }
        }
    }

private:
    TItem **m_Table;
    unsigned int m_Size;
    TItem *m_FirstOrder, *m_LastOrder;
    unsigned int hashFn(string &str)
    {
        std::hash<std::string> hash_fn;
        return hash_fn(str) % m_Size;
    }
};

int main(int argc, char **argv)
{
    CHash hashtable(100);
    assert( hashtable.Ins("h1", "car") );
    assert(!hashtable.Ins("h1", "phone") );
    assert( hashtable.Ins("h2", "field") );
    assert( hashtable.Ins("h3", "house") );
    assert( hashtable.Ins("h4", "tree") );
    assert (hashtable.Ins("g9", "john") );

    hashtable.ForEach([](TItem *it) {
        cout << it->m_Key << " - " << it->m_Val << endl;
    } );

    hashtable.print();

    hashtable.rehash(5);

    hashtable.print();

    assert ( hashtable.Del("h2") );
    assert (!hashtable.Del("h2") );
    assert ( hashtable.Del("h1") );
    assert ( hashtable.Del("g9") );

    hashtable.rehash(101);

    hashtable.ForEach([](TItem *it) {
        cout << it->m_Key << " - " << it->m_Val << endl;
    } );

    return 0;
}