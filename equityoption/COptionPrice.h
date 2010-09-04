/*
 *  COptionPrice.h
 *  
 *
 *  Created by William Wang on 01/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef COPTION_PRICE_H
#define COPTION_PRICE_H

#include <string>
#include <ql/quantlib.hpp>
using std::string;
using QuantLib::Date;

class COptionPrice {
 public:
  virtual ~COptionPrice() {};

  virtual double getPrice(Date settlementDate, 
			  Date dMaturity, 
			  double dSpotPrice, 
			  double dStrikePrice, 
			  double dVolatility, 
			  string& sOptionType) = 0;
};

class CEuropeanOptionPrice : public COptionPrice {
 public:
  double getPrice(Date settlementDate, 
		  Date dMaturity, 
		  double dSpotPrice, 
		  double dStrikePrice, 
		  double dVolatility, 
		  string& sOptionType);
};

class CAmericanOptionPrice : public COptionPrice {
  double getPrice(Date settlementDate, 
		  Date dMaturity, 
		  double dSpotPrice, 
		  double dStrikePrice, 
		  double dVolatility, 
		  string& sOptionType) ;
};

#endif // COPTION_PRICE_H
