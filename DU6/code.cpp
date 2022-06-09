#ifndef __PROGTEST__
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stdexcept>
#include <algorithm>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

// ============================================================================
class AbstractType {
  public:
    virtual ~AbstractType() = default; 
    virtual size_t getSize() const = 0;
    virtual void print(ostream& os) const = 0;
    virtual std::shared_ptr<AbstractType> clone() const = 0;
    virtual bool operator == ( const AbstractType & rhs) const = 0;
    virtual bool operator != ( const AbstractType & rhs) const {
        return ! ( *this == rhs );
    }
};
std::ostream& operator << (std::ostream& os, const AbstractType& obj) {
  obj.print(os);
  return os;
} 
// ============================================================================
class CDataTypeInt : public AbstractType {
  public:
    CDataTypeInt() :AbstractType() {};
    size_t getSize() const override { return 4; }
    void print(ostream& os) const override {
        os << "int";
    }
    std::shared_ptr<AbstractType> clone() const override {
        return make_shared<CDataTypeInt>(*this);
    }
    bool operator == ( const AbstractType & rhs) const override;
};
bool CDataTypeInt::operator == ( const AbstractType & rhs) const {
    const CDataTypeInt* ptr = dynamic_cast<const CDataTypeInt*>(&rhs);
    if (ptr != nullptr) return true;
        return false;
}
// ============================================================================
class CDataTypeDouble : public AbstractType {\
  public:
    CDataTypeDouble() :AbstractType() {};
    size_t getSize() const override { return 8; }
    void print(ostream& os) const override { os << "double"; }
    std::shared_ptr<AbstractType> clone() const override {
        return make_shared<CDataTypeDouble>(*this);
    }
    bool operator == ( const AbstractType & rhs) const override;
};
bool CDataTypeDouble::operator == ( const AbstractType & rhs) const {
    const CDataTypeDouble* ptr = dynamic_cast<const CDataTypeDouble*>(&rhs);
    if (ptr != nullptr) return true;
        return false;
}
// ============================================================================
class CDataTypeEnum : public AbstractType {
  public:
    CDataTypeEnum() :AbstractType() {};
    size_t getSize() const  override { return 4; }
    void print(ostream& os) const override {
        os << "enum";
        os << "{";
        for (auto item:items) {
            os << item;
            if (item != (*(items.end()-1)))
                os << ",";
        }
        os << "}";
    }
    CDataTypeEnum& add( string name ) {
        string exc = "Duplicate enum value: ";
        exc.append(exc);
        for ( auto &item:items ) {
            if ( item == name ) {
              string msg = "Duplicate enum value: ";
              msg.append(name);
              throw invalid_argument(msg);
            }
        }   
        items.push_back(name);
        return *this;
    }
    std::shared_ptr<AbstractType> clone() const override {
        return make_shared<CDataTypeEnum>(*this);
    }
    bool operator == ( const AbstractType & rhs) const override;
  protected:
        vector<string> items;
};
bool CDataTypeEnum::operator == ( const AbstractType & rhs) const {
    const CDataTypeEnum* ptr = dynamic_cast<const CDataTypeEnum*>(&rhs);
    if (ptr == nullptr) return false;
    if (items.size() != ptr->items.size()) return false;
    for (auto it1=items.begin(), it2=ptr->items.begin(); it1 != items.end(); it1++, it2++){
        if ( (*it1) != (*it2) ) return false;
    }
    return true;
}
// ============================================================================
class CDataTypeStruct : public AbstractType {
  public:
    CDataTypeStruct() :AbstractType() {};
    size_t getSize() const override {
        size_t sum = 0;
        for (const auto &item:items) {
            sum += item.second->getSize();
        }
        return sum;
    }
    void print(ostream& os) const override { 
        os << "struct";
        os << "{";
        for (auto item:items) {
            os << (*item.second);
            os << " " << item.first << ";\n";
        }
        os << "}" << endl;
    }
    AbstractType& field( const string name ) const {
        for ( auto item:items ) {
            if (item.first == name)
                return (*item.second);
        }
        string msg = "Unknown field: ";
        msg.append(name);
        throw invalid_argument(msg);
    }
    CDataTypeStruct& addField( const string name, const AbstractType& type ) {
        for ( auto item:items ) {
            if (item.first == name) {
                string msg = "Duplicate field: ";
                msg.append(name);
                throw invalid_argument(msg);
            }
        }
        auto ptr = type.clone();
        items.push_back(make_pair(name, ptr) );
        return *this;
    }
    std::shared_ptr<AbstractType> clone() const override {
        return make_shared<CDataTypeStruct>(*this);
    }
    bool operator == ( const AbstractType & rhs) const override;
  protected:
        vector< pair <string, shared_ptr<AbstractType > > > items;
};
bool CDataTypeStruct::operator == ( const AbstractType & rhs) const {
    const CDataTypeStruct* ptr = dynamic_cast<const CDataTypeStruct*>(&rhs);
    if (ptr == nullptr) return false;
    if (items.size() != ptr->items.size()) return false;
    for (auto it1=items.begin(), it2=ptr->items.begin(); it1 != items.end(); it1++, it2++){
        if ( (*(it1->second)) != (*(it2->second)) ) return false;
    }
    return true;
}
// ============================================================================
#ifndef __PROGTEST__

