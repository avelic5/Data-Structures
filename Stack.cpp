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
    if (lduzina != 0) {
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
  if (dduzina <= 0) {
    prvi = p;
    zadnji = p;
    tekuci = p;
  } else if (dduzina == 1) {
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
  if (lduzina == 0 && !brojElemenata()) {
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
///////OVDJE OVDJE OVDJE OVDJE OVDJE
template <typename Tip> class Stek : private JednostrukaLista<Tip> {

public:
 
  Stek() {JednostrukaLista<Tip>();}
  void brisi();
  void stavi(const Tip &el) {
    JednostrukaLista<Tip>::dodajIspred(el);
    JednostrukaLista<Tip>::pocetak();
  }
  Tip skini();
  Tip &vrh() const { return JednostrukaLista<Tip>::trenutni(); }
  int brojElemenata() const { return JednostrukaLista<Tip>::brojElemenata(); }
};

template <typename Tip> void Stek<Tip>::brisi() {
  while (brojElemenata()) {
    JednostrukaLista<Tip>::obrisi();
  }
}

template <typename Tip> Tip Stek<Tip>::skini() {
  Tip obrisani = vrh();
  JednostrukaLista<Tip>::obrisi();
  if (brojElemenata())
    JednostrukaLista<Tip>::pocetak();
  return obrisani;
}

template <typename Tip> void testobrisi(Stek<Tip> &stek) {
  stek.brisi();
  std::cout << stek.brojElemenata() << std::endl;
}

template <typename Tip> void teststavi(Stek<Tip> &stek) {
  stek.stavi(2);
  std::cout << stek.vrh() << std::endl;
}

template <typename Tip> void testskini(Stek<Tip> &stek) {
  stek.skini();
  std::cout << stek.vrh() << std::endl;
}

template <typename Tip> void testvrh(const Stek<Tip> &stek) {
  std::cout << stek.vrh() << std::endl;
}

template <typename Tip> void testbrojElemenata(const Stek<Tip> &stek) {
  std::cout << stek.brojElemenata() << std::endl;
}

bool provjeri_zagrade(std::string tekst) {
  Stek<char> zagrade;
  for (char c : tekst) {
    if (c == ')'){
      if (zagrade.vrh() != '(')
        return false;
        else zagrade.skini();}
    if (c == ']'){
      if (zagrade.vrh() != '[')
        return false;
        else zagrade.skini();}
    if (c == '}') {
      if (zagrade.vrh() != '{')
        return false;
        else zagrade.skini();
    } else if (c == '(' || c == '[' || c == '{')
      zagrade.stavi(c);
  }
  if(zagrade.brojElemenata()!=0)return false;
  return true;
}

template <typename Tip> int presjek(Stek<Tip> s1, Stek<Tip> s2) {
  int brojac = 0;
  while (s1.brojElemenata() && s2.brojElemenata()) {
    if (s1.vrh() < s2.vrh())
      s1.skini();
    else if (s2.vrh() < s1.vrh())
      s2.skini();
    else {
      brojac++;
      s1.skini();
      s2.skini();
    }
  }
  return brojac;
}

bool palindrom(const JednostrukaLista<char> &rijec) {
  auto pomocnaLista(rijec);
  Stek<char> slova;
  int duzina = rijec.brojElemenata();
  pomocnaLista.pocetak();
  while (duzina) {
    slova.stavi(pomocnaLista.trenutni());
    pomocnaLista.sljedeci();
    duzina--;
  }
  pomocnaLista.pocetak();
  while (slova.brojElemenata()) {
    if (pomocnaLista.trenutni() != slova.vrh())
      return false;
    pomocnaLista.sljedeci();
    slova.skini();
  }
  return true;
}

int spoji(Stek<int> s1, Stek<int> s2, Stek<int> &s3) {
  while (s2.brojElemenata()) {
    s3.stavi(s2.skini());
  }
  while (s1.brojElemenata() && s3.brojElemenata()) {
    if (s1.vrh() > s3.vrh())
      s2.stavi(s1.skini());
    else if (s3.vrh() > s1.vrh())
      s2.stavi(s3.skini());
    else {
      s2.stavi(s1.skini());
      s3.skini();
    }
  }
  while (s1.brojElemenata())
    s2.stavi(s1.skini());
  while (s3.brojElemenata())
    s2.stavi(s3.skini());
  while (s2.brojElemenata()) {
    s3.stavi(s2.skini());
  }
  return 1;
}

int main() {
  Stek<int> novi;
  novi.stavi(1);
  teststavi(novi);
  testbrojElemenata(novi);
  testvrh(novi);
  testskini(novi);
  testobrisi(novi);
  novi.stavi(3);
  novi.stavi(5);
  novi.stavi(7);
  Stek<int> novi2;
  novi2.stavi(2);
  novi2.stavi(3);
  novi2.stavi(6);
  novi2.stavi(8);
  Stek<int> novi3;
  cout << provjeri_zagrade("[()]") << endl;
 
  return 0;
}