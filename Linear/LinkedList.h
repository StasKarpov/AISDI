#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi {

template <typename Type>
class LinkedList {
public:
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    using value_type = Type;
    using pointer = Type*;
    using reference = Type&;
    using const_pointer = const Type*;
  using const_reference = const Type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;
private:
    struct Node
    {


        Node* Prev;
        Node* Next;
        value_type Data;

        Node()
            :Prev(this),Next(this)//list is empty, sentinel pointing on itself
        {}


        Node(Node* prev,Node* next,value_type data)
            :Prev(prev),Next(next),Data(data)
        {
            Prev->Next=this;
            Next->Prev=this;
        }

        ~Node()
        {
            Prev->Next=Next;
            Next->Prev=Prev;
        }
    };

  Node* Sentinel;

public:

  LinkedList()
            : Sentinel(new Node)
  {}

  LinkedList(std::initializer_list<Type> l)
            : Sentinel(new Node)
  {
    for(auto &i:l) append(i);
  }

  LinkedList(const LinkedList& other) :Sentinel(new Node)
  {
    *this=other;
  }

  LinkedList(LinkedList&& other)
  {
    Sentinel=other.Sentinel;

    other.Sentinel=new Node;
  }

  ~LinkedList()
  {
      while(!isEmpty())
      {
          delete Sentinel->Next;
      }
      delete Sentinel;
  }

  LinkedList& operator=(const LinkedList& other)
  {
    if(Sentinel==other.Sentinel) return *this;//if try to coying itself

     while(!isEmpty())//deleting old stuff
      {
          delete Sentinel->Next;
      }
      delete Sentinel;

    Sentinel=new Node;//new list;

    for(iterator it=other.begin();it!=other.end();it++) append(*it);

    return *this;
  }

  LinkedList& operator=(LinkedList&& other)
  {
    if(Sentinel==other.Sentinel) return *this;//if try to coying itself

     while(!isEmpty())//deleting old stuff
      {
          delete Sentinel->Next;
      }
      delete Sentinel;

    Sentinel=other.Sentinel;

    other.Sentinel=new Node;
    return *this;
  }

  bool isEmpty() const
  {
    return Sentinel->Next==Sentinel;
  }

  size_type getSize() const
  {
    size_type i=0;
    for(iterator it=begin();it!=end();it++) ++i;
    return i;
  }

  void append(const Type& item)
  {
    new Node(Sentinel->Prev,Sentinel,item);
  }

  void prepend(const Type& item)
  {
    new Node(Sentinel,Sentinel->Next,item);
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
    new Node(insertPosition.p->Prev,insertPosition.p,item);
  }

  Type popFirst()
  {
    if (isEmpty())
        {
            throw std::logic_error("List is empty (in popFirst)");
        }

    Type tmp=Sentinel->Next->Data;
    delete Sentinel->Next;
    return tmp;
  }

  Type popLast()
  {
    if (isEmpty())
        {
            throw std::logic_error("List is empty (in popLast)");
        }

    Type tmp=Sentinel->Prev->Data;
    delete Sentinel->Prev;
    return tmp;
  }

  void erase(const const_iterator& possition)
  {
    if (possition.p == Sentinel) { throw std::out_of_range("cant erase end erase"); }
    if (isEmpty())             { throw std::out_of_range("list is empty erase"); }

    delete possition.p;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    if(firstIncluded.p == lastExcluded.p) return;

    for(iterator it(firstIncluded.p->Next,Sentinel); ; it++)
    {
            delete it.p->Prev;
            if(it.p==lastExcluded.p)break;
    }
  }

  iterator begin()
  {
    return iterator(Sentinel->Next,Sentinel);
  }

  iterator end()
  {
    return iterator(Sentinel,Sentinel);
  }

  const_iterator cbegin() const
  {
    return const_iterator(Sentinel->Next,Sentinel);
  }

  const_iterator cend() const
  {
    return const_iterator(Sentinel,Sentinel);
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template <typename Type>
class LinkedList<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename LinkedList::value_type;
  using difference_type = typename LinkedList::difference_type;
  using pointer = typename LinkedList::const_pointer;
  using reference = typename LinkedList::const_reference;
private:
    LinkedList<Type>::Node* p;
    LinkedList<Type>::Node* sentinel;

    friend Iterator;
    friend LinkedList;
  explicit ConstIterator()
  {}
public:

  //ConstIterator(Iterator& ci): p(ci.p), sentinel(ci.sentinel) {};


  ConstIterator(LinkedList<Type>::Node* po,LinkedList<Type>::Node* sen): p(po), sentinel(sen) {};

  ConstIterator& operator=(Iterator& ci)
  {
      p=ci.p;
      sentinel=ci.sentinel;
      return *this;
  }

  reference operator*() const
  {
    if (p == sentinel) throw std::out_of_range("cant show end in operator*");

        return p->Data;
  }

  ConstIterator& operator++()
  {
        if (p == sentinel) throw std::out_of_range("out of range in ++");
        p=p->Next;
        return *this;
  }

  ConstIterator operator++(int)
  {
        if (p == sentinel) throw std::out_of_range("out of range in ++(int)");
        ConstIterator it(*this);
        p=p->Next;
        return it;
  }

  ConstIterator& operator--()
  {
        if (p->Prev == sentinel) throw std::out_of_range("out of range in --");
        p=p->Prev;
        return *this;
  }

  ConstIterator operator--(int)
  {
        if (p->Prev == sentinel) throw std::out_of_range("out of range in --(int)");
        ConstIterator it(*this);
        p=p->Prev;
        return it;
  }

  ConstIterator operator+(difference_type d) const
  {
        Iterator it(p,sentinel);
        while (d-- > 0) {
            ++it;
        }
        return it;
  }

  ConstIterator operator-(difference_type d) const
  {
        Iterator it(p,sentinel);
        while (d-- > 0) {
            --it;
        }
        return it;
  }

  bool operator==(const ConstIterator& other) const
  {
    return p==other.p;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return p!=other.p;
  }
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
  using pointer = typename LinkedList::pointer;
  using reference = typename LinkedList::reference;

  explicit Iterator()
  {}

  //Iterator(ConstIterator& ci) :ConstIterator(ci){};
  Iterator(LinkedList<Type>::Node* po,LinkedList<Type>::Node* sen) :ConstIterator(po,sen) {};

  Iterator& operator=(Iterator &it)
  {
      ConstIterator::operator=(it);
      return *this;
  }

  Iterator(const ConstIterator other)
    : ConstIterator(other.p,other.sentinel)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  Iterator operator+(difference_type d) const
  {
    return ConstIterator::operator+(d);
  }

  Iterator operator-(difference_type d) const
  {
    return ConstIterator::operator-(d);
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif // AISDI_LINEAR_LINKEDLIST_H


