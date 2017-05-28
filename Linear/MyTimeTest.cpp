#include <iostream>
#include <chrono>
#include <Vector.h>
#include <LinkedList.h>

#define TESTSIZE 10000
using namespace aisdi;

class MyTimeTest
{
public:
    using List=LinkedList<int>;
    using Vect=Vector<int>;


    void start()
    {
        appendlisttest();
        appendvecttest();
        std::cout <<"\n=============================\n";
        prependlisttest();
        prependvecttest();
        std::cout <<"\n=============================\n";
        WalkThroughListTest();
        WalkThroughVectTest();
        std::cout <<"\n=============================\n";
        PopFirstListTest();
        PopFirstVectTest();
        std::cout <<"\n=============================\n";
        PopLastListTest();
        PopLastVectTest();

    }

    void appendlisttest()
    {
        List  MyList;
        auto start_time = std::chrono::high_resolution_clock::now();
        for(int i=0;i<TESTSIZE;i++)
        {
            MyList.append(i);
        }
        auto end_time = std::chrono::high_resolution_clock::now();

        std::cout <<"Time of appending in list:\n";
        std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count()<<" nanoseconds\n";


    }

    void appendvecttest()
    {
        Vect  MyVect;
        auto start_time = std::chrono::high_resolution_clock::now();
        for(int i=0;i<TESTSIZE;i++)
        {
            MyVect.append(i);
        }
        auto end_time = std::chrono::high_resolution_clock::now();

        std::cout <<"Time of appending in vector:\n";
        std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count()<<" nanoseconds\n";


    }

    void prependlisttest()
    {
        List  MyList;
        auto start_time = std::chrono::high_resolution_clock::now();
        for(int i=0;i<TESTSIZE;i++)
        {
            MyList.prepend(i);
        }
        auto end_time = std::chrono::high_resolution_clock::now();

        std::cout <<"Time of prepending in list:\n";
        std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count()<<" nanoseconds\n";


    }

    void prependvecttest()
    {
        Vect  MyVect;
        auto start_time = std::chrono::high_resolution_clock::now();
        for(int i=0;i<TESTSIZE;i++)
        {
            MyVect.prepend(i);
        }
        auto end_time = std::chrono::high_resolution_clock::now();

        std::cout <<"Time of prepending in vector:\n";
        std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count()<<" nanoseconds\n";
    }


    void WalkThroughListTest()
    {
        List  MyList;
        auto start_time = std::chrono::high_resolution_clock::now();
        for(List::iterator i=MyList.begin();i!=MyList.end();i++);
        auto end_time = std::chrono::high_resolution_clock::now();

        std::cout <<"Time of walking throw the list:\n";
        std::cout << std::chrono::duration_cast<std::chrono::nanosecond>(end_time - start_time).count()<<" nanoseconds\n";
    }

    void WalkThroughVectTest()
    {
        Vect  MyVect;
        auto start_time = std::chrono::high_resolution_clock::now();
        for(Vect::iterator i=MyVect.begin();i!=MyVect.end();i++);
        auto end_time = std::chrono::high_resolution_clock::now();

        std::cout <<"Time of walking throw the vector:\n";
        std::cout << std::chrono::duration_cast<std::chrono::nanosecond>(end_time - start_time).count()<<" nanoseconds\n";
    }

    void PopFirstListTest()
    {
        List  MyList;

        for(int i=0;i<TESTSIZE;i++)
        {
            MyList.append(i);
        }
        auto start_time = std::chrono::high_resolution_clock::now();
        MyList.popFirst();
        auto end_time = std::chrono::high_resolution_clock::now();

        std::cout <<"Time of popping the first element of the list:\n";
        std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count()<<" nanoseconds\n";
    }

    void PopFirstVectTest()
    {
        Vect  MyVect;

        for(int i=0;i<TESTSIZE;i++)
        {
            MyVect.append(i);
        }

        auto start_time = std::chrono::high_resolution_clock::now();
        MyVect.popFirst();
        auto end_time = std::chrono::high_resolution_clock::now();

        std::cout <<"Time of popping the first element of the vector:\n";
        std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count()<<" nanoseconds\n";
    }

     void PopLastListTest()
    {
        List  MyList;

        for(int i=0;i<TESTSIZE;i++)
        {
            MyList.append(i);
        }
        auto start_time = std::chrono::high_resolution_clock::now();
        MyList.popLast();
        auto end_time = std::chrono::high_resolution_clock::now();

        std::cout <<"Time of popping the last element of the list:\n";
        std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count()<<" nanoseconds\n";
    }

    void PopLastVectTest()
    {
        Vect  MyVect;

        for(int i=0;i<TESTSIZE;i++)
        {
            MyVect.append(i);
        }

        auto start_time = std::chrono::high_resolution_clock::now();
        MyVect.popLast();
        auto end_time = std::chrono::high_resolution_clock::now();

        std::cout <<"Time of popping the last element of the vector:\n";
        std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count()<<" nanoseconds\n";
    }



};

