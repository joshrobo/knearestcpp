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
	//positive class
	int pos_class;
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
double cosine_similarity(vector<string> d1, vector<string> d2);
double correlation(vector<string> x, vector<string> y);
double covarianceXY(vector<string> x, vector<string> y, double meanX, double meanY);
double standardDeviation(double variance);
double variance(vector<string>data, double mean);
double arithmeticMean(vector<string>data);

string distanceWeightedVoting(multimap<double, string> kn, datasetinfo dsinfo);
void confusion(vector < vector <int>> & matrix, string actual, string predicted);
void outputConfusionMatrix(vector<vector<int>> & m, datasetinfo dsinfo);

int main(){
	cout << "hello k nearest neighbors" << endl;
	vector<string> a = { "48","32","22","14","4","2" };
	vector<string> b = { "44","18","18","23","2","2" };
	cout << euclidean(a, b) << endl;
	cout << correlation(a, b) << endl;
	cout << cosine_similarity(a, b) << endl;
	
	system("pause");

	vector<datapoint> testVect, trainingVect;
	datasetinfo bupa_data_info;
	bupa_data_info.class_index = 6;
	bupa_data_info.pos_class = 0;
	//read in training set and test set 
	readFrom("bupa_data_testset.csv", "bupa_data_trainset.csv", testVect, trainingVect, bupa_data_info);

	knearestneigbors(4,trainingVect, testVect, euclidean, bupa_data_info);
	system("pause");
	knearestneigbors(4, trainingVect, testVect, cosine_similarity, bupa_data_info);
	system("pause");

	vector<datapoint> testVect2, trainingVect2;
	datasetinfo car_data_info;
	car_data_info.class_index = 6;
	car_data_info.pos_class = 3;
	//read in training set and test set 
	readFrom("car_data_testset.csv", "car_data_trainset.csv", testVect2, trainingVect2,car_data_info);

	knearestneigbors(4, trainingVect2, testVect2, euclidean, car_data_info);

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
	int numClasses = dsinfo.class_instances.size();
	//1. Let k equal the nearest neighbors and D be the set of training examples
	vector<vector<int>> confusion_matrix(numClasses, vector<int>(numClasses, 0));
	
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
				//what if a tie occurs? eg distances are the same
			}
		}
		//output knearest
		cout << "test example class = " << testVect.at(i).attributes.at(dsinfo.class_index) << endl;
		//cout << k << " nearest neighbors: " << endl;
		for (map<double, string>::iterator mapITER = knearest.begin(); mapITER != knearest.end(); mapITER++)
		{
			cout << mapITER->first << ": " << mapITER->second << endl;
			
		}
		//get majority
		//		5. y' = argmax(v) (sum of (xi, yi) within dz) I(v = yi)
		confusion(confusion_matrix, testVect.at(i).attributes.at(dsinfo.class_index), distanceWeightedVoting(knearest, dsinfo));
		
		cout << endl;
		//compare knearest classes to actual

		//store results
		//system("pause");
		cout << endl;

	}

	outputConfusionMatrix(confusion_matrix,dsinfo);
}

void confusion(vector < vector <int>> & matrix, string actual, string predicted) {
	int act = atoi(actual.c_str()) - 1;
	int pre = atoi(predicted.c_str()) - 1;
	//if (actual == predicted) {
		//increase true positive
		matrix.at(act).at(pre) += 1;
	//}
	//else {
	//	//increase 
	//	for (int i = 0; i <= matrix.size()-1; i++) {
	//		if(i == pre){}
	//		else {
	//			matrix.at(i).at(pre) += 1;
	//		}
	//	}
	//}
}

