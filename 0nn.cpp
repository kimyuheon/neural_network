#include <vector>
#include <iostream>
#include <cstdlib>

using namespace std;
typedef vector<int> arr1d;
typedef vector<arr1d> arr3d;
vector<arr3d> outputs;

class Neuron;
typedef vector<Neuron> Layer;

class GetData
{
    public:
        GetData();
        void getTopology(vector<int> &topology, int &argc, char** argv);
};
void GetData::getTopology(vector<int> &topology, int &argc, char** argv) {
    for (int i = 0; i < argc; ++i) {
        topology.push_back(atoi(argv[i]));
        cout << topology[i - 1] << endl;
    }
}
GetData::GetData(){}

class Net
{
    public:
        Net(const vector<int> &topology);
    private:
        vector<Layer> m_layer;
};

Net::Net(const vector<int> &topology) 
{
    int numlayers = topology.size();
    for (int layNum = 0; layNum < numlayers; layNum++) {
        m_layer.push_back(Layer());
        int numOutputs = layNum == topology.size() - 1 ? 0 : topology[layNum + 1];
    }
}

int main(int argc, char** argv) {
    GetData getdata;
    vector<int> topology;
    getdata.getTopology(topology, argc, argv);
    Net myNet(topology);
}