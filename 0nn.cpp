#include <vector>
#include <iostream>
#include <cstdlib>

using namespace std;
typedef vector<int> arr1d;
typedef vector<arr1d> arr3d;
vector<arr3d> outputs;

class Neuron;
typedef vector<Neuron> Layer;

struct Connection
{
    double weight;
    double deltaWeight;
};

class Neuron
{
    public:
        Neuron(unsigned numOutputs, unsigned myIndex);
        void setOutputVal(double val) { m_outputVal = val; }
        static double randomWeight(void) { return rand() / double(RAND_MAX); }
    private:
        double m_outputVal;
        vector<Connection> m_outputWeights;
        unsigned m_myIndex;
};

Neuron::Neuron(unsigned numOutputs, unsigned myIndex)
{
    for (unsigned c = 0; c < numOutputs; ++c) {
        m_outputWeights.push_back(Connection());
        m_outputWeights.back().weight = randomWeight();
    }

    m_myIndex = myIndex;
}

class GetData
{
    public:
        GetData();
        void getTopology(vector<int> &topology, int &argc, char** argv);
};
void GetData::getTopology(vector<int> &topology, int &argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
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
    int numlayers = (int)topology.size();
    for (int layNum = 0; layNum < numlayers; layNum++) {
        m_layer.push_back(Layer());
        int numOutputs = layNum == topology.size() - 1 ? 0 : topology[layNum + 1];
        for (int neuronNum = 0; neuronNum <= topology[layNum]; ++neuronNum) {
            m_layer.back().push_back(Neuron(numOutputs, neuronNum));
            cout << "Made a Neuron!" << endl;
        }
        m_layer.back().back().setOutputVal(1.0);
    }
}

int main(int argc, char** argv) {
    GetData getdata;
    vector<int> topology;
    getdata.getTopology(topology, argc, argv);
    Net myNet(topology);
}