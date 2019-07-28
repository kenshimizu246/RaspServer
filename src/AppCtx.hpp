
#include <string>
#include <exception>
#include <stdexcept>
#include <vector>
#include <memory>

#include "PCA9685.hpp"
#include "HcSr04.hpp"

using namespace std;

#ifndef _AppCtx_hpp
#define _AppCtx_hpp

namespace raspserver {

class AppCtx {
	private:
		vector<shared_ptr<PCA9685>> i2cs;
		vector<shared_ptr<HcSr04>> hcsr04s;

		AppCtx(){}
		~AppCtx(){}

	public:
		unsigned int getSizeOfPCA9685(){
			return i2cs.size();
		}

		void add(shared_ptr<PCA9685> p){
			i2cs.push_back(p);
		}

		void add(shared_ptr<HcSr04> p){
			hcsr04s.push_back(p);
		}

		shared_ptr<PCA9685> getPCA9685(unsigned int i){
			return i2cs.at(i);
		}

		shared_ptr<HcSr04> getHcSr04(unsigned int i){
			return hcsr04s.at(i);
		}

		static AppCtx& getInstance() {
			static AppCtx instance;
			return instance;
		}
};

}

#endif /*_AppCtx_hpp*/
