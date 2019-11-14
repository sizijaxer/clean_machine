#pragma once
#include <cstddef>
#include <stdexcept>

namespace jt {

template <class value>
class queue {
  template <class v>
  friend void _queue_copy(queue<v>& dst, const queue<v>& src);

private:
  struct _queue_impl;
  _queue_impl *_impl = nullptr;
  size_t _siz;

  void _init_empty() {
    _siz = 0;
    if (_impl) delete _impl;
    _impl = new _queue_impl;
  }

  void _destory() { _siz = 0; delete _impl; }

public:
  queue() { _init_empty(); }
  queue(const queue<value> &o) { _queue_copy(*this, o); }
  ~queue() { _destory(); }

  void clear() { _init_empty(); }
  void push(const value &val) { _impl->push(val); ++_siz; }
  size_t size() const { return _siz; }

  value pop() {
    if (_siz == 0)
      throw std::out_of_range("jt::queue::pop() - Empty queue");
    --_siz; return _impl->pop();
  }

  bool empty() const { return _siz == 0; }

  value front() const {
    if (_siz == 0)
      throw std::out_of_range("jt::queue::front() - Empty queue");
    return _impl->f->val;
  }

  value back() const {
    if (_siz == 0)
      throw std::out_of_range("jt::queue::back() - Empty queue");
    return _impl->b->val;
  }
};

template <class value>
static void _queue_copy(queue<value> &dst, const queue<value> &src) {
  dst._init_empty();
  auto **dn = &dst._impl->f; // dest node

  for (auto *s = src._impl->f; s; s = s->next) {
    *dn = new typename queue<value>::_queue_impl::node;
    (*dn)->val = s->val;
    dst._impl->b = *dn;
    dn = &(*dn)->next;
  }

  dst._siz = src._siz;
}

template <class value>
struct queue<value>::_queue_impl {
  struct node {
    value val;
    node *next = nullptr;
  } *f = nullptr, // front
    *b;           // back

  ~_queue_impl() {
    while (f) {
      auto *tmp = f->next;
      delete f;
      f = tmp;
    }
  }

  void push(const value& val) {
    node *t = new node;
    t->val = val;

    if (f) b->next = t;
    else f = t; // if empty, set front

    b = t;
  }

  value pop() {
    value v = f->val;

    node *tmp = f->next;
    delete f;
    f = tmp;

    return v;
  }
};

}
