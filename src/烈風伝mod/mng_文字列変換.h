#pragma once

#include <string>

// �uSystem::String^�v�ˁustd::string�v�ցB.NET��String^������� C++��string�������
std::string to_native_string(System::String^ str);

// �ustd::string�v�ˁuSystem::String^�v�ցBC++��string������� .NET��String^�������
System::String^ to_managed_string(std::string str);

#pragma once
