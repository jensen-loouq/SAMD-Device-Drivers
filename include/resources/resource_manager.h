#ifndef COREZERO_RESOURCE_MANAGER_H
#define COREZERO_RESOURCE_MANAGER_H

#pragma once

#include "../ll_system.h"

namespace ll_system
{
	template <unsigned long NumOfResources>
	struct resource_manager
	{
		resource_manager() = delete;
		resource_manager(const resource_manager&) = delete;
		resource_manager(resource_manager&&) = delete;
		~resource_manager() = delete;


	private:
		static constexpr system_component* m_component[NumOfResources] = nullptr;
	};
}


#endif