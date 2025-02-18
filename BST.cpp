#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

template <typename TipKljuca, typename TipVrijednosti> class Mapa {
  void operator=(const Mapa &);
  Mapa(const Mapa &);

public:
  using par = pair<TipKljuca, TipVrijednosti>;
  Mapa() {}
  virtual const TipVrijednosti operator[](TipKljuca) const = 0;
  virtual TipVrijednosti &operator[](TipKljuca) = 0;
  virtual int brojElemenata() const = 0;
  virtual void obrisi() = 0;
  virtual void obrisi(const TipKljuca &kljuc) = 0;

  virtual ~Mapa() {}
};

template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
  using par = pair<TipKljuca, TipVrijednosti>;
  class BSCvor {
  public:
    BSCvor() { lijevo = desno = nullptr; }
    BSCvor(const TipKljuca &kl, const TipVrijednosti &vr, BSCvor *l = nullptr,
           BSCvor *r = nullptr) {
      kljuc.first = kl;
      kljuc.second = vr;
      lijevo = l;
      desno = r;
    }
    par kljuc;
    BSCvor *lijevo, *desno, *roditelj;
  };
  BSCvor *korijen;
  int velicina;
  void preorder(BSCvor *r, BSCvor *&temp) {
    if (r != 0) {
      temp = new BSCvor(r->kljuc.first, r->kljuc.second);
      velicina++;
      preorder(r->lijevo, temp->lijevo);
      preorder(r->desno, temp->desno);
    }
  }
  void Brisi(BSCvor *korijen) {
    if (korijen) {
      Brisi(korijen->lijevo);
      Brisi(korijen->desno);
      delete korijen;
    }
    velicina = 0;
    this->korijen = nullptr;
  }

public:
  BinStabloMapa() {
    korijen = nullptr;
    velicina = 0;
  }
  ~BinStabloMapa() { obrisi(); }
  BinStabloMapa<TipKljuca, TipVrijednosti> &
  operator=(const BinStabloMapa &stablo);
  BinStabloMapa(const BinStabloMapa &stablo);
  const TipVrijednosti operator[](TipKljuca) const;
  TipVrijednosti &operator[](TipKljuca);
  int brojElemenata() const { return velicina; }
  void obrisi(const TipKljuca &kljuc);
  void obrisi() { Brisi(korijen); }

  bool operator<(BinStabloMapa<TipKljuca, TipVrijednosti> b2){
      if(max()<b2.min())return true;
      return false;

  }
  TipVrijednosti max(){
      auto p=korijen;
      if(p==nullptr)return TipVrijednosti();
      while(p->desno!=nullptr)p=p->desno;
      
      return p->kljuc.second;
  }
  TipVrijednosti min(){
      auto p=korijen;
      if(p==nullptr)return 0;
      while(p->lijevo!=nullptr)p=p->lijevo;
      
      return p->kljuc.second;
  }

 TipVrijednosti prviManji(const TipKljuca& kljuc) {
     auto p=korijen;
     while(p->desno!=nullptr&&p->desno.kljuc.first)p=p->desno;
     return p->kljuc.second;
 }
 // pogledati ovo
/*void inorder(const TipKljuca&a,const TipKljuca&b,std::vector<TipVrijednosti>&v,BSCvor*p){
    if(p->kljuc.second>a&&)
}
 std::vector<TipVrijednosti> od_do(const TipKljuca& a, const TipKljuca& b){
     std::vector<TipVrijednosti>v;
     auto p=korijen;
     inorder(a,b,v,p);
     return v;
 }*/

};

template <typename TipKljuca, typename TipVrijednosti>
BinStabloMapa<TipKljuca, TipVrijednosti> &
BinStabloMapa<TipKljuca, TipVrijednosti>::operator=(
    const BinStabloMapa &stablo) {
  if (korijen == stablo.korijen)
    return *this;
  obrisi();
  // korijen = nullptr;
  // BinStabloMapa();
  // korijen = stablo.korijen;
  preorder(stablo.korijen, korijen);
  return *this;
}

