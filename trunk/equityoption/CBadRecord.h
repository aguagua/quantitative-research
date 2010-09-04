/*
 *  CBadRecord.h
 *  
 *
 *  Created by William Wang on 01/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <exception>

using std::exception;

class CBadRecord : public virtual exception {
 public:
  explicit CBadRecord ( int nColumn );
  virtual const char *what() const throw();

 private:
  int m_nColumn;
};


