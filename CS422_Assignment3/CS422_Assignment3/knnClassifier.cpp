#include "header.h"
#include "knnClassifier.h"
#include "distanceMeasures.h"

namespace knn422 {

	void readFrom(string test, string training, vector<datapoint> & testVect, vector<datapoint> & trainingVect, datasetinfo & dsinfo) {

		//initialize class instance tracker
		/*for (int i = 0; i < dsinfo.classes.size(); ++i) {
		map<string, int> * d = new map<string, int>;
		dsinfo.class_instances.push_back(d);
		}*/

		//store data points from set into corresponding vector
		store(test, testVect, dsinfo);
		store(training, trainingVect, dsinfo);


		//output for debugging
		cout << endl;
		outputSet("test", test, testVect);
		outputSet("training", training, trainingVect);


		outputDataSetInfo(dsinfo);
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
			cout << mapITER->first << ": " << mapITER->second << endl;
		}
		cout << "Number of non-class attributes: " << dsinfo.attributes - 1 << endl << endl;
	}

	//output datapoints and attributes
	void outputSet(string type, string set, vector<datapoint> & vect) {
		cout << type << " set file: " << set << endl;
		vector<datapoint>::iterator it;

		//visit each datapoint
		//for (it = vect.begin(); it != vect.end(); ++it) {
		//	//print each attribute
		//	for (vector<string>::iterator it2 = it->attributes.begin(); it2 != it->attributes.end(); ++it2) {
		//		cout << *it2 << ' ';
		//	}
		//	cout << endl;
		//}
		//cout << endl;
	}

	void knearestneigbors(int k, vector<datapoint> & trainingVect, vector<datapoint> & testVect, double(*dist)(vector<string>, vector<string>), datasetinfo dsinfo) {
		cout << "Computing k nearest neighbors..." << endl;
		cout << "k = " << k << endl;
		system("pause");
		cout << endl;

		int numClasses = dsinfo.class_instances.size();
		//1. Let k equal the nearest neighbors and D be the set of training examples
		vector<vector<int>> confusion_matrix(numClasses, vector<int>(numClasses, 0));

		//2. for each test example z = (x',y') do
		for (int i = 0; i <= testVect.size() - 1; i++) {
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
			cout << k << " nearest neighbors: " << endl;
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

		outputConfusionMatrix(confusion_matrix, dsinfo);
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
		for (int i = 0; i <= m.size() - 1; i++) {
			//every predicted
			for (int j = 0; j <= m.size() - 1; j++) {
				cout << left << setw(5) << m.at(i).at(j);
				if (i == j) {
					correct_pred += m.at(i).at(j);
					if (i != dsinfo.pos_class) FN += m.at(dsinfo.pos_class).at(j); //sum of x col without diagonal
					if (j != dsinfo.pos_class) FP += m.at(i).at(dsinfo.pos_class); //sum of x row without diagonal
				}
				total_pred += m.at(i).at(j);
			}

			cout << endl;
		}
		cout << endl;
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
		for (map<string, double>::iterator mapITER = class_instances.begin(); mapITER != class_instances.end(); mapITER++)
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
}