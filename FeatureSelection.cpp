#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;



int main(int argc, char** argv)
{
  vector<float> numericalData;
  vector<string> data;
  ifstream fin; 
  fin.open(argv[1]);
  float temp; 
  if (!fin.is_open()){
    cout << "Could not open file: " << argv[1] << endl;
  }
  else {
    while (fin >> temp){
      numericalData.push_back(temp);
    }
  
    cout << "Read test" << endl;
    cout << "Data Values read: "<< numericalData.size() << endl;
    for (short i=0; i<numericalData.size(); i++){
      cout << "Data at " << i+1 << ": " << numericalData[i] << endl;
      
    }
  }
  
  
  
  
  
  
  fin.close();
  return 0;
}