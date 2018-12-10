#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <limits>
using namespace std;
bool readData(string filename, vector<vector<double>>& features, vector<int> &classType);
void nearestNeighbor(vector<vector<double>>& features, vector<int> & classType);
double leave1OutCrossValidation(vector<vector<double>>& features, vector<int> &classType);
void featureIsolate(vector<int>& tmpFeatureSet, vector<vector<double>>& zeroedFeatures);
bool readData(string filename, vector<vector<double>>& features, vector<int> &classType)
{
  //if (filename == "large100.txt") -> must redo this function to accomodate
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


//assume you have all the features, and hardcode the distance equation

//features[][] should consist of all zeros besides the features in question. I don't need featureToAdd...
//however, featureToAdd should be passed in to the function that zero's out all features currentFeatureSet+featureToAdd 
//might want to use a tempFeatureSet, and if its more accurate: currentFeatureSet=tempFeatureSet
double leave1OutCrossValidation(vector<vector<double>>& features, vector<int> &classType)
{
  double curMinDist = numeric_limits<double>::infinity();
  double tmpDist = 0.0;
  int nearestN = 0;
  int skippedI = 0;
  double correct = 0;
  while (skippedI < 200){
    for (int i=0; i<features.size(); i++){
      tmpDist = 0.0;
      for (int j=0; j<features[0].size(); j++){
        if (i != skippedI){
          tmpDist += ((features[skippedI][j]-features[i][j])*(features[skippedI][j]-features[i][j]));
        }
      }
      //for every instance: we must see if it is a closer neighbor to skippedI
      tmpDist = sqrt(tmpDist);
      if (tmpDist < curMinDist){
        curMinDist = tmpDist;
        nearestN = i;
      }
      if (classType[skippedI] == classType[nearestN]){
        correct++;
      }
    }
  }
  return (correct/200);
}

//Might want to have this function return a vector, which is vector currentFeatureSet
void nearestNeighbor(vector<vector<double>>& features, vector<int> & classType)
{
  vector<int> currentFeatureSet;
  for (int i=0; i<features.size(); i++){
    cout << "On the " << i+1 << "\'th level of the search tree" << endl;
    double bestSoFarAccuracy = 0;
    double accuracy = 0;
    for (int k=0; k<features[i].size(); k++){
      if ( find(currentFeatureSet.begin(), currentFeatureSet.end(), k) == currentFeatureSet.end() ){ //if isEmpty(intersection(currentFeatureSet, k))
        cout << "--Considering adding the " << k+1 << "feature" << endl;
        
        vector<vector<double>> zeroedFeatures = features;
        vector<int> tmpFeatureSet = currentFeatureSet;
        tmpFeatureSet.push_back(k);
        featureIsolate(tmpFeatureSet, zeroedFeatures);
        
        
        accuracy = leave1OutCrossValidation(zeroedFeatures, classType); //why k+1?
        if (accuracy > bestSoFarAccuracy) {
          bestSoFarAccuracy = accuracy;
          currentFeatureSet.push_back(k); //featureToAddAtThisLevel = k
        }
      }
    }
  }
}


void featureIsolate(vector<int>& tmpFeatureSet, vector<vector<double>>& zeroedFeatures){
  
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


Test 1: Verify that you read everything correctly (for small99.txt) by printing the data and the vector sizes
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