template <typename T_>
static bool        whitespaceMatch                         ( const T_        & x,
                                                             const string    & ref )
{
  ostringstream oss;
  oss << x;
  return whitespaceMatch ( oss . str (), ref );
}
int main ( void )
{


  CDataTypeStruct  a = CDataTypeStruct () .
                        addField ( "m_Length", CDataTypeInt () ) .
                        addField ( "m_Status", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "DEAD" ) ).
                        addField ( "m_Ratio", CDataTypeDouble () );
  
  CDataTypeStruct b = CDataTypeStruct () .
                        addField ( "m_Length", CDataTypeInt () ) .
                        addField ( "m_Status", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "READY" ) ).
                        addField ( "m_Ratio", CDataTypeDouble () );
  
  CDataTypeStruct c =  CDataTypeStruct () .
                        addField ( "m_First", CDataTypeInt () ) .
                        addField ( "m_Second", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "DEAD" ) ).
                        addField ( "m_Third", CDataTypeDouble () );

  CDataTypeStruct  d = CDataTypeStruct () .
                        addField ( "m_Length", CDataTypeInt () ) .
                        addField ( "m_Status", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "DEAD" ) ).
                        addField ( "m_Ratio", CDataTypeInt () );
  assert ( whitespaceMatch ( a, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "}") );

  assert ( whitespaceMatch ( b, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    READY\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "}") );

  assert ( whitespaceMatch ( c, "struct\n"
    "{\n"
    "  int m_First;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Second;\n"
    "  double m_Third;\n"
    "}") );

  assert ( whitespaceMatch ( d, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  int m_Ratio;\n"
    "}") );

  assert ( a != b );
  assert ( a == c );
  assert ( a != d );
  assert ( a . field ( "m_Status" ) == CDataTypeEnum () . add ( "NEW" ) . add ( "FIXED" ) . add ( "BROKEN" ) . add ( "DEAD" ) );
  assert ( a . field ( "m_Status" ) != CDataTypeEnum () . add ( "NEW" ) . add ( "BROKEN" ) . add ( "FIXED" ) . add ( "DEAD" ) );
  assert ( a != CDataTypeInt() );
  assert ( whitespaceMatch ( a . field ( "m_Status" ), "enum\n"
    "{\n"
    "  NEW,\n"
    "  FIXED,\n"
    "  BROKEN,\n"
    "  DEAD\n"
    "}") );

  CDataTypeStruct aOld = a;
  b . addField ( "m_Other", CDataTypeDouble ());

  a . addField ( "m_Sum", CDataTypeInt ());

  assert ( a != aOld );
  assert ( a != c );
  assert ( aOld == c );
  assert ( whitespaceMatch ( a, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "  int m_Sum;\n"
    "}") );

  assert ( whitespaceMatch ( b, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    READY\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "  double m_Other;\n"
    "}") );

  c . addField ( "m_Another", a . field ( "m_Status" ));

  assert ( whitespaceMatch ( c, "struct\n"
    "{\n"
    "  int m_First;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Second;\n"
    "  double m_Third;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Another;\n"
    "}") );

  d . addField ( "m_Another", a . field ( "m_Ratio" ));

  assert ( whitespaceMatch ( d, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  int m_Ratio;\n"
    "  double m_Another;\n"
    "}") );

  assert ( a . getSize () == 20 );
  assert ( b . getSize () == 24 ); 
  try
  {
    a . addField ( "m_Status", CDataTypeInt () );
    assert ( "addField: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( e . what () == "Duplicate field: m_Status"sv );
  }

  try
  {
    cout << a . field ( "m_Fail" ) << endl;
    assert ( "field: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( e . what () == "Unknown field: m_Fail"sv );
  }

  try
  {
    CDataTypeEnum en;
    en . add ( "FIRST" ) .
         add ( "SECOND" ) .
         add ( "FIRST" );
    assert ( "add: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( e . what () == "Duplicate enum value: FIRST"sv );
  }

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
