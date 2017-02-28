#include "header.h"
#include "distanceMeasures.h"
#include "knnClassifier.h"
namespace knn422 {
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
		return 1.0000 - cosSim;
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
		return  1.0000 - corr;
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
	//the average value of vector, the sum of the values divided by the number of values
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
}