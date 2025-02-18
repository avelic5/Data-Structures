#include <iostream>
#include <string>

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

template <typename Tip> class JednostrukaLista : public Lista<Tip> {
private:
  struct Cvor {
    Tip element;
    Cvor *sljedeci;
    Cvor(const Tip &element, Cvor *sljedeci)
        : element(element), sljedeci(sljedeci) {}
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
  JednostrukaLista()
      : prvi(nullptr), zadnji(nullptr), tekuci(nullptr), lduzina(0),
        dduzina(0) {}
  JednostrukaLista(const JednostrukaLista &lista);
  JednostrukaLista &operator=(const JednostrukaLista &lista);
  ~JednostrukaLista();
  int brojElemenata() const { return lduzina + dduzina; }
  Tip &trenutni() const {
    if (brojElemenata() == 0)
      throw "Nista za vratiti!\n";
    // if (tekuci == 0) return prvi->element;
    return tekuci->element;
  }
  bool prethodni() {
    if (lduzina != 0 && brojElemenata() > 1) {
      /*if (lduzina == 1)
        tekuci = 0;
      else {*/
      Cvor *p(prvi);
      while (p->sljedeci != tekuci)
        p = p->sljedeci;
      tekuci = p;
      //}
      lduzina--;
      dduzina++;
      if (lduzina)
        return true;
    }
    return false;
  }
  bool sljedeci() {
    if (dduzina != 0 && brojElemenata() > 1) {
      // if (lduzina == 0) tekuci = prvi;
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
  void izbaciSvakiNTi(int n);
};

template <typename Tip>
JednostrukaLista<Tip>::JednostrukaLista(
    const JednostrukaLista &jednostrukaLista)
    : prvi(0), zadnji(0), dduzina(0), lduzina(0) {
  Cvor *p(jednostrukaLista.prvi);
  while (p != 0) {
    dodajIza(p->element);
    p = p->sljedeci;
    sljedeci();
  }
  lduzina = jednostrukaLista.lduzina;
  dduzina = jednostrukaLista.dduzina;
  pocetak();
}

template <typename Tip>
JednostrukaLista<Tip> &
JednostrukaLista<Tip>::operator=(const JednostrukaLista &jednostrukaLista) {
  if (&jednostrukaLista == this)
    return *this;
  if (brojElemenata()) {
    pocetak();
    while (brojElemenata())
      obrisi();
  }
  Cvor *p(jednostrukaLista.prvi);
  while (p != 0) {
    dodajIza(p->element);
    p = p->sljedeci;
    sljedeci();
  }
  lduzina = jednostrukaLista.lduzina;
  dduzina = jednostrukaLista.dduzina;
  pocetak();
  return *this;
}

template <typename Tip> // Destruktor
JednostrukaLista<Tip>::~JednostrukaLista() {
  if (brojElemenata()) {
    pocetak();
    while (brojElemenata())
      obrisi();
    prvi = zadnji = tekuci = 0;
  }
}

template <typename Tip>
void JednostrukaLista<Tip>::dodajIza(const Tip &element) {
  Cvor *p = new Cvor(element, 0);
  if (!brojElemenata()) {
    prvi = p;
    zadnji = p;
    tekuci = p;
  } else if (dduzina <= 1) {
    tekuci->sljedeci = p;
    zadnji = p;
  } else {
    p->sljedeci = tekuci->sljedeci;
    tekuci->sljedeci = p;
  }
  dduzina++;
}

template <typename Tip>
void JednostrukaLista<Tip>::dodajIspred(const Tip &element) {
  Cvor *p = new Cvor(element, 0);
  if (lduzina == 0) {
    prvi = p;
    zadnji = p;
    tekuci = p;
  } else if (tekuci == prvi) {
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
}

template <typename Tip> void JednostrukaLista<Tip>::obrisi() {
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
    sljedeci();
    // sljedeci();
  }
  delete temp;
  if (tekuci)
    dduzina--;
}

template <typename Tip>
const Tip &JednostrukaLista<Tip>::operator[](int i) const {
  if (i == 0 || tekuci == 0)
    return prvi->element;
  auto p = prvi;
  while (i) {
    p = p->sljedeci;
    i--;
  }
  return p->element;
}

template <typename Tip> Tip &JednostrukaLista<Tip>::operator[](int i) {
  if (i == 0 || tekuci == 0)
    return prvi->element;
  auto p = prvi;
  while (i) {
    p = p->sljedeci;
    i--;
  }
  return p->element;
}

template <typename Tip> class Red : private JednostrukaLista<Tip> {
public:
  Red() { JednostrukaLista<Tip>(); }
  void brisi() {
    while (JednostrukaLista<Tip>::brojElemenata())
      JednostrukaLista<Tip>::obrisi();
  }
  void stavi(const Tip &el) {
    if(brojElemenata()) JednostrukaLista<Tip>::kraj();
    JednostrukaLista<Tip>::dodajIza(el);
  }
  Tip skini() {
    JednostrukaLista<Tip>::pocetak();
    Tip izlaz = JednostrukaLista<Tip>::trenutni();
    JednostrukaLista<Tip>::obrisi();
    return izlaz;
  }
  Tip &celo() {
    JednostrukaLista<Tip>::pocetak();
    return JednostrukaLista<Tip>::trenutni();
  }
  int brojElemenata() { return JednostrukaLista<Tip>::brojElemenata(); }
};

template <typename Tip> bool testRed(const Red<Tip> &red) {
  Red<Tip> novi;
  novi = red;
  return red.brojElemenata() == novi.brojElemenata();
}

template <typename Tip> bool testbrisi(const Red<Tip> &red) {
  red.brisi();
  return true;
}

template <typename Tip> bool teststavi(const Red<Tip> &red) {
  int brEl1 = red.brojElemenata();
  red.stavi(Tip{});
  return brEl1 != red.brojElemenata();
}

template <typename Tip> bool testskini(const Red<Tip> &red) {
  int brEl1 = red.brojElemenata();
  red.skini();
  return brEl1 != red.brojElemenata();
}

template <typename Tip> bool testcelo(const Red<Tip> &red) {
  int brEl1 = red.brojElemenata();
  Tip vrh = red.celo();
  std::cout << vrh;
  return brEl1 == red.brojElemenata();
}

template <typename Tip> bool testbrojElemenata(const Red<Tip> &red) {
  std::cout << red.brojElemenata();
  return red.brojElemenata();
}

int main() {
  return 0;
}