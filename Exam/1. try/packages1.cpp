/**
 * Úkolem je vytvořit systém, který bude spravovat instalaci balíčků.
 * Každý balíček je instanci třídy CPkg, která drží své unikátní jméno. Tato třída má metodu addDep, která přidá k balíčku název balíčku, na kterém závisi.
 * Pokud dostaneme již obsazený balíček, metoda neudělá nic.
 *
 * Třída CPkgSys pak eviduje všechny balíčky, metoda addPkg přidá balíček do seznamu dostupných balíčků(bez instalace).
 * Následně metoda install(list<string> ...) instaluje balíčky ze seznamu včetně všech závislostí rekurzivně (pokud ještě nejsou nainstalované).
 * Pokud se při insatalaci vyhodí výjimka, nic se nenainstaluje.
 * Jestliže je nutný instalovat balíček s neznámým názvem, metoda install vyhodí výjimku invalid_argument("Package not found.").
 * Krom toho je nutno implementovat operátor <<, který vypíše všechny již instalované balíčky v abecedním pořadí.
 */

#include<iostream>
#include<cassert>
#include<vector>
#include<set>
#include<map>
#include<string>
#include<unordered_map>
#include<memory>
#include<unordered_set>
#include<queue>
#include<list>
#include<sstream>
#include<cstring>

using namespace std;

class CPkg{
public:

    //TODO constructor
    CPkg( string nm )
            : name(nm)
    {}

    //TODO addDep(string)
    CPkg& addDep(string depName) {
        MustDependencies.insert(depName);
        return *this;
    }

    CPkg& addMayDep(string depName) {
        MayDependencies.insert(depName);
        return *this;
    }

    string name;
    set<string> MustDependencies;
    set<string> MayDependencies;
private:
};

class CPkgSys{
public:
    //TODO addPkg
    CPkgSys& addPkg(CPkg input);

    //TODO install(list<string> )
    set<string> install(list<string> input);

    //TODO operator <<
    friend ostream & operator << (ostream & os, const CPkgSys & src) {
        size_t count = 1;
        for ( auto& pck : src.installedPkg ) {
            os << pck;
            if ( count != src.installedPkg.size() )
                os << ", ";
            count ++;
        }
        return os;
    }

private:
    unordered_multimap <string, CPkg> packages;
    set<string> installedPkg;
};

CPkgSys& CPkgSys::addPkg(CPkg input) {

    if ( packages.find(input.name) == packages.end() ) {
        packages.insert( make_pair(input.name, input) );
    }
    return *this;
}

set<string> CPkgSys::install(list<string> input) {

    set<string> result;
    queue<string> Q;
    set<string> currInstalled;

    set<string>& installed = installedPkg;

    if ( input.empty() ) return result = {};
    for ( auto& pkg : input ) {
        Q.push(pkg);
    }

    while( !Q.empty() ) {

        string curr = Q.front();
        Q.pop();


        if (installed.count(curr) == 1 || currInstalled.count(curr) == 1 ) {
            continue;
        }

        auto pkg = packages.find(curr);
        if ( pkg == packages.end() ) throw invalid_argument("Package not found.");

        currInstalled.insert(curr);
        result.insert(curr);

        auto dep = pkg->second.MustDependencies;
        for( auto& p : dep )
            Q.push(p);
        dep = pkg->second.MayDependencies;
        for( auto& p : dep )
            Q.push(p);
    }
    for ( auto& pkg : currInstalled )
        installed.insert(pkg);
    return result;
}

