#pragma once

#define CUT_DISALLOW_COPY_AND_ASSIGN(type) type(const type&); void operator=(const type&)
#define CUT_DISALLOW_CONSTRUCTION(type) CUT_DISALLOW_COPY_AND_ASSIGN(type); type(); ~type()
