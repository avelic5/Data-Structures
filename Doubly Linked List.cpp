#include <cmath>
#include <iostream>
#include <limits.h>

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
  virtual const Tip &operator[](int i) const = 0;
};

template <typename Tip> class Iterator;

template <typename Tip> class DvostrukaLista : public Lista<Tip> {
private:
  struct Cvor {
    Tip element;
    Cvor *sljedeci;
    Cvor *prethodni;
    Cvor(const Tip &element, Cvor *sljedeci, Cvor *prethodni = 0)
        : element(element), sljedeci(sljedeci), prethodni(prethodni) {
      if (prethodni != 0)
        prethodni->sljedeci = this;
      if (sljedeci != 0)
        sljedeci->prethodni = this;
    }
    Cvor(Cvor *prevp = 0, Cvor *nextp = 0) : prethodni(prevp), sljedeci(nextp) {
      if (prethodni != 0)
        prethodni->sljedeci = this;
      if (sljedeci != 0)
        sljedeci->prethodni = this;
    }
  };
  int lduzina, dduzina;
  Cvor *prvi;
  Cvor *zadnji;
  Cvor *tekuci;
  int kapacitet;
  int duzina;
  // int tekuci;
  Tip *L;
  void prosiriKapacitet();

public:
  DvostrukaLista()
      : prvi(nullptr), zadnji(nullptr), tekuci(nullptr), lduzina(0),
        dduzina(0) {}
  DvostrukaLista(const DvostrukaLista &lista);
  DvostrukaLista &operator=(const DvostrukaLista &lista);
  ~DvostrukaLista();
  int brojElemenata() const { return lduzina + dduzina; }
  Tip &trenutni() const {
    if (brojElemenata() == 0)
      throw "Nista za vratiti!\n";
    // if (tekuci == 0) return prvi->element;
    return tekuci->element;
  }
  bool prethodni() {
    if (lduzina != 0 && brojElemenata() > 1) {
     
      tekuci = tekuci->prethodni;
      
      lduzina--;
      dduzina++;
      if (lduzina)
        return true;
    }
    return false;
  }
  bool sljedeci() {
    if (dduzina != 0 && brojElemenata() > 1) {
      if (dduzina == 1)
        return false;
      tekuci = tekuci->sljedeci;
      lduzina++;
      dduzina--;
      if (dduzina)
        return true;
    }
    return false;
  }
  void pocetak() {
    if (!brojElemenata())
      throw "Nema nista za vratiti!\n";
    tekuci = prvi;
    dduzina += lduzina;
    lduzina = 0;
  }
  void kraj() {
    if (!brojElemenata())
      throw "Nema nista za vratiti!\n";
    tekuci = zadnji;
    lduzina += dduzina;
    dduzina = 0;
  }
  void obrisi();
  void dodajIspred(const Tip &el);
  void dodajIza(const Tip &el);
  const Tip &operator[](int i) const;
  Tip &operator[](int i);
  friend class Iterator<Tip>;
};

template <typename Tip>
DvostrukaLista<Tip>::DvostrukaLista(const DvostrukaLista &DvostrukaLista)
    : prvi(0), zadnji(0), dduzina(0), lduzina(0) {
  Cvor *p(DvostrukaLista.prvi);
  while (p != 0) {
    dodajIza(p->element);
    p = p->sljedeci;
    sljedeci();
  }
  lduzina = DvostrukaLista.lduzina;
  dduzina = DvostrukaLista.dduzina;
  pocetak();
}

template <typename Tip>
DvostrukaLista<Tip> &
DvostrukaLista<Tip>::operator=(const DvostrukaLista &dvostrukaLista) {
  if (&dvostrukaLista == this)
    return *this;
  if (brojElemenata()) {
    pocetak();
    while (brojElemenata())
      obrisi();
  }
  Cvor *p(dvostrukaLista.prvi);
  while (p != 0) {
    dodajIza(p->element);
    p = p->sljedeci;
    sljedeci();
  }
  lduzina = dvostrukaLista.lduzina;
  dduzina = dvostrukaLista.dduzina;
  pocetak();
  return *this;
}

