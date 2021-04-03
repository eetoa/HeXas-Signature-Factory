#pragma once

// group id					string
// sig1 sig2 sig3 sig4		vector<string>
// map<groupId, sigs>

// group id
// merged sig
// map<groupId, mergedSig>
class Factory
{

public:
	static std::unordered_map<std::string, std::vector<std::string>>  sigMap;
	static std::unordered_map<std::string, std::string>  ProductMap;

public:
	static void ListGroup();							// ls -g
	static bool AddGroup(std::string group);							// new group
	static bool ShowGroup(std::string group);							// show group
	static bool GenerateSig(std::string);
	static bool UpdateSigMap(std::string group, std::string sig);		// group -p cod.exe -a 0x1708E3 -s 0x8
	static bool MergeSigs(std::string group);							// merge group
	static bool GetProduct(std::string group);							// get group

private:
	static std::string Merge(std::vector<std::string>);
};