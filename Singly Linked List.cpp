#include <iostream>
#include <string>
#include <exception>
#include <utility>
#include <typeinfo>
using namespace std;

template <typename Tip> class Lista {
  void operator=(const Lista &);
  Lista(const Lista &);

public:
  Lista() {}
  virtual ~Lista() {}
  virtual int brojElemenata() const = 0;

  virtual Tip &trenutni() const = 0;
  virtual bool prethodni() = 0;
  virtual bool sljedeci() = 0;
  virtual void pocetak() = 0;
  virtual void kraj() = 0;
  virtual void obrisi() = 0;
  virtual void dodajIspred(const Tip &el) = 0;
  virtual void dodajIza(const Tip &el) = 0;
 
  virtual  Tip &operator[](int i)  = 0;
  virtual  const Tip &operator[](int i) const = 0;
};

template <typename tip>
class NizLista:public Lista<tip>{
int duzina,tekuci, kapacitet;
tip** niz;
void Prosirikapacitet(){
    if(duzina==kapacitet){
        auto pomocni=new tip*[2*kapacitet]{};
        try{
            
            for(int i=0;i<kapacitet;i++)
            pomocni[i]=new tip(*niz[i]);
        }
        catch(...){
            std::cout<<"Fali memorije";
            for(int i=0;i<kapacitet;i++)delete pomocni[i];
            delete[] pomocni;
            return;
        }
        for(int i=0;i<kapacitet;i++)delete niz[i];
            delete[] niz;
            niz=pomocni;
            kapacitet*=2;



    }




}


public:
NizLista():duzina(0),tekuci(0),kapacitet(100000),niz(new tip*[kapacitet]{}){};
   ~NizLista(){
       for(int i=0;i<kapacitet;i++)delete niz[i];
            delete[] niz;
   }
   NizLista& operator=(const NizLista&l){
       
          auto pomocni=new tip*[l.kapacitet]{};
           try{
               for(int i=0;i<l.duzina;i++)
               pomocni[i]=new tip(*l.niz[i]);
       }
       catch(...){
           for(int i=0;i<l.duzina;i++)
               delete pomocni[i];
               delete[] pomocni;
               throw;
       }
        for(int i=0;i<l.duzina;i++)
               delete niz[i];
               delete[] niz;
               niz=pomocni;
                duzina=l.duzina;
                kapacitet=l.kapacitet;
                tekuci=0;

   return *this;
   }
   NizLista(const NizLista&l){
     niz=new tip*[l.kapacitet]{};
           try{
               for(int i=0;i<l.duzina;i++)
               niz[i]=new tip(*l.niz[i]);
       }
       catch(...){
           for(int i=0;i<l.duzina;i++)
               delete niz[i];
               delete[] niz;
               throw;
       }
       duzina=l.duzina;
                kapacitet=l.kapacitet;
                tekuci=0;

   }
   const tip &operator[](int i) const override{
     if(i>=duzina||i<0)throw std::domain_error("Neispravan index");
      return *niz[i];


   }
     int brojElemenata() const override{return duzina;}
  tip &trenutni() const override {if(duzina==0)throw std::domain_error("Lista prazna");
      return *niz[tekuci];}
      void Ispisi()const{
          for(int i=0;i<=duzina-1;i++)std::cout<<*niz[i]<<" ";
      }

