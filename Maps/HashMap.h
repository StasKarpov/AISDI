#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <vector>

#define HASH_CAPACITY 100

namespace aisdi
{

template <typename KeyType, typename ValueType>
class HashMap
{

public:
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

  class ConstIterator;
  class Iterator;
  friend ConstIterator;
  friend Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

private:



  struct Cell
  {
      value_type value;
      Cell* next;
      Cell* prev;
      Cell(){next=NULL;prev=NULL;};
  };

  std::vector<Cell*> buckets;
  int nEntries;

    void clearAll()
    {
        for(unsigned i=0;i<buckets.size();i++)
        {
            //std::cout<<"\nfrom clear";
            Cell* cl=buckets[i];
            while(cl != nullptr)
            {
                Cell* tmp=cl->next;
                delete cl;
                cl=tmp;
            }
            buckets[i]=nullptr;
        }
        nEntries = 0;
    }
public:

    /*void printAll()
    {
        Cell* cl;
        for(int i=0;i<HASH_CAPACITY;i++)
        {
            std::cout<<i<<": ";
            cl=buckets[i];
            while(cl)
            {
                std::cout<<cl->value.second<<" ";
                cl=cl->next;
            }
            std::cout<<"\n";
        }
    }*/

  HashMap()
  {
        buckets = std::vector<Cell*>(HASH_CAPACITY, nullptr);
        nEntries = 0;
        //std::cout<<"constructor\n";
  }

  ~HashMap()
  {
     clearAll();
  }

  HashMap(std::initializer_list<value_type> list)
  {

    buckets = std::vector<Cell*>(HASH_CAPACITY, nullptr);
    nEntries = 0;
    for(value_type x:list)
    {
        this->operator[](x.first)=x.second;
    }
  }

  HashMap(const HashMap& other)//copystuff
  {
    buckets = std::vector<Cell*>(HASH_CAPACITY, nullptr);
    nEntries = 0;
    Cell* cl;

    for(int i=0;i<HASH_CAPACITY;i++)
    {
        cl=other.buckets[i];
        while(cl)
        {
            this->operator[](cl->value.first) = cl->value.second;
            cl=cl->next;
        }
    }
  }

  HashMap(HashMap&& other)
  {
    buckets  = other.buckets;
    nEntries = other.nEntries;
    other.buckets = std::vector<Cell*>(HASH_CAPACITY, nullptr);
    other.nEntries = 0;
  }

  HashMap& operator=(const HashMap& other)
  {

    if(*this == other) return *this;
    clearAll();
    //std::cout<<"\n size"<<nEntries;
    Cell* cl;
    for(int i=0;i<HASH_CAPACITY;i++)
    {
        cl=other.buckets[i];
        while(cl)
        {
            this->operator[](cl->value.first) = cl->value.second;
            cl=cl->next;
        }
    }
    return *this;
  }

  HashMap& operator=(HashMap&& other)
  {
    if(*this == other) return *this;

    clearAll();
    buckets  = other.buckets;
    nEntries = other.nEntries;
    other.buckets = std::vector<Cell*>(HASH_CAPACITY, nullptr);
    other.nEntries = 0;
    return *this;
  }

  bool isEmpty() const
  {
    return !nEntries;
  }


mapped_type& operator[](const key_type& key)
{
    int bucket = key % HASH_CAPACITY;

    Cell* lastCell;
    lastCell = NULL;
    Cell*  cl;
    cl=buckets[bucket];
//
    while (cl && !(key == cl->value.first))
    {//search for last
        lastCell = cl;
        cl = cl->next;
    }


    if(!cl)//if empty
    {
        cl = new Cell();

        if (lastCell)
        {
           lastCell->next = cl;
           cl->prev = lastCell;//may not work
        }else buckets[bucket] = cl;

        cl->next = NULL;
        cl->value.first  = key;
        ++nEntries;
    }
    return cl->value.second;
}



  const mapped_type& valueOf(const key_type& key) const
  {
    if(find(key).cl == NULL) throw std::out_of_range("valueOf");
    return find(key).cl->value.second;
  }

  mapped_type& valueOf(const key_type& key)
  {
    if(find(key).cl == NULL) throw std::out_of_range("valueOf");
    return find(key).cl->value.second;
  }

  const_iterator find(const key_type& key) const
  {

    /*const_iterator it = begin();
    for(;it!=end();it++)
    {
        if(it.cl->value.first == key) return it;
    }


    return it;*/

    Cell* cl;
        cl = buckets[key % HASH_CAPACITY];
        while(cl)
        {
            if(cl->value.first == key) return const_iterator(this,cl);
            cl=cl->next;
        }

    return const_iterator(this,nullptr);


  }

  iterator find(const key_type& key)
  {

    /*iterator it = begin();


    for(;it!=end();it++)
    {

        if(it.cl->value.first == key) return it;
    }


    return it;*/


        Cell* cl;
        cl = buckets[key % HASH_CAPACITY];
        while(cl)
        {
            if(cl->value.first == key) return iterator(this,cl);
            cl=cl->next;
        }

    return iterator(this,nullptr);


  }

