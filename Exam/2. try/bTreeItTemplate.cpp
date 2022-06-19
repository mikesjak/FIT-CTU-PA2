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
           // todo
        };
        ~ioIt() = default;
        CNode operator * () const;
        CNode * operator -> ();
        ioIt& operator ++ ();
        ioIt& operator ++ (int n);
        bool operator == (ioIt rhs);
        bool operator != (ioIt rhs);
    private:
        CNode * m_Ptr;
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

// todo
// Compares just key and value not pointers
bool CTree::CNode::operator== ( const CNode& rhs ) const {

}

// todo
bool CTree::CNode::operator!= ( const CNode& rhs ) const {

}

//=========================================================================
// linked list iterator
// todo
CTree::CNode CTree::llIt::operator * () const {

}
// todo
CTree::CNode * CTree::llIt::operator ->() {

}
// todo
CTree::llIt& CTree::llIt::operator ++() {

}
// todo
CTree::llIt& CTree::llIt::operator ++(int n) {

}
// todo
bool CTree::llIt::operator == (llIt rhs) {
    return true;
}
// todo
bool CTree::llIt::operator != (llIt rhs) {

}

//=========================================================================
// inorder iterator
// todo
CTree::CNode CTree::ioIt::operator * () const {

}
// todo
CTree::CNode * CTree::ioIt::operator ->() {

}
// todo
CTree::ioIt& CTree::ioIt::operator ++() {

}
// todo
CTree::ioIt& CTree::ioIt::operator ++(int n) {

}
// todo
bool CTree::ioIt::operator == (ioIt rhs) {
}
// todo
bool CTree::ioIt::operator != (ioIt rhs) {

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

            - move it L (node)
                - if no L move R
            - modify LL

            - set pointers in NTD to nullptr
            - del NTD
    }
     */
    //  move it
    // ret false
}

// todo
bool CTree::isSet( const string & key ) {

}

// todo
bool CTree::insert( const string & key, const string & val ) {

}

// todo
CTree & CTree::operator = ( const CTree & src ) {

}

// todo
void CTree::print() {

}

// todo
void CTree::printInorder( CNode * node ) {

}

// todo
ostream & operator << (ostream & os, const CTree & src) {

}

// todo
void CTree::copyTree( CNode * src ) {

}

// todo
void CTree::delTree( CNode * node) {

}
// todo
CTree::llIt CTree::beginLL() {

}
// todo
CTree::llIt CTree::endLL() {

}
// todo
CTree::ioIt CTree::beginIo() {

}
// todo
CTree::ioIt CTree::endIo() {

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
