#include "terminal.hpp"
////////////////////////////////////////////////
// MÉTODES PRIVATS
////////////////////////////////////////////////
void terminal::recoloca_area_espera()
{
  bool canvi = true;

  while (canvi) {
    canvi = false;

    for (auto it = _espera.begin(); it != _espera.end(); ++it) {
      bool insertat = false;
      if (_est == estrategia::FIRST_FIT) insertat = intenta_inserir_first_fit(*it);
      else if (_est == estrategia::LLIURE) insertat = intenta_inserir_lliure(*it);

      if (insertat) {
        it = _espera.erase(it);
        canvi = true;
        break;
      }
    }
  }
}

bool terminal::posicio_valida(int i, int k, int j, nat mida)
{
  bool ok = true;
  for (nat d = 0; d < mida && ok; d++) {
    if (_mag[i][k+d][j] != "") ok = false;
    else if (j > 0 && _mag[i][k+d][j-1] == "") ok = false;
  }
  return ok;
}

void terminal::emmagatzema_contenidor(int i, int k, int j, nat mida, const contenidor &c)
{
  for (nat d = 0; d < mida; d++)
    _mag[i][k+d][j] = c.matricula();

  node aux;
  aux._i = i;
  aux._k = k;
  aux._j = j;
  aux._l = c.longitud();
  _cat.assig(c.matricula(), aux);

  _grua++;
}

bool terminal::intenta_inserir_first_fit(const contenidor &c)
{
  nat mida = c.longitud()/10;

  for (nat i = 0; i < _n; i++) {
    for (nat k = 0; k + mida <= _m; k++) {
      for (nat j = 0; j < _h; j++) {
        if (posicio_valida(i, k, j, mida)) {
          emmagatzema_contenidor(i, k, j, mida, c);
          return true;
        }
      }
    }
  }
  return false;
}

bool terminal::intenta_inserir_lliure(const contenidor &c)
{
  nat mida = c.longitud()/10;
  int iaux = 0, kaux = 0, jaux = 0;
  bool insereix = false;

  for (int i = 0; i < _n; i++) {
    for (int k = 0; k + mida <= _m; k++) {
      for (int j = 0; j < _h; j++) {
        if (posicio_valida(i, k, j, mida)) {
          if (!insereix || j < jaux || j == jaux && k < kaux || j == jaux && k == kaux && i < iaux) {
            insereix = true;
            iaux = i;
            kaux = k;
            jaux = j;
          }
        }

      }
    }
  }

  if (insereix) {
    emmagatzema_contenidor(iaux, kaux, jaux, mida, c);
    return true;
  }
  return false;
}

void terminal::retira_contenidor_auxiliar(const string &m)
{
  int i = _cat[m]._i, j = _cat[m]._j, k = _cat[m]._k;

  for (int y = 0; y < _cat[m]._l/10; y++) {
    if (j < _h-1 && _mag[i][k+y][j+1] != "") {
      int iaux = _cat[_mag[i][k+y][j+1]]._i;
      int jaux = _cat[_mag[i][k+y][j+1]]._j;
      int kaux = _cat[_mag[i][k+y][j+1]]._k;
      retira_contenidor_auxiliar(_mag[iaux][kaux][jaux]);
    }
  }

  nat mida = _cat[m]._l;
  _espera.push_front(contenidor(m, mida));
  _cat.elimina(m);

  mida /= 10;
  for (int x = 0; x < mida; x++) {
    _mag[i][k+x][j] = "";
  }
  _grua++;
}

////////////////////////////////////////////////
// MÉTODES PÚBLICS
////////////////////////////////////////////////
terminal::terminal(nat n, nat m, nat h, estrategia st)
  : _n(n), _m(m), _h(h), _grua(0), _cat(n * m * h), _est(st)
{
  if (n == 0) throw error(NumFileresIncorr);
  if (m == 0) throw error(NumPlacesIncorr);
  if (h == 0 || h > HMAX) throw error(AlcadaMaxIncorr);
  if (st != estrategia::FIRST_FIT && st != estrategia::LLIURE)
    throw error(EstrategiaIncorr);

  _mag = new string **[_n];
  for (nat i = 0; i < _n; ++i) {
    _mag[i] = new string *[_m];
    for (nat j = 0; j < _m; ++j) {
      _mag[i][j] = new string[_h];
      for (nat k = 0; k < _h; ++k)
        _mag[i][j][k] = "";
    }
  }
}

terminal::terminal(const terminal& b)
  : _n(b._n), _m(b._m), _h(b._h), _grua(b._grua), _cat(b._cat), _est(b._est)
{
  _mag = new string**[_n];
  for (nat i = 0; i < _n; ++i) {
    _mag[i] = new string*[_m];
    for (nat j = 0; j < _m; ++j) {
      _mag[i][j] = new string[_h];
      for (nat k = 0; k < _h; ++k)
        _mag[i][j][k] = b._mag[i][j][k];
    }
  }
}


