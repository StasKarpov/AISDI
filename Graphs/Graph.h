#include <iostream>
#include <fstream>
#include <new>
#include <string.h>

class Graph
{
private:
    struct AdjacentcyMatrix
    {
      private:
        bool *M;
        int  N;
      public:
        AdjacentcyMatrix(){};

        AdjacentcyMatrix(int n):N(n)
        {
            M = new bool [n*n];

            for(int i = 0; i<N ; i++ )
            {
                for(int j = 0 ; j<N; j++)
                {
                    *(M+N*i+j) = 0;
                }
            }
        }

        ~AdjacentcyMatrix()
        {
           delete[] M;
        }


        bool& get(int x,int y)
        {
           return *(M+N*x+y);
        }

        void print()//for testing
        {
            for(int i = 0; i<N ; i++ )
            {
                for(int j = 0 ; j<N; j++)
                {
                    std::cout<<(*(M+N*i+j))<<" ";
                }
                std::cout<<"\n";
            }
        }

    } *Matrix;

    int NumNode;
    int NumEdge;
public:
    Graph(){}

    Graph(int N): NumNode(N),NumEdge(0)
    {
      Matrix = new AdjacentcyMatrix(N);
    }

   /* Graph(Graph& g):NumNode(g.getNumNode()),NumEdge(0)//copy konstruct
    {
      std::cout<<"hello2\n";
      Matrix = new AdjacentcyMatrix(g.getNumNode());

      for(int i = 0; i<g.getNumNode(); i++)
      {
          for(int j = 0; j<g.getNumNode(); j++)
          {
              if(g.isConnected(i,j)) addEdge(i,j);
          }
      }

    }*/


    Graph(std::ifstream &infile)
    {
        infile >> NumNode;
        NumEdge = 0 ;
        Matrix = new AdjacentcyMatrix(NumNode);
        int a, b;
        while (infile >> a >> b)
            addEdge(a, b);

    }

    ~Graph()
    {}

    bool operator==(Graph& g)
    {
       if(NumNode != g.NumNode || NumEdge != g.NumEdge) return 0;

       for(int i = 0; i<NumNode; i++)
          {
              for(int j = 0; j<NumNode; j++)
              {
                  if(isConnected(i,j) != g.isConnected(i,j)) return 0;
              }
          }

       return 1;
    }

    void addEdge(int x,int y)
    {
        if(!isConnected(x,y))
        {
        Matrix->get(x,y) = 1;
        Matrix->get(y,x) = 1;
        ++NumEdge;
        }
    }

    bool isConnected(int x,int y)
    {
        return Matrix->get(x,y);
    }

    void PrintMatrix()
    {
        Matrix->print();
    }

    int getNumNode()
    {
        return NumNode;
    }

    int getNumEdge()
    {
        return NumEdge;
    }
};
