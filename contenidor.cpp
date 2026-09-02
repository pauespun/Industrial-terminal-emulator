#include "contenidor.hpp"

contenidor::contenidor(const string &m, nat l) : _m(m), _l(l) {
    if (m.size() < 1) throw error(MatriculaIncorrecta);
    for (const char &c : m) {
        if (!((c >= 65 && c <= 90) || (c >= 48 && c <= 57))) throw error(MatriculaIncorrecta);
    }
    if (!(l == 10 || l == 20 || l == 30)) throw error(LongitudIncorrecta);
}

contenidor::contenidor(const contenidor &u) : _m(u._m), _l(u._l) {}

typename contenidor::contenidor& contenidor::operator=(const contenidor &u) {
    this->_m = u._m;
    this->_l = u._l;
    return *this; 
}
contenidor::~contenidor() noexcept {}

nat contenidor::longitud() const noexcept {
    return _l;
}
string contenidor::matricula() const noexcept {
    return _m;
}

bool contenidor::operator==(const contenidor &c) const noexcept {
    return _m == c._m && _l == c._l;
}

bool contenidor::operator!=(const contenidor &c) const noexcept {
    return !(*this == c);
}

bool contenidor::operator<(const contenidor &c) const noexcept {
    return _m == c._m ? _l < c._l : _m < c._m;
}

bool contenidor::operator<=(const contenidor &c) const noexcept {
    return *this == c or *this < c;
}

bool contenidor::operator>(const contenidor &c) const noexcept {
    return !(*this <= c);
}

bool contenidor::operator>=(const contenidor &c) const noexcept {
    return !(*this < c);
}