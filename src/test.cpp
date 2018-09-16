#include "hash_specialization.hpp"
#include <tuple>
#include "HashPropensity.hpp"
#include "SamplableSetCR.hpp"
#include <iostream>

using namespace std;
using namespace sset;
int main()
{

    SamplableSetCR<tuple<int,int,int> > s(1,10);
    tuple<int,int,int> e1(1,0,1);
    tuple<int,int,int> e2(1,0,2);
    tuple<int,int,int> e3(1,5,1);
    s.insert(e1, 1);
    s.insert(e2, 4);
    s.insert(e3, 9);

    cout << "size : " << s.size() << endl;

    for(int i = 0; i< 100; i++)
    {
        tuple<int,int,int> e = s.sample();
        cout << get<0>(e) << ", " << get<1>(e) << ", " << get<2>(e) << endl;
    }


    return 0;
}
