#include<iostream>
#include<string>
#include<cassert>
#include<sstream>
#include<stack>

using namespace std;

class CTree{
public:
    CTree() = default;
    CTree(const CTree & src) {
        copyTree(src.m_Root);
    }
    ~CTree() {
        /*
        auto it = m_First;
        auto tmp = it;
        while ( it != nullptr ) {
            tmp = it;
            it = it->m_NextOrder;
            delete tmp;
        }
         */

        delTree(m_Root);
    }

    bool remove( const string & key );
    bool isSet( const string & key );
    bool insert( const string & key, const string & val );
    CTree& invert ();
    size_t size();
    CTree & operator = ( const CTree & src );
    bool operator == ( const CTree & rhs );
    bool operator != ( const CTree & rhs );
    void print();
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

        bool operator== ( const CNode& rhs ) const;
        bool operator!= ( const CNode& rhs ) const;
    };

    CNode * m_Root = nullptr;
    CNode * m_First = nullptr, * m_Last = nullptr;
    friend int main();

    size_t m_Size = 0;

    bool cmpTree( CNode * lhs, CNode * rhs );
    void copyTree( CNode * src );
    void delTree( CNode * node);
    void invertNodes( CNode * node );
    void printInorder( CNode * node );
};
//=========================================================================
// CNode

// todo - DONE
// Compares just key and value not pointers
bool CTree::CNode::operator== ( const CNode& rhs ) const {
    if ( m_Key != rhs.m_Key ) return false;
    if ( m_Val != rhs.m_Val ) return false;
    return true;
}

// todo - DONE
bool CTree::CNode::operator!= ( const CNode& rhs ) const {
    return !( *this == rhs );
}

//=========================================================================
// CTree

// todo
bool CTree::remove( const string & key ) {
    //set iterator

    CNode ** it = &m_Root;

    while( *it != nullptr ) {
        CNode * nodeToDel = *it;
        if ( nodeToDel->m_Key == key ) {
            if ( nodeToDel == m_Last ) {
                m_Last->m_PrevOrder->m_NextOrder = nullptr;
                m_Last = m_Last->m_PrevOrder;
            }

            if ( nodeToDel->m_L != nullptr && nodeToDel->m_R != nullptr ) { //has two children
                it = &(*it)->m_R;
                while ( (*it)->m_L != nullptr )
                    it = &(*it)->m_L;
                nodeToDel->m_Key = (*it)->m_Key;
                nodeToDel->m_Val = (*it)->m_Val;

                if ( (*it)->m_PrevOrder != nullptr && (*it)->m_PrevOrder != nodeToDel )
                    nodeToDel->m_PrevOrder = (*it)->m_PrevOrder;
                if ( (*it)->m_NextOrder != nullptr && (*it)->m_NextOrder != nodeToDel )
                    nodeToDel->m_NextOrder = (*it)->m_NextOrder;

                nodeToDel = *it;
            }
            if ( nodeToDel->m_PrevOrder != nullptr ) {
                nodeToDel->m_PrevOrder->m_NextOrder = nodeToDel->m_NextOrder;
            }
            if ( nodeToDel->m_NextOrder != nullptr )
                nodeToDel->m_NextOrder->m_PrevOrder = nodeToDel->m_PrevOrder;

            if ( (*it)->m_L != nullptr )
                (*it) = (*it)->m_L;
            else
                (*it) = (*it)->m_R;

            nodeToDel->m_NextOrder = nodeToDel->m_PrevOrder = nullptr;
            nodeToDel->m_L = nodeToDel->m_R = nullptr;
            delete nodeToDel;
            m_Size--;
            return true;
        }
        if ( (*it)->m_Key > key )
            it = &nodeToDel->m_L;
        else
            it = &nodeToDel->m_R;
    }
    return false;

    /*finding node to delete {
        - declare CNode * nodeToDel
        - found
            - if m_Last
            - if has both children
                - go one right
                - go left as far as i can
                - change data
                - modify LL
                - set nodeToDel to iterator

            -modify LL

            - move it L
                - if no L move R
            - modify LL

            - set pointers in NTD to nullptr
            - del NTD
    }
     */
    //  move it
    // ret false
}

// todo - DONE
bool CTree::isSet( const string & key ) {
    auto it = m_Root;

    while ( it != nullptr ) {
        if ( key > it->m_Key ) {
            it = it->m_R;
        }
        else if ( key < it->m_Key ) {
            it = it->m_L;
        }
        else {
            return true;
        }
    }
    return false;
}

// todo - DONE
bool CTree::insert( const string & key, const string & val ) {

    CNode * nodeToAdd = new CNode(key, val);

    if ( m_Root == nullptr ) {
        m_First = nodeToAdd;
        m_Last = nodeToAdd;
        m_Root = nodeToAdd;
        m_Size++;
        return true;
    }

    auto it = m_Root;
    while ( it != nullptr ) {
        if ( key > it->m_Key ) {
            if ( it->m_R == nullptr ) {
                m_Last->m_NextOrder = nodeToAdd;
                nodeToAdd->m_PrevOrder = m_Last;
                m_Last = nodeToAdd;
                it->m_R = nodeToAdd;
                m_Size++;
                return true;
            }
            it = it->m_R;
        }
        else if ( key < it->m_Key ) {
            if ( it->m_L == nullptr ) {
                m_Last->m_NextOrder = nodeToAdd;
                nodeToAdd->m_PrevOrder = m_Last;
                m_Last = nodeToAdd;
                it->m_L = nodeToAdd;
                m_Size++;
                return true;
            }
            it = it->m_L;
        }
        else {
            break;
        }
    }
    delete nodeToAdd;
    return false;
}

