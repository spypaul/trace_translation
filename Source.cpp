#include <iostream>
#include <iomanip>
#include <fstream>
#include <String>
#include <sstream>
#include <set>
#include <list>
#include <map>

using namespace std; 


int main() {



	uint64_t count{ 0 };
	double prevtime{ 0 };
	uint64_t sum_of_time{ 0 };

	uint64_t prevaddr{ 0 };

	list<uint64_t> addr_tracking;

	map<uint64_t, uint64_t> addr_m;
	uint64_t time{ 0 };

	uint64_t global_start_time{ 0 };

	map<uint64_t, string> tm;

	set<uint64_t>mfootprint;

	cout << "Please enter the trace file name (*.pout or *.vout): ";

	string filename{ "" };
	cin >> filename;

	ifstream inputfile{ filename };
	if (!inputfile.is_open()) {
		cout << "File does not exist" << endl;
		return 0;
	}
	string outname{ "" };
	if (filename.find(".pout") == string::npos && filename.find(".vout") == string::npos) {
		cout << "Please only enter .pout or .vout files" << endl;
		return 0;
	}
	if (filename.find(".pout") != string::npos) {
		outname = filename.replace(filename.find(".pout"), 6, ".trace");
	}
	if (filename.find(".vout") != string::npos) {
		outname = filename.replace(filename.find(".vout"), 6, ".trace");
	}


	ofstream outputfile{ outname };

	while (!inputfile.eof()) {
		string line{ "" };
		getline(inputfile, line);
		bool va{ 0 };

		if ( va && (line.size() < 3 || !(line[1] == 'R' || line[1] == 'W'))) { 
			continue; 
		}
		if (line == "") continue;
		count++;
		
		
		if (va) {
			line.erase(line.begin());
			line.erase(line.begin() + line.size() - 1);
		}
		
		stringstream ss;

		ss << line;

		string rws{ "" };
		uint64_t addr{0};
		double timestamp{ 0 };

		ss >> rws >> hex >> addr >> fixed>>std::setprecision(9) >> timestamp;
		timestamp *= 1000000;
		//if (count == 1) global_start_time = timestamp;
		uint64_t time{ static_cast<uint64_t> (timestamp) };
		
		int rw{ (rws == "R") ? 1 : 0 };
		string printinfo;

		mfootprint.insert(addr / 4096);
		printinfo = " 0 " + std::to_string(addr) + " 8 " + std::to_string(rw);
		tm.emplace(time, printinfo);

		if (count % 1000000 == 0) {
			std::cout << count << endl;
		}
		if (addr == 0 && rw == 0) {
			std::cout << "check" << endl;
		}


	}
	std::cout << "Memory Footprint: " << mfootprint.size() * 4096 / 1024 / 1024 << "MB" << endl;
	count = 0;
	uint64_t scaler{ 1000 / 90 }; //140, 106,165,142, 117, 108
								   //124, 165, 142, 117,108
									//lu 185 splash radiosity 119 raytrace 139 volrend 88
									//page_rank 147
									//bert 57
									//mcf 80 omnet 126 x264 155 xz 90
	uint64_t ops{ 20000000 };
	uint64_t read_counts{ 0 };

	auto it{ tm.begin() };

	for (auto& i : tm) {
		count++;

		if (count == 1) global_start_time = i.first;
		outputfile << (i.first - global_start_time)* scaler <<i.second << endl;
		
		if (i.second[i.second.size()-1] == '1') {
			read_counts++;
		}

		
		if (count != 1) sum_of_time += (i.first - global_start_time - prevtime)* scaler;
		prevtime = i.first - global_start_time ;

		if (count % 1000000 == 0) {
			std::cout << count << endl;
		}

		//ops--;
		//if (ops == 0) {
		//	break;
		//}
	}
	std::cout << "Application End Time(ns): " << prevtime*scaler << endl;
	std::cout << "Memory Footprint: " << mfootprint.size() * 4096 / 1024 / 1024 << "MB" << endl;
	std::cout << "Average Interarrival Time: " << sum_of_time / (count - 1) << endl;;
	std::cout << "Read counts:" << read_counts << " Write counts: " << count - read_counts << endl;
	return 0; 
}