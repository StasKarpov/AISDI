#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#define ELEMENTARY_CAPACITY 50

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <iostream>

namespace aisdi
{

template <typename Type>
class Vector
{
public:
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    using value_type = Type;
    using pointer = Type*;
    using reference = Type&;
    using const_pointer = const Type*;
    using const_reference = const Type&;

    class ConstIterator;
    //friend ConstIterator;
    class Iterator;
    using iterator = Iterator;
    using const_iterator = ConstIterator;
private:
    pointer Beginning;
    pointer End;//point after last element
    size_type Capacity;

    void Reallocate()
    {


            pointer NewBeg=new value_type[Capacity+ELEMENTARY_CAPACITY];//creating a new vector
            pointer NewEnd=NewBeg;                                      //with same values
            Capacity+=ELEMENTARY_CAPACITY;                                                     //but bigger capacity
            for(iterator it=begin();it!=end();it++)
            {
                *NewEnd=*it;
                ++NewEnd;
            }

            delete[] Beginning;//deleting old vector with small capacity
            Beginning=NewBeg;
            End=NewEnd;
    }


public:

        Vector()
        {
            Beginning=new value_type[ELEMENTARY_CAPACITY];
            Capacity=ELEMENTARY_CAPACITY;
            End=Beginning;
        }


        Vector(size_type n)//create a vector where capacity=n
        {
            Beginning=new value_type[n];
            Capacity=n;
            End=Beginning;
        }


        Vector(std::initializer_list<Type> l)
        {
            Beginning=new value_type[ELEMENTARY_CAPACITY];
            Capacity=ELEMENTARY_CAPACITY;
            End=Beginning;
            for (Type x: l)
            {
                if(getSize()==Capacity) Reallocate();

                append(x);
            };
            //throw std::runtime_error("TODO");
        }

        Vector(const Vector& other)//copy constructor
        {
            if(Beginning==other.Beginning) return;

            Beginning=new value_type[other.getCapacity()];
            Capacity=other.getCapacity();
            End=Beginning;

            for(iterator i=other.begin(); i!=other.end(); i++)
            {
                *End=*i;
                ++End;
            }
          //throw std::runtime_error("TODO");
        }

        Vector(Vector&& other)//move constructor
        {
           if(Beginning==other.Beginning) return;

           Beginning=other.begin().p;
           End=other.end().p;
           Capacity=other.getCapacity();

           other.Beginning=new value_type[ELEMENTARY_CAPACITY];//realocating of other vector
           other.Capacity=ELEMENTARY_CAPACITY;
           other.End=other.Beginning;

            //throw std::runtime_error("TODO");
        }

        ~Vector()
        {
            delete[] Beginning;
        }


        Vector& operator=(const Vector& other)//copy
        {
            if(Beginning==other.Beginning) return *this;

            delete[] Beginning;//delete old stuff



            if(other.getCapacity()>Capacity)//increase cappacity if needed
            {
                Beginning=new value_type[other.getCapacity()];
                Capacity=other.getCapacity();
            }
            else  Beginning=new value_type[Capacity];


            End=Beginning;//we have "clean" vector

            for(iterator i=other.begin(); i!=other.end(); i++)
            {
                *End=*i;
                ++End;
            }
            return *this;

            //throw std::runtime_error("TODO");
        }


        Vector& operator=(Vector&& other)//move
        {
           if(Beginning==other.Beginning) return *this;

           delete[] Beginning;
           Beginning=other.begin().p;
           End=other.end().p;
           Capacity=other.getCapacity();

           other.Beginning=new value_type[ELEMENTARY_CAPACITY];//realocating  other vector
           other.Capacity=ELEMENTARY_CAPACITY;
           other.End=other.Beginning;

           return *this;


        }


        Type operator[](difference_type i)
        {
            return *(Beginning+i);
        }


        bool isEmpty() const
        {
            return getSize()==0;

        }


        std::size_t getSize() const
        {
            return (End-Beginning);

        }

        size_type getCapacity() const
        {
            return Capacity;

        }

        void append(const Type& item)//push_back
        {
            if(getSize()==Capacity) Reallocate();


            *End=item;
            ++End;

        }


        void prepend(const Type& item)
        {
             if(getSize()==Capacity) Reallocate();


            for(difference_type i=End-begin().p;i!=0;i--) *(begin().p+i)=*(begin().p+i-1);



            *(begin().p)=item;
            End++;

        }


        void insert(const const_iterator& insertPosition, const Type& item)
        {
            if(insertPosition.p>End || insertPosition.p<Beginning)
                throw std::out_of_range("Iterator is out of range");

            Type* p;

            difference_type index=insertPosition.p-Beginning;

            if(getSize()==Capacity) Reallocate();//if function works we have to find our insertPosition pointer again

            p=Beginning+index;

            for(difference_type i=End-p;i!=0;i--)*(p+i)=*(p+i-1);
            *p=item;
            ++End;
            //throw std::runtime_error("TODO");
        }

