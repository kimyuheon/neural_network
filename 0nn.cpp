#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>

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
Neuron::Neuron(unsigned numOutput, unsigned myIndex)
{
    for (unsigned c = 0; c < numOutput; ++c) {
        m_outputWeights.push_back(Connection());
        m_outputWeights.back().weight = randomWeight();
    }
    m_myIndex = myIndex;
}

class GetData
{
    public:
        GetData(const string filename);
        void getTopology(vector<int> &topology, int &argc, char** argv);
        int getNextInputs(vector<int> &inputVals);
        int getTargetOutputs(vector<int> &targetOutputVals);
        bool isEof(void) { return dataFile.eof(); }
    private:
        ifstream dataFile;
};
void GetData::getTopology(vector<int> &topology, int &argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        topology.push_back(atoi(argv[i]));
    }
}
GetData::GetData(const string filename){
    dataFile.open(filename.c_str());

}
int GetData::getNextInputs(vector<int> &inputVals) {
    inputVals.clear();

    string line;
    getline(dataFile, line);
    stringstream ss(line);

    string label;
    ss >> label;
    if (label.compare("in:") == 0) {
        double oneValue;
        while (ss >> oneValue)
        {
            inputVals.push_back(oneValue);
        }
    }
    return inputVals.size();
}
int GetData::getTargetOutputs(vector<int> &targetOutputVals) {
    targetOutputVals.clear();

    string line;
    getline(dataFile, line);
    stringstream ss(line);

    string label;
    ss >> label;
    if (label.compare("out:") == 0) {
        double oneValue;
        while (ss >> oneValue)
        {
            targetOutputVals.push_back(oneValue);
        }
    }
    return targetOutputVals.size();
}

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
        cout << endl;
        for (int neuronNum = 0; neuronNum <= topology[layNum]; ++neuronNum) {
            m_layer.back().push_back(Neuron(numOutputs, neuronNum));
            cout << "Made Neuron" << neuronNum + 1 << " in Layer" << layNum + 1 << endl;
        }
        m_layer.back().back().setOutputVal(1.0);
    }
}

int main(int argc, char** argv) {
    GetData getdata("out.txt");
    vector<int> topology;
    getdata.getTopology(topology, argc, argv);
    Net myNet(topology);

    while(!getdata.isEof()) {
        
    }
}