template <typename TipKljuca, typename TipVrijednosti>
BinStabloMapa<TipKljuca, TipVrijednosti>::BinStabloMapa(
    const BinStabloMapa &stablo) {
  BinStabloMapa();
  velicina = 0;
  // korijen = stablo.korijen;
  // BSCvor *temp = korijen;
  preorder(stablo.korijen, korijen);
}

template <typename TipKljuca, typename TipVrijednosti>
void BinStabloMapa<TipKljuca, TipVrijednosti>::obrisi(const TipKljuca &kljuc) {
  ;
  BSCvor *p = korijen, *q = 0;
  BSCvor *tmp = 0, *pp = 0, *rp = 0;
  while (p != 0 && kljuc != p->kljuc.first) {
    q = p;
    if (kljuc < p->kljuc.first)
      p = p->lijevo;
    else
      p = p->desno;
  }
  if (p == 0)
    return;
  if (p->lijevo == 0)
    rp = p->desno;
  else if (p->desno == 0)
    rp = p->lijevo;
  else {
    pp = p;
    rp = p->lijevo;
    tmp = rp->desno;
    while (tmp != 0) {
      pp = rp;
      rp = tmp;
      tmp = rp->desno;
    }
    if (pp != p) {
      pp->desno = rp->lijevo;
      rp->lijevo = p->lijevo;
    }
    rp->desno = p->desno;
  }
  if (q == 0)
    korijen = rp;
  else if (p == q->lijevo)
    q->lijevo = rp;
  else
    q->desno = rp;
  delete p;
  velicina--;
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti &
BinStabloMapa<TipKljuca, TipVrijednosti>::operator[](TipKljuca kl) {
  BSCvor *y = 0, *x = korijen;
  while (x != 0) {
    y = x;
    if (kl < x->kljuc.first)
      x = x->lijevo;
    else if (kl > x->kljuc.first)
      x = x->desno;
    else
      return x->kljuc.second;
  }
  if (y == 0) {
    korijen = new BSCvor(kl, TipVrijednosti());
    velicina++;
    return korijen->kljuc.second;
  } else {
    if (kl < y->kljuc.first) {
      y->lijevo = new BSCvor(kl, TipVrijednosti());
      velicina++;
      return y->lijevo->kljuc.second;
    } else {
      y->desno = new BSCvor(kl, TipVrijednosti());
      velicina++;
      return y->desno->kljuc.second;
    }
  }

  /*if (!korijen) {
    korijen = new BSCvor(kl, TipVrijednosti());
    return korijen->kljuc.second;
  }
  BSCvor *kor = korijen;
  while (kor != nullptr && kor->kljuc.first != kl) {
    if (kor->kljuc.first < kl)
      kor = kor->lijevo;
    else if (kor->kljuc.first > kl)
      kor = kor->desno;
  }
  if (kor)
    return kor->kljuc.second;
  else
    kor = new BSCvor(kl, TipVrijednosti());
  velicina++;
  return kor->kljuc.second;*/
}

template <typename TipKljuca, typename TipVrijednosti>
const TipVrijednosti
BinStabloMapa<TipKljuca, TipVrijednosti>::operator[](TipKljuca kl) const {
  BSCvor *x = korijen;
  while (x != 0) {
    if (kl < x->kljuc.first)
      x = x->lijevo;
    else if (kl > x->kljuc.first)
      x = x->desno;
    else
      return x->kljuc.second;
  }
  return TipVrijednosti();
  /*BSCvor *kor = korijen;
  while (kor != nullptr && kor->kljuc.first != kl) {
    if (kor->kljuc.first < kl)
      kor = kor->lijevo;
    else if (kor->kljuc.first > kl)
      kor = kor->desno;
  }
  if (kor)
    return kor->kljuc.second;
  return TipVrijednosti();*/
}

template <typename TipKljuca, typename TipVrijednosti> class Iterator;

template <typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca, TipVrijednosti> {
  using par = pair<TipKljuca, TipVrijednosti>;

private:
  int kapacitet;
  int duzina;
  par **mapa;
  void prosiriKapacitet();

public:
  NizMapa(int size = 100000) {
    kapacitet = size;
    duzina = 0;
    mapa = new par *[kapacitet];
  }
  NizMapa(const NizMapa &mapa);
  NizMapa<TipKljuca, TipVrijednosti> &operator=(const NizMapa &mapa);
  ~NizMapa() {
    for (int i = 0; i < duzina; i++)
      delete mapa[i];
    delete[] mapa;
  }
  int brojElemenata() const { return duzina; }
  void obrisi() {
    for (int i = 0; i < duzina; i++)
      delete mapa[i];
    duzina = 0;
  }
  void obrisi(const TipKljuca &kljuc) {
      bool pronadjeno = false;
    for (int i = 0; i < duzina; i++) {
      if ((*(mapa[i])).first == kljuc) {
        delete mapa[i];
        pronadjeno = true;
      }
      if(pronadjeno && i+1 != duzina) {
        mapa[i] = mapa[i+1];
      }
    }
    duzina--;
  }
  const TipVrijednosti operator[](TipKljuca kljuc) const {
    for (int i = 0; i < duzina; i++) {
      if ((*(mapa[i])).first == kljuc)
        return (*(mapa[i])).second;
    }
    return TipVrijednosti();
    //throw;
  }
  TipVrijednosti &operator[](TipKljuca kljuc) {
    for (int i = 0; i < duzina; i++) {
      if ((*(mapa[i])).first == kljuc)
        return (*(mapa[i])).second;
    }
    if (duzina == kapacitet)
      prosiriKapacitet();
    mapa[duzina] = new par(kljuc, TipVrijednosti());
    duzina++;
    return (*(mapa[duzina - 1])).second;
  }
  friend class Iterator<TipKljuca,TipVrijednosti>;
};

template <typename TipKljuca, typename TipVrijednosti> class Iterator {
  using par = pair<TipKljuca, TipVrijednosti>;
  par **niz;
  int tekuci;
  int brojElemenata;

public:
  Iterator(const NizMapa<TipKljuca, TipVrijednosti> &nizMapa)
      : niz((nizMapa.mapa)), tekuci(0), brojElemenata(nizMapa.duzina) {}
  par trenutni() const;
  par &trenutni();
  TipKljuca &kljuc();
  par &prethodni();
  par &sljedeci();
  par &pocetak();
  par &kraj();
};

template <typename TipKljuca, typename TipVrijednosti> pair<TipKljuca, TipVrijednosti> Iterator<TipKljuca, TipVrijednosti>::trenutni() const {
    if (!brojElemenata)
      throw "Nema elemenata";
    return (**niz)[tekuci];
}

template <typename TipKljuca, typename TipVrijednosti> pair<TipKljuca, TipVrijednosti> &Iterator<TipKljuca, TipVrijednosti>::trenutni() {
    trenutni();
}

template <typename TipKljuca, typename TipVrijednosti> TipKljuca &Iterator<TipKljuca, TipVrijednosti>::kljuc() {
    if (!brojElemenata)
      throw "Nema elemenata";
    return (*niz)[tekuci].first;
}

template <typename TipKljuca, typename TipVrijednosti> pair<TipKljuca, TipVrijednosti> &Iterator<TipKljuca, TipVrijednosti>::prethodni() {
    if (!brojElemenata || !tekuci)
      throw "Neispravan pristup";
    tekuci--;
    return (*niz)[tekuci];
}

template <typename TipKljuca, typename TipVrijednosti> pair<TipKljuca, TipVrijednosti> &Iterator<TipKljuca, TipVrijednosti>::sljedeci() {
    if (!brojElemenata || tekuci+1==brojElemenata)
      throw "Neispravan pristup";
    tekuci++;
    return (*niz)[tekuci];
}

template <typename TipKljuca, typename TipVrijednosti> pair<TipKljuca, TipVrijednosti> &Iterator<TipKljuca, TipVrijednosti>::pocetak() {
    if (!brojElemenata)
      throw "Neispravan pristup";
    tekuci=0;
    return (**niz)[tekuci];
}

template <typename TipKljuca, typename TipVrijednosti> pair<TipKljuca, TipVrijednosti> &Iterator<TipKljuca, TipVrijednosti>::kraj() {
    if (!brojElemenata)
      throw "Neispravan pristup";
    tekuci=brojElemenata-1;
    return (**niz)[tekuci];
}



template <typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca, TipVrijednosti>::NizMapa(const NizMapa &nizMapa) {
  kapacitet = nizMapa.kapacitet;
  duzina = nizMapa.duzina;
  mapa = new par *[kapacitet];
  for (int i = 0; i < duzina; i++) {
    mapa[i] = new par;
    *mapa[i] = *(nizMapa.mapa[i]);
  }
}

template <typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca, TipVrijednosti> &
NizMapa<TipKljuca, TipVrijednosti>::operator=(const NizMapa &nizMapa) {
  auto noviProstor = new par *[nizMapa.kapacitet];
  for (int i = 0; i < nizMapa.duzina; i++) {
    noviProstor[i] = new par;
    *noviProstor[i] = *(nizMapa.mapa[i]);
    if (i < duzina)
      delete mapa[i];
  }
  delete[] mapa;
  kapacitet = nizMapa.kapacitet;
  duzina = nizMapa.duzina;
  mapa = noviProstor;
  return *this;
}

template <typename TipKljuca, typename TipVrijednosti>
void NizMapa<TipKljuca, TipVrijednosti>::prosiriKapacitet() {
  auto temp = new par *[duzina];
  for (int i = 0; i < duzina; i++) {
    temp[i] = new par;
    *temp[i] = *mapa[i];
    delete mapa[i];
  }
  delete[] mapa;
  kapacitet *= 2;
  mapa = new par *[kapacitet];
  for (int i = 0; i < duzina; i++) {
    mapa[i] = new par;
    *mapa[i] = *temp[i];
    delete temp[i];
  }
  delete[] temp;
  temp = nullptr;
}


int main() {
 // Malo jaci test performansi BinStabloMape gdje se vidi 
// da li je zaista O(log n)
clock_t vrijeme1 = clock();
BinStabloMapa <int, int> m;
int vel(10000);
for (int i(0); i<vel; i++)
    m[rand()%(2*vel)-vel] = i;
for (int i(-vel); i<vel; i++)
    m[i] = i;
for (int i(-vel); i<vel; i++)
    m.obrisi(i);
cout << "OK" << endl;
clock_t vrijeme2 = clock();
    int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme izvrsenja binStabloMapa: " << ukvrijeme << " ms." << endl;;


// Malo jaci test performansi BinStabloMape gdje se vidi 
// da li je zaista O(log n)
clock_t vrijeme3 = clock();
NizMapa<int, int> m2;
for (int i(0); i<vel; i++)
    m2[rand()%(2*vel)-vel] = i;
for (int i(-vel); i<vel; i++)
    m2[i] = i;
for (int i(-vel); i<vel; i++)
    m2.obrisi(i);
cout << "OK"<< endl;
clock_t vrijeme4 = clock();
    int ukvrijeme2 = (vrijeme4 - vrijeme3) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme izvrsenja NizMapa: " << ukvrijeme2 << " ms." << endl;

//  cca. 200x duze vrijeme izvrsenja operacija u nizMapa nego u binStabloMapa
  return 0;
}