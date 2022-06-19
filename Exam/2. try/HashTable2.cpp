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
    CHash(int m) : m_Table(NULL), m_Size(m), m_FirstOrder(NULL), m_LastOrder(NULL)
    {
        m_Table = new TItem *[m];
        for (int i = 0; i < m; i++)
            m_Table[i] = NULL;
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
        int hash= hashFn(key);
        TItem * itemToAdd = new TItem(key, val, nullptr, nullptr, nullptr);

        if ( m_FirstOrder == nullptr ) {
            m_FirstOrder = m_LastOrder = itemToAdd;
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
        else if ( m_Table[hash] != nullptr ) {
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
            it->m_NextHash = itemToAdd;
            m_LastOrder->m_NextOrder = itemToAdd;
            itemToAdd->m_PrevOrder = m_LastOrder;
            m_LastOrder = itemToAdd;
            return true;
        }
        delete itemToAdd;
        return false;
    }
    bool Del(string key) {
        int hash = hashFn(key);

        if ( m_Table[hash] == nullptr ) return false;

        TItem * it = m_Table[hash];
        TItem * prev = nullptr;
        while ( it->m_Key != key && it != nullptr ) {
            prev = it;
            if ( it->m_NextHash->m_Key == key )
                prev = it;
            it = it->m_NextHash;
        }
        if (it == nullptr ) return false;
        if ( it->m_PrevOrder )
            it->m_PrevOrder->m_NextOrder = it->m_NextOrder;
        if ( it->m_NextOrder )
            it->m_NextOrder->m_PrevOrder = it->m_PrevOrder;
        if ( prev )
            prev->m_NextHash = it->m_NextHash;
        if ( m_Table[hash] == it )
            m_Table[hash] = it->m_NextHash;
        delete it;
        return true;
    }
    bool IsSet(string key) {
        TItem *tmp = m_Table[hashFn(key)];
        while (tmp != NULL && tmp->m_Key != key)
            tmp = tmp->m_NextHash;
        if (tmp == NULL)
            return false;
        return true;
    }
    template <typename func>
    void ForEach(func f)
    {
        TItem *tmp = m_FirstOrder;
        while (tmp != NULL)
        {
            f(tmp);
            tmp = tmp->m_NextOrder;
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
    });

    assert ( hashtable.Del("h2") );
    assert (!hashtable.Del("h2") );
    assert ( hashtable.Del("h1") );
    assert ( hashtable.Del("g9") );

    return 0;
}