template <typename Tip> // Destruktor
DvostrukaLista<Tip>::~DvostrukaLista() {
  if (brojElemenata()) {
    pocetak();
    while (brojElemenata())
      obrisi();
    prvi = zadnji = tekuci = 0;
  }
}

template <typename Tip> void DvostrukaLista<Tip>::dodajIza(const Tip &element) {
  Cvor *p = new Cvor(element, 0);
  if (dduzina <= 0 && !brojElemenata()) {
    prvi = p;
    zadnji = p;
    tekuci = p;
  } else if (tekuci == zadnji) {
    tekuci->sljedeci = p;
    zadnji = p;
    zadnji->prethodni = tekuci;
  } else {
    tekuci->sljedeci->prethodni = p;
    p->sljedeci = tekuci->sljedeci;
    tekuci->sljedeci = p;
    p->prethodni = tekuci;
  }
  dduzina++;
}

template <typename Tip>
void DvostrukaLista<Tip>::dodajIspred(const Tip &element) {
  Cvor *p = new Cvor(element, 0);
  if (lduzina == 0 && !brojElemenata()) {
    prvi = p;
    zadnji = p;
    tekuci = p;
  } else if (tekuci == prvi) {
    p->sljedeci = prvi;
    tekuci->prethodni = p;
    prvi = p;
  } else {
    p->prethodni = tekuci->prethodni;
    p->sljedeci = tekuci;
    tekuci->prethodni->sljedeci = p;
    tekuci->prethodni = p;

   
  }
  lduzina++;
}

template <typename Tip> void DvostrukaLista<Tip>::obrisi() {
  Cvor *temp;
  if (prvi == zadnji) {
    temp = zadnji;
    lduzina = 0;
    dduzina = 0;
    tekuci = nullptr;
    prvi = nullptr;
    zadnji = nullptr;
  } else if (prvi == tekuci) {
    temp = prvi;
    prvi = temp->sljedeci;
    tekuci = tekuci->sljedeci;
  } else if (zadnji == tekuci) {
    temp = zadnji;
    prethodni();
    zadnji = tekuci;
  } else {
    temp = tekuci;
    prethodni();
    tekuci->sljedeci = temp->sljedeci;
    temp->sljedeci->prethodni = tekuci;
    sljedeci();
    // sljedeci();
  }
  delete temp;
  if (tekuci)
    dduzina--;
}

template <typename Tip>
const Tip &DvostrukaLista<Tip>::operator[](int i) const {
  
  if (i == 0 || tekuci == 0)
    return prvi->element;
 
  int ld = lduzina;
  int dd = dduzina;
  if (ld > dd)
    ld--;
  else
    dd--;
  
  int k = 0;
  auto tek = tekuci;
  if (i < std::abs(i - ld) && i < std::abs(ld + dd) / 2) {
    k = i;
    tek = prvi;
  } else if (std::abs(i - ld) <= std::abs(ld + dd - i))
    k = i - ld;
  else {
    k = i - ld - dd - 1;
    tek = zadnji;
  }
  auto p = tek;
  if (k < 0)
    for (int j = 0; j < std::abs(k); j++) {
      p = p->prethodni;
    }
  else
    for (int j = 0; j < k; j++) {
      p = p->sljedeci;
    }
  return p->element;
}

template <typename Tip> Tip &DvostrukaLista<Tip>::operator[](int i) {
  if (i == 0 || tekuci == 0)
    return prvi->element;
  int ld = lduzina;
  int dd = dduzina;
  if (ld > dd)
    ld--;
  else
    dd--;
  int k = 0;
  auto tek = tekuci;
  if (i < std::abs(i - ld) && i < std::abs(ld + dd) / 2) {
    k = i;
    tek = prvi;
  } else if (std::abs(i - ld) <= std::abs(ld + dd - i + 1))
    k = i - ld;
  else {
    k = i - ld - dd - 1;
    tek = zadnji;
  }
  auto p = tek;
  if (k < 0)
    for (int j = 0; j < std::abs(k); j++) {
      p = p->prethodni;
    }
  else
    for (int j = 0; j < k; j++) {
      p = p->sljedeci;
    }
  return p->element;
 
}

