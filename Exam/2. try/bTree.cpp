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
        auto tmp = it;

        while ( it != nullptr ) {
            tmp = it;
            it = it->m_NextOrder;
            delete tmp;
        }
    }
    bool remove(const string & key);
    bool isSet(const string & key);
    bool insert(const string & key, const string & val);
    friend ostream & operator << (ostream & os, const CTree & src);
protected:
    class CNode{
    public:
        CNode(const string & key, const string & val)
        :m_Key(key), m_Val(val) {}
        string m_Key, m_Val;
        CNode * m_L = nullptr, * m_R = nullptr;
        CNode * m_NextOrder = nullptr;
        CNode * m_PrevOrder = nullptr;
    };
    CNode * m_Root = nullptr;
    CNode * m_First = nullptr, * m_Last = nullptr;
    friend int main();
};

bool CTree::remove(const string & key) {

    CNode ** it = &m_Root;

    while ( *it != nullptr ) {
        CNode * nodeToDel = *it;
        if ( nodeToDel->m_Key == key ) {

            //if ( nodeToDel == m_First )
            //    m_First = m_First->m_NextOrder;
            if ( nodeToDel == m_Last )
                m_Last = m_Last->m_PrevOrder;
            if ( nodeToDel == m_First )
                m_Last = m_First->m_NextOrder;

            if ( nodeToDel->m_L != nullptr && nodeToDel->m_R != nullptr ) {
                it = &(*it)->m_R;
                while ( (*it)->m_L )
                    it =  &(*it)->m_L;
                nodeToDel->m_Key = (*it)->m_Key;
                nodeToDel->m_Val = (*it)->m_Val;

                if ( (*it)->m_PrevOrder && (*it)->m_PrevOrder != nodeToDel )
                    nodeToDel->m_PrevOrder = (*it)->m_PrevOrder;
                if ( (*it)->m_NextOrder && (*it)->m_NextOrder != nodeToDel )
                    nodeToDel->m_NextOrder = (*it)->m_NextOrder;
                nodeToDel = *it;
            }
            else {
                if ((*it)->m_PrevOrder)
                    (*it)->m_PrevOrder->m_NextOrder = (*it)->m_NextOrder;
                if ((*it)->m_NextOrder)
                    (*it)->m_NextOrder->m_PrevOrder = (*it)->m_NextOrder;
            }
            if ( nodeToDel->m_L != nullptr )
                *it = nodeToDel->m_L;
            else
                *it = nodeToDel->m_R;

            nodeToDel->m_NextOrder = nodeToDel->m_PrevOrder = nullptr;
            nodeToDel->m_L = nodeToDel->m_R = nullptr;
            delete nodeToDel;
            return true;
        }

        if ( nodeToDel->m_Key > key )
            it = &nodeToDel->m_L;
        else
            it = &nodeToDel->m_R;
    }
    return false;
}

bool CTree::isSet(const string & key) {

    auto it = m_Root;

    while ( it != nullptr ) {
        if ( it->m_Key == key ) return true;
        if ( it->m_Key < key ) it = it->m_R;
        else  it = it->m_L;
    }

    return false;
}

bool CTree::insert(const string & key, const string & val) {

    CNode * nodeToInsert = new CNode(key, val);

    if ( m_Root == nullptr ) {
        m_Root = m_First = m_Last = nodeToInsert;
        return true;
    }

    auto it = m_Root;
    while ( it != nullptr ) {
        if ( it->m_Key == key ) {
            delete nodeToInsert;
            return false;
        }
        if ( it->m_Key > key ) {
            if ( it->m_L == nullptr ) {
                it->m_L = nodeToInsert;
                m_Last->m_NextOrder = nodeToInsert;
                nodeToInsert->m_PrevOrder = m_Last;
                m_Last = nodeToInsert;
                return true;
            }
            it = it->m_L;
            continue;
        }
        else if ( it->m_Key < key ) {
            if ( it->m_R == nullptr ) {
                it->m_R = nodeToInsert;
                m_Last->m_NextOrder = nodeToInsert;
                nodeToInsert->m_PrevOrder = m_Last;
                m_Last = nodeToInsert;
                return true;
            }
            it = it->m_R;
            continue;
        }
        return false;
    }

    return false;
}

ostream & operator << (ostream & os, const CTree & src) {

    int count = 0;
    auto it = src.m_First;

    os << "{";
    while ( it != nullptr ) {
        if ( count != 0 )
            os << ", ";
        os << it->m_Key << " => " << it->m_Val;

        count++;
        it = it->m_NextOrder;
    }
    os << "}";

    return os;
}

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

    //======================================================


    assert (t.insert("AAG", "Pepa"));
    assert (t.insert("AG1", "Josef"));
    assert (t.insert("ZDM", "bomba"));
    assert (t.insert("ZMA", "pecka"));

    assert (t.remove("PA1"));
    assert (t.remove("ZDM"));
    assert (t.remove("AAG"));
    assert (!t.remove("AAG"));
    assert("PA2" == t.m_First->m_Key);

    return 0;
}