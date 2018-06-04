#include <type_traits>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <unordered_map>
using namespace std;
// policy based design
// I have a few existing algorithms running on legacy systems.
// Now I have to deliver a new algorithm running on a different set of data and only
// running on newer systems
// Legacy system shouldn't be affected by this change and I don't want touch it at all
// and I know which system should be running on legacy algorithms and which system
// should be running on new algorithm
// policy based design seem to be a good solution
struct OldData {
    int data;
    OldData(int _data):data(_data){};
};

struct NewData{
    vector<int> data;
    NewData(vector<int> &&_data):data(move(_data)){};
};

template <typename DataType>
class OldAlg {
public:
    OldAlg (DataType &&_t):data(forward<DataType>(_t)){};
    OldAlg (DataType &_t):data(_t){};
    auto run() {
        cout<<"running old algorithm "<<endl;
        typedef  decltype(data.data) result_type;
        result_type result;
        result = data.data * data.data;
        return result;
    }
private:
    DataType data;
};


template <typename DataType>
class NewAlg {
public:
    NewAlg (DataType &&_t):data(forward<DataType>(_t)){};
    NewAlg (DataType &_t):data(_t){};
    auto run() {
        cout<<"running new algorithm "<<endl;
        using result_type = typename decltype(data.data)::value_type;
        result_type result = (result_type)0 ;
        for (auto &e:data.data)
            result += e* 1.5;
        return result;
    }
private:
    DataType data;
};

// this function could also be part of bigger class
template <typename DataType,
            template <typename>class AlgType = OldAlg>
auto run_algorithm(DataType &data){
    AlgType<DataType> alg_instance(data);
    auto result = alg_instance.run();
    cout<<"result: "<<result<<endl;
    return result;
}



int main () {
    OldData test_data1(100);
    NewData test_data2{{100,300,500,900,1100}};
    cout<<run_algorithm<OldData>(test_data1)<<endl; // default to old algorithm
    cout<<run_algorithm<OldData,OldAlg>(test_data1)<<endl;
    cout<<run_algorithm<NewData,NewAlg >(test_data2)<<endl; //switch to new algorithm
    return 0;
}
