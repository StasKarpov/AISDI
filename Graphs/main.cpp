#include <iostream>
#include <Comparator.h>
#include <stdexcept>

using namespace std;

int main(int argc, char* argv[]) {

    if(argc < 3){
        throw std::invalid_argument("Missing graph files.");
    }
    std::ifstream file1(argv[1]);
    std::ifstream file2(argv[2]);


    Graph MyGraph(file1);
    Graph MyGraph1(file2);




    /*Graph MyGraph(4);
    MyGraph.addEdge(2,0);
    MyGraph.addEdge(3,0);
    MyGraph.addEdge(1,3);
    Graph MyGraph1(4);
    MyGraph1.addEdge(3,1);
    MyGraph1.addEdge(0,1);
    MyGraph1.addEdge(2,0);*/

    Comparator Comp;





    if(Comp.compare(MyGraph,MyGraph1))
    {
        cout<<"Izomorficzne\n";
        pair<int*,int> Bijection;//tablica ktora przechowywuje funkcje przejscie
        Bijection = Comp.getBijection();
        for(int i = 0;i < Bijection.second;i++)
        {
            cout<<i<<" --> "<<*(Bijection.first+i)<<"\n";
        }


    }else
    {
        cout<<"Nieizomorficzne\n";
    }









}
