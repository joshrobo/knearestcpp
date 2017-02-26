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

union attribute {
	int i; 
	float f;
	string s;
};

class datapoint {
public:
	vector<string> attributes;
};

class datasetinfo {
public:
	//indexes of what attributes are classes
	set<int> classes;
	//count of attributes
	int attributes;
	//map to count of different instances of classes
	vector<map<string, int>*> class_instances;
};

void readFrom(string test, string training, vector<datapoint> & testVect, vector<datapoint> & trainingVect, datasetinfo & dsinfo);
void store(string set, vector<datapoint> & vect, datasetinfo & dsinfo);
void countClassInstances(datasetinfo & dsinfo, int attribute_index, int &class_index, string token);
void outputDataSetInfo(datasetinfo & dsinfo);
void outputSet(string type, string set, vector<datapoint> & vect);

void knearestneigbors(int k, vector<datapoint> & trainingVect, double(*dist)(vector<attribute>, vector<attribute>));


int main(){
	cout << "hello k nearest neighbors" << endl;

	vector<datapoint> testVect, trainingVect;
	datasetinfo bupa_data_info;
	bupa_data_info.classes = {6};
	//read in training set and test set 
	readFrom("bupa_data_testset.csv", "bupa_data_trainset.csv", testVect, trainingVect, bupa_data_info);


	system("pause");
	return 0;
}

void readFrom(string test, string training, vector<datapoint> & testVect, vector<datapoint> & trainingVect, datasetinfo & dsinfo) {
	
	//initialize class instance tracker
	for (int i = 0; i < dsinfo.classes.size(); ++i) {
		map<string, int> * d = new map<string, int>;
		dsinfo.class_instances.push_back(d);
	}

	//store data points from set into corresponding vector
	store(test, testVect, dsinfo);
	store(training, trainingVect, dsinfo);

	outputDataSetInfo(dsinfo);

	//output for debugging
	outputSet("test", test, testVect);
	outputSet("training", training, trainingVect);
}

//store data points from set into vector
void store(string set, vector<datapoint> & vect, datasetinfo & dsinfo) {
	std::set<int>::iterator setIT;
	int attributeMax = 0;
	string line;
	string token;

	ifstream testFile;
	testFile.open(set);



	if (testFile.is_open()) {
		//read in each line of file
		while (getline(testFile, line)) {
			istringstream ss(line);
			datapoint d;
			int attribute_index = 0;
			int class_index = 0;
			//split line on delimiter ','
			while (getline(ss, token, ',')) {
				//cout << token << '\n';
				//store tokens in datapoint attributes
				d.attributes.push_back(token);
				//cout << "c_i" << class_index << endl;
				//if attribute is class, record and count instances
				countClassInstances(dsinfo, attribute_index, class_index, token);
				attribute_index++;

				//track number of attributes
				if (attributeMax < attribute_index) attributeMax = attribute_index;
			}
			//store datapoint in testVect
			vect.push_back(d);
		}
		//set number of attributes
		dsinfo.attributes = attributeMax;
	}
	else {
		cout << "file not found" << endl;
	}
	testFile.close();
}

void countClassInstances(datasetinfo & dsinfo, int attribute_index, int & class_index, string token) {
	std::set<int>::iterator setIT;
	for (setIT = dsinfo.classes.begin(); setIT != dsinfo.classes.end(); setIT++) {
		if (attribute_index == *setIT) {
			//cout << "a_i: " << attribute_index << endl;
			//attribute is a class
			//if class not added to class list, add to class list

			//map<string, int> * d = new map<string, int>;
			//dsinfo.class_instances.push_back(d);

			//cout << "c_i: " << class_index << endl;
			std::map<string, int>::iterator IT = dsinfo.class_instances.at(class_index)->find(token);
			if (IT == dsinfo.class_instances.at(class_index)->end())
			{
				//add instance to map list with count equals 1
				//cout << "adding new instance" << endl;
				dsinfo.class_instances.at(class_index)->insert(std::pair<string, int>(token, 1));
			}
			else {
				//increment count of instance
				//cout << "increment instance" << endl;
				IT->second += 1;
			}
			class_index++;
		}
	}
}

//output summary of how many classes there are, how many instances of each class there are, and how many attributes there are.
void outputDataSetInfo(datasetinfo & dsinfo) {
	vector<map<string, int>*>::iterator listITER;
	map<string, int>::iterator mapITER;
	int i = 1;

	cout << "Number of classes: " << dsinfo.classes.size() << endl;
	cout << "Instances of each class: " << endl;
	//for each class
	for (listITER = dsinfo.class_instances.begin(); listITER != dsinfo.class_instances.end(); listITER++) {
		cout << "Class #" << i << endl;
		//for each instance of class, output count of instance
		for (mapITER = (*listITER)->begin(); mapITER != (*listITER)->end(); mapITER++)
		{
			cout << mapITER->first << ':'<< mapITER->second <<endl;
		}
		i++;
	}
	cout << "Number of attributes: " << dsinfo.attributes << endl << endl;
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

void knearestneigbors(int k, vector<datapoint> & trainingVect, double (*dist)(vector<attribute>, vector<attribute>)) {
	//1. Let k equal the nearest neighbors and D be the set of training examples
	//2. for each test example z = (x',y') do
	//		3. Compute d(x',x), the distance between z and every example (x,y) within D
	//		4. Select Dz subset or equal to D, the set of k closest training examples to z.
	//		5. y' = argmax(v) (sum of (xi, yi) within dz) I(v = yi)

}