template <typename Tip> class Iterator {
  DvostrukaLista<Tip> *lista;
  
  typename DvostrukaLista<Tip>::Cvor *trenutniLista;

public:
 
  Iterator(const Lista<Tip> &dvostrukaLista) {
    lista = static_cast<DvostrukaLista<Tip> *>(
        const_cast<Lista<Tip> *>(&dvostrukaLista));
    trenutniLista = lista->prvi;

 
  }
  Tip trenutni() const;
  Tip &trenutni();
  bool sljedeci();
};

template <typename Tip> Tip Iterator<Tip>::trenutni() const {

  return trenutniLista->element;
}

template <typename Tip> Tip &Iterator<Tip>::trenutni() {

  return trenutniLista->element;
}

template <typename Tip> bool Iterator<Tip>::sljedeci() {

  if (lista) {
   
    if (trenutniLista->sljedeci) {
      trenutniLista = trenutniLista->sljedeci;
      return true;
    }
  }
  return false;
}

template <typename Tip> Tip dajMaksimum(const Lista<Tip> &n) {
  Iterator<Tip> iterator(n);
  auto maxElement = iterator.trenutni();
  while (iterator.sljedeci()) {
    if (iterator.trenutni() > maxElement)
      maxElement = iterator.trenutni();
  }
  return maxElement;
}

template <typename Tip>
void TestbrojElemenata(const DvostrukaLista<Tip> &lista) {
  std::cout << lista.brojElemenata() << std::endl;
}

template <typename Tip> void Testtrenutni(DvostrukaLista<Tip> &lista, Tip el) {
  lista.trenutni() = el;
  std::cout << lista.trenutni() << std::endl;
}

template <typename Tip> void Testprethodni(DvostrukaLista<Tip> &lista) {
  lista.prethodni();
  std::cout << lista.trenutni() << std::endl;
}

template <typename Tip> void Testsljedeci(DvostrukaLista<Tip> &lista) {
  lista.sljedeci();
  std::cout << lista.trenutni() << std::endl;
}

template <typename Tip> void Testpocetak(DvostrukaLista<Tip> &lista) {
  lista.pocetak();
  std::cout << lista.trenutni() << std::endl;
}

template <typename Tip> void Testkraj(DvostrukaLista<Tip> &lista) {
  lista.kraj();
  std::cout << lista.trenutni() << std::endl;
}

template <typename Tip> void Testobrisi(DvostrukaLista<Tip> &lista) {
  lista.obrisi();
  std::cout << lista.trenutni() << std::endl;
}

template <typename Tip>
void TestdodajIspred(DvostrukaLista<Tip> &lista, const Tip &el) {
  lista.dodajIspred(el);
  lista.prethodni();
  std::cout << lista.trenutni() << std::endl;
}

template <typename Tip>
void TestdodajIza(DvostrukaLista<Tip> &lista, const Tip &el) {
  lista.dodajIza(el);
  lista.sljedeci();
  std::cout << lista.trenutni() << std::endl;
}

template <typename Tip>
void TestIndeksiranje(DvostrukaLista<Tip> &lista, int i) {
  lista[i] = 50;
  std::cout << lista[i] << std::endl;
}

template <typename Tip>
bool TestMaksimum(DvostrukaLista<Tip> &lista) {
  lista.dodajIza(INT_MAX);
  return dajMaksimum(lista) == INT_MAX;
}

int main() {
DvostrukaLista<int> l;
for (int i(0); i<=100000; i++){
    l.dodajIza(i);
    l.sljedeci();
}
std::cout << TestMaksimum(l);

  return 0;
}