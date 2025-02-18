#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

template <typename TipOznake> class Grana;
template <typename TipOznake> class Cvor;
template <typename TipOznake> class GranaIterator;

template <typename TipOznake> class UsmjereniGraf {
public:
  UsmjereniGraf(int brojCvorova);
  UsmjereniGraf() {}
  virtual ~UsmjereniGraf() {}
  UsmjereniGraf(const UsmjereniGraf &graf);
  void operator=(const UsmjereniGraf &graf);
  virtual int dajBrojCvorova() const = 0;
  virtual void postaviBrojCvorova(int brojCvorova) = 0;
  virtual void dodajGranu(int polazniCvor, int dolazniCvor,
                          float tezinaGrane = 0) = 0;
  virtual void obrisiGranu(int polazniCvor, int dolazniCvor) = 0;
  virtual void postaviTezinuGrane(int polazniCvor, int dolazniCvor,
                                  float tezinaGrane) = 0;
  virtual float dajTezinuGrane(int polazniCvor, int dolazniCvor) const = 0;
  virtual bool postojiGrana(int polazniCvor, int dolazniCvor) const = 0;
  virtual void postaviOznakuCvora(int cvor, const TipOznake &oznaka) = 0;
  virtual TipOznake dajOznakuCvora(int cvor) const = 0;
  virtual void postaviOznakuGrane(int polazniCvor, int dolazniCvor,
                                  const TipOznake &oznaka) = 0;
  virtual TipOznake dajOznakuGrane(int polazniCvor, int dolazniCvor) const = 0;
  Grana<TipOznake> dajGranu(int polazniCvor, int dolazniCvor) {
    return Grana<TipOznake>(this, polazniCvor, dolazniCvor);
  }
  Cvor<TipOznake> dajCvor(int redniBroj) {
    return Cvor<TipOznake>(this, redniBroj);
  }
  GranaIterator<TipOznake> dajGranePocetak() const {
    return ++GranaIterator<TipOznake>(this, 0, -1);
  }
  GranaIterator<TipOznake> dajGraneKraj() const {
    return GranaIterator<TipOznake>(this, dajBrojCvorova(), 0);
  }
  // virtual GranaIterator<TipOznake> dajSljedecuGranu(int polazniCvor, int
  // dolazniCvor) const = 0;
};

template <typename TipOznake> class Grana {
  int polazniCvor, dolazniCvor;
  bool postojiGrana;
  // TipOznake oznaka;
  UsmjereniGraf<TipOznake> *usmjereniGraf;

public:
  Grana(UsmjereniGraf<TipOznake> *usmjereniGraf, int polazniCvor,
        int dolazniCvor)
      : polazniCvor(polazniCvor), dolazniCvor(dolazniCvor),
        usmjereniGraf(usmjereniGraf) {}
  // Grana(const Grana &grana) {*this = grana;}
  // void operator =(const Grana &grana)
  // {polazniCvor=grana.polazniCvor;dolazniCvor=grana.dolazniCvor;oznaka=grana.oznaka;}
  float dajTezinu() const {
    return usmjereniGraf->dajTezinuGrane(polazniCvor, dolazniCvor);
  }
  void postaviTezinu(float tezina) {
    usmjereniGraf->postaviTezinuGrane(polazniCvor, dolazniCvor, tezina);
  }
  TipOznake dajOznaku() const {
    return usmjereniGraf->dajOznakuGrane(polazniCvor, dolazniCvor);
  }
  void postaviOznaku(const TipOznake &oznaka) {
    usmjereniGraf->postaviOznakuGrane(polazniCvor, dolazniCvor, oznaka);
  }
  Cvor<TipOznake> dajPolazniCvor() const {
    return usmjereniGraf->dajCvor(polazniCvor);
  }
  Cvor<TipOznake> dajDolazniCvor() const {
    return usmjereniGraf->dajCvor(dolazniCvor);
  }
};

