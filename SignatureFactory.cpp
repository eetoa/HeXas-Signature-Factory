#pragma once
#include "stdafx.h"
#include "color.hpp"

std::unordered_map<std::string, std::vector<std::string>>	Factory::sigMap;
std::unordered_map<std::string, std::string>				Factory::productMap;


// ls -g
Status Factory::ListGroup()
{
	if (sigMap.empty())
		return NoneGroup;

	int i = 0;
	std::unordered_map<std::string, std::vector<std::string>>::iterator iter;
	for (iter = sigMap.begin(); iter != sigMap.end(); iter++)
	{
		std::cout << dye::black_on_light_blue(iter->first) << std::endl;
		i++;
	}
	std::cout << std::endl;
	return Success;
}

// show -all
Status Factory::ShowAllGroup()
{
	if (sigMap.empty())
		return NoneGroup;

	std::unordered_map<std::string, std::vector<std::string>>::iterator iter;
	for (iter = sigMap.begin(); iter != sigMap.end(); iter++)
	{
		std::cout << dye::black_on_light_blue(iter->first + ":")  << std::endl;
		for (int i = 0; i < iter->second.size(); i++)
			std::cout << dye::on_light_blue(iter->second[i]) << std::endl;

	}
	std::cout << std::endl;
	return Success;
}

// merge -all
Status Factory::MergeAllGroup()
{
	if (sigMap.empty())
		return NoneGroup;

	int cnt = 0;
	std::unordered_map<std::string, std::vector<std::string>>::iterator iter;
	for (iter = sigMap.begin(); iter != sigMap.end(); iter++)
		if (MergeGroup(iter->first) == SigNotFoundError) cnt++;

	if (cnt) return SigNotFoundError;
	return Success;
}

// get -all
Status Factory::GetAllProduct()
{
	if (productMap.empty())
		return NoneGroup;

	std::unordered_map<std::string, std::string>::iterator iter;
	for (iter = productMap.begin(); iter != productMap.end(); iter++)
	{
		std::cout << dye::black_on_light_blue(iter->first + ":") << std::endl;
		std::cout << dye::on_light_blue(iter->second) << std::endl;
	}
	std::cout << std::endl;
	return Success;
}

// new group
Status Factory::AddGroup(std::string group)
{
	// group exists
	if (sigMap.count(group))
		return GroupExistsError;

	std::vector<std::string> sigContainer;
	sigMap.insert({ group, sigContainer });
	return Success;
}

// show group
Status Factory::ShowGroup(std::string group)
{
	std::unordered_map<std::string, std::vector<std::string>>::iterator iter;
	iter = sigMap.find(group);
	if (iter != sigMap.end())
	{
		// found it
		std::cout << dye::black_on_light_blue(group + ":") << std::endl;
		for (int i = 0; i < iter->second.size(); i++)
			std::cout << dye::on_light_blue(iter->second[i]) << std::endl;

		std::cout << std::endl;
		return Success;
	}
	else
	{
		return GroupNotFoundError;
	}
}

Status Factory::GenerateSig(std::string group)
{
	// group not exists
	if (!sigMap.count(group))
		return GroupNotFoundError;

	Proc::GetProcId();
	if (!g_procId) 
		return OpenProcessError;
	
	Proc::GetProcHandle();
	
	std::vector<byte> res;
	for (unsigned int i = 0; i < g_size; i++) {
		byte cur;
		res.push_back(ReadEx<byte>(g_address + i));
	}
	std::string sig = "";
	for (int i = 0; i < res.size(); i++) {
		std::string unit = Utils::ToHexString((unsigned int)res[i]);;
		if (unit.length() == 1) unit = "0" + unit;
		sig += unit;
		sig += " ";
	}
	std::cout << dye::on_light_blue(sig);
	std::cout << std::endl << std::endl;

	if (!UpdateSigMap(group, sig))
		return GroupNotFoundError;

	return Success;
}

// group -p cod.exe -a 0x1708E3 -s 0x8
bool Factory::UpdateSigMap(std::string group, std::string sig)
{
	// group not exists
	if (!sigMap.count(group))
		return false;

	std::unordered_map<std::string, std::vector<std::string>>::iterator iter;
	iter = sigMap.find(group);
	if (iter != sigMap.end())
	{
		// found it
		iter->second.push_back(sig);
	}
	return true;
}

// merge group
Status Factory::MergeGroup(std::string group)
{
	// group not exists
	if (!sigMap.count(group))
		return GroupNotFoundError;

	std::unordered_map<std::string, std::vector<std::string>>::iterator iter;
	iter = sigMap.find(group);
	if (iter != sigMap.end())
	{
		if (!iter->second.size())
			return SigNotFoundError;
		// if product exists, and wanna remerge ,just overwrite with new merge
		productMap[group] = Merge(iter->second);
	}
	return Success;
}

