#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

//! x86
typedef unsigned long DWORD;

//! RScript *xfl Package Signature
const DWORD RSCRIPT_SIGNATURE = 0x0001424C;

/*!
	RScript *.xfl Package Header
*/
struct RScriptHeader{
	DWORD signature;
	DWORD begin;
	DWORD size;
};
/*!
	RScript *.xfl Package File Info
*/
struct RScriptFileInfo{
	char  filename[32];
	DWORD begin;
	DWORD size;
};

int main(int argc, char* argv[]){
	//! argument check.
	if(argc < 2){
		std::cout << "Usage : " << argv[0] << " [*.xfl]" << std::endl;
		return 0;
	}

	//! file open.
	std::ifstream ifs(argv[1], std::ios::binary | std::ios::in);	//!< rb mode.
	if(ifs.fail()){
		std::cerr << "Error: File '" << argv[1] << "' cannot open." << std::endl;
		return 1;
	}
	//! file header check.
	RScriptHeader header;
	ifs.read(reinterpret_cast<char*>(&header), sizeof(RScriptHeader));
	if(header.signature != RSCRIPT_SIGNATURE){
		std::cerr << "Error: File is not xfl Package." << std::endl;
		return 1;
	}
	//! read file info
	typedef std::vector<RScriptFileInfo> FileInfo_t;
	FileInfo_t infos;
	for(unsigned int i = 0; i < header.size; ++i){
		RScriptFileInfo info;
		ifs.read(reinterpret_cast<char*>(&info), sizeof(RScriptFileInfo));
		infos.push_back(info);
	}
	for(FileInfo_t::iterator it = infos.begin(); it != infos.end(); ++it){
		std::ofstream ofs((*it).filename, std::ios::binary | std::ios::out);
		if(ofs.fail()){
			std::cerr << "Error: File '" << (*it).filename << "' cannot create." << std::endl;
			continue;
		}
		std::unique_ptr<char> buff(new char[(*it).size]);
		ifs.read(buff.get(), (*it).size);
		ofs.write(buff.get(), (*it).size);
		ofs.close();
	}
/*
	for(int i = 0; i < header.size; ++i){
		std::ofstream ofs;
		if(ofs.open(pFileInfo.get()[i].filename)){
			std::err << "Error: " << qFileInfo.get()[i].filename << " cannot create." << std::endl;
			continue;
		}
		std::shared_ptr<char> buff(new char[qFileInfo.get()[i].size]);
		ifs.read(buff.get(), qFileInfo.get()[i].size);
		ofs.write(buff.get(), qFileInfo.get()[i].size);
		ofs.close();
	}
*/
	return 0;
}
