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
    // Linked list iterator
    class llIt {
    public:
        llIt() = default;
        llIt( CNode * ptr ) : m_Ptr(ptr) {};
        ~llIt() = default;
        CNode operator * () const;
        CNode * operator ->();
        llIt& operator ++();
        llIt& operator ++(int n);
        bool operator == (llIt rhs);
        bool operator != (llIt rhs);
    private:
        CNode * m_Ptr;
    };
    // inorder operator
    class ioIt {
    public:
        ioIt() = default;
        ioIt( CNode * ptr ) : m_Ptr(ptr) {
            moveLeft(ptr);
            m_Ptr = s.top();
            s.pop();
        };
        ~ioIt() = default;
        CNode operator * () const;
        CNode * operator -> ();
        ioIt& operator ++ ();
        ioIt& operator ++ (int n);
        bool operator == (ioIt rhs);
        bool operator != (ioIt rhs);
        bool hasNext();
    protected:
        CNode * m_Ptr;
        stack<CNode *> s;
        void moveLeft(CNode * node);
    };

    CNode * m_Root = nullptr;
    CNode * m_First = nullptr, * m_Last = nullptr;
    friend int main();

    llIt beginLL();
    llIt endLL();
    ioIt beginIo();
    ioIt endIo();

    // helping functions
    void copyTree( CNode * src );
    // recursive destructing of tree
    void delTree( CNode * node);
    // prints tree in order L->root->R
    void printInorder( CNode * node );
};
//=========================================================================
// CNode

// todo - DONE
// Compares just key and value not pointers
bool CTree::CNode::operator== ( const CNode& rhs ) const {
    if ( m_Key != rhs.m_Key ) return false;
    if ( m_Val != rhs.m_Val ) return false;
    if ( (m_L && !rhs.m_L) || (!m_L && rhs.m_L) ) return false;
    if ( (m_R && !rhs.m_R) || (!m_R && rhs.m_R) ) return false;
    return true;
}

// todo - DONE
bool CTree::CNode::operator!= ( const CNode& rhs ) const {
    return !(*this == rhs);
}

//=========================================================================
// linked list iterator
// todo - DONE
CTree::CNode CTree::llIt::operator * () const {
    return *m_Ptr;
}
// todo - DONE
CTree::CNode * CTree::llIt::operator ->() {
    return m_Ptr;
}
// todo - DONE
CTree::llIt& CTree::llIt::operator ++() {
    m_Ptr = m_Ptr->m_NextOrder;
    return *this;
}
// todo - DONE
CTree::llIt& CTree::llIt::operator ++(int n) {
    auto tmp = *this;
    ++(*this);
    return tmp;
}
// todo - DONE
bool CTree::llIt::operator == (llIt rhs) {
    if ( *(*this) != *rhs ) return false;
    if ( m_Ptr != rhs.m_Ptr ) return false;
    return true;
}
// todo - DONE
bool CTree::llIt::operator != (llIt rhs) {
    return !(*this == rhs);
}

//=========================================================================
// inorder iterator
// todo - DONE
CTree::CNode CTree::ioIt::operator * () const {
    return *m_Ptr;
}
// todo - DONE
CTree::CNode * CTree::ioIt::operator ->() {
    return m_Ptr;
}
// todo - DONE
CTree::ioIt& CTree::ioIt::operator ++() {
    m_Ptr = s.top();
    s.pop();
    if ( m_Ptr->m_R )
        moveLeft(m_Ptr->m_R);
    return *this;
}
// todo - DONE
CTree::ioIt& CTree::ioIt::operator ++(int n) {
    auto tmp = *this;
    ++(*this);
    return tmp;
}
// todo - DONE
bool CTree::ioIt::operator == (ioIt rhs) {
    if ( *m_Ptr != *(rhs.m_Ptr) ) return false;
    if ( m_Ptr != rhs.m_Ptr ) return false;
    return true;
}
// todo - DONE
bool CTree::ioIt::operator != (ioIt rhs) {
    return !(*this == rhs);
}

void CTree::ioIt::moveLeft(CNode * node) {
    if ( node == nullptr ) return;
    while( node ) {
        s.push(node);
        node = node->m_L;
    }
}

bool CTree::ioIt::hasNext() {
    return s.size();
}

