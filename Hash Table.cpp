// Hesiranje
// Da provjerimo da li je negdje del napravimo privatni atribut koji ima
// defaultnu vrijednost Par() Redimenzionisanje se vrsi kada se hashMapa popuni
// na 1% kapaciteta jer zelimo da imamo brzu prettragu Na ispitu dodje 10 15 el
// data hes fja i vr, koja ce biti vr u hesh mapi Na vjezbi se radi lineratno
// otvoreno adresiranje

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
  void najblizi(BSCvor *r, const TipKljuca &kljuc, par &min) {
    if (r != 0) {
      if (r->kljuc.first > min.first && r->kljuc.first < kljuc)
        min = r->kljuc;
      najblizi(r->lijevo, kljuc, min);
      najblizi(r->desno, kljuc, min);
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
  BSCvor *MinCvor(BSCvor *r) {
    if (!r)
      throw "Stablo je prazno!";
    while (r->lijevo)
      r = r->lijevo;
    return r;
  }
  BSCvor *MaxCvor(BSCvor *r) {
    if (!r)
      throw "Stablo je prazno!";
    while (r->desno)
      r = r->desno;
    return r;
  }
  void inorder(BSCvor *r, const TipKljuca &a, const TipKljuca &b,
               vector<TipVrijednosti> &rezultat) {
    if (r) {
      if (r->desno && r->desno->kljuc.first < b)
        inorder(r->desno, a, b, rezultat);
      if (r->kljuc.first > a && r->kljuc.first < b)
        rezultat.push_back(r->kljuc.first);
      if (r->lijevo && r->lijevo->kljuc.first > a)
        inorder(r->lijevo, a, b, rezultat);
    }
  }
  void umetanje(BSCvor *r, unsigned int *kljucevi, unsigned int *vrijednosti,
                int &pozicija) {
    if (!pozicija) {
      r = new BSCvor(kljucevi[0], vrijednosti[0]);
      pozicija++;
    } else {
      int nivo = 0;
    }
  }

public:
  BinStabloMapa() {
    korijen = nullptr;
    velicina = 0;
  }
  BinStabloMapa(unsigned int *kljucevi, unsigned int *vrijednosti) {
    umetanje(kljucevi, vrijednosti, 0);
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
  bool operator<(const BinStabloMapa &stablo) {
    return MinCvor(korijen)->kljuc.first < MinCvor(stablo.korijen)->kljuc.first;
  }
  bool operator>(const BinStabloMapa &stablo) {
    return MaxCvor(korijen)->kljuc.first > MaxCvor(stablo.korijen)->kljuc.first;
  }
  TipVrijednosti prviManji(const TipKljuca &kljuc) {
    BSCvor *minKljuc = MinCvor(korijen);
    BSCvor *temp = korijen;
    najblizi(temp, kljuc, minKljuc->kljuc);
    return minKljuc->kljuc.second;
  }
  vector<TipVrijednosti> od_do(const TipKljuca &a, const TipKljuca &b) {
    vector<TipVrijednosti> rezultat;
    BSCvor *temp = korijen;
    inorder(temp, a, b, rezultat);
    return rezultat;
  }
};

template <typename TipKljuca, typename TipVrijednosti>
BinStabloMapa<TipKljuca, TipVrijednosti> &
BinStabloMapa<TipKljuca, TipVrijednosti>::operator=(
    const BinStabloMapa &stablo) {
  if (korijen == stablo.korijen)
    return *this;
  obrisi();
  preorder(stablo.korijen, korijen);
  return *this;
}

template <typename TipKljuca, typename TipVrijednosti>
BinStabloMapa<TipKljuca, TipVrijednosti>::BinStabloMapa(
    const BinStabloMapa &stablo) {
  BinStabloMapa();
  velicina = 0;
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
}

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
      if (pronadjeno && i + 1 != duzina) {
        mapa[i] = mapa[i + 1];
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
};

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

template <typename TipKljuca, typename TipVrijednosti> class par2 {
public:
  TipKljuca first;
  TipVrijednosti second;
  bool DEL;
  par2(TipKljuca kljuc, TipVrijednosti vrijednost, bool obrisano = false) {
    first = kljuc;
    second = vrijednost;
    DEL = obrisano;
  };
};

template <typename TipKljuca, typename TipVrijednosti>
class HashMapa : public Mapa<TipKljuca, TipVrijednosti> {
  using par = par2<TipKljuca, TipVrijednosti>;

private:
  int kapacitet;
  int duzina;
  par **mapa;
  unsigned int (*h)(TipKljuca k, unsigned int i);
  void prosiriKapacitet();
  unsigned int TraziHash(const TipKljuca &k) const {
    int indeks, i(1);
    do {
      indeks = h(k, i);
      if (mapa[indeks] && (mapa[indeks])->first == k)
        return indeks;
      i = i + 1;
    } while (mapa[indeks] && i != kapacitet);
    return -1;
  }

public:
  void definisiHashFunkciju(unsigned int (*hf)(TipKljuca k, unsigned int i)) {
    h = hf;
  }
  HashMapa(int size = 100000) {
    kapacitet = size;
    duzina = 0;
    mapa = new par *[kapacitet] {};
    h = nullptr;
  }
  HashMapa(const HashMapa &mapa);
  HashMapa<TipKljuca, TipVrijednosti> &operator=(const HashMapa &mapa);
  ~HashMapa() {
    obrisi();
    delete[] mapa;
  }
  int brojElemenata() const {return duzina;}
  void obrisi() {
    for (int i = 0; i < kapacitet; i++) {
        delete mapa[i];
        mapa[i] = nullptr;
    }
    duzina = 0;
  }
  void obrisi(const TipKljuca &kljuc) {
    unsigned int indeks = TraziHash(kljuc);
    if (indeks >= 0) {
      (mapa[indeks])->DEL = true;
      duzina--;
    } else
      throw;
  }

  const TipVrijednosti operator[](TipKljuca kljuc) const {
    if (!h)
      throw "Greska";
    int indeks = TraziHash(kljuc);
    if (indeks >= 0 && (mapa[indeks])->DEL == false)
      return (mapa[indeks])->second;
    return TipVrijednosti();
  }

  TipVrijednosti &operator[](TipKljuca kljuc) {
    if (!h)
      throw "Greska";
    int indeks = TraziHash(kljuc);
    if (indeks >= 0 && (mapa[indeks])->DEL == false)
      return (mapa[indeks])->second;

    int j, i = 1;
    if (duzina == kapacitet)
      prosiriKapacitet();
    do {
      j = h(kljuc, i);
      if (!mapa[j] || (mapa[j])->DEL) {
        if (mapa[j])
          delete mapa[j];
        mapa[j] = new par(kljuc, TipVrijednosti());
        duzina++;
        return (mapa[j])->second;
      }
      i++;
    } while (i < kapacitet);
    throw;
  }
};

template <typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca, TipVrijednosti>::HashMapa(const HashMapa &nizMapa) {
  kapacitet = nizMapa.kapacitet;
  duzina = nizMapa.duzina;
  mapa = new par *[kapacitet]{};
  h = nizMapa.h;
  for (int i = 0; i < duzina; i++) {
    mapa[i] = new par((nizMapa.mapa[i])->first, (nizMapa.mapa[i])->second, (nizMapa.mapa[i])->DEL);
  }
}

template <typename TipKljuca, typename TipVrijednosti>
HashMapa<TipKljuca, TipVrijednosti> &
HashMapa<TipKljuca, TipVrijednosti>::operator=(const HashMapa &nizMapa) {
  auto noviProstor = new par *[nizMapa.kapacitet]{};
  for (int i = 0; i < nizMapa.duzina; i++) {
    noviProstor[i] = new par((nizMapa.mapa[i])->first, (nizMapa.mapa[i])->second, (nizMapa.mapa[i])->DEL);
    if (i < duzina)
      delete mapa[i];
  }
  delete[] mapa;
  kapacitet = nizMapa.kapacitet;
  duzina = nizMapa.duzina;
  h = nizMapa.h;
  mapa = noviProstor;
  return *this;
}

template <typename TipKljuca, typename TipVrijednosti>
void HashMapa<TipKljuca, TipVrijednosti>::prosiriKapacitet() {
  auto temp = new par *[duzina]{};
  for (int i = 0; i < duzina; i++) {
    temp[i] = new par((mapa[i])->first, (mapa[i])->second, (mapa[i])->DEL);
    delete mapa[i];
  }
  delete[] mapa;
  kapacitet *= 2;
  mapa = new par *[kapacitet]{};
  for (int i = 0; i < duzina; i++) {
    mapa[i] = new par((temp[i])->first, (temp[i])->second, (temp[i])->DEL);
    delete temp[i];
  }
  delete[] temp;
  temp = nullptr;
}

unsigned int h(int k,unsigned int i) {
 return (k % 100000 + i) % 100000;
 }

int main() {
  // Malo jaci test performansi BinStabloMape gdje se vidi
  // da li je zaista O(log n)
  clock_t vrijeme1 = clock();
  BinStabloMapa<int, int> m;
  int vel(5000);
     for (int i(0); i < vel; i++)
       m[rand() % (2 * vel) - vel] = i;
  for (int i(-vel); i < vel; i++)
    m[i] = i;
  for (int i(-vel); i < vel; i++)
    m.obrisi(i);
  cout << "OK" << endl;
  clock_t vrijeme2 = clock();
  int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja binStabloMapa: " << ukvrijeme << " ms." << endl;

  // Test za NizMapu
  clock_t vrijeme3 = clock();
  NizMapa<int, int> m2;
  for (int i(0); i < vel; i++)
    m2[rand() % (2 * vel) - vel] = i;
  for (int i(-vel); i < vel; i++)
    m2[i] = i;
  for (int i(-vel); i < vel; i++)
    m2.obrisi(i);
  cout << "OK" << endl;
  clock_t vrijeme4 = clock();
  int ukvrijeme2 = (vrijeme4 - vrijeme3) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja NizMapa: " << ukvrijeme2 << " ms." << endl;
  //  cca. 200x duze vrijeme izvrsenja operacija u nizMapa nego u binStabloMapa

  // Test za NizMapu
  clock_t vrijeme5 = clock();
  HashMapa<int, int> m3;
  m3.definisiHashFunkciju(h);
  for (int i(0); i < vel; i++)
    m3[rand() % (2 * vel) - vel] = i;
  for (int i(-vel); i < vel; i++)
    m3[i] = i;
  for (int i(-vel); i < vel; i++)
    m3.obrisi(i);
  cout << "OK" << endl;
  clock_t vrijeme6 = clock();
  int ukvrijeme3 = (vrijeme6 - vrijeme5) / (CLOCKS_PER_SEC / 1000);
  cout << "Vrijeme izvrsenja HashMapa: " << ukvrijeme2 << " ms." << endl;
  // U ovoj varijanti hash funkcije vrijeme izvrsavanja hash mape je jednako vremenu izvrsavanja niz mape
  return 0;
}