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
			
			//This returns only double. You need to modify the as<> to give you a return value. Note that you do not have to use this class if you are using a sequential implementation, only if you want to use RInside in a multithreaded context.
			double value = Rcpp::as<double>(R.parseEval(command_to_execute));
			mtx.unlock();
			return value;
		}
	private:
		RInside R{};
		std::mutex mtx;
};

#endif // RINSIDE_CONTAINER_H