//=========================================================================
// CTree
// todo
bool CTree::remove( const string & key ) {
    //declare iterator

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
    CNode ** it = &m_Root;

    while ( *it != nullptr ) {
        // found
        if ( (*it)->m_Key == key ) {
            CNode * nodeToDel = (*it);
            if ( nodeToDel->m_L && nodeToDel->m_R ) {
                it = &(*it)->m_R;
                while ( (*it)->m_L != nullptr )
                    it = &(*it)->m_L;

                nodeToDel->m_Key = (*it)->m_Key;
                nodeToDel->m_Val = (*it)->m_Val;
                nodeToDel->m_NextOrder = (*it)->m_NextOrder;
                nodeToDel->m_PrevOrder = (*it)->m_PrevOrder;

                if ( (*it)->m_NextOrder )
                    (*it)->m_NextOrder->m_PrevOrder = nodeToDel;
                if ( (*it)->m_PrevOrder )
                    (*it)->m_PrevOrder->m_NextOrder = nodeToDel;

                nodeToDel = (*it);
            }

            if ( nodeToDel->m_NextOrder )
                nodeToDel->m_NextOrder->m_PrevOrder = nodeToDel->m_PrevOrder;
            if ( nodeToDel->m_PrevOrder )
                nodeToDel->m_PrevOrder->m_NextOrder = nodeToDel->m_NextOrder;

            if ( nodeToDel->m_L )
                (*it)=nodeToDel->m_L;
            else
                (*it)=nodeToDel->m_R;

            nodeToDel->m_R = nodeToDel->m_L = nullptr;
            nodeToDel->m_NextOrder = nodeToDel->m_PrevOrder = nullptr;

            delete nodeToDel;
            return true;
        }
        else if ( (*it)->m_Key > key )
            it = &(*it)->m_L;
        else
            it = &(*it)->m_R;
    }

    return false;
}
// todo - DONE
bool CTree::isSet( const string & key ) {
    CNode ** it = &m_Root;

    while ( *it != nullptr ) {
        if ( (*it)->m_Key == key ) return true;
        if ( (*it)->m_Key > key )
            it = &(*it)->m_L;
        else
            it = &(*it)->m_R;
    }
    return false;
}
// todo - DONE
bool CTree::insert( const string & key, const string & val ) {

    CNode * nodeToAdd = new CNode(key, val);

    if ( m_Root == nullptr ) {
        m_Root = nodeToAdd;
        m_Last = nodeToAdd;
        m_First = nodeToAdd;
        return true;
    }

    CNode ** it = &m_Root;
    while ( (*it) != nullptr ) {
        if ( (*it)->m_Key > key ) {
            if ( (*it)->m_L == nullptr ) {
                m_Last->m_NextOrder = nodeToAdd;
                nodeToAdd->m_PrevOrder = m_Last;
                m_Last = nodeToAdd;
                (*it)->m_L = nodeToAdd;
                return true;
            }
            it = &(*it)->m_L;
        }
        else if ( (*it)->m_Key < key ) {
            if ( (*it)->m_R == nullptr ) {
                m_Last->m_NextOrder = nodeToAdd;
                nodeToAdd->m_PrevOrder = m_Last;
                m_Last = nodeToAdd;
                (*it)->m_R = nodeToAdd;
                return true;
            }
            it = &(*it)->m_R;
        }
        else
            break;
    }
    delete nodeToAdd;
    return false;
}
// todo - DONE
CTree & CTree::operator = ( const CTree & src ) {
    if ( this == &src ) return *this;
    //delTree(m_Root);
    copyTree(src.m_First);
    return *this;
}
// todo - DONE
void CTree::print() {
    cout << endl;
    printInorder(m_Root);
}
// todo - DONE
void CTree::printInorder( CNode * node ) {
    if ( node == nullptr ) return;

    printInorder(node->m_L);
    cout << "|" << node->m_Key << " => " << node->m_Val << "|";
    printInorder(node->m_R);
}
// todo - DONE
ostream & operator << (ostream & os, const CTree & src) {
    CTree::CNode* it = src.m_First;
    int count = 0;

    os << "{";
    while ( it != nullptr ) {
        if ( count )
            os << ", ";
        os << it->m_Key << " => " << it->m_Val;

        it = it->m_NextOrder;
        count ++;
    }
    os << "}";
    return os;

}
// todo - DONE
void CTree::copyTree( CNode * node ) {
    if ( node == nullptr ) return;

    while ( node != nullptr ) {
        insert(node->m_Key, node->m_Val);
        node = node->m_NextOrder;
    }
}
// todo - DONE
void CTree::delTree( CNode * node) {
    if ( node == nullptr ) return;

    delTree(node->m_L);
    delTree(node->m_R);

    delete node;
}
// todo - DONE
CTree::llIt CTree::beginLL() {
    return llIt(m_First);
}
// todo - DONE
CTree::llIt CTree::endLL() {
    return llIt(m_Last);
}
// todo - DONE
CTree::ioIt CTree::beginIo() {
    return ioIt(m_Root);
}
// todo - DONE
CTree::ioIt CTree::endIo() {
    CNode ** it = &m_Root;
    while ( (*it)->m_R != nullptr )
        it = &(*it)->m_R;
    return ioIt(*it);
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

    t.print();

    //======================================================
    // Removing

    assert (t.insert("AAG", "Pepa"));
    assert (t.insert("AG1", "Josef"));
    assert (t.insert("ZDM", "bomb"));
    assert (t.insert("ZMA", "banana"));
    assert (t.m_Last->m_Key == "ZMA");

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

    ss << t;
    assert(ss.str() == "{PA2 => fail, UOS => funny, CAO => lul, LIN => F, SAP => shit, AG1 => Josef, ZMA => banana}");
    ss.clear();
    ss.str("");
    t.print();


    //======================================================
    // Iterator - LL

    CTree::llIt it = t.beginLL();
    assert ( *it == *t.m_First );
    ++it;
    assert ( *it == *t.m_First->m_NextOrder );
    it++;
    assert ( *it == *t.m_First->m_NextOrder->m_NextOrder );
    (++it)++;
    assert ( (*it).m_Key == t.m_First->m_NextOrder->m_NextOrder->m_NextOrder->m_NextOrder->m_Key );

    while ( (*it).m_NextOrder )
        it++;
    auto endLL = t.endLL();
    assert ( it == endLL );

    //======================================================
    // Iterator - inorder
    auto inorder = t.beginIo();
    inorder++;
    assert(inorder->m_Key == "CAO");
    inorder++;
    inorder++;
    assert(inorder->m_Key == "PA2");
    while ( inorder.hasNext() ) {
        inorder++;
    }
    auto end=t.endIo();
    assert( inorder->m_Key == "ZMA" );
    assert( inorder == end );

    return 0;
}