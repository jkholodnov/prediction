#ifndef RINSIDE__CONTAINER_H
#define RINSIDE__CONTAINER_H

struct RInside_Container{
	double use(string command_to_execute){
		mtx.lock();
		double value = Rcpp::as<double>(R.parseEval(command_to_execute));
		mtx.unlock();
		return value;
	}
	Rinside R{};
	std::mutex mtx;
};
#endif // RINSIDE__CONTAINER_H