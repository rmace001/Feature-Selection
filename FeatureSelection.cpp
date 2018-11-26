#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;
int main(int argc, char** argv)
{
  vector<float> Numericaldata;
  vector<string> data;
  ifstream fin; 
  fin.open(argv[1]);
  string temp; 
  if (!fin.is_open()){
    cout << "Could not open file: " << argv[1] << endl;
  }
  else {
    while (fin >> temp){
      data.push_back(temp);
    }
  }
  
  
  float n = -1.5841842e+00;
  float n2 = 2*n;
  cout << n2 << endl;
  
  
  
  
  
  
  
  fin.close();
  return 0;
}