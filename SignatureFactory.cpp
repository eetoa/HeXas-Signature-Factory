#include "stdafx.h"
#include "color.hpp"

std::unordered_map<std::string, std::vector<std::string>>	Factory::sigMap;
std::unordered_map<std::string, std::string>				Factory::ProductMap;


bool Factory::GenerateSig(std::string group)
{
	Proc::GetProcId();
	Proc::GetProcHandle();
	std::cout << g_procId << " - " << g_procHandle << std::endl;

	std::vector<byte> res;
	for (unsigned int i = 0; i < g_size; i++) {
		byte cur;
		res.push_back(ReadEx<byte>(g_address + i));
	}
	/*
	for (int i = 0; i < res.size(); i++) {
		std::cout << std::hex << dye::on_light_blue((unsigned int)res[i]) << dye::on_light_blue(" ");
	}
	std::cout << std::endl << std::endl;
	*/
	std::string sig = "";
	for (int i = 0; i < res.size(); i++) {
		std::string unit = Utils::ToHexString((unsigned int)res[i]);;
		if (unit.length() == 1) unit = "0" + unit;
		sig += unit;
		sig += " ";
	}
	std::cout << dye::on_light_blue(sig);
	std::cout << std::endl << std::endl;

	//UpdateSigMap(group, sig);
}


// hexas new group
bool Factory::AddGroup(std::string group)
{
	// group exists
	if (sigMap.count(group))
	{
		return false;
	}
	std::vector<std::string> sigContainer;
	sigMap.insert({ group, sigContainer });
	return true;
}

// hexas group -p cod.exe -a 0x1708E3 -s 0x8
bool Factory::UpdateSigMap(std::string group, std::string sig)
{
	// group not exists
	if (!sigMap.count(group))
	{
		return false;
	}
	std::unordered_map<std::string, std::vector<std::string>>::iterator iter;
	iter = sigMap.find(group);
	if (iter != sigMap.end())
	{
		// found it
		iter->second.push_back(sig);
	}
	return true;
}

// hexas merge group
bool Factory::MergeSigs(std::string group)
{
	// group not exists
	if (!sigMap.count(group))
	{
		return false;
	}
	std::unordered_map<std::string, std::vector<std::string>>::iterator iter;
	iter = sigMap.find(group);
	if (iter != sigMap.end())
	{
		ProductMap.insert({ group, Merge(iter->second) });
	}
	return true;
}

std::string Factory::Merge(std::vector<std::string>)
{
	// TODO: merge algorithm implementaion
	return "";
}

// hexas get group
bool Factory::GetProduct(std::string group)
{
	// group not exists
	if (!sigMap.count(group))
	{
		return false;
	}
	std::unordered_map<std::string, std::string>::iterator iter;
	iter = ProductMap.find(group);
	if (iter != ProductMap.end())
	{
		std::cout << std::hex << iter->second << " ";
	}
}
