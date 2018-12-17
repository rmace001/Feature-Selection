#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <limits>
#include <chrono>
using namespace std;
using namespace std::chrono;
bool readData(string filename, vector<vector<double>>& features, vector<int> &classType);
vector<int>  nearestNeighbor(vector<vector<double>>& features, vector<int> & classType, vector<double> & AccuracyVector);
void backwardSearch(vector<vector<double>>& features, vector<int> & classType, vector<double> & AccuracyVector);
void rSearch(vector<vector<double>>& features, vector<int> & classType, vector<double> & AccuracyVector);
double leave1OutCrossValidation(vector<vector<double>>& features, vector<int> &classType);
double leave1OutCrossValidation_v2(vector<vector<double>>& features, vector<int> &classType, int& prevIncorrect, int& minIncorrect);
void featureIsolate(vector<int>& tmpFeatureSet, vector<vector<double>>& zeroedFeatures);
int globalIncorrect = 0;


bool readData(string filename, vector<vector<double>>& features, vector<int> &classType)
{
  if (filename == "small99.txt" || filename == "108.txt"){
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
  }
  else{
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
        if (count <= 101){
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
  }
  
  
  return true;
}

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
  // cout << "correct: " << correct << endl;
  globalIncorrect = 200-correct;
  return (correct/200);
}



double leave1OutCrossValidation_v2(vector<vector<double>>& features, vector<int> &classType, int& prevIncorrect, int& minIncorrect)
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
    else{
      prevIncorrect++;///////////////////////////////////////////////////////////////////////////////////////////
      if (prevIncorrect > minIncorrect){////////////////////////////////////////////////////////////////////////
        return 0;
      }
    }
    skippedI++;
  }
  
  // cout << "correct: " << correct << endl;
  // cout << "incorrect: " << prevIncorrect << endl;
  
  return (correct/200);
}