template <typename TipOznake> class Cvor {
  UsmjereniGraf<TipOznake> *usmjereniGraf;
  int redniBroj;
  // TipOznake oznaka;
public:
  Cvor(UsmjereniGraf<TipOznake> *usmjereniGraf, int redniBroj)
      : usmjereniGraf(usmjereniGraf), redniBroj(redniBroj) {}
  // Cvor(int redniBroj) : redniBroj(redniBroj) {}
  // Cvor(const Cvor &cvor) {*this = cvor;}
  // void operator =(const Cvor &cvor)
  // {redniBroj=cvor.redniBroj;oznaka=cvor.oznaka;}
  TipOznake const dajOznaku() {
    return usmjereniGraf->dajOznakuCvora(redniBroj);
  }
  void postaviOznaku(TipOznake oznaka) {
    usmjereniGraf->postaviOznakuCvora(redniBroj, oznaka);
    dajOznaku();
  }
  int const dajRedniBroj() { return redniBroj; }
};

template <typename TipOznake> class GranaIterator {
  int polazniCvor, dolazniCvor;
  UsmjereniGraf<TipOznake> *usmjereniGraf;
  void povecaj(int &polazniCvor, int &dolazniCvor) {
    if (dolazniCvor + 1 >= usmjereniGraf->dajBrojCvorova()) {
      dolazniCvor = 0;
      polazniCvor++;
    } else {
      dolazniCvor++;
    }
  }

public:
  GranaIterator(const UsmjereniGraf<TipOznake> *usmjereniGraf, int polazniCvor,
                int dolazniCvor)
      : usmjereniGraf(const_cast<UsmjereniGraf<TipOznake> *>(usmjereniGraf)),
        polazniCvor(polazniCvor), dolazniCvor(dolazniCvor) {}
  GranaIterator(UsmjereniGraf<TipOznake> *usmjereniGraf, int polazniCvor,
                int dolazniCvor)
      : usmjereniGraf(usmjereniGraf), polazniCvor(polazniCvor),
        dolazniCvor(dolazniCvor) {}
  Grana<TipOznake> operator*() {
    return Grana<TipOznake>(usmjereniGraf, polazniCvor, dolazniCvor);
  }
  bool operator==(const GranaIterator &iter) const {
    return (polazniCvor == iter.polazniCvor &&
            dolazniCvor == iter.dolazniCvor &&
            usmjereniGraf == iter.usmjereniGraf);
  }
  bool operator!=(const GranaIterator &iter) const { return !(*this == iter); }
  GranaIterator &operator++() {
    do {
      povecaj(polazniCvor, dolazniCvor);
    } while (polazniCvor < usmjereniGraf->dajBrojCvorova() &&
             !usmjereniGraf->postojiGrana(polazniCvor, dolazniCvor));
    return *this;
  }
  GranaIterator operator++(int) {
    GranaIterator<TipOznake> temp = *this;
    ++*this;
    return temp;
  }
};

