#pragma once


enum  Status
{
	Success = 0,
	GroupNotFoundError = 1,
	NoneGroup = 2,
	GroupExistsError = 3,
	SigNotFoundError = 4,
};

class Factory
{

public:
	static std::unordered_map<std::string, std::vector<std::string>>  sigMap;
	static std::unordered_map<std::string, std::string>  productMap;

public:
	static Status ListGroup();											// ls -g
	static Status AddGroup(std::string group);							// new group
	static Status ShowGroup(std::string group);							// show group
	static Status ShowAllGroup();											// show -all
	static Status MergeAllGroup();										// merge -all
	static Status GetAllProduct();										// get -all
	static Status GenerateSig(std::string group);
	static bool UpdateSigMap(std::string group, std::string sig);		// group -p cod.exe -a 0x1708E3 -s 0x8
	static Status MergeGroup(std::string group);							// merge group
	static Status GetProduct(std::string group);							// get group

private:
	static std::string Merge(std::vector<std::string>);
};

