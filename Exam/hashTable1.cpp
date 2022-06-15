#include <string>
#include <iostream>
#include <cassert>

using namespace std;

struct TItem {
    TItem(string key, string val, TItem* nextHash,TItem* nextOrd, TItem * prevOrd)
    :m_Key(key),
    m_Val(val),
    m_NextHash(nextHash),
    m_NextOrder(nextOrd),
    m_PrevOrder(prevOrd)
    {}

    string m_Key, m_Val;
    TItem * m_NextHash, * m_NextOrder, * m_PrevOrder;
};

class CHash{
public:
    // Constructor
    CHash (int m)
    :m_Table(nullptr),
     m_Size(m),
     m_FirstOrder(nullptr),
     m_LastOrder(nullptr)
     {
        m_Table = new TItem* [m];
        for(int i=0; i<m;i++)
            m_Table[i] = nullptr;
    }

    // Destructor
    ~CHash() {
        auto it = m_FirstOrder;
        while ( it != nullptr ) {
            TItem* tmp = it;
            TItem* tmp2 = it;
            it = it->m_NextOrder;
            delete tmp;
        }
        delete [] m_Table;
    }

    // Finds out if given key exists in hash table
    bool IsSet(string key) {
        auto hash = hashFn(key);
        TItem * it = m_Table[hash];

        while ( it != nullptr && key != it->m_Key ) {
            it = it->m_NextHash;
        }
        if ( it == nullptr ) return false;
        return true;
        
        /*
        TItem * it = m_FirstOrder;
        while ( it != nullptr ) {
            if ( it->m_key == key ) return true; 
        } 
        return false;
        */
    }

    // Both not tested
    CHash(CHash &) = delete;
    CHash & operator = (CHash &) = delete;

    // Adds these 2 parameters to hash table
    bool Ins(string key, string val) {

        auto hash = hashFn(key);
        TItem * tmp = m_Table[hash];

        // heading to first free space
        while ( tmp != nullptr ) {
            if ( tmp->m_Key == key ) return false;
            tmp = tmp->m_NextHash;
        }

        TItem * item = new TItem(key, val, m_Table[hash], nullptr, m_LastOrder );

        if ( m_LastOrder != nullptr )
            m_LastOrder->m_NextOrder = item;
        m_LastOrder = item;

        // Hashmap is empty
        if ( !m_FirstOrder ) {
            m_FirstOrder = item;
        }

        m_Table[hash] = item;
        return true;
    }

    // Goes through whole table and calls given function for each CItem
    template <typename func>
    void ForEach(func f) {
        auto it = m_FirstOrder;
        while ( it != nullptr ) {
            f(it);
            auto it2 = it->m_NextHash;
            it = it->m_NextOrder;
        }
    }
private:
    TItem **        m_Table;
    unsigned int    m_Size;
    TItem *         m_FirstOrder, * m_LastOrder;

    // Hash function
    unsigned int hashFn(string & str){
        std::hash<std::string> hash_fn;
        return hash_fn(str)%m_Size;
    }
};

int main(int argc, char** argv) {
    CHash hashtable(100);
    hashtable.Ins("h1","car");
    hashtable.Ins("h1","phone");
    hashtable.Ins("h2","field");
    hashtable.Ins("h3","house");
    hashtable.Ins("h4","tree");
    hashtable.Ins("g9", "Dengue");

    hashtable.ForEach([](TItem * it ){
        cout<<it->m_Key<<" - "<<it->m_Val<<endl;
    });

    assert( hashtable.IsSet("h1") );
    assert( !hashtable.IsSet("h6") );

    return 0;
}
