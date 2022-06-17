#include <string>
#include <iostream>
#include <cassert>
//#include <iterator>

using namespace std;

struct TItem
{
    TItem(string key, string val, TItem *nextHash, TItem *nextOrd, TItem *prevOrd)
            : m_Key(key), m_Val(val), m_NextHash(nextHash), m_NextOrder(nextOrd), m_PrevOrder(prevOrd) {}

    string m_Key, m_Val;
    TItem *m_NextHash, *m_NextOrder, *m_PrevOrder;

    friend bool operator == ( TItem a, TItem b ) {
        if ( a.m_Key != b.m_Key ) return false;
        if ( a.m_Val != b.m_Val ) return false;
        if ( a.m_NextHash != b.m_NextHash ) return false;
        if ( a.m_NextOrder != b.m_NextOrder ) return false;
        if ( a.m_PrevOrder != b.m_PrevOrder ) return false;
        return true;
    }
};

class CHash
{
public:
    CHash(int m)
            :m_Table(nullptr),
             m_Size(m),
             m_FirstOrder(nullptr),
             m_LastOrder(nullptr)
    {
        m_Table = new TItem *[m];
        for (int i = 0; i < m; i++)
            m_Table[i] = nullptr;
    }
    ~CHash()
    {
        for (int i = 0; i < m_Size; i++)
        {
            TItem *save, *it = m_Table[i];
            while (it)
            {
                save = it;
                it = it->m_NextHash;
                delete save;
            }
        }
        delete[] m_Table;
    }
    CHash(CHash &) = delete;
    CHash &operator=(CHash &) = delete;

    bool Ins(string key, string val) {
        int hash = hashFn(key);
        TItem * tmp = new TItem( key, val, nullptr, nullptr, m_LastOrder );

        if ( m_FirstOrder == nullptr )
            m_FirstOrder = tmp;

        if ( m_Table[hash] == nullptr ) {
            m_Table[hash] = tmp;
            if ( m_LastOrder != nullptr )
                m_LastOrder->m_NextOrder = tmp;
            m_LastOrder = tmp;
            return true;
        }

        auto it = m_Table[hash];

        while ( it != nullptr && key != it->m_Key ) {
            it = it->m_NextHash;
        }
        if ( it == nullptr ) {
            tmp->m_NextHash = m_Table[hash];
            m_Table[hash] = tmp;
            m_LastOrder->m_NextOrder = tmp;
            m_LastOrder = tmp;
            return true;
        }
        return false;
    }
    bool Del(string key) {
        int hash = hashFn(key);

        TItem * itemToDel,
                * itemBeforeITD;

        if ( m_Table[hash] == nullptr )
            return false;

        itemToDel = m_Table[hash];


        while ( itemToDel != nullptr && key != itemToDel->m_Key) {
            itemBeforeITD = itemToDel;
            itemToDel = itemToDel->m_NextHash;
        }
        if ( itemToDel == nullptr )
            return false;


        if ( itemToDel == m_FirstOrder ) {
            if ( itemToDel->m_NextOrder != nullptr ) {
                m_FirstOrder = itemToDel->m_NextOrder;
            }
            else {
                m_FirstOrder = nullptr;
            }
        }
        if ( itemToDel == m_LastOrder ) {
            if ( itemToDel->m_PrevOrder != nullptr ) {
                m_LastOrder = itemToDel->m_PrevOrder;
            }
            else {
                m_LastOrder = nullptr;
            }
        }

        if ( itemToDel->m_PrevOrder != nullptr ) {
            if ( itemToDel->m_NextOrder != nullptr ) {
                itemToDel->m_PrevOrder->m_NextOrder = itemToDel->m_NextOrder;
                itemToDel->m_NextOrder->m_PrevOrder = itemToDel->m_PrevOrder;
            }
            else {
                itemToDel->m_PrevOrder->m_NextOrder = nullptr;
            }
        }
        if ( itemToDel->m_NextHash != nullptr ) {
            if ( itemToDel == m_Table[hash] ) {
                m_Table[hash] = itemToDel->m_NextHash;
            }
            else {
                itemBeforeITD->m_NextHash = itemToDel->m_NextHash;
            }
        }
        if ( itemBeforeITD != nullptr ) {
            if ( itemToDel->m_NextHash != nullptr )
                itemBeforeITD->m_NextHash = itemToDel->m_NextHash;
            else
                itemBeforeITD->m_NextHash = nullptr;
        }
        delete itemToDel;
        return true;

    }
    bool IsSet(string key)
    {
        TItem *tmp = m_Table[hashFn(key)];
        while (tmp != nullptr && tmp->m_Key != key)
            tmp = tmp->m_NextHash;
        if (tmp == nullptr)
            return false;
        return true;
    }
    template <typename func>
    void ForEach(func f)
    {
        TItem *tmp = m_FirstOrder;
        while (tmp != nullptr)
        {
            f(tmp);
            tmp = tmp->m_NextOrder;
        }
    }

    class Iterator {
    public:
        Iterator( TItem * ptr ) : m_Ptr(ptr) {};

        TItem operator*() const {
            return *m_Ptr;
        }
        TItem * operator->() {
            return m_Ptr;
        }
        Iterator& operator++() {
            m_Ptr = m_Ptr->m_NextOrder; return *this;
        }
        Iterator operator++(int) {
            Iterator tmp = (*this);
            ++(*this);
            return tmp;
        }
        friend bool operator== (const Iterator& a, const Iterator& b) {
            return a.m_Ptr == b.m_Ptr;
        }
        friend bool operator!= (const Iterator& a, const Iterator& b) {
            return a.m_Ptr != b.m_Ptr;
        }

    private:
        TItem * m_Ptr;
    };

    Iterator begin() { return Iterator(m_FirstOrder); }
    Iterator end()   { return Iterator(m_LastOrder); }

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
    assert ( hashtable.Ins("h1", "car") );
    assert (!hashtable.Ins("h1", "phone") );
    assert ( hashtable.Ins("h2", "field") );
    assert ( hashtable.Ins("h3", "house") );
    assert ( hashtable.Ins("h4", "tree") );
    assert ( hashtable.Ins("g9", "pencil") );

    hashtable.ForEach([](TItem *it) {
        cout << it->m_Key << " - " << it->m_Val << endl;
    });

    assert(  hashtable.IsSet("h1") );
    assert( !hashtable.IsSet("h0") );

    // Iterator testing
    CHash::Iterator it = hashtable.begin();
    it++;

    assert (  hashtable.Del( "h1" ) );
    assert ( !hashtable.Del( "h0" ) );

    // Iterator testing
    CHash::Iterator it2 = hashtable.begin();
    assert( it == it2 );
    it++;
    assert( it != it2 );

    while ( it != nullptr ) {
        it2 = it;
        it++;
    }
    it = hashtable.end();
    assert( it == it2 );
    it++;
    assert ( it != it2 );
    it2++;
    assert( it == it2 );

    return 0;
}