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
class AVLStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
  using par = pair<TipKljuca, TipVrijednosti>;
  class AVLCvor {
  public:
    AVLCvor() { lijevo = desno = nullptr; }
    AVLCvor(const TipKljuca &kl, const TipVrijednosti &vr, AVLCvor *roditelj,
            int balans = 0) {
      kljuc.first = kl;
      kljuc.second = vr;
      lijevo = nullptr;
      desno = nullptr;
      this->roditelj = roditelj;
      this->balans = balans;
    }
    par kljuc;
    AVLCvor *lijevo, *desno, *roditelj;
    int balans;
  };
  AVLCvor *korijen;
  int velicina;
  void preorder(AVLCvor *r, AVLCvor *&temp) {
    if (r != 0) {
      temp =
          new AVLCvor(r->kljuc.first, r->kljuc.second, r->roditelj, r->balans);
      velicina++;
      preorder(r->lijevo, temp->lijevo);
      preorder(r->desno, temp->desno);
    }
  }
  void Preorder(AVLCvor *r) {
    static int duzina;
    if (r == korijen)
      duzina = velicina;   
    if (r) {
      cout << r->kljuc.first << ",";
      duzina--;
      Preorder(r->lijevo);
      Preorder(r->desno);
    }
  }
  void Brisi(AVLCvor *k) {
    if (k) {
      Brisi(k->lijevo);
      Brisi(k->desno);
      delete k;
    }
    this->korijen = nullptr;
  }

public:
  AVLStabloMapa() {
    korijen = nullptr;
    velicina = 0;
  }
  ~AVLStabloMapa() { obrisi(); }
  AVLStabloMapa<TipKljuca, TipVrijednosti> &
  operator=(const AVLStabloMapa &stablo);
  AVLStabloMapa(const AVLStabloMapa &stablo);
  const TipVrijednosti operator[](TipKljuca) const;
  TipVrijednosti &operator[](TipKljuca);
  int brojElemenata() const { return velicina; }
  void obrisi(const TipKljuca &kljuc);
  void obrisi() {
    Brisi(korijen);
    velicina = 0;
  }
  TipVrijednosti &Umetni(AVLCvor *r, AVLCvor *y, TipKljuca kl);
  void azurirajBalans(AVLCvor *y);
  void rotacijaUlijevo(AVLCvor *y);
  void rotacijaUdesno(AVLCvor *y);
  TipVrijednosti &rotacija(AVLCvor *r, AVLCvor *y, TipKljuca kl);
  void Preorder() { Preorder(korijen); }
};

template <typename TipKljuca, typename TipVrijednosti>
AVLStabloMapa<TipKljuca, TipVrijednosti> &
AVLStabloMapa<TipKljuca, TipVrijednosti>::operator=(
    const AVLStabloMapa &stablo) {
  if (korijen == stablo.korijen)
    return *this;
  obrisi();
  // korijen = nullptr;
  // AVLStabloMapa();
  // korijen = stablo.korijen;
  preorder(stablo.korijen, korijen);
  return *this;
}

template <typename TipKljuca, typename TipVrijednosti>
AVLStabloMapa<TipKljuca, TipVrijednosti>::AVLStabloMapa(
    const AVLStabloMapa &stablo) {
  AVLStabloMapa();
  velicina = 0;
  // korijen = stablo.korijen;
  // AVLCvor *temp = korijen;
  preorder(stablo.korijen, korijen);
}

