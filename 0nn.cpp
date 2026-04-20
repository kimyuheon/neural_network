#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cassert>

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
        double getOutputVal() { return m_outputVal; }
        static double randomWeight(void) { return rand() / double(RAND_MAX); }
        double transferFunctionDerivative(double x);
        void calOutputGradients(double targetCals);
        void calcHiddenGradients(Layer &nextLayer);
        void updateInputWeights(Layer &prevLayer);
        double sumDOW(Layer &nextLayer);
        void feedForward(Layer& preLayer);
        double transferFunction(double x);
        double getWeight(double x);
    private:
        static double eta;
        static double alpha;
        double m_outputVal;
        vector<Connection> m_outputWeights;
        unsigned m_myIndex;
        double m_gradient;
};
double Neuron::eta = 0.15;
double Neuron::alpha = 0.5;
Neuron::Neuron(unsigned numOutput, unsigned myIndex)
{
    for (unsigned c = 0; c < numOutput; ++c) {
        m_outputWeights.push_back(Connection());
        m_outputWeights.back().weight = randomWeight();
    }
    m_myIndex = myIndex;
}
double Neuron::transferFunction(double x) {
    return tanh(x);
}
void Neuron::feedForward(Layer& preLayer) {
    double sum = 0.0;
    for (int n = 0; n < preLayer.size(); ++n) {
        sum += preLayer[n].getOutputVal() * preLayer[n].m_outputWeights[m_myIndex].weight;
    }
    m_outputVal = Neuron::transferFunction(sum);
}

double Neuron::transferFunctionDerivative(double x) {
    return 1.0 - x * x;
}

void Neuron::calOutputGradients(double targetVal) {
    double delta = targetVal - m_outputVal;
    m_gradient = delta * transferFunctionDerivative(m_outputVal);
}

double Neuron::sumDOW(Layer &nextLayer)
{
    double sum = 0;
    for (int n = 0; n < nextLayer.size() - 1; n++) {
        sum += m_outputWeights[n].weight * nextLayer[n].m_gradient;
    }
    return sum;
}

void Neuron::calcHiddenGradients(Layer &nextLayer) {
    double dow = sumDOW(nextLayer);
    m_gradient = dow * transferFunctionDerivative(m_outputVal);
}

void Neuron::updateInputWeights(Layer &prevLayer) {
    for (int n = 0; n < prevLayer.size(); n++) {
        Neuron &neuron = prevLayer[n];
        double oldDeltaWeight = neuron.m_outputWeights[m_myIndex].deltaWeight;
        double newDeltaWeight =  
            eta * neuron.getOutputVal() * m_gradient + alpha * oldDeltaWeight;
        neuron.m_outputWeights[m_myIndex].deltaWeight = newDeltaWeight;
        neuron.m_outputWeights[m_myIndex].weight += newDeltaWeight;
    }
}

double Neuron::getWeight(double x) {
    return m_outputWeights[x].weight;
}

