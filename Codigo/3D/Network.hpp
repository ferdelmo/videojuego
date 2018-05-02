#ifndef NETWORK
#define NETWORK

#include <iostream>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/function.hpp>

using namespace boost::numeric::ublas;

typedef boost::function<boost::numeric::ublas::vector<double>* (const boost::numeric::ublas::vector<double>&)> Function;


class Network
{
	double												learningRate;
	int 													nbLayers;
	std::vector<boost::numeric::ublas::vector<double>*>	errors;
	std::vector<boost::numeric::ublas::vector<double>*>	inputs;
	std::vector<boost::numeric::ublas::vector<double>*>	outputs;
	std::vector<matrix<double>*>	weights;
	std::vector<boost::numeric::ublas::vector<double>*>	biases;

	std::vector<Function*>				functions;
	std::vector<Function*>				derivates;

public:
	Network(const std::vector<int>&, double=0.01);
	virtual ~Network();

	void						fit(matrix<double>&, matrix<double>&, const int=100);
	boost::numeric::ublas::vector<double> 	*predict(const boost::numeric::ublas::vector<double>&);

	friend std::ostream& operator<<(std::ostream&, const Network&);

private:

	void 						initializeNetwork(const std::vector<int>&);
	void 						updateWeights(boost::numeric::ublas::vector<double>*, boost::numeric::ublas::vector<double>*);
	boost::numeric::ublas::vector<double> 	*feedForward(const boost::numeric::ublas::vector<double>&);
	boost::numeric::ublas::vector<double> 	*row2vec(const matrix_row<matrix<double> >&) const;

	static boost::numeric::ublas::vector<double>	*sigmoid(const boost::numeric::ublas::vector<double>&);
	static boost::numeric::ublas::vector<double> *sigmoidPrime(const boost::numeric::ublas::vector<double>&);
	static boost::numeric::ublas::vector<double>	*identity(const boost::numeric::ublas::vector<double>&);
	static boost::numeric::ublas::vector<double>	*identityPrime(const boost::numeric::ublas::vector<double>&);
};

#endif