terminal& terminal::operator=(const terminal& b)
{
  if (this == &b) return *this;

  for (nat i = 0; i < _n; ++i) {
    for (nat j = 0; j < _m; ++j)
      delete[] _mag[i][j];
    delete[] _mag[i];
  }
  delete[] _mag;

  _n = b._n;
  _m = b._m;
  _h = b._h;
  _cat = b._cat;
  _grua = b._grua;
  _est = b._est;

  _mag = new string**[_n];
  for (nat i = 0; i < _n; ++i) {
    _mag[i] = new string*[_m];
    for (nat j = 0; j < _m; ++j) {
      _mag[i][j] = new string[_h];
      for (nat k = 0; k < _h; ++k)
        _mag[i][j][k] = b._mag[i][j][k];
    }
  }
  return *this;
}


terminal::~terminal() noexcept
{
  for (nat i = 0; i < _n; ++i) {
    for (nat j = 0; j < _m; ++j)
      delete[] _mag[i][j];
    delete[] _mag[i];
  }
  delete[] _mag;
}

void terminal::insereix_contenidor(const contenidor &c)
{
  if (_cat.existeix(c.matricula()))
    throw error(MatriculaDuplicada);

  if (_est == estrategia::FIRST_FIT) {
    if (intenta_inserir_first_fit(c)) {
      recoloca_area_espera();
      return;
    }
  } else {
    if (intenta_inserir_lliure(c)) {
      recoloca_area_espera();
      return;
    }
  }

  _espera.push_front(c);

  node aux;
  aux._i = -1;
  aux._k = 0;
  aux._j = 0;
  aux._l = c.longitud();
  _cat.assig(c.matricula(), aux);
}

void terminal::retira_contenidor(const string &m)
{
  if (!_cat.existeix(m)) throw error(MatriculaInexistent);

  int i = _cat[m]._i, j = _cat[m]._j, k = _cat[m]._k;

  if (i == -1) {
    for (auto it = _espera.begin(); it != _espera.end(); it++) {
      if (it->matricula() == m) {
        _espera.erase(it);
        _cat.elimina(m);
        return;
      }
    }
  }

  for (int y = 0; y < _cat[m]._l/10; y++) {
    if (j < _h-1 && _mag[i][k+y][j+1] != "") {
      int iaux = _cat[_mag[i][k+y][j+1]]._i;
      int jaux = _cat[_mag[i][k+y][j+1]]._j;
      int kaux = _cat[_mag[i][k+y][j+1]]._k;
      retira_contenidor_auxiliar(_mag[iaux][kaux][jaux]);
    }
  }

  nat mida = _cat[m]._l/10;
  for (int x = 0; x < mida; x++) {
    _mag[i][k+x][j] = "";
  }
  _cat.elimina(m);
  _grua++;

  recoloca_area_espera();
}



ubicacio terminal::on(const string &m) const noexcept
{
  if (!_cat.existeix(m)) return ubicacio(-1, -1, -1);

  return ubicacio(_cat[m]._i, _cat[m]._k, _cat[m]._j);
}


nat terminal::longitud(const string &m) const
{
  if (!_cat.existeix(m))
    throw error(MatriculaInexistent);
  return _cat[m]._l;
}


void terminal::contenidor_ocupa(const ubicacio &u, string &m) const
{
  if (u.filera() < 0 || u.filera() >= (int)_n ||
      u.placa() < 0 || u.placa() >= (int)_m ||
      u.pis() < 0 || u.pis() >= (int)_h)
    throw error(UbicacioNoMagatzem);

  m = _mag[u.filera()][u.placa()][u.pis()];
}

nat terminal::fragmentacio() const noexcept
{
  nat frag = 0;

  for (nat i = 0; i < _n; ++i) {
    for (nat k = 0; k < _h; ++k) {

      nat run = 0;
      for (nat j = 0; j < _m; ++j) {
        bool buit = (_mag[i][j][k] == "");
        bool suport = (k == 0) || (_mag[i][j][k-1] != "");

        if (buit && suport) run++;
        else {
          if (run == 1) frag += 1;
          run = 0;
        }
      }
      if (run == 1) frag += 1;
    }
  }
  return frag;
}


nat terminal::ops_grua() const noexcept
{
  return _grua;
}

void terminal::area_espera(list<string> &l) const noexcept
{
  l.clear();
  for (const auto &c : _espera)
    l.push_back(c.matricula());
  l.sort();
}

nat terminal::num_fileres() const noexcept
{
  return _n;
}

nat terminal::num_places() const noexcept
{
  return _m;
}

nat terminal::num_pisos() const noexcept
{
  return _h;
}

terminal::estrategia terminal::quina_estrategia() const noexcept
{
  return _est;
}
