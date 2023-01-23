#include <boost/program_options.hpp>
#include <string>
#include <sys/stat.h>
#include <fmt/format.h>
#include <iostream>

namespace po = boost::program_options;

inline bool exist(std::string& name){
	struct stat buffer;
	return (stat (name.c_str(), &buffer) == 0);
}

int main(int argc, char** argv){
	po::options_description desc("Building options");
	std::string java;
	std::string res;
	std::string manifest;
	std::string jar;
	std::string obj;
	std::string ks;
	int kpass;
	desc.add_options()
		("h", "produces help")
		("j", po::value<std::string>(&java), "java dir path (required)")
		("r", po::value<std::string>(&res), "resources dir path (required)")
		("m", po::value<std::string>(&manifest), "manifest file path (required)")
		("i", po::value<std::string>(&jar), "android.jar file path (required)")
		("obj", po::value<std::string>(&obj), "object files directory (required)")
		("k", po::value<std::string>(&ks), ".ks keystore file (required)")
		("kp", po::value<int>(&kpass), "keystore password (required)")
		;
	po::variables_map vm;
	po::parsed_options parsed = po::command_line_parser(argc, argv).options(desc).run();
	po::store(parsed, vm);
	po::notify(vm);

	if(vm.count("h")){
		std::cout << desc << "\n";
		return 1;
	}
	if(!vm.count("j") or !vm.count("r") or !vm.count("m") or !vm.count("i") or !vm.count("obj") or !vm.count("k") or !vm.count("kp")){
		//std::cout << argv[0] << ": You must specify: --j, --r, --m, --i, --obj, --k, --kp. Use --h for description" << std::endl;
		std::cout << desc << std::endl;
		return 1;
	}
	if(!exist(java) or !exist(res) or !exist(manifest) or !exist(jar) or !exist(obj) or !exist(ks)){
		std::cout << argv[0] << ": File(s) or Dir(s) doesn't exist!" << std::endl;
	}
	//running compiler
	std::cout << "Deleting old obj files..." << std::endl;
	system(fmt::format("rm {} -r", obj).c_str());
	std::cout << "...Done. Generating R.java..." << std::endl;
	system(fmt::format("aapt package -m -f -M {0} -S {1} -J {2} -I {3}", manifest, res, java, jar).c_str());
	std::cout << "...Done. Compiling .java..." << std::endl;
	system(fmt::format("ecj-24 -d {0} -sourcepath {1} $(find {1} -type f -name \"*.java\")", obj, java).c_str());
	std::cout << "...Done. Merging apk..." << std::endl;
	system(fmt::format("aapt package -M {0} -S {1} -F gen.apk -f", manifest, res).c_str());
	std::cout << "...Done. Dexing..." << std::endl;
	system(fmt::format("dx --dex --output=classes.dex {}", obj).c_str());
	std::cout << "...Done. Adding classes.dex to apk..." << std::endl;
	system(fmt::format("aapt add gen.apk classes.dex").c_str());
	std::cout << "...Done. Signing..." << std::endl;
	system(fmt::format("apksigner sign --ks {0} --ks-pass pass:{1} --in gen.apk --out gen-signed.apk", ks, kpass).c_str());
	std::cout << "...Done building apk. Saved in gen-signed.apk" << std::endl;
}
