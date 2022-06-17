#include<iostream>
#include<string>
#include<cassert>
#include<sstream>
using namespace std;

class CTree{
public:
    CTree() = default;
    CTree(const CTree & src) = delete;
    CTree & operator = (const CTree & src) = delete;
    ~CTree(){
        auto it = m_First;

        while ( it != nullptr ) {
            auto tmp = it;
            it = it->m_NextOrder;
            delete tmp;
        }
    }
    bool isSet(const string & key){

        auto it = m_First;

        while ( it != nullptr && key != it->m_Key ) {
            it = it->m_NextOrder;
        }
        if ( it == nullptr )
            return false;
        return true;

        /*
        if ( m_Root == nullptr ) return false;
        CNode * curr = m_Root;
        while ( true ) {
            if ( curr->m_Key == key ) return true;
            if ( curr->m_Key < key ) {
                if (curr->m_R == nullptr) return false;
                curr = curr->m_R;
            }
            else if ( curr->m_Key > key ) {
                if (curr->m_L == nullptr) return false;
                curr = curr->m_L;
            }
        }
        */

    }
    bool insert(const string & key, const string & val){

        CNode * tmp = new CNode(key, val);

        if ( m_Root == nullptr ) {
            m_Root  = tmp;
            m_First = tmp;
            m_Last  = tmp;
            return true;
        }

        auto curr = m_Root;

        while ( curr != nullptr ) {
            if ( curr->m_Key > tmp->m_Key ) {
                if ( curr->m_L == nullptr ) {
                    curr->m_L = tmp;
                    m_Last->m_NextOrder = tmp;
                    m_Last = tmp;
                    return true;
                }
                curr = curr->m_L;
            }
            else if ( curr->m_Key < tmp->m_Key ) {
                if ( curr->m_R == nullptr ) {
                    curr->m_R = tmp;
                    m_Last->m_NextOrder = tmp;
                    m_Last = tmp;
                    return true;
                }
                curr = curr->m_R;
            }
            else return false;
        }
        return false;
    }
    friend ostream & operator << (ostream & os, const CTree & src){
        auto it = src.m_First;
        int counter = 0;
        os << "{";
        while ( it != nullptr ) {
            if ( counter != 0 )
                os << ", ";
            os << it->m_Key << " => " << it->m_Val;
            it = it->m_NextOrder;
            counter++;
        }
        os << "}";
        return os;
    }
protected:
    class CNode{
    public:
        CNode(const string & key, const string & val)
                :m_Key(key),
                 m_Val(val) {}
        string m_Key,
               m_Val;
        CNode * m_L = nullptr,
              * m_R = nullptr;
        CNode * m_NextOrder = nullptr;
    };
    CNode * m_Root = nullptr;
    CNode * m_First = nullptr, * m_Last = nullptr;
    friend int main();
};

int main(void){
    CTree t;
    stringstream ss;
    ss << t;
    assert(ss.str() == "{}");
    ss.clear();
    ss.str("");
    assert(t.insert("PA1", "done"));
    assert(t.m_First == t.m_Last);
    assert(t.m_First->m_Key == "PA1");
    assert(!t.isSet("UOS"));
    assert(t.insert("PA2", "fail"));
    assert(t.insert("UOS", "funny"));

    ss << t;
    assert(ss.str() == "{PA1 => done, PA2 => fail, UOS => funny}");
    ss.clear();
    ss.str("");

    assert(t.m_Root->m_L== nullptr);
    assert(t.m_Last->m_Key == "UOS");
    assert(t.m_Root->m_R->m_Key == "PA2");
    assert(t.m_Root->m_R->m_L == nullptr);
    assert(t.m_Root->m_R->m_R->m_Key == "UOS");
    assert(t.m_First->m_NextOrder->m_NextOrder == t.m_Last);
    assert(t.isSet("PA2"));

    assert(t.insert("CAO", "lul"));
    assert(t.insert("LIN", "F"));
    assert(t.m_Root->m_L->m_Key == "CAO");
    assert(t.m_Root->m_L->m_L == nullptr);
    assert(t.m_Root->m_L->m_R->m_Key == "LIN");
    assert(t.m_Last == t.m_Root->m_L->m_R );
    assert(t.m_Root->m_L->m_R->m_L == nullptr);
    assert(t.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder = t.m_Root->m_L->m_R);
    assert(t.isSet("LIN"));

    ss << t;
    assert(ss.str() == "{PA1 => done, PA2 => fail, UOS => funny, CAO => lul, LIN => F}");
    ss.clear();
    ss.str("");

    assert(t.insert("SAP", "shit"));
    assert(t.m_Root->m_R->m_R->m_L->m_Key == "SAP");
    assert(t.m_Last == t.m_Root->m_R->m_R->m_L);


    ss << t;
    assert(ss.str() == "{PA1 => done, PA2 => fail, UOS => funny, CAO => lul, LIN => F, SAP => shit}");
    ss.clear();
    ss.str("");

    assert(!t.isSet("PA3"));
    assert(t.isSet("LIN"));
    assert(t.isSet("SAP"));

    return 0;
}