  void remove(const key_type& key)
  {
    if(!(find(key).cl)) throw std::out_of_range("remove");//just to check if key exist;

    Cell* cl = buckets[key % HASH_CAPACITY];
    if (!cl) return;//if key is empty
    if(!nEntries) throw std::out_of_range("remove entries");

    if(!cl->next) //if only one cell
    {
        delete buckets[key % HASH_CAPACITY];
        buckets[key % HASH_CAPACITY]=NULL;
        --nEntries;
        return;
    }

    while (cl && cl->value.first != key) cl=cl->next;
    if (!cl) return;//if key is not found



    if(cl->next) cl->next->prev = cl->prev;
    if(cl->prev) cl->prev->next = cl->next;
    else
    {
        buckets[key % HASH_CAPACITY]=cl->next;
    }
    --nEntries;
    delete cl;

  }

  void remove(const const_iterator& it)
  {
    if(it == end()) throw std::out_of_range("in remove");

    remove(it->first);
  }

  size_type getSize() const
  {
    return nEntries;
  }

  bool operator==(const HashMap& other) const
  {
    iterator it = begin();
    iterator itOther = other.begin();
    while(it.cl && itOther.cl)
    {
        if (it.cl->value != itOther.cl->value) return 0;
        it++;
        itOther++;
    }
    return it.cl == itOther.cl;//
  }

  bool operator!=(const HashMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    iterator it(this,nullptr);

    if(!nEntries) return it;//if empty
    Cell* cl;

    for(int i=0;i<HASH_CAPACITY;i++)
        {

            cl=buckets[i];
            if(cl)
            {
                it.cl=cl;

                return it;
            }
        }
    return it;
  }

  iterator end()
  {
      iterator it(this,nullptr);
     return  it;
  }

  const_iterator cbegin() const
  {
    const_iterator it(this,nullptr);

    if(!nEntries) return it;//if empty
    Cell* cl;

    for(int i=0;i<HASH_CAPACITY;i++)
        {

            cl=buckets[i];
            if(cl)
            {
                it.cl=cl;

                return it;
            }
        }
    return it;
  }

  const_iterator cend() const
  {
    const_iterator it(this,nullptr);
    return  it;
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

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::ConstIterator
{
        const HashMap* mp;
        Cell* cl;

public:
  using reference = typename HashMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename HashMap::value_type;
  using pointer = const typename HashMap::value_type*;
  friend HashMap;


  explicit ConstIterator()
  {}

  ConstIterator(const HashMap* m,Cell* c):   mp(m), cl(c)
  {}



  ConstIterator(const ConstIterator& other): mp(other.mp),cl(other.cl)
  {}

  ConstIterator& operator++()
  {
    if (!cl) throw std::out_of_range("Map is empty (in ++)");

    if (cl->next)
    {
        cl=cl->next;
        return *this;
    }//else searching for next bucket

    int bucket = cl->value.first % HASH_CAPACITY;
    ++bucket;
    for (;bucket<HASH_CAPACITY; bucket++)
    {

        if(mp->buckets[bucket])
        {
            cl=mp->buckets[bucket];
            return *this;
        }
    }

    cl=NULL;//if end reached;
    return *this;
  }

  ConstIterator operator++(int)
  {
    auto result = *this;
    this->operator++();
    return result;
  }

  ConstIterator& operator--()
  {
    if (!cl)//if map is empty or end iterator
    {
        if(!mp->nEntries)
        {
            throw std::out_of_range("Map is empty (in ++)");
        }
        int lastBucket = 0;
        for(int bucket = 0 ;bucket<HASH_CAPACITY;bucket++ )
        {
            if(mp->buckets[bucket]) lastBucket = bucket;
        }//we found last bucket
        cl = mp->buckets[lastBucket];
        while(cl->next)
        {
            cl=cl->next;
        }
        return *this;
    }


    if (cl->prev)
    {
        cl=cl->prev;
        return *this;
    }//else searching for next bucket

    int bucket = cl->value.first % HASH_CAPACITY;
    --bucket;

    for (;bucket>0; bucket--)
    {

        if(mp->buckets[bucket])
        {
            cl = mp->buckets[bucket];
            while(cl->next)
            {
                cl=cl->next;
            }
            return *this;
        }
    }

    throw std::out_of_range("begin is reached");

  }

  ConstIterator operator--(int)
  {
    auto result = *this;
    this->operator--();
    return result;
  }

  reference operator*() const
  {
    if(!cl) throw std::out_of_range("in * cl == NULL");
    return cl->value;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    if(!cl || !other.cl) return (!other.cl)&&(!cl);

    return cl->value==other.cl->value;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::reference;
  using pointer = typename HashMap::value_type*;

  explicit Iterator()
  {}

  Iterator (const HashMap* m,Cell* c)
      : ConstIterator(m,c)
  {}


  Iterator(const ConstIterator& other)
    : ConstIterator(other)
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

  pointer operator->() const
  {
    return &this->operator*();
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif /* AISDI_MAPS_HASHMAP_H */
