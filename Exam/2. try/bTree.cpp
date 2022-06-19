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
        m_Root = nullptr;
        copyTree(src.m_Root);
    }
    ~CTree(){
        auto it = m_First;
        auto tmp = it;

        while ( it != nullptr ) {
            tmp = it;
            it = it->m_NextOrder;
            delete tmp;
        }
    }
    bool remove( const string & key );
    bool isSet( const string & key );
    bool insert( const string & key, const string & val );
    CTree& invertTree ();
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

    class treeIterator {
    public:
        treeIterator() = default;
        explicit treeIterator( CNode * ptr ) : m_Ptr(ptr) {};
        ~treeIterator() = default;

        CNode operator*() const;
        CNode * operator->();
        treeIterator& operator++();
        treeIterator& operator--();
        treeIterator operator++( int );
        treeIterator operator--( int );
        bool operator== ( const treeIterator& rhs ) const;
        bool operator!= ( const treeIterator& rhs ) const;
    private:
        CNode * m_Ptr;
    };
    class inIt {
    public:
        inIt() = default;
        inIt( CNode * ptr ) : m_Ptr(ptr) {
            moveLeft(ptr);
            m_Ptr = s.top();
            s.pop();
        };
        ~inIt() = default;
        CNode operator*() const {
            return *m_Ptr;
        }
        CNode * operator->() {
            return m_Ptr;
        }
        inIt& operator++() {

            m_Ptr = s.top();
            s.pop();

            if ( m_Ptr->m_R != nullptr ) {
                moveLeft(m_Ptr->m_R);
            }

            return *this;
        }
    private:
        void moveLeft( CNode * node ) {
            while (node != nullptr) {
                s.push(node);
                node = node->m_L;
            }
        }

        bool hasNext() {
            return !s.empty();
        }

        CNode * m_Ptr;
        stack<CNode*> s;
    };
    class preIt {
    public:
    private:
        CNode * m_Ptr;
    };
    class postIt {
    public:
    private:
        CNode * m_Ptr;
    };
    CNode * m_Root = nullptr;
    CNode * m_First = nullptr, * m_Last = nullptr;
    friend int main();

    size_t m_Size = 0;

    bool cmpTree( CNode * lhs, CNode * rhs );
    void copyTree( CNode * src );
    void invertNodes( CNode * node );
    void printIn( CNode * node );
    void printPost( CNode * node );
    void printPre( CNode * node );

    treeIterator end();
    treeIterator begin();
    inIt inEnd();
    inIt inBegin();
};

//================================================================================
// Node
bool CTree::CNode::operator== ( const CNode& rhs ) const {
    if ( m_Key != rhs.m_Key ) return false;
    if ( m_Val != rhs.m_Val ) return false;
    return true;
}

bool CTree::CNode::operator!= ( const CNode& rhs ) const {
    return ! ( *this == rhs );
}
//================================================================================
// Iterator - LL
CTree::CNode CTree::treeIterator::operator*() const {
    return *m_Ptr;
}

CTree::CNode * CTree::treeIterator::operator->() {
    return m_Ptr;
}

CTree::treeIterator& CTree::treeIterator::operator++() {
    if ( m_Ptr->m_NextOrder )
        m_Ptr = (*m_Ptr).m_NextOrder;
    else
        throw invalid_argument("Entering mordor");
    return *this;
}

CTree::treeIterator& CTree::treeIterator::operator--() {
    m_Ptr = (*m_Ptr).m_PrevOrder;
    return *this;
}

CTree::treeIterator CTree::treeIterator::operator++(int) {
    treeIterator tmp = (*this);
    ++(*this);
    return tmp;
}

CTree::treeIterator CTree::treeIterator::operator--(int) {
    treeIterator tmp = (*this);
    --(*this);
    return tmp;
}

bool CTree::treeIterator::operator== ( const treeIterator& rhs ) const {
    return ( this->m_Ptr == rhs.m_Ptr );
}

bool CTree::treeIterator::operator!= ( const treeIterator& rhs ) const {
    return ( this->m_Ptr != rhs.m_Ptr );
}

CTree::treeIterator CTree::end() {
    return treeIterator(m_Last);
}

CTree::treeIterator CTree::begin() {
    return treeIterator(m_First);
}

//================================================================================
// Iterator - inorder

CTree::inIt CTree::inEnd() {
    auto it = m_Root;
    while ( it->m_R )
        it = it->m_R;
    return inIt(it);
}

CTree::inIt CTree::inBegin() {
    return inIt(m_Root);
}

//================================================================================
// Tree
bool CTree::remove(const string & key) {

    CNode ** it = &m_Root;

    while ( *it != nullptr ) {
        CNode * nodeToDel = *it;
        if ( nodeToDel->m_Key == key ) {

            if ( nodeToDel == m_Last ) {
                m_Last = m_Last->m_PrevOrder;
                m_Last->m_NextOrder = nullptr;
            }

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
                    (*it)->m_NextOrder->m_PrevOrder = (*it)->m_PrevOrder;
            }
            if ( nodeToDel->m_L != nullptr )
                *it = nodeToDel->m_L;
            else
                *it = nodeToDel->m_R;

            if ( nodeToDel->m_NextOrder )
                nodeToDel->m_NextOrder->m_PrevOrder = nodeToDel->m_PrevOrder;
            if ( nodeToDel->m_PrevOrder)
                nodeToDel->m_PrevOrder->m_NextOrder = nodeToDel->m_NextOrder;

            nodeToDel->m_NextOrder = nodeToDel->m_PrevOrder = nullptr;
            nodeToDel->m_L = nodeToDel->m_R = nullptr;
            delete nodeToDel;
            m_Size--;
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
        m_Size++;
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
                m_Size++;
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
                m_Size++;
                return true;
            }
            it = it->m_R;
            continue;
        }
        return false;
    }

    return false;
}

