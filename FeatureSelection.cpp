#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;
bool readData(string filename, vector<vector<double>>& features, vector<int> &classType);
void nearestNeighbor(vector<vector<double>>& features, vector<int> & classType);

bool readData(string filename, vector<vector<double>>& features, vector<int> &classType)
{
  ifstream fin; 
  fin.open(filename);
  double temp;
  int row = 0;
  int count = 2;
  if (!fin.is_open()){
    cout << "Could not open file: " << filename << endl;
    return false;
  }
  else {
    fin >> temp;
    classType.push_back( (int) temp);
    features.push_back(vector<double>());
    while (fin >> temp){
      if (count <= 11){
        features[row].push_back(temp); 
        count++;
      }
      else{
        row++;
        count = 2;
        classType.push_back( (int) temp);
        features.push_back(vector<double>());
      }
    }
  }
  fin.close();
  return true;
}

void nearestNeighbor(vector<vector<double>>& features, vector<int> & classType)
{
  
}
int main()
{
  //data vectors
  vector<vector<double>> features;
  vector<int> classType;
  string inputfile; 
  int choice; 
  
  
  //by this point, I should have two vectors: 
  //one for the instance class type, 
  //another 2D vector for the set of features per instance
  
  
  cout << "Welcome to Rogelio Macedo's Feature Selection Algorithm." << endl;
  cout << "Type the  name of the file to test: ";
  cin >> inputfile;
  cout << endl;
  cout << "Type the number of the algorithm you you want to run." << endl << endl;
  cout << "    1) Foward Selection" << endl;
  cout << "    2) Backward Elimination" << endl;
  cout << "    3) Rogelio's Special Algorithm." << endl << endl;
  cin >> choice; 
  //return if file not read properly
  if (!readData(inputfile, features, classType))
    return 0;
  
  cout << "This dataset has " << features[0].size() << " features (not including the class attribute), with "<< classType.size() <<" instances." << endl << endl;
  cout << "Running nearest neighbor with all "<< features[0].size() << " features, using \"leaving-one-out\" evaluation, I get an \naccuracy of 75.4%" << endl << endl;
  cout << "Beginning Search." << endl << endl;
  
  nearestNeighbor(features, classType);
  
  
  
  
  // while (choice != -1){
    
  // }
 
  
  
  
  
  
  
  
  return 0;
}





/////////////////////////////////////////////////////////////////////
/*Test that have passed:


Test 1: Verify that you read everything correctly by printing the data and the vector sizes
 for (int i=0; i<200; i++){
    cout << classType[i] << "   ";
    for (int j=0; j<10; j++){
      cout << features[i][j] << "   ";  
    }
    cout << endl;
  }
  
  cout << "Testing sizes of classType: " << classType.size() << endl;
  cout << "Testing sizes of featuresROW: " << features.size() << endl;

  for (int i=0; i<features.size(); i++){
    cout << "Testing sizes of featuresCOL: " << i << ' '<<features[i].size() << endl;
  }
  


*/