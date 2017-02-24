/*This program runs the classification algortigm K Nearest Neighbors
The program reads in the training set and the test set for a data set.
It prints out a summary of how many classes there are,
how many instances of each class there are,
and how many attributes there are.

The knn classifier is then used with the training set to access the the classification accauracy over the test set.
The process is doing using three different similarity/dissimilarity measures: 
-Euclidean distance
-cosine similarity
-Pearson correlation
-others if implemented

The programs output a report for each dataset to a text file:
-the name if the training set input file
-the name of the test set input file
-the name of the similarity measure used
-the overall accuraccy of your decision tree (what percentage of teh points of each class were classified correctly)
-the precision and recall of each class
-a confucion matrix of how often each class was predicted as belonging to every class

*/

//include program dependancies
#include "header.h"
using namespace std;

class attributes {

};

class datapoint {
	vector<attributes> attributes;
};

void readFrom(string test, string training, vector<datapoint> testVect, vector<datapoint> trainingVect) {
	string line;
	//store data points from test set into test vector
	ifstream testFile;
	testFile.open(test);

	if (testFile.is_open()) {
		while (getline(testFile, line)) {
			cout << line << '\n';
		}
	}
	else {
		cout << "file not found" << endl;
	}
	testFile.close();

	//store data points from training set into training vector

}

int main(){
	cout << "hello k nearest neighbors" << endl;
	vector<datapoint> testVect, trainingVect;
	//read in training set and test set 
	readFrom("car_data_testset.csv", "car_data_trainset.csv", testVect, trainingVect);
	system("pause");
	return 0;
}