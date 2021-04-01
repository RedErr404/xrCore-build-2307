////////////////////////////////////////////////////////////////////////////
//	Module 		: location_manager.cpp
//	Created 	: 27.12.2003
//  Modified 	: 27.12.2003
//	Author		: Dmitriy Iassenev
//	Description : Location manager
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "location_manager.h"
#include "gameobject.h"

extern void	setup_location_types	(GameGraph::TERRAIN_VECTOR &m_vertex_types, CInifile *ini, LPCSTR string);

CLocationManager::~CLocationManager	()
{
}

void CLocationManager::Load			(LPCSTR section)
{
	setup_location_types			(m_vertex_types,pSettings,pSettings->r_string(section,"terrain"));
}

void CLocationManager::reload		(LPCSTR section)
{
	if (!m_object->spawn_ini() || !m_object->spawn_ini()->section_exist("alife") || !m_object->spawn_ini()->line_exist("alife","terrain"))
		return;
	setup_location_types			(m_vertex_types,m_object->spawn_ini(),m_object->spawn_ini()->r_string("alife","terrain"));
}