void outputConfusionMatrix(vector<vector<int>> & m, datasetinfo dsinfo) {
	//every actual
	double overall_accuracy;
	double correct_pred = 0, total_pred = 0;
	double individual_accuracy;
	double corr_indiv = 0, total_indiv = 0;
	double precision;
	double recall;
	int TP = m.at(dsinfo.pos_class).at(dsinfo.pos_class);
	double FP = 0, FN = 0;
	for (int i = 0; i <= m.size()-1; i++) {
		//every predicted
		for (int j = 0; j <= m.size()-1; j++) {
			cout << m.at(i).at(j) << " ";
			if (i == j) {
				correct_pred += m.at(i).at(j);
				if (i != dsinfo.pos_class) FN += m.at(dsinfo.pos_class).at(j); //sum of x col without diagonal
				if (j != dsinfo.pos_class) FP += m.at(i).at(dsinfo.pos_class); //sum of x row without diagonal
			}
			total_pred += m.at(i).at(j);
		}
		
		cout << endl;
	}
	overall_accuracy = correct_pred / total_pred;
	cout << "Overall Accuracy = " << overall_accuracy << endl;
	cout << "Individual Accuaracy" << endl;
	//each class
	bool isCalc = false;
	for (int i = 0; i <= m.size() - 1; i++) {
		//its total is sum of column
		for (int j = 0; j <= m.size() - 1; j++) {
			if (i == j && isCalc == false) {
				corr_indiv = m.at(j).at(i);
				isCalc = true;
			}
				total_indiv += m.at(j).at(i);
		}
		cout << "Class" << i << " accuracy = " << corr_indiv / total_indiv << endl;
		corr_indiv = total_indiv = 0;
		isCalc = false;
	}
	precision = TP / (TP + FP);
	cout << "Precision = " << precision << endl;
	recall = TP / (TP + FN);
	cout << "Recall = " << recall << endl;
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

// Cosine Similarity
double cosine_similarity(vector<string> d1, vector<string> d2) {
	int i;
	double accumulatorDot = 0, accumulatorD1 = 0, accumulatorD2 = 0;

	// calculate dot product
	for (i = 0; i < d1.size(); i++) {
		accumulatorDot += atof(d1[i].c_str()) * atof(d2[i].c_str());
	}

	// calculate length of d1
	for (i = 0; i < d1.size(); i++) {
		accumulatorD1 += pow(atof(d1[i].c_str()), 2);
	}
	accumulatorD1 = pow(accumulatorD1, 0.5);

	// calculate length of d2
	for (i = 0; i < d1.size(); i++) {
		accumulatorD2 += pow(atof(d2[i].c_str()), 2);
	}
	accumulatorD2 = pow(accumulatorD2, 0.5);

	double cosSim = accumulatorDot / (accumulatorD1 * accumulatorD2);
	//cout << "cosine similarity = " << cosSim << endl;
	return cosSim;
}

// Correlation 
double correlation(vector<string> x, vector<string> y) {
	double meanX = arithmeticMean(x);
	double meanY = arithmeticMean(y);
	double covariance = covarianceXY(x, y, meanX, meanY);
	double stdX = standardDeviation(variance(x, meanX));
	double stdY = standardDeviation(variance(y, meanY));

	double corr = covariance / (stdX*stdY);
	//if ((stdX*stdY) == 0)
	//	cout << "correlation = division by zero" << endl;
	//else
	//	cout << "correlation = " << corr << endl;
	return corr;
}

// Calculating the covariance for correlation
double covarianceXY(vector<string> x, vector<string> y, double meanX, double meanY) {
	float sub = 0, n = x.size(), covar;
	for (int i = 0; i <= n - 1; i++) {
		sub += (atof(x[i].c_str()) - meanX)*(atof(y[i].c_str()) - meanY);
	}
	covar = sub;
	return double(covar);
}

// Calculating the standard deviation
//The extent of deviation for a group as a whole, square root of the variance
double standardDeviation(double variance) {
	return pow(variance, .5);
}

// Calculating the variance
// How far a set of numbers are spread out from their mean, the average of the squared differences from the mean
double variance(vector<string>data, double mean) {
	double variance, sub = 0, n = 0;
	//sum square of difference of value and mean
	for (string i : data) {
		sub += pow((atof(i.c_str()) - mean), 2.0);
		n++;
	}
	//divide by number of values
	variance = sub;
	return variance;
}

// Calculating the mean
//he average value of vector, the sum of the values divided by the number of values
double arithmeticMean(vector<string>data) {
	int n = data.size(); //total number of values
	double total = 0;

	//sum values in vector
	for (string i : data) {
		total += atof(i.c_str());
	}

	double mean = total / n;
	return mean;
}

string distanceWeightedVoting(multimap<double, string> kn, datasetinfo dsinfo) {
	
	map<string, double> class_instances;
	for (multimap<double, string>::iterator mapITER = kn.begin(); mapITER != kn.end(); mapITER++)
	{
		//calc weight of class instances in nearest
		double weight = 1 / pow(mapITER->first, 2);
		std::map<string, double>::iterator IT = class_instances.find(mapITER->second);
		if (IT == class_instances.end())
		{
			//add instance to map list with sum weight
		
			class_instances.insert(std::pair<string, int>(mapITER->second, weight));
		}
		else {
			//increment count of instance
			//cout << "increment instance" << endl;
			IT->second += weight;
		}
	}
	double max = class_instances.begin()->second;
	string maxClass = class_instances.begin()->first;
	for (map<string, double>::iterator mapITER = class_instances.begin(); mapITER !=class_instances.end(); mapITER++)
	{
		//if class instance count is greater than max
		cout << mapITER->second << endl;
		if (mapITER->second > max) {
			max = mapITER->second;
			maxClass = mapITER->first;
		}
	}
	cout << "Majority goes to: " << maxClass << endl;
	return maxClass;
}