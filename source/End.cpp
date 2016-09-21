#include "End.h"

bool End::end;

void End::Die()
{
	end = true;
}

void End::SubEnd()
{
	end = true;
}

bool End::Get()
{
	return end;
}