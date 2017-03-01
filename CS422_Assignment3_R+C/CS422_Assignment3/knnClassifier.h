#ifndef KNNCLASSIFIER_H
#define KNNCLASSIFIER_H

namespace knn422 {
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
	//
		map<int,map<string, string>> scale_setting;
	};


	void readFrom(string test, string training, vector<datapoint> & testVect, vector<datapoint> & trainingVect, datasetinfo & dsinfo);
	string scaleBasedOn(string token, int att_index, map<int, map<string, string>> scale_set);
	void store(string set, vector<datapoint> & vect, datasetinfo & dsinfo);
	void countClassInstances(datapoint & d, datasetinfo & dsinfo, int attribute_index, string token);
	void outputDataSetInfo(datasetinfo & dsinfo);
	void outputSet(string type, string set, vector<datapoint> & vect);

	void knearestneigbors(int k, vector<datapoint> & trainingVect, vector<datapoint> & testVect, double(*dist)(vector<string>, vector<string>), datasetinfo);

	string distanceWeightedVoting(multimap<double, string> kn, datasetinfo dsinfo);
	void confusion(vector < vector <int>> & matrix, string actual, string predicted);
	void outputConfusionMatrix(vector<vector<int>> & m, datasetinfo dsinfo);
}
#endif // !KNNCLASSIFIER_H

