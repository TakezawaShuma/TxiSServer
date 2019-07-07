#include "../stdafx.h"
#include "DataBase.h"

DB* DB::m_instans = nullptr;

DB * DB::GetInstans()
{
	if (m_instans == nullptr) { m_instans = new DB(); } 
	return m_instans;
}
