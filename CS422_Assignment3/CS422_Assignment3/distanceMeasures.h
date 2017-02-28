#ifndef DISTANCEMEASURES_H
#define DISTANCEMEASURES_H
namespace knn422 {
	double euclidean(vector<string> p, vector<string> q);
	double cosine_similarity(vector<string> d1, vector<string> d2);
	double correlation(vector<string> x, vector<string> y);
	double covarianceXY(vector<string> x, vector<string> y, double meanX, double meanY);
	double standardDeviation(double variance);
	double variance(vector<string>data, double mean);
	double arithmeticMean(vector<string>data);
}
#endif // 
