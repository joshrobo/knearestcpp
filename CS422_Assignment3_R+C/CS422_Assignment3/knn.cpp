//CS422 Assignment 3
//K - Nearest Neighbors Implementation
//Joshua Robinson + Dolen Case

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

#include "header.h"
void test_bupa_dataset();
void test_car_dataset();



int main() {
		cout << "hello k nearest neighbors" << endl;
		system("pause");

		//////////////////////////////////////////////////////////////////////////////////
		//BUPA Dataset
		test_bupa_dataset();

		//////////////////////////////////////////////////////////////////////////////////
		//CAR Dataset
		test_car_dataset();
		
		////////////////////////////////////////////////////////////////////////////////
		cout << endl << "TESTING DONE" << endl;
		system("pause");
		return 0;
}

void test_bupa_dataset() {
	//Initializing vectors to store datapoints
	vector<knn422::datapoint> testVect, trainingVect;
	knn422::datasetinfo bupa_data_info;
	bupa_data_info.class_index = 6; //label class index
	bupa_data_info.pos_class = 0; //label positive class index
								  //read in training set and test set and store in respective vectors
	knn422::readFrom("bupa_data_testset.csv", "bupa_data_trainset.csv", testVect, trainingVect, bupa_data_info);

	/*for (int i = 0; i <= 3; i++) {
		for (int k = 1; k <= 10; k += 3) {
			if (i == 0) {
				cout << "***BUPA dataset w/ euclidean distance***" << endl << endl;
				knn422::knearestneigbors(k, trainingVect, testVect, knn422::euclidean, bupa_data_info);
			}
			if (i == 1) {
				cout << "***BUPA dataset w/ cosine similarity distance***" << endl;
				knn422::knearestneigbors(k, trainingVect, testVect, knn422::cosine_similarity, bupa_data_info);
			}
			if (i == 2) {
				cout << "***BUPA dataset w/ Pearson Correlation***" << endl;
				knn422::knearestneigbors(k, trainingVect, testVect, knn422::correlation, bupa_data_info);
			}
			system("pause");
			cout << endl;
		}
	}*/
	//switching BUPA training set and test set
	for (int i = 0; i <= 3; i++) {
		for (int k = 1; k <= 10; k += 3) {
			if (i == 0) {
				cout << "***SWITCHED BUPA dataset w/ euclidean distance***" << endl << endl;
				knn422::knearestneigbors(k, testVect, trainingVect, knn422::euclidean, bupa_data_info);
			}
			if (i == 1) {
				cout << "***SWITCHED BUPA dataset w/ cosine similarity distance***" << endl;
				knn422::knearestneigbors(k, testVect, trainingVect, knn422::cosine_similarity, bupa_data_info);
			}
			if (i == 2) {
				cout << "***SWITCHED BUPA dataset w/ Pearson Correlation***" << endl;
				knn422::knearestneigbors(k, testVect, trainingVect, knn422::correlation, bupa_data_info);
			}
			system("pause");
		}
	}
}

void test_car_dataset() {
	//Initializing vectors to store datapoints
	vector<knn422::datapoint> testVect2, trainingVect2;
	knn422::datasetinfo car_data_info;
	car_data_info.class_index = 6; //label class index
	car_data_info.pos_class = 3; //label positive class index

	map<string, string> buying;
	buying["vhigh"] = "100";
	buying["high"] = "70";
	buying["med"] = "30";
	buying["low"] = "0";
	map<string, string> maint;
	maint["vhigh"] = "100";
	maint["high"] = "70";
	maint["med"] = "40";
	maint["low"] = "0";
	map<string, string> doors;
	doors["5more"] = "100";
	doors["4"] = "70";
	doors["3"] = "40";
	doors["2"] = "0";
	map<string, string> persons;
	persons["more"] = "100";
	persons["4"] = "50";
	persons["2"] = "0";
	map<string, string> lug_boot;
	lug_boot["big"] = "100";
	lug_boot["med"] = "50";
	lug_boot["small"] = "0";
	map<string, string> safety;
	safety["high"] = "100";
	safety["med"] = "50";
	safety["low"] = "0";
	map<string, string> Class;
	Class["unacc"] = "1";
	Class["acc"] = "2";
	Class["good"] = "3";
	Class["vgood"] = "4";
	
	car_data_info.scale_setting[0] = buying;
	car_data_info.scale_setting[1] = maint;
	car_data_info.scale_setting[2] = doors;
	car_data_info.scale_setting[3] = persons;
	car_data_info.scale_setting[4] = lug_boot;
	car_data_info.scale_setting[5] = safety;
	car_data_info.scale_setting[6] = Class;
	
	//read in training set and test set and store in respective vectors
	//knn422::readFrom("car_data_testset.csv", "car_data_trainset.csv", testVect2, trainingVect2, car_data_info);
	//second set
	knn422::readFrom("car_data_testset.csv", "car_data_trainset.csv", testVect2, trainingVect2, car_data_info);

	/*for (int i = 0; i <= 3; i++) {
		for (int k = 1; k <= 10; k += 3) {
			if (i == 0) {
				cout << "***CAR dataset w/ euclidean distance***" << endl << endl;
				knn422::knearestneigbors(k, testVect2, trainingVect2, knn422::euclidean, car_data_info);
			}
			if (i == 1) {
				cout << "***CAR dataset w/ cosine similarity distance***" << endl;
				knn422::knearestneigbors(k, testVect2, trainingVect2, knn422::cosine_similarity, car_data_info);
			}
			if (i == 2) {
				cout << "***CAR dataset w/ Pearson Correlation***" << endl;
				knn422::knearestneigbors(k, testVect2, trainingVect2, knn422::correlation, car_data_info);
			}
		}
	}*/
	//switching BUPA training set and test set
	for (int i = 0; i <= 3; i++) {
		for (int k = 1; k <= 10; k += 3) {
			if (i == 0) {
				cout << "***SWITCHED CAR dataset w/ euclidean distance***" << endl << endl;
				knn422::knearestneigbors(k, trainingVect2, testVect2, knn422::euclidean, car_data_info);
			}
			if (i == 1) {
				cout << "***SWITCHED CAR dataset w/ cosine similarity distance***" << endl;
				knn422::knearestneigbors(k, trainingVect2, testVect2, knn422::cosine_similarity, car_data_info);
			}
			if (i == 2) {
				cout << "***SWITCHED CAR dataset w/ Pearson Correlation***" << endl;
				knn422::knearestneigbors(k, trainingVect2, testVect2, knn422::correlation, car_data_info);
			}
		}
	}
}

/*//testing distance measures
/*vector<string> a = { "48","32","22","14","4","2" };
vector<string> b = { "44","18","18","23","2","2" };
cout << euclidean(a, b) << endl;
cout << correlation(a, b) << endl;
cout << cosine_similarity(a, b) << endl;*/