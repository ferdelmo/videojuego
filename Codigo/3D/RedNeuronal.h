#ifndef RED_H_
#define RED_H_

#include <iostream>
#include <glm/glm.hpp>

using namespace std;

class RedNeuronal {
	public:
		vector<double> pesFinal;
		vector<vector<double>> neuronas

		RedNeuronal(int n, string fichEntreno);

		static double sig(double a) {
		
		}

		glm::vec2 calcular(double j, double c) {
			double resul = { 0,0 };
			for (int i = 0; i < neuronas.size(); i++) {
				resul += pesFinal[i]*(j*neuronas[ì][0] + c * neuronas[i][1]);
			}
			return resul;
		}

};

#endif // !RED_H_
