#include <iostream>
#include <string>
#include <utility>
#include <vector>

// NizMapa - radi indeksiranje preko proizvoljnog tipa vrijednosti indeksa
// NizMapa O(n) kompleksnost pristupa vrijednosti
// Jedna varijanta : u pozadini koristiti niz parova
// Sekvencijalno prolazimo kroz niz da nadjemo vr kljjuca koja se trazi
// Nece biti mape na predavnmjima

// 1. Iteraror
// 2. Mapa ke korisna za brzom pristpu stvarima u datoteki - kluc je ta rijec, a
// vrijendost je lista putanja datooteke u kojoj se file nakazi Za ispit:

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

bool test1() {
  NizMapa<int, int> tacke;
  for (int i = 0; i < 5; i++)
    tacke[i] = i;
  for (int i = 0; i < 5; i++)
    if (tacke[i] != i)
      return false;
  return true;
}

bool test2() {
  NizMapa<double, double> tacke;
  for (int i = 0; i < 5; i++)
    tacke[i] = i;
  for (int i = 0; i < 5; i++)
    if (tacke[i] != i)
      return false;
  return true;
}

bool test3() {
  // Treba biti false
  NizMapa<double, double> tacke;
  for (int i = 0; i < 5; i++)
    tacke[i] = i;
  for (int i = 0; i < 6; i++)
    if (tacke[i] != i)
      return false;
  return true;
}

bool test4() {
  // Brisanje
  NizMapa<double, double> tacke;
  for (int i = 0; i < 5; i++)
    tacke[i] = i;
  for (int i = 0; i < 5; i++)
    tacke.obrisi(i);
  return tacke.brojElemenata();
}

bool test5() {
  // Brisanje
  NizMapa<double, double> tacke;
  for (int i = 0; i < 5; i++)
    tacke[i] = i;
  tacke.obrisi();
  return tacke.brojElemenata();
}

int main() {
  // Const korektnost i test rada sa tipom int
NizMapa <int, int> m;
m[100] = 200;
 const NizMapa <int, int>& m2(m);
 m[200] = 300;
 cout << m2.brojElemenata() << " "; // 2
 cout << m2[200] << " "; // 300

 // Ovdje se sigurno nece kreirati objekat m2[300] jer je m2 const
 cout << m2[300] << " "; // 0
 cout << m2.brojElemenata() << " "; // 2
 m[300] = 400;
 cout << m2[300] << " "; // 400
 cout << m2.brojElemenata(); // 3
  return 0;
}