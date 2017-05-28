#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace aisdi
{

template <typename KeyType, typename ValueType>
class TreeMap
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
  using iterator = Iterator;
  using const_iterator = ConstIterator;

  struct Node
  {
      value_type value;
      Node* parent;
      Node* right;
      Node* left;

      Node():
          parent(nullptr),
          right(nullptr),
          left(nullptr)


      {}


      Node(std::pair<key_type,mapped_type> p):
          value(p),
          right(nullptr),
          left(nullptr),
          parent(nullptr)
      {}


  };

  Node  *root;
  int nEntries;

  /*void printMap(Node* root)
  {
      if(root->left)
      {
          printMap(root->left);
      }
      std::cout<<"\n"<<root->value.second;
      if(root->right)
      {
          printMap(root->right);
      }
  }*/

  Node* copyTree(Node* const r,Node* parent) {
        if (!r) {
            return nullptr;
        } else {
            Node* np = new Node;
            np->value.first = r->value.first;
            np->value.second = r->value.second;
            np->parent = parent;
            np->left = copyTree(r->left,np);
            np->right = copyTree(r->right,np);
            return np;
        }
    }

   void deleteTree(Node* n) {

        if (n)
        {
            deleteTree(n->left);
            deleteTree(n->right);
            delete n;
        }
    }


  TreeMap()
  {
      root = new Node;
      nEntries = 0;
  }

  ~TreeMap()
  {
      //deleteTree(root);
  }

  TreeMap(std::initializer_list<value_type> list)
  {

    root = new Node;
    nEntries=0;
    for(value_type x:list)
    {
        this->operator[](x.first)=x.second;
    }

  }

  TreeMap(const TreeMap& other)
  {
   root = copyTree(other.root,nullptr);
   nEntries = other.nEntries;
  }

  TreeMap(TreeMap&& other)
  {
    root = other.root;
    nEntries = other.nEntries;
    other.nEntries = 0;
    other.root = NULL;
  }

  TreeMap& operator=(const TreeMap& other)
  {
    if(*this == other) return *this;
    deleteTree(root);
    root = copyTree(other.root,nullptr);
    nEntries = other.nEntries;
    return *this;
  }

  TreeMap& operator=(TreeMap&& other)
  {
    if(*this == other) return *this;
    deleteTree(root);
    root = other.root;
    nEntries = other.nEntries;
    other.nEntries = 0;
    other.root = NULL;
    return *this;
  }

  bool isEmpty() const
  {
    return(!nEntries);
  }

  mapped_type& operator[](const key_type& key)
  {

    if(!nEntries)//tree is empty
    {
        root->value.first = key;
        ++nEntries;
        return root->value.second;
    }
    Node* Curent = root;
    iterator it = find(key);
    if(it != end()) return (*it).second;//if key already exist;

    while(1)
    {
        if(Curent->value.first >= key)//go left
        {
            if(!Curent->left)
            {
                Curent->left = new Node;
                Curent->left->value.first = key;
                Curent->left->parent = Curent;
                ++nEntries;
                return Curent->left->value.second;
            }
            Curent=Curent->left;
        }else //go right
        {
            if(!Curent->right)
            {
                Curent->right = new Node;
                Curent->right->value.first = key;
                Curent->right->parent = Curent;
                ++nEntries;
                return Curent->right->value.second;
            }
            Curent=Curent->right;
        }

    }
    return root->value.second; //wont work

  }

  const mapped_type& valueOf(const key_type& key) const
  {
    const_iterator it = find(key);
    if(it == end()) throw std::out_of_range("in value of");
    return it.node->value.second;

  }

  mapped_type& valueOf(const key_type& key)
  {
    iterator it = find(key);
    if(it == end()) throw std::out_of_range("in value of");
    return it.node->value.second;
  }

  const_iterator find(const key_type& key) const
  {

    Node* n=root;
    if(!nEntries) return const_iterator(this,nullptr);

    while(n->value.first != key)
    {

        if(n->value.first >= key)//go left
           {
              n = n->left;
           }
        else // go right
           {
             n = n->right;
           }
           if(!n) return const_iterator(this,nullptr); // if end reached
    }

    return const_iterator(this,n);
  }

  iterator find(const key_type& key)
  {

    Node* n=root;
    if(!nEntries) return iterator(this,nullptr);

    while(n->value.first != key)
    {
        if(n->value.first >= key)//go left
        {
            n = n->left;
        }

        else // go right
        {
            n = n->right;
        }
        if(!n) return iterator(this,nullptr);
    }
    return iterator(this,n);

  }

  void remove(const key_type& key)
  {

      const_iterator cit= find(key);
      remove(cit);
  }

  void remove(const const_iterator& it)
  {

    if(it == end()) throw std::out_of_range("in remove");

    if(it.node->left && it.node->right)//if node have two sons
    {
        const_iterator it2(it.tr,it.node);
        ++it2;
        it.node->value = it2.node->value ;//swap with successor value
        remove(it2);
    }else//if node have only one son or dont have it
    {
        if(it.node == root)//if it is root
        {

           if(it.node->right)// if it has right son
           {
               root = it.node->right;
               root->parent = NULL;

           }else if(it.node->left)
           {
               root = it.node->left;
               root->parent = NULL;
           }
        }else
        {
            if(it.node->parent->left == it.node)//this is left son
            {
                if(it.node->right) //if node have right son
                {
                    it.node->parent->left = it.node->right;
                    it.node->right->parent = it.node->parent;
                }
                else if(it.node->left) // if node have left son
                {
                    it.node->parent->left = it.node->left;
                    it.node->left->parent = it.node->parent;
                }else // if node have no sons
                {
                    it.node->parent->left = NULL;
                }
            }else //this is right son
            {
                if(it.node->right) //if node have right son
                {
                    it.node->parent->right = it.node->right;
                    it.node->right->parent = it.node->parent;
                }
                else if(it.node->left)  //if node have left son
                {
                    it.node->parent->right = it.node->left;
                    it.node->left->parent = it.node->parent;
                }
                else                    //node have no sons
                {
                    it.node->parent->right = NULL;
                }
            }
        }

        --nEntries;
        delete it.node;

        if(!nEntries)//we deleted root
        {
            root = new Node;
        }
    }

  }

  size_type getSize() const
  {
    return nEntries;
  }

  bool operator==(const TreeMap& other) const
  {
    iterator it  = this->begin();
    iterator itO = other.begin();


    while(it != this->end())
    {
        if (it!=itO)return 0;
        ++it;
        ++itO;
    }
    return 1;
  }

  bool operator!=(const TreeMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    if(!nEntries) return iterator(this,nullptr);
    Node* n = root;
    while (n->left) n = n->left;
    return iterator(this,n);
  }

  iterator end()
  {
    return iterator(this,nullptr);

  }

  const_iterator cbegin() const
  {
    if(!nEntries) return const_iterator(this,nullptr);
    Node* n = root;
    while (n->left) n = n->left;
    return const_iterator(this,n);
  }

  const_iterator cend() const
  {
    return const_iterator(this,nullptr);
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
class TreeMap<KeyType, ValueType>::ConstIterator
{
   const TreeMap* tr;
   Node*    node;
public:
  using reference = typename TreeMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename TreeMap::value_type;
  using pointer = const typename TreeMap::value_type*;
  friend TreeMap;

  explicit ConstIterator()
  {}

  ConstIterator(const TreeMap* t,Node* n):tr(t),node(n){}

  ConstIterator(const ConstIterator& other)
  {
    tr=other.tr;
    node=other.node;
  }

  ConstIterator& operator++()
  {
    if (tr->isEmpty()) throw std::out_of_range("++ is empty");


    if (!node) throw std::out_of_range("++ end operator");

    if (*this == --(tr->end()))
    {
        node = nullptr;
        return *this;
    }


    if (node->right)
    {

        node = node->right;
        while(node->left) node = node->left;
        return *this;
    }else//if no children
    {
        Node* prev;
        while(1)
        {
        prev = node;
        node = node->parent;
        if(node->left == prev) return *this;
        }
    }
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
    if (tr->isEmpty()) throw std::out_of_range("-- is empty");

    if (!node)//we have end operator;
    {
        node = tr->root;
        while (node->right) node = node->right;
        return *this;
    }

    if (*this == tr->begin()) throw std::out_of_range("-- ");

    if (node->left)
    {
        node = node->left;
        while(node->right) node = node->right;
        return *this;
    }else
    {
        Node* prev;

        while(1)
        {
        prev = node;
        node = node->parent;
        if(node->right == prev) return *this;
        }
    }
    return *this;

  }

  ConstIterator operator--(int)
  {
     auto result = *this;
     this->operator--();
     return result;
  }

  reference operator*() const
  {
    if(!node) throw std::out_of_range("op*");
    return node->value;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    if(!node || !other.node) return (!other.node)&&(!node);
    return node->value == other.node->value;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::reference;
  using pointer = typename TreeMap::value_type*;

  explicit Iterator()
  {}

  Iterator(TreeMap* t,Node* n):ConstIterator(t,n){}

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

#endif /* AISDI_MAPS_MAP_H */
