//#ifndef COMPARATOR_CPP
//#define COMPARATOR_CPP

#include <iostream>
#include <Graph.h>
#include <algorithm>


class Comparator
{
private:
    struct  bijection // array which show how number of nodes will change
                      // f.e.  2,1,3,0 => 0->2, 1->1, 2->3, 3->0;
    {
     private:
        int *B; //array
        int length;
     public:
        bijection(int N):length(N)
        {
            B = new int [N];
            for(int i = 0;i<length ; i++)
            {
                *(B+i) = i;
            }
        }

        bool next()
        {
            return std::next_permutation(B,B+length);
        }

        int getBije(int i)
        {
            return *(B+i);
        }

        int getLength()
        {
            return length;
        }
        int* getB()
        {
            return B;
        }

    }*Biject;

public:
    Comparator()
    {}



    Graph transform(Graph& g) // transforms Graph with bijection
    {
        Graph tempG(g.getNumNode());

        for(int i = 0;i<g.getNumNode();i++)
        {
            for(int j = 0 ; j<g.getNumNode() ; j++)
            {
                if(g.isConnected( Biject->getBije(i), Biject->getBije(j) ))//we make conection in temp if in g bijected nodes are connected
                   {
                       tempG.addEdge(i,j);
                   }
            }
        }

        return tempG;
    }




    bool compare(Graph& g1,Graph& g2)
    {
          if(g1.getNumNode() != g2.getNumNode()) // check if they have different size
          {
             return 0;
          }

          if(g1.getNumEdge() != g2.getNumEdge())
          {
             return 0;
          }
          //if we here grahps have the same num of nodes and edges
          //we start comparing


          Biject = new bijection(g1.getNumNode());



          do{
              auto g = transform(g2);
              if(g1 == g) return 1;
          }while(Biject->next());

          return 0;
    }

    void printBijection()//for testing
    {
        for(int i = 0; i<Biject->getLength() ;i++)
        {
            std::cout<<i<<" --> "<<Biject->getBije(i)<<"\n";
        }
    }

    std::pair<int*,int> getBijection()
    {
        return std::pair<int*,int>(Biject->getB(),Biject->getLength());
    }



};
