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
-the overall accuracy of your decision tree (what percentage of the points of each class were classified correctly)
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
	string Class;
	vector<string> attributes;
	
};

class datasetinfo {
public:
	//index of what attributes are classes
	int class_index;
	//count of attributes
	int attributes;
	//map to count of different instances of classes
	map<string, int> class_instances;
};

void readFrom(string test, string training, vector<datapoint> & testVect, vector<datapoint> & trainingVect, datasetinfo & dsinfo);
void store(string set, vector<datapoint> & vect, datasetinfo & dsinfo);
void countClassInstances(datapoint & d, datasetinfo & dsinfo, int attribute_index, string token);
void outputDataSetInfo(datasetinfo & dsinfo);
void outputSet(string type, string set, vector<datapoint> & vect);

template <class T>
void knearestneigbors(int k, vector<datapoint> & trainingVect, vector<datapoint> & testVect, double(*dist)(vector<T>, vector<T>), datasetinfo);
double euclidean(vector<string> p, vector<string> q);
double euclidean2(vector<double> p, vector<double> q);

int main(){
	cout << "hello k nearest neighbors" << endl;
	vector<double> a = { 48,32,22,14,4,2 };
	vector<double> b = { 44,18,18,23,2,2 };
	cout << euclidean2(a, b);
	system("pause");

	vector<datapoint> testVect, trainingVect;
	datasetinfo bupa_data_info;
	bupa_data_info.class_index = 6;
	//read in training set and test set 
	readFrom("bupa_data_testset.csv", "bupa_data_trainset.csv", testVect, trainingVect, bupa_data_info);


	knearestneigbors(3,trainingVect, testVect, euclidean, bupa_data_info);

	system("pause");
	return 0;
}

void readFrom(string test, string training, vector<datapoint> & testVect, vector<datapoint> & trainingVect, datasetinfo & dsinfo) {
	
	//initialize class instance tracker
	/*for (int i = 0; i < dsinfo.classes.size(); ++i) {
		map<string, int> * d = new map<string, int>;
		dsinfo.class_instances.push_back(d);
	}*/

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
			//int class_index = 0;
			//split line on delimiter ','
			while (getline(ss, token, ',')) {
				//cout << token << '\n';
				//store tokens in datapoint attributes
				d.attributes.push_back(token);
				//cout << "c_i" << class_index << endl;
				//if attribute is class, record and count instances
				countClassInstances(d, dsinfo, attribute_index, token);
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

void countClassInstances(datapoint & d, datasetinfo & dsinfo, int attribute_index, string token) {
	if (attribute_index == dsinfo.class_index) {
		std::map<string, int>::iterator IT = dsinfo.class_instances.find(token);
		if (IT == dsinfo.class_instances.end())
		{
			//add instance to map list with count equals 1
			//cout << "adding new instance" << endl;
			dsinfo.class_instances.insert(std::pair<string, int>(token, 1));
		}
		else {
			//increment count of instance
			//cout << "increment instance" << endl;
			IT->second += 1;
		}
		d.Class = token;
	}
}

//output summary of how many classes there are, how many instances of each class there are, and how many attributes there are.
void outputDataSetInfo(datasetinfo & dsinfo) {
	map<string, int>::iterator mapITER;

	cout << "Number of classes: " << dsinfo.class_instances.size() << endl;
	cout << "Instances of each class: " << endl;
	//for each instance of class, output count of instance
	for (mapITER = dsinfo.class_instances.begin(); mapITER != dsinfo.class_instances.end(); mapITER++)
	{
		cout << mapITER->first << ": " << mapITER->second <<endl;
	}
	cout << "Number of non-class attributes: " << dsinfo.attributes - 1 << endl << endl;
}

//output datapoints and attributes
void outputSet(string type, string set, vector<datapoint> & vect) {
	cout << type << " set file: " << set << endl;
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

template <class T>
void knearestneigbors(int k, vector<datapoint> & trainingVect, vector<datapoint> & testVect, double (*dist)(vector<T>, vector<T>), datasetinfo dsinfo) {
	//1. Let k equal the nearest neighbors and D be the set of training examples

	//2. for each test example z = (x',y') do
	for (int i = 0; i <= testVect.size()-1; i++) {
		//		3. Compute d(x',x), the distance between z and every example (x,y) within D
		multimap<double, string> knearest;
		for (int j = 0; j <= trainingVect.size() - 1; j++) {
			//comupte similarity distance
			double simdist = dist(testVect.at(i).attributes, trainingVect.at(j).attributes);
			//		4. Select Dz subset or equal to D, the set of k closest training examples to z.
			knearest.emplace(simdist, trainingVect.at(j).attributes.at(dsinfo.class_index));
			if (knearest.size() > k) {
				//map is sorted smallest to largest so delete element at end
				knearest.erase(--knearest.end());
			}
		}
		//output knearest
		cout << "test emaple class = " << testVect.at(i).attributes.at(dsinfo.class_index) << endl;
		cout << k << " nearest neighbors: " << endl;
		for (map<double, string>::iterator mapITER = knearest.begin(); mapITER != knearest.end(); mapITER++)
		{
			cout << mapITER->first << ": " << mapITER->second << endl;
			
		}
		//get majority
		//compare knearest classes to actual

		//store results
		system("pause");
		cout << endl;
		
		//		5. y' = argmax(v) (sum of (xi, yi) within dz) I(v = yi)
	}
}

// Euclidean Distance
double euclidean(vector<string> p, vector<string> q) {
	int k;
	double returnVal = 0;
	std::set<int>::iterator setIT;

	//summation: all attibutes excluding last one (class)
	for (k = 0; k <= p.size() - 2; k++) {
		double diff = atof(p[k].c_str()) - atof(q[k].c_str());
		returnVal += pow(diff, 2.0);
	}

	// take the square root of the end result from the summation and return that value
	returnVal = sqrt(returnVal);
	//cout << "Euclidean Distance = " << returnVal << endl;
	return returnVal;
}

// Euclidean Distance
double euclidean2(vector<double> p, vector<double> q) {
	int k;
	double returnVal = 0;
	std::set<int>::iterator setIT;

	// the summation part in the slides
	for (k = 0; k <= p.size() - 1; k++) {
				double diff = p[k]- q[k];
				returnVal += pow(diff, 2.0);
	}

	// take the square root of the end result from the summation and return that value
	returnVal = sqrt(returnVal);
	//cout << "Euclidean Distance = " << returnVal << endl;
	return returnVal;
}