template <typename TipKljuca, typename TipVrijednosti>
void AVLStabloMapa<TipKljuca, TipVrijednosti>::obrisi(const TipKljuca &kljuc) {
  ;
  AVLCvor *p = korijen, *q = 0;
  AVLCvor *tmp = 0, *pp = 0, *rp = 0;
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
    while (tmp != nullptr) {
      pp = rp;
      rp = tmp;
      tmp = rp->desno;
    }
    if (pp != p) {
      pp->desno = rp->lijevo;
      rp->lijevo = p->lijevo;
      pp->roditelj = rp;
      rp->roditelj = q;
      if (pp->desno)
        pp->desno->roditelj = pp;
    }
    rp->desno = p->desno;
    p->desno->roditelj = rp;
  }
  if (q == 0) {
    korijen = rp;
    if (korijen)
      korijen->roditelj = nullptr;
  } else if (p == q->lijevo) {
    q->lijevo = rp;
    if (rp)
      rp->roditelj = q;
  } else {
    q->desno = rp;
    if (rp)
      rp->roditelj = q;
  }

  if (p->roditelj && p->roditelj->lijevo == p)
    p->roditelj->balans--;
  else if (p->roditelj)
    p->roditelj->balans++;
  delete p;
  velicina--;
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti &
AVLStabloMapa<TipKljuca, TipVrijednosti>::rotacija(AVLCvor *r, AVLCvor *y,
                                                   TipKljuca kl) {
  if (y == 0) {
    korijen = new AVLCvor(kl, TipVrijednosti(), nullptr);
    velicina++;
    return korijen->kljuc.second;
  } else {
    if (kl < y->kljuc.first) {
      y->lijevo = new AVLCvor(kl, TipVrijednosti(), y);
      velicina++;
      AVLCvor *x = y;
      while (x != nullptr) {
        if (x->balans >= 0)
          (x->balans)++;
        else
          (x->balans)--;
        if (x->balans > 1) { // jednostruka desna rotacija
          auto temp = x;
          x = x->lijevo;
          x->desno = temp;
          x->roditelj = temp->roditelj;
          x->desno->lijevo = nullptr;
          x->desno->roditelj = x;
          x->balans = 0;
          x->desno->balans = 0;
          if (korijen->roditelj)
            korijen = korijen->roditelj;
          if (x->lijevo->balans < 0) { // dvostruka desna rotacija
          }
          break;
        }
        x = x->roditelj;
      }
      return y->lijevo->kljuc.second;
    } else {
      y->desno = new AVLCvor(kl, TipVrijednosti(), y);
      velicina++;
      AVLCvor *x = y;
      while (x != nullptr) {
        (x->balans)--;
        if (x->balans < -1 &&
            x->desno->balans < 0) { // jednostruka lijeva rotacija
          auto temp = x;
          x = x->desno;
          x->lijevo = temp;
          x->roditelj = temp->roditelj;
          x->lijevo->desno = nullptr;
          x->lijevo->roditelj = x;
          x->balans = 0;
          x->lijevo->balans = 0;
          x->desno->roditelj = x;
          if (x->roditelj)
            x->roditelj->lijevo = x;
          if (x->roditelj == nullptr)
            korijen = x;
          break;
        } else if (x->balans < -1 &&
                   x->desno->balans > 0) { // dvostruka lijeva rotacija
          auto temp2 = x->desno;
          x->desno = x->desno->lijevo;
          x->desno->desno = temp2;
          x->desno->roditelj = temp2->roditelj;
          x->desno->desno->lijevo = nullptr;
          x->desno->desno->roditelj = x->desno;
          x->desno->balans = 0;
          x->desno->desno->balans = 0;
          auto temp = x;
          x = x->desno;
          x->lijevo = temp;
          x->roditelj = temp->roditelj;
          x->lijevo->desno = nullptr;
          x->lijevo->roditelj = x;
          x->balans = 0;
          x->lijevo->balans = 0;
          break;
        }
        x = x->roditelj;
      }
      return y->desno->kljuc.second;
    }
  }
}

template <typename TipKljuca, typename TipVrijednosti>
void AVLStabloMapa<TipKljuca, TipVrijednosti>::rotacijaUlijevo(AVLCvor *cvor) {
  if (cvor->roditelj && cvor->roditelj->lijevo == cvor)
    cvor->roditelj->lijevo = cvor->desno;
  else if (cvor->roditelj)
    cvor->roditelj->desno = cvor->desno;
  AVLCvor *temp = cvor->desno;
  bool desno = false;
  if (cvor->desno->lijevo) {
    cvor->desno = temp->lijevo;
    temp->lijevo->roditelj = cvor;
    temp->lijevo = nullptr;
    desno = true;
  }
  temp->roditelj = cvor->roditelj;
  if (cvor->roditelj)
    cvor->roditelj = temp;
  else {
    cvor->roditelj = temp;
    korijen = temp;
  }
  temp->lijevo = cvor;
  cvor->balans = 0;
  temp->balans = 0;
  if (!desno)
    cvor->desno = nullptr;
}

template <typename TipKljuca, typename TipVrijednosti>
void AVLStabloMapa<TipKljuca, TipVrijednosti>::rotacijaUdesno(AVLCvor *cvor) {
  if (cvor->roditelj && cvor->roditelj->lijevo == cvor)
    cvor->roditelj->lijevo = cvor->lijevo;
  else if (cvor->roditelj)
    cvor->roditelj->desno = cvor->lijevo;
  AVLCvor *temp = cvor->lijevo;
  bool lijevo = false;
  if (cvor->lijevo->desno) {
    cvor->lijevo = temp->desno;
    temp->desno->roditelj = cvor;
    temp->desno = nullptr;
    lijevo = true;
  }
  temp->roditelj = cvor->roditelj;
  if (cvor->roditelj)
    cvor->roditelj = temp;
  else {
    cvor->roditelj = temp;
    korijen = temp;
  }
  temp->desno = cvor;
  cvor->balans = 0;
  temp->balans = 0;
  if (!lijevo)
    cvor->lijevo = nullptr;
}

