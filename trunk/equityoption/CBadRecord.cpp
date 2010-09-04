/*
 *  CBadRecord.cpp
 *  
 *
 *  Created by William Wang on 01/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "CBadRecord.h"

CBadRecord::CBadRecord(int nColumn)
    :  m_nColumn( nColumn)
{}

const char* CBadRecord::what() const throw()
{
    return "Failed to parse data";
}

