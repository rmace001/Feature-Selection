#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <limits>
using namespace std;
bool readData(string filename, vector<vector<double>>& features, vector<int> &classType);
vector<int>  nearestNeighbor(vector<vector<double>>& features, vector<int> & classType);
void backwardSearch(vector<vector<double>>& features, vector<int> & classType);
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
  double tmpDist = 0.0;
  int nearestN = 0;
  int skippedI = 0;
  double correct = 0;
  while (skippedI < 200){
    nearestN = 0;
    double curMinDist = numeric_limits<double>::infinity();
    for (int i=0; i<features.size(); i++){
      
      tmpDist = 0.0;
      if (i != skippedI){
        for (int j=0; j<features[0].size(); j++){
          tmpDist += ((features[skippedI][j]-features[i][j])*(features[skippedI][j]-features[i][j]));
        }
      
        tmpDist = sqrt(tmpDist);
        if (tmpDist < curMinDist){
          curMinDist = tmpDist;
          nearestN = i;
        }
      }
    }
    if (classType[skippedI] == classType[nearestN]){
      correct++;
    }
    skippedI++;
  }
  cout << "correct: " << correct << endl;
  return (correct/200);
}

//Might want to have this function return a vector, which is vector currentFeatureSet
vector<int> nearestNeighbor(vector<vector<double>>& features, vector<int> & classType)
{
  vector<int> currentFeatureSet;
  vector<int> bestOverall;
  bool breakflag = false;
  double levelBest=0;
  for (int i=0; i<features.size(); i++){
    cout << "On the " << i+1 << "\'th level of the search tree" << endl;
    double bestSoFarAccuracy = 0;
    double accuracy = 0;
    int featureToAddAtThisLevel;
    for (int k=0; k<features[i].size(); k++){
      if ( find(currentFeatureSet.begin(), currentFeatureSet.end(), k) == currentFeatureSet.end() ){ //if isEmpty(intersection(currentFeatureSet, k))
        cout << "--Considering adding the " << k+1 << "feature" << endl;
        
        vector<vector<double>> zeroedFeatures = features;
        vector<int> tmpFeatureSet = currentFeatureSet;
        tmpFeatureSet.push_back(k);
        featureIsolate(tmpFeatureSet, zeroedFeatures);
        
        
        accuracy = leave1OutCrossValidation(zeroedFeatures, classType); //why k+1?
        
      }
      if (accuracy > bestSoFarAccuracy) {
        cout << accuracy << endl;
        bestSoFarAccuracy = accuracy;
        featureToAddAtThisLevel = k;
      }
    }
    if (levelBest < bestSoFarAccuracy){
      levelBest = bestSoFarAccuracy;
      breakflag = false;
    }
    else{
      if (breakflag){
        for (int i=0; i<bestOverall.size(); i++){
          cout << bestOverall[i]+1 << ", ";
        }
        break;
      }
      breakflag = true;
      cout << "(Warning, Accuracy has decreased! Continuing search in case of local maxima)" << endl; 
    }
    
    
    cout << "On level " << i+1 << " we added " << featureToAddAtThisLevel+1 << " to the  current set." << endl; 
    currentFeatureSet.push_back(featureToAddAtThisLevel); //featureToAddAtThisLevel = k
    bestOverall.push_back(featureToAddAtThisLevel);
    if (!breakflag){
      bestOverall = currentFeatureSet;
    }
  }
  return currentFeatureSet;
}


void featureIsolate(vector<int>& tmpFeatureSet, vector<vector<double>>& zeroedFeatures){
  for (int i=0; i <zeroedFeatures.size(); i++){
    for (int j=0; j<zeroedFeatures[0].size(); j++){
      if ( find(tmpFeatureSet.begin(), tmpFeatureSet.end(), j) == tmpFeatureSet.end() ){ //if isEmpty(intersection(currentFeatureSet, k))
        zeroedFeatures[i][j] = 0.0;
      }
    }
  }
  
}





//the cheat is to push it back again at the end. where is the end? where should i readd? 


void backwardSearch(vector<vector<double>>& features, vector<int> & classType){
  vector<int> currentFeatureSet = {0,1,2,3,4,5,6,7,8,9};
  vector<int> bestOverall = currentFeatureSet;
  bool breakflag = false;
  double levelBest=0;
  
  
  
  for (int i=0; i<features.size(); i++){
    cout << "On the " << i+1 << "\'th level of the search tree" << endl;
    
    
    
    double bestSoFarAccuracy = 0;
    double accuracy = 0;
    int featureToAddAtThisLevel;
    
    
    
    for (int k=0; k<features[i].size(); k++){
      currentFeatureSet.erase(currentFeatureSet.begin()+k);
      
        
        
        
        
        vector<vector<double>> zeroedFeatures = features;
        vector<int> tmpFeatureSet = currentFeatureSet;
        featureIsolate(tmpFeatureSet, zeroedFeatures);
        
        
        accuracy = leave1OutCrossValidation(zeroedFeatures, classType); //why k+1?
        
      if (accuracy > bestSoFarAccuracy) {
        cout << accuracy << endl;
        bestSoFarAccuracy = accuracy;
        featureToAddAtThisLevel = k; //this is essentially featureToRemove
      }
      currentFeatureSet.insert(currentFeatureSet.begin()+k,k);
    }
    if (levelBest < bestSoFarAccuracy){
      levelBest = bestSoFarAccuracy;
      breakflag = false;
    }
    else{
      if (breakflag){
        for (int i=0; i<bestOverall.size(); i++){
          cout << bestOverall[i]+1 << ", ";
        }
        break;
      }
      breakflag = true;
      cout << "(Warning, Accuracy has decreased! Continuing search in case of local maxima)" << endl; 
    }
    
    cout << "On level " << i+1 << " we remove " << featureToAddAtThisLevel+1 << " to the  current set." << endl; 
    currentFeatureSet.erase(currentFeatureSet.begin()+featureToAddAtThisLevel); //erase instead of push_back
    bestOverall.erase(currentFeatureSet.begin()+featureToAddAtThisLevel);
    
    if (!breakflag){
      bestOverall = currentFeatureSet;
    }
  }
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
  if (choice == 1){
    cout << "This dataset has " << features[0].size() << " features (not including the class attribute), with "<< classType.size() <<" instances." << endl << endl;
    cout << "Running nearest neighbor with all "<< features[0].size() << " features, using \"leaving-one-out\" evaluation, I get an \naccuracy of " << leave1OutCrossValidation(features, classType)*100<< "%" << endl << endl;
    cout << "Beginning Search." << endl << endl;
    
    vector<int> bestFeatures = nearestNeighbor(features, classType);
  
    cout << endl;
  }
  else if (choice == 2){
    cout << "This dataset has " << features[0].size() << " features (not including the class attribute), with "<< classType.size() <<" instances." << endl << endl;
    cout << "Running nearest neighbor with all "<< features[0].size() << " features, using \"leaving-one-out\" evaluation, I get an \naccuracy of " << leave1OutCrossValidation(features, classType)*100<< "%" << endl << endl;
    cout << "Beginning Search." << endl << endl;
    
    backwardSearch(features, classType);
    cout << endl;
  }
  else{
    
    cout << endl;
  }
  
  
  
  
  
  
  
  
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