int main(void){
    CPkgSys s;
    stringstream ss;


    s.addPkg(CPkg("ssh").addDep("sudo").addDep("apt"))
            .addPkg(CPkg("sudo").addDep("git").addDep("c++"));
    s.addPkg(CPkg("apt"))
            .addPkg(CPkg("c++").addDep("c").addDep("asm").addDep("fortran"));
    s.addPkg(CPkg("git"))
            .addPkg(CPkg("c").addDep("kekw"))
            .addPkg(CPkg("kekw"))
            .addPkg(CPkg("asm"))
            .addPkg(CPkg("fortran"));
    s.addPkg(CPkg("python").addDep("bash").addDep("sadge"))
            .addPkg(CPkg("karel").addDep("python"))
            .addPkg(CPkg("bash").addDep("sadge"))
            .addPkg(CPkg("sadge"))
            .addPkg(CPkg("cython").addDep("dev"));
    s.addPkg(CPkg("perl"));

    ss << s;
    assert(ss.str() == "");
    ss.clear();
    ss.str("");

    set<string> t1 = s.install(list<string> {"sudo"});
    assert(t1 == (set<string> {"asm", "c", "c++", "fortran", "git", "kekw", "sudo"}));
    set<string> t2 = s.install(list<string> {"ssh", "c++"});
    assert(t2 == (set<string> {"apt", "ssh"}));

    ss << s;
    assert(ss.str() == "apt, asm, c, c++, fortran, git, kekw, ssh, sudo");
    ss.clear();
    ss.str("");

    try{
        set<string> t3 = s.install(list<string> {"karel", "cython"});
        assert("Sem ses nemel dostat debilku" == nullptr);
    }
    catch(const invalid_argument & e){
        assert(strcmp("Package not found.", e.what()) == 0);
    }
    set<string> t3 = s.install(list<string> {"karel", "fortran", "git"});
    assert(t3 == (set<string> {"bash", "karel", "python", "sadge"}));

    s.addPkg(CPkg("java").addDep("utils"))
            .addPkg(CPkg("utils").addDep("VB"))
            .addPkg(CPkg("VB").addDep("java"));

    set<string> t4 = s.install(list<string> {"java", "perl"});
    assert(t4 == (set<string> {"VB", "java", "perl", "utils"}));

    ss << s;
    assert(ss.str() == "VB, apt, asm, bash, c, c++, fortran, git, java, karel, kekw, perl, python, sadge, ssh, sudo, utils");
    ss.clear();
    ss.str("");

    CPkgSys k;
    k.addPkg(CPkg("ssh").addDep("sudo").addDep("apt"))
            .addPkg(CPkg("sudo").addDep("git"));
    k.addPkg(CPkg("apt"));
    k.addPkg(CPkg("git"))
            .addPkg(CPkg("c").addDep("kekw"))
            .addPkg(CPkg("kekw"))
            .addPkg(CPkg("asm"))
            .addPkg(CPkg("fortran"));
    k.addPkg(CPkg("python").addDep("bash").addDep("sadge"))
            .addPkg(CPkg("karel").addDep("python"))
            .addPkg(CPkg("bash").addDep("sadge"))
            .addPkg(CPkg("sadge"));
    k.addPkg(CPkg("perl").addDep("no"));

    set<string> t5 = k.install(list<string> {"asm"});
    assert(t5 == (set<string> {"asm"}));
    set<string> t6 = k.install(list<string> {"python", "ssh"});
    assert(t6 == (set<string> {"apt", "bash", "git", "python", "sadge", "ssh", "sudo"}));

    try{
        set<string> t7 = k.install(list<string> {"perl", "c"});
        assert("Sem ses nemel dostat debilku" == nullptr);
    }
    catch(const invalid_argument & e){
        assert(strcmp("Package not found.", e.what()) == 0);
    }
    set<string> t8 = k.install(list<string> {"c", "ssh", "karel"});

    assert(t8 == (set<string> {"c", "karel", "kekw"}));

    ss << k;
    assert(ss.str() == "apt, asm, bash, c, git, karel, kekw, python, sadge, ssh, sudo");
    ss.clear();
    ss.str("");
    k.addPkg(CPkg("lol")).addPkg(CPkg("wow"))
     .addPkg(CPkg("mc").addMayDep("wow").addDep("lol"));
    k.install(list<string> {"mc"});
    ss << k;
    assert(ss.str() == "apt, asm, bash, c, git, karel, kekw, lol, mc, python, sadge, ssh, sudo, wow");
}