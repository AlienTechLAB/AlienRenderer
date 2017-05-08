#pragma once
#include <exception>

class eVoException : public std::exception
{
	//---------------------------------------------------------------------------------------------------------

	public:eVoException(const char* message) : exception(message)
	{
	}

   //---------------------------------------------------------------------------------------------------------
};