  bool prethodni()override {if(duzina==0)throw std::domain_error("Lista prazna");
      if(tekuci!=0){tekuci--;return true;}return false;}
   bool sljedeci()override {if(duzina==0)throw std::domain_error("Lista prazna");
       if(tekuci!=duzina-1){tekuci++;return true;}return false;}
  void pocetak()override {if(duzina==0)throw std::domain_error("Lista prazna");tekuci=0;}
   void kraj() override {if(duzina==0)throw std::domain_error("Lista prazna");tekuci=duzina-1;}
  void obrisi() override{if(duzina==0)throw std::domain_error("Lista prazna");
      for(int i=tekuci;i<duzina-1;i++)*niz[i]=*niz[i+1];
      if(tekuci==duzina-1)tekuci--;
      duzina--;
  }
  void dodajIspred(const tip &el) override;
  void dodajIza(const tip &el)override ;
   tip &operator[](int i) override {
      if(i>=duzina||i<0)throw std::domain_error("Neispravan index");
      return *niz[i];
  }

};
            template<typename tip>
    void NizLista<tip>::dodajIspred(const tip &el){
Prosirikapacitet();
if(duzina==0){niz[tekuci]=new tip(el);duzina++;return;}
        for(int i=duzina;i>tekuci;i--){
            niz[i]=niz[i-1];
        }
        niz[tekuci]=new tip(el);
        tekuci++;
        duzina++;

    }
         template<typename tip>
    void NizLista<tip>::dodajIza(const tip &el){
        Prosirikapacitet();
if(duzina==0){niz[tekuci]=new tip(el);duzina++;return;}
for(int i=duzina;i>tekuci+1;i--){
            niz[i]=niz[i-1];
        }

niz[tekuci+1]=new tip(el);

duzina++;

    }

    

    template <typename tip>
    class JednostrukaLista:public Lista<tip>{

  struct Cvor {
    tip x;
    Cvor *sljedeci;
    Cvor(const tip &element, Cvor *sljedeci)
        : x(element), sljedeci(sljedeci) {}
  };
  int lduzina, dduzina,duzina;
  Cvor *prvi;
  Cvor *zadnji;
  Cvor *tekuci;
  
  
 public:
JednostrukaLista():duzina(0),tekuci(nullptr),dduzina(0),lduzina(0),prvi(nullptr),zadnji(nullptr){};
  ~JednostrukaLista(){
      for(auto p=prvi;p!=zadnji;){
          auto pomocna=p->sljedeci;
          delete p;
          p=pomocna;
      }
      
      prvi=nullptr;zadnji=nullptr;tekuci=nullptr;lduzina=0;dduzina=0;duzina=0;
  }



void Metoda3(Cvor*b){
    int suma=0;
    while(prvi!=b->sljedeci){
        suma+=prvi->x;
        if(prvi==trenutni())trenutni()=kraj();
        auto pom=prvi->sljedeci;
        delete prvi;
        prvi=pom;
    }
    zadnji->sljedeci=new Cvor{suma,nullptr};
    zadnji=zadnji->sljedeci;
    
}

void metoda4(int n){

    int i=0;
    while(i<n){
        auto pomocna=prvi->sljedeci;
        delete prvi;
        prvi=pomocna;
        duzina--;
        i++;
    }
    auto pomocna=prvi;
    i=0;
    while(i<n-1){
        pomocna=pomocna->sljedeci;
        i++;

    }
    zadnji=pomocna;
    pomocna=pomocna->sljedeci;
    zadnji->sljedeci=nullptr;
    while(pomocna!=nullptr){
        auto pom2=pomocna->sljedeci;
        delete pomocna;
        pomocna=pom2;
    }
}
  JednostrukaLista(const JednostrukaLista& l) {
    if (l.prvi == nullptr) {  // Ako je lista prazna
        prvi = zadnji = nullptr;
        tekuci = nullptr;
        duzina = lduzina = dduzina = 0;
        return;
    }
    if(l.prvi==l.zadnji){
         prvi = zadnji = new Cvor(l.prvi->x,nullptr);
        tekuci = nullptr;
        duzina = dduzina=1;
        lduzina=0;
        return;
    }
    
    Cvor* pomocna = nullptr;

    for (auto p = l.prvi; p != l.zadnji; p = p->sljedeci) {
        if (prvi == nullptr) {  // Prvi element kopiramo posebno
            prvi = new Cvor(p->x, nullptr);
            pomocna = prvi;
        } else {  // Kopiramo naredne elemente
            pomocna->sljedeci = new Cvor(p->x, nullptr);
            pomocna = pomocna->sljedeci;
        }
    }

    // Na kraju postavimo zadnji čvor
    zadnji = new Cvor(l.zadnji->x,nullptr);
    tekuci = nullptr;
    lduzina = 0;
    dduzina = l.duzina;
    duzina = l.duzina;
}

   
const tip & operator[](int i) const override {
  if (i == 0)return prvi->x;
  auto p = prvi;
  while (i) {
    p = p->sljedeci;
    i--;
  }
  return p->x;
}
     int brojElemenata() const override{return duzina;}
  tip &trenutni() const override {
    if (brojElemenata() == 0)
      throw "Nista za vratiti!\n";
     if (tekuci == 0) return prvi->x;
    return tekuci->x;
  }  
    JednostrukaLista& operator=(const JednostrukaLista &jednostrukaLista) {
  if (&jednostrukaLista == this)
    return *this;
  if (duzina) {
    pocetak();
    while (brojElemenata())
      obrisi();
  }
  Cvor *p(jednostrukaLista.prvi);
  while (p != 0) {
    dodajIza(p->x);
    p = p->sljedeci;
  }
  lduzina = jednostrukaLista.lduzina;
  dduzina = jednostrukaLista.dduzina;
  return *this;
}



  bool prethodni()override {if(duzina==0)throw std::domain_error("Lista prazna");
        Cvor*p;
        if(lduzina==0)return false;
        if(lduzina==1){tekuci=nullptr;
         lduzina--;
        dduzina++;return true;}
        for( p=prvi;p->sljedeci!=tekuci;p=p->sljedeci){
        }
        tekuci=p;
        lduzina--;
        dduzina++;
        return true;
     }
   bool sljedeci()override {
       if(duzina==0)throw std::domain_error("Lista prazna");
       if(dduzina==0)return false;
       tekuci=tekuci->sljedeci;

       dduzina--;
       lduzina++;
       return true;
       
       
       
       }
  void pocetak()override {if(duzina==0)throw std::domain_error("Lista prazna");tekuci=nullptr;}
   void kraj() override {if(duzina==0)throw std::domain_error("Lista prazna");tekuci=zadnji;}
  void obrisi() override{if(duzina==0)throw std::domain_error("Lista prazna");
       auto p=tekuci->sljedeci->sljedeci;
       delete tekuci->sljedeci;
       tekuci->sljedeci=p;
  }
  void dodajIspred(const tip &el) override;
  void dodajIza(const tip &el)override ;
    tip & operator[](int i)  override {
  if (i == 0)
    return prvi->x;
  auto p = prvi;
  while (i) {
    p = p->sljedeci;
    i--;
  }
  return p->x;
}


    };
    template <typename Tip>
