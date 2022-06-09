#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream> 
#include <iomanip> 
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

//TCompany* position = (TCompany*) bsearch( &company_to_find, &m_registerName, register_size, sizeof(TCompany), (int(*)(const void*, const void*)) comparatorName );

class CVATRegister
{
  public:
    CVATRegister ( void ){}
    ~CVATRegister ( void ){
      m_registerName.clear();
    }

    bool newCompany ( const string & name, const string & addr, const string & taxID ) {
      TCompany tmp;


      tmp.m_name = stringToLower(name);
      tmp.m_address = stringToLower(addr);
      tmp.m_ID = taxID;
      tmp.m_unchanged_address = addr;
      tmp.m_unchanged_name = name;

      for(size_t i = 0; i < m_registerName.size();i++){
        if( (tmp.m_name == m_registerName[i].m_name && tmp.m_address == m_registerName[i].m_address) || (tmp.m_ID == m_registerName[i].m_ID) ){
          cout << "newCompany to NAME FAILED!" << endl;
          return false;
        }
      }

      for(size_t i = 0; i < m_registerName.size();i++){
        if( (tmp.m_name == m_registerID[i].m_name && tmp.m_address == m_registerID[i].m_address) || (tmp.m_ID == m_registerID[i].m_ID) ) {
          cout << "newCompany to ID FAILED!" << endl;
          return false;
        }
      }

      m_registerName.insert(lower_bound(  m_registerName.begin(), m_registerName.end(), tmp, comparatorName ) , tmp );
      m_registerID.insert(lower_bound(  m_registerID.begin(), m_registerID.end(), tmp, comparatorID ) , tmp );
      
      return true;
    }

    bool cancelCompany ( const string  & name, const string & addr ) {

      TCompany company_to_find;
      company_to_find.m_name = stringToLower(name);
      company_to_find.m_address = stringToLower(addr);

      int status = 0;

      for(size_t i = 0; i < m_registerName.size();i++){
        if(company_to_find.m_name == m_registerName[i].m_name && company_to_find.m_address == m_registerName[i].m_address){
          company_to_find.m_ID = m_registerName[i].m_ID;
          m_registerName.erase(m_registerName.begin() + i);
          status ++;
          break; 
        }
      }

      for(size_t i = 0; i < m_registerID.size();i++){
        if(company_to_find.m_name == m_registerID[i].m_name && company_to_find.m_address == m_registerID[i].m_address){
          m_registerID.erase(m_registerID.begin() + i);
          status ++;
          break; 
        }
      }

      if (status != 2) {
        cout << "cancelCompany 2 FAILED!" << endl;
        return false;
      }

      return true;
    }


    bool cancelCompany  ( const string & taxID ) {

      TCompany company_to_find;
      company_to_find.m_ID = taxID;

      int status = 0;

      auto iterator = lower_bound(  m_registerID.begin(), m_registerID.end(), company_to_find, comparatorID );
      if ( iterator->m_ID == taxID ) {
        company_to_find.m_name = iterator->m_name;
        company_to_find.m_address = iterator->m_address;
        m_registerID.erase(iterator);
        status++;
      }

      iterator = lower_bound(  m_registerName.begin(), m_registerName.end(), company_to_find, comparatorName );
      if ( iterator->m_ID == taxID ) {
        m_registerName.erase(iterator);
        status++;
      }

      if (status != 2) {
        cout << "cancelCompany 1 FAILED!" << endl;
        return false;
      }

      return true;
    }

    bool invoice ( const string & taxID, unsigned int amount ) {

      TCompany company_to_find;
      company_to_find.m_ID = taxID;
      int status = 0;

      auto iterator = lower_bound(  m_registerID.begin(), m_registerID.end(), company_to_find, comparatorID );
      if ( iterator->m_ID == taxID ) {
        company_to_find.m_name = iterator->m_name;
        company_to_find.m_address = iterator->m_address;
        iterator->earnings += amount;
        status++;
      }

      iterator = lower_bound(  m_registerName.begin(), m_registerName.end(), company_to_find, comparatorName );
      if ( iterator->m_ID == taxID ) {
        iterator->earnings += amount;
        status++;
      }

      if ( status != 2 ) {
        cout << "Invoice 1 FAILED!" << endl;
        return false;
      } 

      invoices.insert( lower_bound( invoices.begin(), invoices.end(), amount ), amount );
      return true;
    }
    bool invoice ( const string & name, const string & addr, unsigned int amount ) {

      TCompany company_to_find;
      company_to_find.m_name = stringToLower(name);
      company_to_find.m_address = stringToLower(addr);
      int status = 0;

      auto iterator = lower_bound(  m_registerName.begin(), m_registerName.end(), company_to_find, comparatorName );
      //cout << iterator->m_name << " " << iterator->m_address << endl;
      //cout << company_to_find.m_name << " " << company_to_find.m_address << endl;
    
      if ( iterator->m_name == company_to_find.m_name && iterator->m_address == company_to_find.m_address ) {

        company_to_find.m_ID = iterator->m_ID;
        iterator->earnings += amount;
        status++;
      }

      iterator = lower_bound(  m_registerID.begin(), m_registerID.end(), company_to_find, comparatorID );
      if ( iterator->m_ID == company_to_find.m_ID ) {
        iterator->earnings += amount;
        status++;
      }

      cout << status << endl;

      if (status != 2) {
        cout << "Invoice 2 FAILED!" << endl;
        return false;
      }

      invoices.insert( lower_bound( invoices.begin(), invoices.end(), amount ), amount );
      return true;
    }

