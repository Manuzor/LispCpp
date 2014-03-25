#include "stdafx.h"
#include "lcpp/core/reader.h"

#include "lcpp/exceptions/exceptions.h"

lcpp::Reader::Reader()
{
    printf("Created reader.\n");
}

lcpp::Reader::~Reader()
{
    printf("Destroyed reader.\n");
}