template <typename TipKljuca, typename TipVrijednosti>
void AVLStabloMapa<TipKljuca, TipVrijednosti>::azurirajBalans(AVLCvor *y) {
  if (y->roditelj == nullptr)
    return;
  if (!(y->lijevo && y->desno && y->balans == 0)) {
    if (y->roditelj->lijevo == y)
      y->roditelj->balans++;
    else
      y->roditelj->balans--;
  }
  bool dalje = true;
  AVLCvor *rod = y->roditelj;
  if (y->roditelj->balans < -1) {
    if (y->balans == 1)
      rotacijaUdesno(y);
    rotacijaUlijevo(rod);
    dalje = false;
  } else if (y->roditelj->balans > 1) {
    if (y->balans == -1)
      rotacijaUlijevo(y);
    rotacijaUdesno(rod);
    dalje = false;
  }
  if (dalje)
    azurirajBalans(y->roditelj);
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti &AVLStabloMapa<TipKljuca, TipVrijednosti>::Umetni(AVLCvor *r,
                                                                 AVLCvor *y,
                                                                 TipKljuca kl) {
  if (y == 0) {
    korijen = new AVLCvor(kl, TipVrijednosti(), nullptr);
    velicina++;
    return korijen->kljuc.second;
  } else {
    if (kl < y->kljuc.first) {
      y->lijevo = new AVLCvor(kl, TipVrijednosti(), y);
      velicina++;
      AVLCvor *temp = y->lijevo;
      azurirajBalans(y->lijevo);
      return temp->kljuc.second;
    } else {
      y->desno = new AVLCvor(kl, TipVrijednosti(), y);
      velicina++;
      AVLCvor *temp = y->desno;
      azurirajBalans(y->desno);
      return temp->kljuc.second;
    }
  }
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti &
AVLStabloMapa<TipKljuca, TipVrijednosti>::operator[](TipKljuca kl) {
  AVLCvor *y = 0, *x = korijen;
  while (x != 0) {
    y = x;
    if (kl < x->kljuc.first)
      x = x->lijevo;
    else if (kl > x->kljuc.first)
      x = x->desno;
    else
      return x->kljuc.second;
  }
  if (x)
    return x->kljuc.second;
  return Umetni(korijen, y, kl);
}

template <typename TipKljuca, typename TipVrijednosti>
const TipVrijednosti
AVLStabloMapa<TipKljuca, TipVrijednosti>::operator[](TipKljuca kl) const {
  AVLCvor *x = korijen;
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
#include <iomanip>

void primjer1() {
  AVLStabloMapa<int, int> avl;
  avl[7] = 7;
  avl[2] = 2;
  avl[5] = 5;
  avl[17] = 17;
  avl[91] = 91;
  avl[12] = 12;
  avl.Preorder();
  std::cout << endl;
}

void primjer2() {
  AVLStabloMapa<int, int> avl;
  avl[6] = 6;
  avl[3] = 3;
  avl[2] = 2;
  avl[1] = 1;
  avl[9] = 9;
  avl[12] = 12;
  avl.Preorder();
  std::cout << endl;
}

void primjer3() {
  AVLStabloMapa<int, int> avl;
  avl[15] = 15;
  avl[13] = 13;
  avl[20] = 20;
  avl[1] = 1;
  avl[9] = 9;
  avl[30] = 30;
  avl.Preorder();
  std::cout << endl;
}

void primjer4() {
  AVLStabloMapa<int, int> avl;
  avl[2] = 2;
  avl[3] = 3;
  avl[20] = 20;
  avl[10] = 10;
  avl[9] = 9;
  avl[30] = 30;
  avl.Preorder();
  std::cout << endl;
}

void primjer5() {
  AVLStabloMapa<int, int> avl;
  avl[21] = 21;
  avl[13] = 13;
  avl[22] = 22;
  avl[12] = 12;
  avl[40] = 40;
  avl[30] = 30;
  avl.Preorder();
  std::cout << endl;
}

int main() {
  primjer5();
  return 0;
}