#ifndef RINSIDE_CONTAINER_H
#define RINSIDE_CONTAINER_H

#include <string>
#include <mutex>
#include <RInside.h>

using namespace std;

class RInside_Container{
	public:
		double use(string command_to_execute){
			mtx.lock();
			double value = Rcpp::as<double>(R.parseEval(command_to_execute));
			mtx.unlock();
			return value;
		}
	private:
		RInside R{};
		std::mutex mtx;
};

#endif // RINSIDE_CONTAINER_H