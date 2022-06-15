#include<iostream>
#include<string>
#include<cassert>
#include<sstream>
using namespace std;

class CTree{
public:
    CTree() = default;
    CTree(const CTree & src) {
        m_Root = nullptr;
        copyTree(src.m_Root);
    }
    CTree & operator = ( CTree & src) {

        if ( src == *this ) return *this;

        m_Root = nullptr;
        copyTree(src.m_First);
        return *this;
    };

    bool operator == ( const CTree& src ) {

        return cmpTree(m_Root, src.m_Root);
    }

    ~CTree(){
        auto curr = m_First;
        while ( curr != nullptr ) {
            auto tmp = curr;
            curr = curr->m_NextOrder;
            delete tmp;
        }
    }
    bool isSet(const string & key){
        auto it = m_Root;


        while ( it != nullptr ) {
            if ( it->m_Key == key ) return true;
            else if ( it->m_Key > key ) it = it->m_L;
            else if ( it->m_Key < key ) it = it->m_R;
        }
        return false;
    }
    bool insert(const string & key, const string & val){
        CNode * nodeToInsert = new CNode(key, val);

        if ( m_Root == nullptr ) {
            m_Root = nodeToInsert;
            m_First = nodeToInsert;
            m_Last = nodeToInsert;
            return true;
        }

        auto it = m_Root;

        while( it != nullptr ) {
            if ( it->m_Key > key ) {
                if ( it->m_L == nullptr ) {
                    it->m_L = nodeToInsert;
                    m_Last->m_NextOrder = nodeToInsert;
                    m_Last = nodeToInsert;
                    return true;
                }
                else it = it->m_L;
            }
            else if ( it->m_Key < key ) {
                if ( it->m_R == nullptr ) {
                    it->m_R = nodeToInsert;
                    m_Last->m_NextOrder = nodeToInsert;
                    m_Last = nodeToInsert;
                    return true;
                }
                else it = it->m_R;
            }
            else {
                return false;
            }
        }

        return false;
    }
    friend ostream & operator << (ostream & os, const CTree & src){
        auto curr = src.m_First;
        int count = 0;

        os << "{";
        while ( curr != nullptr ) {
            if ( count ) os << ", ";
            os << curr->m_Key << " => " << curr->m_Val;
            count ++;
            curr = curr->m_NextOrder;
        }
        os << "}";
        return os;
    }


protected:
    class CNode{
    public:
        CNode(const string & key, const string & val)
                :m_Key(key), m_Val(val) {}
        string m_Key, m_Val;
        CNode * m_L = nullptr, * m_R = nullptr;
        CNode * m_NextOrder = nullptr;
    };
    CNode * m_Root = nullptr;
    CNode * m_First = nullptr, * m_Last = nullptr;
    friend int main();

    void copyTree(CNode * src) {

        auto itSrc = src;
        while( itSrc != nullptr ) {
            insert(itSrc->m_Key, itSrc->m_Val);
            itSrc = itSrc->m_NextOrder;
        }
    }

    bool cmpTree( CNode * lhs, CNode * rhs ) {

        if ( lhs == nullptr && rhs == nullptr ) return true;
        if ( lhs != nullptr && rhs != nullptr ) {
            if ( lhs->m_Key == rhs->m_Key && lhs->m_Val == rhs->m_Val
              && cmpTree(lhs->m_L, rhs->m_L)
              && cmpTree(lhs->m_R, rhs->m_R) )
                return true;
            return false;
        }
        else
            return false;
    }
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


    CTree a = t, b = t;
    b = a;
    CTree c;

    assert( a == b );
    assert( !(c == b) );
    return 0;
}
