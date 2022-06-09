#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <memory>
#include <functional>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

struct TNode {
  TNode * m_Left;
  TNode * m_Right;
  char m_data;
  bool m_isList;
};

// Taken from https://www.tutorialspoint.com/update-the-bit-in-the-given-position-or-index-of-a-number-using-cplusplus
void modifyBit(char &c, int bit, int pos) {
    int clearBit= ~(1<<pos);
    int mask= c & clearBit;
    c = mask |(bit<<pos);
}

// Taken from https://www.tutorialspoint.com/update-the-bit-in-the-given-position-or-index-of-a-number-using-cplusplus
void modifyBit(int &c, int bit, int pos) {
    int clearBit= ~(1<<pos);
    int mask= c & clearBit;
    c = mask |(bit<<pos);
}

char getChar( const vector<bool> array, int & index) {

  char c = '0';
  for ( int i = 7; i > -1; i-- ) {
    modifyBit( c , array[index++], i );
  }
  return c;
}

int getNum( const vector<bool> array, int & index) {

  int c = 0;

  for ( int i = 11; i > -1; i-- ) {
    modifyBit( c , array[index++], i );
  }

  return c;
}

TNode * createNode( char m_data) {
  TNode * node = new TNode;
  node->m_Left=nullptr;
  node->m_Right=nullptr;
  node->m_data = m_data;
  node->m_isList = false;
  return node;
}

TNode * createTree ( const vector<bool> array, int & index ) {
  char c = '0';
  TNode * root = nullptr;
  bool value = array[index];
  //cout << value << endl;
  if ( (size_t)index >= array.size()) throw "RIP ja";

  if (!value){
    root = createNode('0');
    //cout << value << "-";
    index++;
    root->m_Left = createTree( array, index);
    root->m_Right = createTree( array, index);
    return root;
  }
  else if (value) {
    //cout << "New byte ";
    c = getChar( array, ++index );
    root = createNode(c);
    root->m_isList = true;
    return root;
  }
  return root;
}

bool loading ( const char * inFileName, vector<bool> & array, int & counter) {

  char x, y;
  ifstream in(inFileName, ios::in | ios::binary);

  if ( ! in.good() ) return false;

  while ( in.get ( x ) ) {
    if ( ! in.good() ) return false;
    for ( int i = 7; i >= 0; i--){
      y = x & 1 << i;
      array.push_back(y);
      counter++;
    }
  }
  if ( ! in.eof() ) return false;
  return true;
}

char movingInTree ( TNode *& root, vector<bool> & array, int & index ) {

  TNode * current = root;

  while ( current ) {
    if ( current->m_isList ) {
      //cout << current->m_data;
      return current->m_data;
    }
    else if ( array[index] == 0 ) {
      index++;
      current = current->m_Left;
    }
    else {
      current = current->m_Right;
      index++;
    }
  }
  return '0';
}

bool decompressFile ( const char * inFileName, const char * outFileName )
{

  ofstream out(outFileName, ios::out | ios::binary);
  
  if ( ! out.good() ) return false;

  int counter = 0;
  vector<bool> array;
  int index = 0;

  // nacitani jednotlivych boolu
  if (! loading(inFileName, array, counter) ) return false;
  
  try {
    TNode * root = createTree(array, index);

    int num = 0;
    char c;
  
    while ( array[index] ) {
      index++;
      for ( int i = 0; i < 4096; i++) {
        if ( (size_t)index >= array.size()) return false;
        c = movingInTree( root, array, index );
        if ( ! out.good() ) return false;
        out << c; 
      }
    }
    cout << array[index] << " " << index << endl;
    if ( array[index++] == 0 ) {
      num = getNum( array, index );
      cout << num << endl;
    if ( num == 0)
      return true;

    cout << num << endl;
    for ( int i = 0; i < num; i++) {
      if ( (size_t)index >= array.size()) return false;
        c = movingInTree( root, array, index );
        if ( ! out.good() ) return false;
        out << c; 
      }
    }
  }
  catch( const char * a) {
    return false;
  }

  // testing print 
  //for ( int i = 0; i < counter; i++ ) {
  //  cout << bitset<1>(array[i]); 
  //  if ( (i+1) % 8 == 0 ) {
  //    cout << endl;
  //  }
  //}

  out << flush;
  return true;
}

//===================================================================================
bool compressFile ( const char * inFileName, const char * outFileName )
{
  // keep this dummy implementation (no bonus) or implement the compression (bonus)
  return false;
}
#ifndef __PROGTEST__
bool identicalFiles(const std::string& p1, const std::string& p2) {
  return true;
}

int main ( void )
{

  assert ( decompressFile ( "tests/test0.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test0.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test1.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test1.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test2.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test2.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test3.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test3.orig", "tempfile" ) );
  
  assert ( decompressFile ( "tests/test4.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/test4.orig", "tempfile" ) );

  assert ( ! decompressFile ( "tests/test5.huf", "tempfile" ) );
  assert ( decompressFile ( "tests/test6.bin", "tempfile" ) ); 
  assert ( decompressFile ( "tests/test7.bin", "tempfile") );

/*
  assert ( decompressFile ( "tests/extra0.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra0.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra1.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra1.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra2.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra2.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra3.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra3.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra4.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra4.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra5.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra5.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra6.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra6.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra7.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra7.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra8.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra8.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra9.huf", "tempfile" ) );
  assert ( identicalFiles ( "tests/extra9.orig", "tempfile" ) );
 */
  return 0;
}
#endif /* __PROGTEST__ */