// todo - DONE
CTree& CTree::invert () {
    invertNodes(m_Root);
    return *this;
}

// todo - DONE
size_t CTree::size() {
    return m_Size;
}

// todo - DONE
CTree & CTree::operator = ( const CTree & src ) {
    if ( this == &src )
        return *this;

    m_Root = nullptr;
    copyTree(src.m_First);
    return *this;
}

// todo - DONE
bool CTree::operator == ( const CTree & rhs ) {
    return cmpTree(m_Root, rhs.m_Root);
}

// todo - DONE
bool CTree::operator != ( const CTree & rhs ) {
    return !cmpTree(m_Root, rhs.m_Root);
}

// todo - DONE
void CTree::print() {
    printInorder(m_Root);
    cout << "\n";
}

// todo - DONE
void CTree::printInorder( CNode * node ) {
    if ( node == nullptr ) return;

    printInorder(node->m_L);
    cout << "| " << node->m_Key << " => " << node->m_Val << " |";
    printInorder(node->m_R);
}

// todo - DONE
ostream & operator << (ostream & os, const CTree & src) {
    auto it = src.m_First;
    int count = 0;

    os << "{";
    while ( it != nullptr ) {
        if ( count )
            os << ", ";
        os << it->m_Key << " => " << it->m_Val;

        it = it->m_NextOrder;
        count++;
    }
    os << "}";
    return os;
}

// todo - DONE
bool CTree::cmpTree( CNode * lhs, CNode * rhs ) {
    if ( lhs == nullptr && rhs == nullptr )
        return true;
    if ( lhs != nullptr && rhs != nullptr ) {
        if ( lhs->m_Key == rhs->m_Key && lhs->m_Val== rhs->m_Val
          && cmpTree( lhs->m_R, rhs->m_R )
          && cmpTree( lhs->m_L, rhs->m_L ) )
            return true;
        return false;
    }
    return false;
}

// todo - DONE
void CTree::copyTree( CNode * src ) {
    auto it = src;
    while ( it != nullptr ) {
        insert(it->m_Key, it->m_Val);
        it = it->m_NextOrder;
    }
}

// todo - DONE
void CTree::delTree( CNode * node) {
    if ( node == nullptr )
        return;

    delTree(node->m_L);
    delTree(node->m_R);
    delete node;
}

// todo - DONE
void CTree::invertNodes( CNode * node ) {
    if ( node == nullptr ) return;

    auto tmp = node->m_L;
    node->m_L = node->m_R;
    node->m_R = tmp;

    invertNodes(node->m_L);
    invertNodes(node->m_R);
}

int main() {
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

    assert(t.m_Root->m_L == nullptr);
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
    assert(t.m_Last == t.m_Root->m_L->m_R);
    assert(t.m_Root->m_L->m_R->m_L == nullptr);
    assert(t.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder = t.m_Root->m_L->m_R);
    assert(t.isSet("LIN"));

    ss << t;
    assert(ss.str() == "{PA1 => done, PA2 => fail, UOS => funny, CAO => lul, LIN => F}");
    ss.clear();
    ss.str("");
    assert(t.size() == 5);

    assert(t.insert("SAP", "shit"));
    assert(t.m_Root->m_R->m_R->m_L->m_Key == "SAP");
    assert(t.m_Last == t.m_Root->m_R->m_R->m_L);
    assert(t.size() == 6);

    ss << t;
    assert(ss.str() == "{PA1 => done, PA2 => fail, UOS => funny, CAO => lul, LIN => F, SAP => shit}");
    ss.clear();
    ss.str("");


    assert(!t.isSet("PA3"));
    assert(t.isSet("LIN"));
    assert(t.isSet("SAP"));

    t.print();

    //======================================================
    // Inverting

    CTree a, b;
    a = t;
    b = a;
    assert (a == t);
    assert (b == a);
    a.invert();
    //a.print();
    b.invert();
    assert (a != t);
    assert (b == a);
    a.invert();
    //a.print();
    assert (a == t);

    //======================================================
    // Removing

    assert (t.insert("AAG", "Pepa"));
    assert (t.insert("AG1", "Josef"));
    assert (t.insert("ZDM", "bomb"));
    assert (t.insert("ZMA", "banana"));
    assert (t.m_Last->m_Key == "ZMA");
    assert(t.size() == 10);

    ss << t;
    assert(ss.str() ==
           "{PA1 => done, PA2 => fail, UOS => funny, CAO => lul, LIN => F, SAP => shit, AAG => Pepa, AG1 => Josef, ZDM => bomb, ZMA => banana}");
    ss.clear();
    ss.str("");

    assert (t.remove("PA1"));
    assert (t.m_Last->m_Key == "ZMA");
    assert (t.remove("ZDM"));
    assert (t.m_Last->m_Key == "ZMA");
    assert (t.remove("AAG"));
    assert (t.m_Last->m_Key == "ZMA");
    assert (!t.remove("AAG"));
    assert("PA2" == t.m_First->m_Key);
    assert(t.size() == 7);

    ss << t;
    assert(ss.str() == "{PA2 => fail, UOS => funny, CAO => lul, LIN => F, SAP => shit, AG1 => Josef, ZMA => banana}");
    ss.clear();
    ss.str("");

    return 0;
}