std::string Factory::Merge(std::vector<std::string> sigs)
{
	// at this point , sigs container has at least one item
	std::string res = "";
	int rows = sigs.size();
	if (rows == 1)
		return sigs[0];

	int min = 1e6 + 10;
	for (int r = 0; r < rows; r++)
		min = sigs[r].length() < min ? sigs[r].length() : min;

	for (int c = 0; c < min; c++)
	{
		char cmp = sigs[0][c];
		bool isMatch = false;
		for (int r = 1; r < rows; r++)
		{
			if (sigs[r][c] != cmp)
			{
				isMatch = false;
				break;
			}
			else
			{
				isMatch = true;
			}

		}
		res += isMatch ? cmp : '?';
	}
	// handle the single '?' issue
	for (int i = 0, j = 0, cnt = 0; i < res.length(); i++)
	{
		char c = res[i];
		if (c == '?') cnt++;
		if (c == ' ')
		{
			if (cnt & 1) 
				for (; j < i; j++) res[j] = '?';
			else
				j = i;
			j++;
			cnt = 0;
		}
	}
	return res;
}

// get group
Status Factory::GetProduct(std::string group)
{
	// group not exists
	if (!productMap.count(group))
		return GroupNotFoundError;

	std::unordered_map<std::string, std::string>::iterator iter;
	iter = productMap.find(group);
	std::cout << dye::black_on_light_blue(group) << std::endl;
	if (iter != productMap.end())
		std::cout << dye::on_light_blue(iter->second);

	std::cout << std::endl << std::endl;

	return Success;
}


using namespace std;

void Info::Help()
{
	cout << "usage: new <GroupName>" << endl;
	cout << "       <GroupName> [-p] <ProcessName.exe> [-a] <TargetAddress> [-s] <SignatureSize>" << endl;
	cout << "       <GroupName> [-p] <ProcessNumber> [-a] <TargetAddress> [-s] <SignatureSize>" << endl;
	cout << "       <GroupName> [-r] [-a] <TargetAddress>" << endl;
	cout << "       show <GroupName>" << endl;
	cout << "       merge <GroupName>" << endl;
	cout << "       get <GroupName>" << endl;
	cout << "       ls [-p]" << endl;
	cout << "       ls [-g]" << endl;
	cout << "       show [-all]" << endl;
	cout << "       merge [-all]" << endl;
	cout << "       get [-all]" << endl;
	cout << "Press END on keyboard to exit HeXas." << endl;
	cout << endl;
	cout << "Common steps are as follow:" << endl;
	cout << endl;
	cout << "You wanna find the localPlayer base offset of module base address, so creat a new group" << endl;
	cout << "    $ new localPlayer" << endl;
	cout << "Then you can create your signature for it, try" << endl;
	cout << "    $ localPlayer -p ac_client.exe -a 0x50F4F4 -s 0x9" << endl;
	cout << "to get a signature with process address, target address and signature length" << endl;
	cout << "Or, you can get a process list with" << endl;
	cout << "    $ ls -p" << endl;
	cout << "Then chose your target process with the front number" << endl;
	cout << "    $ localPlayer -p 1 -a 0x50F4F4 -s 0x9" << endl;
	cout << "Now you have a signature in the localPlayer container, try" << endl;
	cout << "    $ show localPlayer" << endl;
	cout << "to get the signatures" << endl;
	cout << "You can easily repeat generate signature with" << endl;
	cout << "    $ localPlayer -r -a 50F4F4" << endl;
	cout << "Program will use the last process info and signature size info" << endl;
	cout << "After you get several signatures, try" << endl;
	cout << "    $ merge localPlayer" << endl;
	cout << "to merge all these signatures to product container" << endl;
	cout << "Try" << endl;
	cout << "    $ get localPlayer" << endl;
	cout << "to get the final signature with wildcard" << endl;
	cout << "You can create several groups to do diferrent project at the same time since you can create several sigs each time you restart the game. Try" << endl;
	cout << "    $ new weapon" << endl;
	cout << "    $ new viewMatrix" << endl;
	cout << "Add several sigs for each group. Then try" << endl;
	cout << "    $ show -all" << endl;
	cout << "to show all the signatures of each group. Then try" << endl;
	cout << "    $ merge -all" << endl;
	cout << "to merge each group to individual single signature. Then try" << endl;
	cout << "    $ get -all" << endl;
	cout << "to get the product." << endl;
	cout << endl;
}

void Info::GroupNotFound()
{
	cout << dye::black_on_light_yellow("Specific group is not exists in container. \nPlease make sure group name is matching with the one you just created. \nOr you showld try: $ new GroupName before operating.\nTry $ --help for more infomation.") << endl << endl;
}

void Info::NoneGroup()
{
	cout << dye::black_on_light_yellow("Container is empty.\nTry $ --help for more infomation.") << endl << endl;
}

void Info::GroupExistsError()
{
	cout << dye::black_on_light_yellow("Group already exists in container.\nTry $ --help for more infomation.") << endl << endl;
}

void Info::SigNotFoundError()
{
	cout << dye::black_on_light_yellow("There are no signatures in current group container. \nTry $ --help for more infomation.") << endl << endl;
}

void Info::OpenProcessError()
{
	cout << dye::black_on_light_yellow("Error occur when attach to specific process. \n Make sure your process name is right or try $ ls -p to get a list of processes.\nSometimes, a game with anti-cheat system will cause this error too.\nTry $ --help for more infomation.") << endl << endl;
}