        Type popFirst()
        {
            if(isEmpty()) throw std::logic_error("Vector is empty");
            Type tmp=*Beginning;
            erase(begin());
            return tmp;
        }


        Type popLast()
        {
            if(isEmpty()) throw std::logic_error("Vector is empty");
            --End;
            return *End;
        }


        void erase(const const_iterator& possition)
        {
            if(isEmpty() )
                throw std::out_of_range("Iterator is out of range");

            for(difference_type i=0;possition+i+1!=this->end();i++)
            {
               *(possition.p+i)=*(possition.p+i+1);
            }
            --End;
            //throw std::runtime_error("TODO");
        }


        void erase( const const_iterator& firstIncluded, const  const_iterator& lastExcluded)
        {
            if(isEmpty() || firstIncluded.p>End || firstIncluded.p<Beginning || lastExcluded.p>End || lastExcluded.p<Beginning )
                throw std::out_of_range("Iterator is out of range");

            pointer NewEnd=End-(lastExcluded.p-firstIncluded.p);
            for(difference_type i=0;lastExcluded+i!=this->end();i++)
            {
               *(firstIncluded.p+i)=*(lastExcluded.p+i);
            }
            End=NewEnd;


            //throw std::runtime_error("TODO");
        }



        iterator begin()
        {

            return iterator(const_iterator(Beginning, Beginning, End));
        }


        iterator end()
        {

            return iterator(const_iterator(End, Beginning, End));
                   //throw std::runtime_error("TODO");
        }


        const_iterator cbegin() const
        {

            return const_iterator(Beginning, Beginning, End);
            //throw std::runtime_error("TODO");
        }


        const_iterator cend() const
        {

           return const_iterator(End, Beginning, End);
            //throw std::runtime_error("TODO");
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
class Vector<Type>::ConstIterator
{
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename Vector::value_type;
    using difference_type = typename Vector::difference_type;
    using pointer = typename Vector::const_pointer;
    using reference = typename Vector::const_reference;
private:
    friend Vector<Type>;
    Type* p;
    Type* Beginning;
    Type* End;



public:

    explicit ConstIterator()
    {
        p=NULL;
    }

    ConstIterator(Type* point, Type* b,Type* e): p(point), Beginning(b), End(e)
    {}

    ConstIterator& operator=(Type* pother)
    {
        p=pother;
        return *this;
        //throw std::runtime_error("TODO");
    }

    ConstIterator& operator=(iterator it)
    {
        p=it.p;
        return *this;
        //throw std::runtime_error("TODO");
    }

    Type& operator*() const
    {
        if(Beginning==End) throw std::out_of_range("Vector is empty");
        return *p;
        //throw std::runtime_error("TODO");
    }

    ConstIterator& operator++()
    {
        if(p==End)
        {
            throw std::out_of_range("Iterator is out of range");
            return *this;
        }

        ++p;
        return *this;
              // throw std::runtime_error("TODO");
    }

    ConstIterator operator++(int)
    {
        if(p==End)
        {
            throw std::out_of_range("Iterator is out of range");
            return *this;
        }
        auto result = *this;
        ++p;
        return result;
        //throw std::runtime_error("TODO");
    }

    ConstIterator& operator--()
    {
        if(p==Beginning)
        {
            throw std::out_of_range("Iterator is out of range");
            return *this;
        }

        --p;
        return *this;
        //throw std::runtime_error("TODO");
    }

    ConstIterator operator--(int)
    {
        if(p==Beginning)
        {
            throw std::out_of_range("Iterator is out of range");
            return *this;
        }

        auto result = *this;
        --p;
        return result;
        //throw std::runtime_error("TODO");
    }

    ConstIterator operator+(difference_type d) const
    {
        auto result=*this;
        for(;d!=0;d--) result++;
        return result;
        //throw std::runtime_error("TODO");
    }

    ConstIterator operator-(difference_type d) const
    {
        auto result=*this;
        for(;d!=0;d--) result--;
        return result;
        //throw std::runtime_error("TODO");
    }

    bool operator==(const ConstIterator& other) const
    {
        return p==other.p;
        //throw std::runtime_error("TODO");
    }

    bool operator!=(const ConstIterator& other) const
    {
        return p!=other.p;
        //throw std::runtime_error("TODO");
    }

};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
    using pointer = typename Vector::pointer;
    using reference = typename Vector::reference;

    explicit Iterator()
    {}

    Iterator(const ConstIterator& other)
        : ConstIterator(other)
    {}


    Iterator& operator=(Type* pother)
    {
        ConstIterator::operator=(pother);
        return *this;
        //throw std::runtime_error("TODO");
    }

    Iterator& operator=(iterator it)
    {
        ConstIterator::operator=(it);
        return *this;
        //throw std::runtime_error("TODO");
    }

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

#endif // AISDI_LINEAR_VECTOR_H
