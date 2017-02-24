/*This program runs the classification algortigm K Nearest Neighbors
The program reads in the training set and the test set for a data set.
It prints out a summary of how many classes there are,
how many instances of each class there are,
and how many attributes there are.

The knn classifier is then used with the training set to access the the classification accauracy over the test set.
The process is done using three different similarity/dissimilarity measures: 
-Euclidean distance
-cosine similarity
-Pearson correlation
-others if implemented

The programs output a report for each dataset to a text file:
-the name if the training set input file
-the name of the test set input file
-the name of the similarity measure used
-the overall accuraccy of your decision tree (what percentage of the points of each class were classified correctly)
-the precision and recall of each class
-a confucion matrix of how often each class was predicted as belonging to every class

*/

//include program dependancies
#include "header.h"
using namespace std;

class attributes {
	string s;
};

class datapoint {
public:
	vector<string> attributes;
};

void readFrom(string test, string training, vector<datapoint> & testVect, vector<datapoint> & trainingVect);
void store(string set, vector<datapoint> & vect);
void outputSet(string type, string set, vector<datapoint> & vect);

int main(){
	cout << "hello k nearest neighbors" << endl;

	vector<datapoint> testVect, trainingVect;
	//read in training set and test set 
	readFrom("bupa_data_testset.csv", "bupa_data_trainset.csv", testVect, trainingVect);

	//prints out a summary of how many classes there are,
	//how many instances of each class there are,
	//and how many attributes there are.

	system("pause");
	return 0;
}

void readFrom(string test, string training, vector<datapoint> & testVect, vector<datapoint> & trainingVect) {
	//store data points from set into corresponding vector
	store(test, testVect);
	store(training, trainingVect);

	//output for debugging
	outputSet("test", test, testVect);
	outputSet("training", training, trainingVect);
}

//store data points from set into vector
void store(string set, vector<datapoint> & vect) {
	string line;
	string token;

	ifstream testFile;
	testFile.open(set);

	if (testFile.is_open()) {
		//read in each line of file
		while (getline(testFile, line)) {
			istringstream ss(line);
			datapoint d;
			//split line on delimiter ','
			while (getline(ss, token, ',')) {
				//cout << token << '\n';
				//store tokens in datapoint attributes
				d.attributes.push_back(token);
			}
			//store datapoint in testVest
			vect.push_back(d);
		}
	}
	else {
		cout << "file not found" << endl;
	}
	testFile.close();
}

//output datapoints and attributes
void outputSet(string type, string set, vector<datapoint> & vect) {
	cout << type << " set file:" << set << endl;
	vector<datapoint>::iterator it;

	//visit each datapoint
	for (it = vect.begin(); it != vect.end(); ++it) {
		//print each attribute
		for (vector<string>::iterator it2 = it->attributes.begin(); it2 != it->attributes.end(); ++it2) {
			cout << *it2 << ' ';
		}
		cout << endl;
	}
	cout << endl;
}