template <typename TipOznake>
class MatricaGraf : public UsmjereniGraf<TipOznake> {
  const float INF = std::numeric_limits<float>::infinity();
  // int brojCvorova, brojGrana;
  std::vector<std::vector<float>> m;
  std::vector<TipOznake> oznakeCvorova;
  std::vector<std::vector<TipOznake>> oznakeGrana;
  void ovjeriCvor(int redniBroj) const {
    if (redniBroj < 0 || redniBroj >= dajBrojCvorova())
      throw;
  }
  void ovjeriGranu(int polazniCvor, int dolazniCvor) const {
    ovjeriCvor(polazniCvor);
    ovjeriCvor(dolazniCvor);
  };
  void ovjeriPostojanjeGrane(int polazniCvor, int dolazniCvor) const {
    ovjeriGranu(polazniCvor, dolazniCvor);
    if (!postojiGrana(polazniCvor, dolazniCvor))
      throw;
  }

public:
  MatricaGraf(int brojCvorova) {
    if (brojCvorova < 0)
      throw;
    m = vector<vector<float>>(brojCvorova, vector<float>(brojCvorova, INF));
    oznakeCvorova = vector<TipOznake>(brojCvorova);
    oznakeGrana =
        vector<vector<TipOznake>>(brojCvorova, vector<TipOznake>(brojCvorova));
  }
  int dajBrojCvorova() const { return m.size(); }
  void postaviBrojCvorova(int brojCvorova) {
    if (brojCvorova < dajBrojCvorova())
      throw;
    m.resize(brojCvorova);
    for (auto &grane : m)
      grane.resize(brojCvorova, INF);
    oznakeCvorova.resize(brojCvorova);
    oznakeGrana.resize(brojCvorova);
    for (auto &oznake : oznakeGrana)
      oznake.resize(brojCvorova);
  }
  void dodajGranu(int polazniCvor, int dolazniCvor, float tezinaGrane) {
    ovjeriGranu(polazniCvor, dolazniCvor);
    m[polazniCvor][dolazniCvor] = tezinaGrane;
  }
  void obrisiGranu(int polazniCvor, int dolazniCvor) {
    ovjeriPostojanjeGrane(polazniCvor, dolazniCvor);
    m[polazniCvor][dolazniCvor] = INF;
  }
  void postaviTezinuGrane(int polazniCvor, int dolazniCvor, float tezinaGrane) {
    ovjeriPostojanjeGrane(polazniCvor, dolazniCvor);
    m[polazniCvor][dolazniCvor] = tezinaGrane;
  }
  float dajTezinuGrane(int polazniCvor, int dolazniCvor) const {
    ovjeriPostojanjeGrane(polazniCvor, dolazniCvor);
    return m[polazniCvor][dolazniCvor];
  }
  bool postojiGrana(int polazniCvor, int dolazniCvor) const {
    ovjeriGranu(polazniCvor, dolazniCvor);
    return m[polazniCvor][dolazniCvor] != INF;
  }
  void postaviOznakuCvora(int cvor, const TipOznake &oznaka) {
    ovjeriCvor(cvor);
    oznakeCvorova[cvor] = oznaka;
  }
  TipOznake dajOznakuCvora(int cvor) const {
    ovjeriCvor(cvor);
    return oznakeCvorova[cvor];
  }
  void postaviOznakuGrane(int polazniCvor, int dolazniCvor,
                          const TipOznake &oznaka) {
    ovjeriPostojanjeGrane(polazniCvor, dolazniCvor);
    oznakeGrana[polazniCvor][dolazniCvor] = oznaka;
  }
  TipOznake dajOznakuGrane(int polazniCvor, int dolazniCvor) const {
    ovjeriPostojanjeGrane(polazniCvor, dolazniCvor);
    return oznakeGrana[polazniCvor][dolazniCvor];
  }
};

template <typename TipOznake>
void dfs(UsmjereniGraf<TipOznake> *graf, vector<Cvor<TipOznake>> &dfs_obilazak, Cvor<TipOznake> cvor) {
    cvor.postaviOznaku(1);
    dfs_obilazak.push_back(cvor);
      for(GranaIterator<TipOznake> iter = graf->dajGranePocetak();iter != graf->dajGraneKraj();++iter) {
          Cvor<TipOznake> susjedni = (*iter).dajDolazniCvor();
          if((*iter).dajPolazniCvor().dajRedniBroj() == cvor.dajRedniBroj() && susjedni.dajOznaku() != 1) dfs(graf, dfs_obilazak, susjedni); 
      }
}

template <typename TipOznake>
void bfs(UsmjereniGraf<TipOznake> *graf, vector<Cvor<TipOznake>> &bfs_obilazak, Cvor<TipOznake> pocetniCvor) {
    pocetniCvor.postaviOznaku(1);
    bfs_obilazak.push_back(pocetniCvor);
    queue<Cvor<TipOznake>> red;
    red.push(pocetniCvor);
    while(!red.empty()) {
        Cvor<TipOznake> c = red.front();
        red.pop();
        for(GranaIterator<TipOznake> iter = graf->dajGranePocetak();iter != graf->dajGraneKraj();++iter) {
            Cvor<TipOznake> susjedni = (*iter).dajDolazniCvor();
            if((*iter).dajPolazniCvor().dajRedniBroj() == c.dajRedniBroj() && susjedni.dajOznaku() != 1) {
                susjedni.postaviOznaku(1);
                bfs_obilazak.push_back(susjedni);
                red.push(susjedni);
            }
        }
    }
}

int main() {
  return 0;
}