void JednostrukaLista<Tip>::dodajIza(const Tip &element) {
  Cvor *p = new Cvor(element, 0);
  if (!brojElemenata()) {
    prvi = p;
    zadnji = p;
    tekuci = nullptr;
  } else if (dduzina == 0) {
    tekuci->sljedeci = p;
    zadnji = p;
  } else {
    p->sljedeci = tekuci->sljedeci;
    tekuci->sljedeci = p;
  }
  dduzina++;
  duzina++;
}
template <typename Tip>
void JednostrukaLista<Tip>::dodajIspred(const Tip &element) {
  Cvor *p = new Cvor(element, 0);
  if (duzina == 0) {
    prvi = p;
    zadnji = p;
    tekuci = p;
  } else if (tekuci == nullptr) {
    p->sljedeci = prvi;
    prvi = p;
  } else {
    auto temp = prvi;
    int brojac = lduzina - 1;
    while (brojac > 1) {
      brojac--;
      temp = temp->sljedeci;
    }
    temp->sljedeci = p;
    p->sljedeci = tekuci;
  }
  lduzina++;
  duzina++;
}


template <typename Tip> void TestbrojElemenata(const NizLista<Tip> &lista) {
  std::cout << lista.brojElemenata() << std::endl;
}

/*template <typename Tip> void Testtrenutni(const NizLista<Tip> &lista) {
  std::cout << lista.trenutni() << std::endl;
}*/

template <typename Tip> void Testtrenutni(NizLista<Tip> &lista, Tip el) {
  lista.trenutni() = el;
  std::cout << lista.trenutni() << std::endl;
}

template <typename Tip> void Testprethodni(NizLista<Tip> &lista) {
  lista.prethodni();
  std::cout << lista.trenutni() << std::endl;
}

template <typename Tip> void Testsljedeci(NizLista<Tip> &lista) {
  lista.sljedeci();
  std::cout << lista.trenutni() << std::endl;
}

template <typename Tip> void Testpocetak(NizLista<Tip> &lista) {
  lista.pocetak();
  std::cout << lista.trenutni() << std::endl;
}

template <typename Tip> void Testkraj(NizLista<Tip> &lista) {
  lista.kraj();
  std::cout << lista.trenutni() << std::endl;
}

template <typename Tip> void Testobrisi(NizLista<Tip> &lista) {
  lista.obrisi();
  std::cout << lista.trenutni() << std::endl;
}

template <typename Tip>
void TestdodajIspred(NizLista<Tip> &lista, const Tip &el) {
  lista.dodajIspred(el);
  lista.prethodni();
  std::cout << lista.trenutni() << std::endl;
}

template <typename Tip> void TestdodajIza(NizLista<Tip> &lista, const Tip &el) {
  lista.dodajIza(el);
  lista.sljedeci();
  std::cout << lista.trenutni() << std::endl;
}






int main() {
JednostrukaLista<int> lista;
for (int i(1); i<=5; i++)
	lista.dodajIspred(i);
{
    JednostrukaLista<int> lista2(lista);
    JednostrukaLista<int> lista3;

    
    
    
}
std::cout << lista.brojElemenata();

    return 0;
}