class GetData
{
    public:
        GetData(const string filename);
        void getTopology(vector<int> &topology, int &argc, char** argv);
        int getNextInputs(vector<double> &inputVals);
        int getTargetOutputs(vector<double> &targetOutputVals);
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
int GetData::getNextInputs(vector<double> &inputVals) {
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
int GetData::getTargetOutputs(vector<double> &targetOutputVals) {
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
        void feedForward(const vector<double> &inputVals);
        void getResults(vector<double> &resultVals);
        void backProp(vector<double> &targetVals);
        void printWeight();
        double getRecentAverageError(void) const { return m_recentAverageError; } 

    private:
        vector<Layer> m_layer;
        double m_error = 0.0;
        double m_recentAverageError = 0.0;
        double m_recentAverageSmoothingFactor = 100;
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

void Net::feedForward(const vector<double> &inputVals) {
    assert(inputVals.size() == m_layer[0].size() - 1);
    for (int i = 0; i < inputVals.size(); i++) {
        m_layer[0][i].setOutputVal(inputVals[i]);
    }
    for (int layerNum = 1; layerNum < m_layer.size(); ++layerNum) {
        Layer &preLayer = m_layer[layerNum - 1];
        for (int neurons = 0; neurons< m_layer[layerNum].size() - 1; ++neurons) {
            m_layer[layerNum][neurons].feedForward(preLayer);
        }
    }
}

void Net::getResults(vector<double> &resultVals) {
    resultVals.clear();
    for(int n = 0; n < m_layer.back().size() - 1; n++) {
        resultVals.push_back(m_layer.back()[n].getOutputVal());
    }
}

void Net::backProp(vector<double> &targetVals) {
    Layer &outputLayer = m_layer.back();
    m_error = 0.0;
    for(int n = 0; n < outputLayer.size() - 1; n++) {
        double delta = targetVals[n] - outputLayer[n].getOutputVal();
        m_error += delta * delta;
    }
    m_error /= outputLayer.size() - 1;
    m_error = sqrt(m_error);
    m_recentAverageError = (m_recentAverageError * m_recentAverageSmoothingFactor + m_error) / 
                           (m_recentAverageSmoothingFactor + 1.0);
    for(int n = 0; n < outputLayer.size()-1; n++) {
        outputLayer[n].calOutputGradients(targetVals[n]);
    }
    for(int layerNum = m_layer.size() - 2; layerNum > 0; layerNum--) {
        Layer &hiddenLayer = m_layer[layerNum];
        Layer &nextLayer = m_layer[layerNum + 1];
        for(int n = 0; n < hiddenLayer.size(); n++) {
            hiddenLayer[n].calcHiddenGradients(nextLayer);
        }
    }
    for(int layerNum = m_layer.size() - 1; layerNum > 0; layerNum--) {
        Layer &layer = m_layer[layerNum];
        Layer &prevLayer = m_layer[layerNum - 1];
        for(int n = 0; n < layer.size() - 1; n++) {
            layer[n].updateInputWeights(prevLayer);
        }
    }
}

void Net::printWeight() {
    for (int x= 0; x < m_layer.size() - 1; x++) {
        cout << endl;
        cout << "layer" << x << ":" << endl;
        cout << "-----------" << endl;
        for (int y = 0; y < m_layer[x].size(); y++) {
            cout << "Neuron " << y << ":= ";
            for (int z = 0; z < m_layer[x + 1].size() - 1; z++) {
                cout << m_layer[x][y].getWeight(z) << ",";
            }
            cout << endl;
         }
    }
}

void showVectorVals(string label, vector<double> &v) {
    cout << label << " ";
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    } cout << endl;
}

int main(int argc, char** argv) {
    GetData getdata("out.txt");
    vector<int> topology;
    getdata.getTopology(topology, argc, argv);
    Net myNet(topology);
    vector<double> inputVals, tagetVals, resultVals;
    int count = 0;
    const double NORM = 255.0;
    while(!getdata.isEof()) {
        count++;
        cout << "feeding input No: " << count << " to our neural network" << endl;
        if (getdata.getNextInputs(inputVals) != topology[0])
            break;
        assert(inputVals.size() == topology[0]);
        showVectorVals("inputs : ", inputVals);

        for (int i = 0; i < inputVals.size(); i++) inputVals[i] /= NORM;

        myNet.feedForward(inputVals);
        myNet.getResults(resultVals);

        vector<double> displayResults = resultVals;
        for (int i = 0; i < displayResults.size(); i++) displayResults[i] *= NORM;
        showVectorVals("outputs : ", displayResults);

        getdata.getTargetOutputs(tagetVals);
        showVectorVals("target : ", tagetVals);
        assert(tagetVals.size() == topology.back());

        for (int i = 0; i < tagetVals.size(); i++) tagetVals[i] /= NORM;

        myNet.backProp(tagetVals);
        cout<<"Net recent average error: "<< myNet.getRecentAverageError() << endl;
    }

    myNet.printWeight();
    cout << endl << "Done "<< endl;
    cout << "lets carryout a test" << endl;

    // int input = 0.63*r + 0.58*g + 0.11*b;
    inputVals.clear();
    inputVals.push_back(100.0 / NORM);
    inputVals.push_back(120.0 / NORM);
    inputVals.push_back(140.0 / NORM);
    double targetValue = 0.63*100 + 0.58*120 + 0.11 * 140;
    showVectorVals("inputs (normalized): ", inputVals);
    myNet.feedForward(inputVals);
    myNet.getResults(resultVals);

    double denormalized = resultVals[0] * NORM;
    cout << "Our expected output is " << targetValue << ", while our result is "
         << denormalized << endl;
}