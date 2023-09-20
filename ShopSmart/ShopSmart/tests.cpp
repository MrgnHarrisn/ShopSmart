#pragma once

#include <assert.h>

#include "SupermarketAPI.h"

void testSuperMarketFetch()
{
	SupermarketAPI sapi;

	auto markets = sapi.fetchSupermarkets();

	assert(markets.size() == 375);
}