//Might want to have this function return a vector, which is vector currentFeatureSet
vector<int> nearestNeighbor(vector<vector<double>>& features, vector<int> & classType, vector<double> & AccuracyVector)
{
  vector<int> currentFeatureSet;
  vector<int> bestOverall;
  double globalBest=0;
  double levelBest=0;
  for (int i=0; i<features[0].size(); i++){
    double bestSoFarAccuracy = 0;
    double accuracy = 0;
    int featureToAddAtThisLevel;
    for (int k=0; k<features[i].size(); k++){
      if ( find(currentFeatureSet.begin(), currentFeatureSet.end(), k) == currentFeatureSet.end() ){ //if isEmpty(intersection(currentFeatureSet, k))
        vector<vector<double>> zeroedFeatures = features;
        vector<int> tmpFeatureSet = currentFeatureSet;
        tmpFeatureSet.push_back(k);
        cout << "        Using feature(s) {";
        cout << tmpFeatureSet[0]+1;
        for (int i=1; i<tmpFeatureSet.size(); i++){
          cout<< "," << tmpFeatureSet[i]+1;
        }
        cout << "} ";
        featureIsolate(tmpFeatureSet, zeroedFeatures);
        
        
        accuracy = leave1OutCrossValidation(zeroedFeatures, classType); 
        cout << "accuracy is "<< accuracy*100 <<"%"<< endl;
        AccuracyVector.push_back(accuracy);
      }
      if (accuracy > bestSoFarAccuracy) {
        bestSoFarAccuracy = accuracy;
        featureToAddAtThisLevel = k;
      }
    }
    if (levelBest < bestSoFarAccuracy){
      levelBest = bestSoFarAccuracy;
      if (globalBest < levelBest){
        globalBest = levelBest;
        bestOverall = currentFeatureSet;
        bestOverall.push_back(featureToAddAtThisLevel);
      }
    }
    else{
      if (i<features[0].size()-1)
        cout << "\n(Warning, Accuracy has decreased! Continuing search in case of local maxima)" ; 
    }
    
    if ( find(currentFeatureSet.begin(), currentFeatureSet.end(), featureToAddAtThisLevel) == currentFeatureSet.end() ){
      currentFeatureSet.push_back(featureToAddAtThisLevel); 
        if (i<features[0].size()-1){
          cout << "\nFeature set {";
          cout << currentFeatureSet[0]+1;
          for (int l=1; l<currentFeatureSet.size(); l++){
            cout<< "," << currentFeatureSet[l]+1;
          }
          cout << "} was best, accuracy is " << bestSoFarAccuracy*100 << "%\n \n";
        }
    }

  }
  cout << "\nFinished Search!! The best feature subset is {";
  cout << bestOverall[0]+1;
  for (int i=1; i<bestOverall.size(); i++){
    cout  << ", " << bestOverall[i]+1;
  }
  cout <<"}, which has an accuracy of " << globalBest*100 << "%"<< endl;
  return bestOverall;
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


void backwardSearch(vector<vector<double>>& features, vector<int> & classType, vector<double> & AccuracyVector){
  vector<int> currentFeatureSet;
  
  
  for (int i=0; i<features[0].size(); i++){
    currentFeatureSet.push_back(i);
  }
  for (int i=0; i<currentFeatureSet.size(); i++){
    cout << currentFeatureSet[i];
  }
  
  cout << endl;
  vector<int> bestOverall;// = currentFeatureSet;
  bool breakflag = false;
  double levelBest=0;
  double globalBest =0;
  for (int i=0; i<currentFeatureSet.size(); i++){

    double bestSoFarAccuracy = 0;
    double accuracy = 0;
    int featureToAddAtThisLevel;
    
    for (int k=0; k<currentFeatureSet.size(); k++){ //1:04pm: changing features[i].size() to currentFeatures.size() 
      
      
      currentFeatureSet.erase(currentFeatureSet.begin()+k);
      if (currentFeatureSet.size() >=1){  
        cout << "        Using feature(s) {";
        cout << currentFeatureSet[0]+1;
        for (int i=1; i<currentFeatureSet.size(); i++){
          cout<< "," << currentFeatureSet[i]+1;
        }
        cout << "} ";
      }
      vector<vector<double>> zeroedFeatures = features;
      vector<int> tmpFeatureSet = currentFeatureSet;
      
      featureIsolate(tmpFeatureSet, zeroedFeatures);
      accuracy = leave1OutCrossValidation(zeroedFeatures, classType); 
      AccuracyVector.push_back(accuracy);
      cout << "accuracy is "<< accuracy*100 <<"%"<< endl;
      if (accuracy > bestSoFarAccuracy) {
        bestSoFarAccuracy = accuracy;
        featureToAddAtThisLevel = k; //this is essentially featureToRemove
        
      }
      currentFeatureSet.insert(currentFeatureSet.begin()+k,k);
    }
    if (levelBest < bestSoFarAccuracy){
      
      levelBest = bestSoFarAccuracy;
      if (globalBest < levelBest){
        
        globalBest = levelBest;
        
        bestOverall = currentFeatureSet;
        //bestOverall.push_back(featureToAddAtThisLevel);
        bestOverall.erase(bestOverall.begin()+featureToAddAtThisLevel);
      }
    }
    else{
      cout << "(Warning, Accuracy has decreased! Continuing search in case of local maxima)" << endl; 
    }
    currentFeatureSet.erase(currentFeatureSet.begin()+featureToAddAtThisLevel); //erase instead of push_back
    cout << "\nFeature set {";
    cout << currentFeatureSet[0]+1;
    for (int l=1; l<currentFeatureSet.size(); l++){
      cout<< "," << currentFeatureSet[l]+1;
    }
    cout << "} was best, accuracy is " << bestSoFarAccuracy*100 << "%\n \n";
  }
  cout << "\nFinished Search!! The best feature subset is {";
  cout << bestOverall[0]+1;
  for (int i=1; i<bestOverall.size(); i++){
    cout  << ", " << bestOverall[i]+1;
  }
  cout <<"}, which has an accuracy of " << globalBest*100 << "%"<< endl;
}










void rSearch(vector<vector<double>>& features, vector<int> & classType, vector<double> & AccuracyVector)
{
  vector<int> currentFeatureSet;
  vector<int> bestOverall;
  double levelBest=0;
  double globalBest=0;
  int levelIncorrect = globalIncorrect;
  for (int i=0; i<features[0].size(); i++){
    double bestSoFarAccuracy = 0;
    //levelBest=0;
    int minIncorrect = 0;///////////////////////////////
    double accuracy;
    int prevIncorrect = 0;////////////////////////////////////////////////////////////////////////////////////////////
    int featureToAddAtThisLevel;
    for (int k=0; k<features[i].size(); k++){
      accuracy = 0;/////////////////////////////////////////////////////////////////////////////////////////////////
      prevIncorrect = 0;/////////////////////////////////////////////////////////////////////////////////////////////
      if ( find(currentFeatureSet.begin(), currentFeatureSet.end(), k) == currentFeatureSet.end() ){ //if isEmpty(intersection(currentFeatureSet, k))
        vector<vector<double>> zeroedFeatures = features;
        vector<int> tmpFeatureSet = currentFeatureSet;
        tmpFeatureSet.push_back(k);
        cout << "        Using feature(s) {";
        cout << tmpFeatureSet[0]+1;
        for (int i=1; i<tmpFeatureSet.size(); i++){
          cout<< "," << tmpFeatureSet[i]+1;
        }
        cout << "} ";
        featureIsolate(tmpFeatureSet, zeroedFeatures);
        accuracy = leave1OutCrossValidation_v2(zeroedFeatures, classType, prevIncorrect, levelIncorrect);
        cout << "accuracy is "<< accuracy*100 <<"%"<< endl;
        AccuracyVector.push_back(accuracy);
      }
      if (accuracy > bestSoFarAccuracy) {
        bestSoFarAccuracy = accuracy;
        minIncorrect = prevIncorrect;/////////////////////////////////////////////////////////////////////////////////
        featureToAddAtThisLevel = k;
      }
    }
    if (levelBest < bestSoFarAccuracy)
    {
      levelBest = bestSoFarAccuracy;
      levelIncorrect = minIncorrect;
      if (globalBest < levelBest){
        globalBest = levelBest;
        bestOverall = currentFeatureSet;
        bestOverall.push_back(featureToAddAtThisLevel);
      }
    }
    else
    {
      if (i<features[0].size()-1)
        cout << "\n(Warning, Accuracy has decreased! Continuing search in case of local maxima)\n"; 
    }
    
    if ( find(currentFeatureSet.begin(), currentFeatureSet.end(), featureToAddAtThisLevel) == currentFeatureSet.end() ){
      currentFeatureSet.push_back(featureToAddAtThisLevel); //featureToAddAtThisLevel = k
      if (i<features[0].size()-1){
          cout << "\nFeature set {";
          cout << currentFeatureSet[0]+1;
          for (int l=1; l<currentFeatureSet.size(); l++){
            cout<< "," << currentFeatureSet[l]+1;
          }
          cout << "} was best, accuracy is " << bestSoFarAccuracy*100 << "%\n \n";
        }
    }
  }
  
  cout << "\nFinished Search!! The best feature subset is {";
  cout << bestOverall[0]+1;
  for (int i=1; i<bestOverall.size(); i++){
    cout  << ", " << bestOverall[i]+1;
  }
  cout <<"}, which has an accuracy of " << globalBest*100 << "%"<< endl;
  return;
}



int main()
{
  //data vectors
  vector<vector<double>> features;
  vector<int> classType;
  vector<double> AccuracyVector;
  string inputfile; 
  int choice; 
  
  
  cout << "Welcome to Rogelio's Feature Selection Algorithm." << endl;
  cout << "Type the  name of the file to test: ";
  cin >> inputfile;
  cout << endl;
  cout << "Type the number of the algorithm you you want to run." << endl << endl;
  cout << "    1) Foward Selection" << endl;
  cout << "    2) Backward Elimination" << endl;
  cout << "    3) rSearch Special Algorithm." << endl << endl << "                                  ";
  cin >> choice; 
  //return if file not read properly
  if (!readData(inputfile, features, classType))
    return 0;
  if (choice == 1){
    cout << "This dataset has " << features[0].size() << " features (not including the class attribute), with "<< classType.size() <<" instances." << endl << endl;
    cout << "Running nearest neighbor with all "<< features[0].size() << " features, using \"leaving-one-out\" evaluation, I get an \naccuracy of " << leave1OutCrossValidation(features, classType)*100<< "%" << endl << endl;
    cout << "Beginning Search." << endl << endl;
    
    auto start = high_resolution_clock::now(); 
    vector<int> bestFeatures = nearestNeighbor(features, classType, AccuracyVector);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start); 
    cout << "Time taken by function: "
        << duration.count() << " seconds" << endl;
  }
  else if (choice == 2){
    cout << "This dataset has " << features[0].size() << " features (not including the class attribute), with "<< classType.size() <<" instances." << endl << endl;
    cout << "Running nearest neighbor with all "<< features[0].size() << " features, using \"leaving-one-out\" evaluation, I get an \naccuracy of " << leave1OutCrossValidation(features, classType)*100<< "%" << endl << endl;
    cout << "Beginning Search." << endl << endl;
    
    auto start = high_resolution_clock::now(); 
    backwardSearch(features, classType, AccuracyVector);
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<seconds>(stop - start); 
    cout << "Time taken by function: "
        << duration.count() << " seconds" << endl;
  }
  else{
    cout << "This dataset has " << features[0].size() << " features (not including the class attribute), with "<< classType.size() <<" instances." << endl << endl;
    cout << "Running nearest neighbor with all "<< features[0].size() << " features, using \"leaving-one-out\" evaluation, I get an \naccuracy of " << leave1OutCrossValidation(features, classType)*100<< "%" << endl << endl;
    cout << "Beginning Search." << endl << endl;
    auto start = high_resolution_clock::now(); 
    rSearch(features, classType, AccuracyVector);
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<seconds>(stop - start); 
    cout << endl <<"Time taken by function: "
        << duration.count() << " seconds" << endl;
  }
  cout << AccuracyVector.size();
  // cout << "Accuracies" << endl;
  // for (int i=0; i<AccuracyVector.size(); i++){
  //   cout << AccuracyVector[i]*100 << "\n";
  // }
  
  cout << endl;
  
  
  
  
  return 0;
}


