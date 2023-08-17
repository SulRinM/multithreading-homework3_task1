#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <future>
#include <exception>

using namespace std;


void minElement(const vector<int>::iterator& iStart, vector<int>& v, promise<vector<int>::iterator>& prom) {
	if (v.empty())
		throw runtime_error("Vector is empty!");
	prom.set_value(min_element(iStart, v.end()));
}

void printVector(vector<int>& v) {
	if (v.empty())
		throw runtime_error("Vector is empty!");
	for (const auto& v : v)
		cout << v << " ";
	cout << '\n';
}

int main() {
	try {
		vector<int> v(10);
		generate(v.begin(), v.end(), []() {return rand() % 10; });
		printVector(v);
		auto itrStart = v.begin();
		do {
			promise<vector<int>::iterator> pRes;
			auto fRes = pRes.get_future();
			future<void> result = async(minElement, ref(itrStart), ref(v), ref(pRes));
			fRes.wait();
			swap(*itrStart, *fRes.get());
			++itrStart;
		} while (itrStart < v.end());
		printVector(v);
	}
	catch (runtime_error& ex) {
		cout << ex.what();
	}
	catch (...) { cout << "Some exception has occurred!"; }

	return 0;
}