    bool audit ( const string & name, const string & addr, unsigned int & sumIncome ) const {

      TCompany company_to_find;
      company_to_find.m_name = stringToLower(name);
      company_to_find.m_address = stringToLower(addr);

      auto iterator = lower_bound(  m_registerName.begin(), m_registerName.end(), company_to_find, comparatorName );
      if ( iterator->m_name == company_to_find.m_name && iterator->m_address == company_to_find.m_address ) {
        sumIncome = iterator->earnings;
        cout << "sumIncome = " << sumIncome << endl;
        return true;
      }
      
      return false;
    }
    bool audit ( const string & taxID, unsigned int & sumIncome ) const {
      TCompany company_to_find;
      company_to_find.m_ID = taxID;

      auto iterator = lower_bound(  m_registerID.begin(), m_registerID.end(), company_to_find, comparatorID );
      if ( iterator->m_ID == taxID ) {
        sumIncome = iterator->earnings;
        cout << "sumIncome = " << sumIncome << endl;
        return true;
      }
      return false;
    }

    bool firstCompany ( string & name, string & addr ) const {
      if ( (int)m_registerID.size() == 0 || (int)m_registerName.size() == 0  ) return false;

      name = m_registerName[0].m_unchanged_name;
      addr = m_registerName[0].m_unchanged_address;

      return true;
    }

    bool nextCompany ( string & name, string & addr ) const {

      if ( (int)m_registerID.size() == 0 || (int)m_registerName.size() == 0  ) return false;

      TCompany company_to_find;
      int size = m_registerName.size();

      company_to_find.m_name = stringToLower(name);
      company_to_find.m_address = stringToLower(addr);

      auto iterator = lower_bound(  m_registerName.begin(), m_registerName.end(), company_to_find, comparatorName );
      
      if ( iterator->m_ID == m_registerName[size-1].m_ID || iterator->m_unchanged_name == m_registerName[size-1].m_unchanged_name) return false;

      cout << m_registerName[size-1].m_unchanged_name << " " << m_registerName[size-1].m_unchanged_address << endl;

      auto new_iterator = next(iterator, 1); 

      cout << new_iterator->m_unchanged_name << " " << new_iterator->m_unchanged_address << endl;

      name = new_iterator->m_unchanged_name;
      addr = new_iterator->m_unchanged_address;
      return true;
    }

    unsigned int medianInvoice ( void ) const {

      int size = (int)invoices.size();

      if ( size == 0 ) return 0;

      return invoices[size/2];
    }

    void printVectors( void ) {
      cout << "\n";
      printRegisterName();
      printRegisterID();
      cout << "\n";
    }


  
  private:
    struct TCompany {
      string m_name;
      string m_address;
      string m_ID;
      unsigned int earnings = 0;
      string m_unchanged_name;
      string m_unchanged_address;
    };

    vector<TCompany> m_registerName;
    vector<TCompany> m_registerID;
    vector<unsigned int> invoices;


    static bool comparatorID(const TCompany & comp1, const TCompany & comp2) {
      return comp1.m_ID < comp2.m_ID;
    }
    
    static bool comparatorName(const TCompany & comp1, const TCompany & comp2) {
      if ( comp1.m_name == comp2.m_name ){
        return comp1.m_address < comp2.m_address;
      }
      else
        return comp1.m_name < comp2.m_name;
    }

    void printRegisterName ( void ) {
      cout << "-----------------------------------NAME---------------------------------------" << endl;
      for ( auto i = 0; i < (int)m_registerName.size(); i++ ){
        cout << "Name = " << m_registerName[i].m_unchanged_name << " Address = " <<  m_registerName[i].m_unchanged_address << " ID = " << m_registerName[i].m_ID << " Amount = " << m_registerName[i].earnings << endl;
      }
    }

    void printRegisterID ( void ) {
      cout << "------------------------------------ID----------------------------------------" << endl;
      for ( auto i = 0; i < (int)m_registerID.size(); i++ ){
        cout << "Name = " << m_registerID[i].m_name << " Address = " <<  m_registerID[i].m_address << " ID = " << m_registerID[i].m_ID << " Amount = " << m_registerID[i].earnings << endl;
      }
      cout << "------------------------------------------------------------------------------" << endl;
    }

    static string stringToLower ( string name ) {
      for ( auto i = 0; i < (int)name.size(); i++ ){
        name[i] = tolower(name[i]);
      }
      return name;
    }
};

