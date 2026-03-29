#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>

using namespace std;
typedef vector<int> arr1d;
typedef vector<arr1d> arr3d;
vector<arr3d> outputs;

int main()
{
	for (int i = 0; i < 256; i++) {
		outputs.push_back(arr3d());
	}
	for (int r = 0; r <= 255; r+=10) {
		for (int g = 1; g <= 255; g+=10) {
			for (int b = 2; b <= 255; b+=10) {
				int input = 0.63*r + 0.58*g + 0.11*b;
				if(outputs[input].size() <= 5 && input <= 255){
					outputs[input].push_back({r,g,b});
				}
				
			}
		}
	}
	for (int a = 0; a < outputs.size(); a++) {
		for (int b = 0; b < outputs[a].size(); b++) {
			cout<<"in: "<<outputs[a][b][0]<<" "<<outputs[a][b][1]<<" "<<outputs[a][b][2]<<endl;
			cout<<"out: "<<a<<endl;
		}
	}
}