CTree& CTree::invertTree () {
    invertNodes( m_Root );
    return *this;
}

size_t CTree::size() {
    return m_Size;
}

void CTree::invertNodes( CNode * node ) {
    if ( node == nullptr )
        return;

    auto tmp = node->m_L;
    node->m_L = node ->m_R;
    node->m_R = tmp;

    if ( node->m_L )
        invertNodes(node->m_L);
    if ( node->m_R )
        invertNodes(node->m_R);
}

bool CTree::cmpTree( CNode * lhs, CNode * rhs ) {

    if ( lhs == nullptr && rhs == nullptr ) return true;
    else if ( lhs != nullptr && rhs != nullptr ) {
        if ( lhs->m_Key == rhs->m_Key && lhs->m_Val == rhs->m_Val
             && cmpTree(lhs->m_L, rhs->m_L)
             && cmpTree(lhs->m_R, rhs->m_R) )
            return true;
        return false;
    }
    return false;
}

void CTree::copyTree(CTree::CNode *src) {
    CNode ** it = &src;
    while ( *it != nullptr ) {
        insert( (*it)->m_Key, (*it)->m_Val );
        *it = (*it)->m_NextOrder;
    }
}

void CTree::print() {
    cout << "\n================================================================" << endl;
    cout << "\nIn order: ";
    printIn(m_Root);
    cout << "\nPre order: ";
    printPre(m_Root);
    cout << "\nPost order: ";
    printPost(m_Root);
    cout << "\n================================================================" << endl;
}

void CTree::printIn( CNode * node ) {
    if ( node == nullptr ) return;

     printIn(node->m_L);
     cout << node->m_Key << " => ";
     printIn(node->m_R);
}

void CTree::printPost( CNode * node ) {
    if ( node == nullptr ) return;

    printPost(node->m_L);
    printPost(node->m_R);
    cout << node->m_Key << " => ";
}

void CTree::printPre( CNode * node ) {
    if ( node == nullptr ) return;

    cout << node->m_Key << " => ";
    printPre(node->m_L);
    printPre(node->m_R);
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

CTree & CTree::operator = ( const CTree & src ) {
    if ( *this == src ) return *this;

    m_Root = nullptr;
    copyTree( src.m_Root );
    return *this;
}

bool CTree::operator == ( const CTree & rhs ) {
    return cmpTree( m_Root, rhs.m_Root );
}

bool CTree::operator != ( const CTree & rhs ) {
    return !cmpTree(m_Root, rhs.m_Root);
}


int main(){
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
    assert ( a == t );
    assert ( b == a );
    a.invertTree();
    b.invertTree();
    assert ( a != t );
    assert ( b == a );
    a.invertTree();
    assert ( a == t );

    //======================================================
    // Removing

    assert (t.insert("AAG", "Pepa"));
    assert (t.insert("AG1", "Josef"));
    assert (t.insert("ZDM", "bomb"));
    assert (t.insert("ZMA", "banana"));
    assert (t.m_Last->m_Key == "ZMA" );
    assert(t.size() == 10);

    ss << t;
    assert(ss.str() == "{PA1 => done, PA2 => fail, UOS => funny, CAO => lul, LIN => F, SAP => shit, AAG => Pepa, AG1 => Josef, ZDM => bomb, ZMA => banana}");
    ss.clear();
    ss.str("");

    assert (t.remove("PA1"));
    assert (t.m_Last->m_Key == "ZMA" );
    assert (t.remove("ZDM"));
    assert (t.m_Last->m_Key == "ZMA" );
    assert (t.remove("AAG"));
    assert (t.m_Last->m_Key == "ZMA" );
    assert (!t.remove("AAG"));
    assert("PA2" == t.m_First->m_Key);
    assert(t.size() == 7);

    ss << t;
    assert(ss.str() == "{PA2 => fail, UOS => funny, CAO => lul, LIN => F, SAP => shit, AG1 => Josef, ZMA => banana}");
    ss.clear();
    ss.str("");

    //======================================================
    // Iterator - LL

    auto it = t.begin();
    assert ( *it == *t.m_First );
    ++it;
    assert ( *it == *t.m_First->m_NextOrder );
    it++;
    assert ( *it == *t.m_First->m_NextOrder->m_NextOrder );
    assert ( (*it++) == *t.m_First->m_NextOrder->m_NextOrder );

    while ( (*it).m_NextOrder )
        it++;
    assert ( it == t.end() );
    auto end = t.end();
    assert ( *end == *t.m_Last );
    assert ( *(--end) == *t.m_Last->m_PrevOrder );
    while ( end != t.begin() )
        --end;
    assert( end == t.begin() );

    //======================================================
    // Iterator - inorder
    t.print();
    auto inorder = t.inBegin();
    while ( *inorder != *(t.inEnd()) ) {
        cout << inorder->m_Key << ", ";
        ++inorder;
    }
    cout << inorder->m_Key;

    return 0;
}