#ifndef __PROGTEST__
int               main           ( void )
{
  string name, addr;
  unsigned int sumIncome;
  CVATRegister b1;
/*
  b1 . newCompany ( "A", "Thakurova", "666/666" );
  b1 . printVectors();
  b1 . newCompany ( "m", "Tahakurova", "666/666" );
  b1 . newCompany ( "A", "Thakurova", "666/666" );
  b1 . newCompany ( "A", "Thaakurova", "6661/666" );
  b1 . newCompany ( "E", "Kolejni", "566/666/666");
  b1 . newCompany ( "B", "Kolejni", "166/666/666");
  b1 . newCompany ( "C", "Kolejni", "266/666/666");
  b1 . newCompany ( "D", "Kolejni", "4366/666/666");
  b1 . newCompany ( "F", "Kolejni", "666/666/666");
  b1 . printVectors();
  b1 . cancelCompany( "B", "Kolejni" );
  b1 . cancelCompany( "666/666" );
  b1 . cancelCompany( "666/666" );
  b1 . invoice ( "666/666", 100 );
  b1 . invoice ( "566/666/666", 100 );
  b1 . invoice ( "B", "Kolejni", 10000 );
  b1 . invoice ( "A", "Thaakurova", 10000 );
  b1 . printVectors();
  b1 . audit ( "A", "Thaakurova", sumIncome );
  b1 . audit ( "6661/666", sumIncome );
*/
  assert ( b1 . newCompany ( "ACME", "Thakurova", "666/666" ) );
  assert ( b1 . newCompany ( "ACME", "Kolejni", "666/666/666" ) );
  assert ( b1 . newCompany ( "Dummy", "Thakurova", "123456" ) );
  assert ( b1 . invoice ( "666/666", 2000 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "666/666/666", 3000 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 4000 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "aCmE", "Kolejni", 5000 ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . audit ( "ACME", "Kolejni", sumIncome ) && sumIncome == 8000 );
  assert ( b1 . audit ( "123456", sumIncome ) && sumIncome == 4000 );
  b1 . printVectors();
  assert ( b1 . firstCompany ( name, addr ) && name == "ACME" && addr == "Kolejni" );
  assert ( b1 . nextCompany ( name, addr ) && name == "ACME" && addr == "Thakurova" );
  assert ( b1 . nextCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
  b1 . printVectors();
  assert ( ! b1 . nextCompany ( name, addr ) );
  assert ( b1 . cancelCompany ( "ACME", "KoLeJnI" ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . cancelCompany ( "666/666" ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . invoice ( "123456", 100 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 300 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 200 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 230 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 830 ) );
  assert ( b1 . medianInvoice () == 830 );
  assert ( b1 . invoice ( "123456", 1830 ) );
  assert ( b1 . medianInvoice () == 1830 );
  assert ( b1 . invoice ( "123456", 2830 ) );
  assert ( b1 . medianInvoice () == 1830 );
  assert ( b1 . invoice ( "123456", 2830 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 3200 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . firstCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
  assert ( ! b1 . nextCompany ( name, addr ) );
  assert ( b1 . cancelCompany ( "123456" ) );
  assert ( ! b1 . firstCompany ( name, addr ) );


  CVATRegister b2;
  assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
  assert ( b2 . newCompany ( "Dummy", "Kolejni", "123456" ) );
  assert ( ! b2 . newCompany ( "AcMe", "kOlEjNi", "1234" ) );
  assert ( b2 . newCompany ( "Dummy", "Thakurova", "ABCDEF" ) );
  assert ( b2 . medianInvoice () == 0 );
  assert ( b2 . invoice ( "ABCDEF", 1000 ) );
  assert ( b2 . medianInvoice () == 1000 );
  assert ( b2 . invoice ( "abcdef", 2000 ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( b2 . invoice ( "aCMe", "kOlEjNi", 3000 ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( ! b2 . invoice ( "1234567", 100 ) );
  assert ( ! b2 . invoice ( "ACE", "Kolejni", 100 ) );
  assert ( ! b2 . invoice ( "ACME", "Thakurova", 100 ) );
  assert ( ! b2 . audit ( "1234567", sumIncome ) );
  assert ( ! b2 . audit ( "ACE", "Kolejni", sumIncome ) );
  assert ( ! b2 . audit ( "ACME", "Thakurova", sumIncome ) );
  assert ( ! b2 . cancelCompany ( "1234567" ) );
  assert ( ! b2 . cancelCompany ( "ACE", "Kolejni" ) );
  assert ( ! b2 . cancelCompany ( "ACME", "Thakurova" ) );
  assert ( b2 . cancelCompany ( "abcdef" ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( ! b2 . cancelCompany ( "abcdef" ) );
  assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
  assert ( b2 . cancelCompany ( "ACME", "Kolejni" ) );
  assert ( ! b2 . cancelCompany ( "ACME", "Kolejni" ) );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
