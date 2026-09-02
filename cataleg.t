template <typename Valor>
int cataleg<Valor>::h(const string &k) const {
    nat r = 0;
    for (char c : k) r = (r * 131 + c) % _M;
    return r;
}

template <typename Valor>
cataleg<Valor>::node::node(const string &k, const Valor &v, node* seg) : _k(k), _v(v), _seg(seg) {}

template <typename Valor>
cataleg<Valor>::cataleg(nat numelems) : _M(numelems + 1), _quants(0) {
    _taula = new node*[_M];
    for (nat i = 0; i < _M; ++i) _taula[i] = nullptr;
}

template <typename Valor>
cataleg<Valor>::cataleg(const cataleg& c) {
    this->_taula = c._taula;
    this->_M = c._M;
    this->_quants = c._quants;
}

template <typename Valor>
typename cataleg<Valor>::cataleg& cataleg<Valor>::operator=(const cataleg& c) {
    this->_taula = c._taula;
    this->_M = c._M;
    this->_quants = c._quants;
    return *this;
}

template <typename Valor>
cataleg<Valor>::~cataleg() noexcept {
    for (nat i = 0; i < _M; ++i) {
        node* p = _taula[i];
        while (p) {
            node* paux = p;
            p = p->_seg;
            delete paux;
        }
    }
    delete[] _taula;
}

template <typename Valor>
void cataleg<Valor>::assig(const string &k, const Valor &v) {
    if (k.empty()) throw error(ClauStringBuit);
    
    int i = h(k);
    node *p = _taula[i];
    bool trobat = false;
    while (p != nullptr and not trobat) {
        if (p->_k == k) {
            trobat = true;
        }
        else {
            p = p->_seg;
        }
    }
    if (trobat) {
        p->_v = v;
    }
    else {
        _taula[i] = new node(k, v, _taula[i]);
        _quants++;
    }
}

template <typename Valor>
void cataleg<Valor>::elimina(const string &k) {
    nat i = h(k);
    node *p = _taula[i], *ant = nullptr;
    bool trobat = false;
    while (p != nullptr and not trobat) {
        if (p->_k == k) {
            trobat = true;
        }
        else {
            ant = p;
            p = p->_seg;
        }
    }
    if (trobat) {
        if (ant == nullptr) {
            _taula[i] = p->_seg;
        }
        else {
            ant->_seg = p->_seg;
        }
        delete(p);
        _quants--;
    } else throw error(ClauInexistent);
}

template <typename Valor>
bool cataleg<Valor>::existeix(const string &k) const noexcept {
    int i = h(k);
    node *p = _taula[i];
    bool trobat = false;
    while (p != nullptr and not trobat) {
        if (p->_k == k) {
            trobat = true;
        }
        else {
            p = p->_seg;
        }
    }
    return trobat;
}

template <typename Valor>
Valor cataleg<Valor>::operator[](const string &k) const {
    if (k.empty()) throw error(ClauStringBuit);

    int i = h(k);
    node *p = _taula[i];

    while (p) {
        if (p->_k == k) return p->_v;
        p = p->_seg;
    }

    throw error(ClauInexistent);
}

template <typename Valor>
nat cataleg<Valor>  ::quants() const noexcept {
    return _quants;
}