#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;
bool readData(string filename);

bool readData(string filename, vector<float>& vec){
  ifstream fin; 
  fin.open(filename);
  float temp; 
  if (!fin.is_open()){
    cout << "Could not open file: " << filename << endl;
    return false;
  }
  else {
    while (fin >> temp){
      vec.push_back(temp);
    }
  }
  fin.close();
  return true;
}


int main(int argc, char** argv)
{
  vector<float> numericalData;
  vector<string> data;
  if (!readData(argv[1], numericalData))
    return 0;
  
  cout << "Read test" << endl;
  cout << "Data Values read: "<< numericalData.size() << endl;
  for (short i=0; i<numericalData.size(); i++){
    cout << "Data at " << i+1 << ": " << numericalData[i] << endl;
    
  }
  
  return 0;
}