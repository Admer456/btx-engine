// SPDX-FileCopyrightText: 2022 Admer Šuko
// SPDX-License-Identifier: MIT

#include "Precompiled.hpp"
#include "Console.hpp"

// The file logger is currently not implemented
IConsoleListener* Console::CreateListenerFileOut()
{
	return nullptr;
}
