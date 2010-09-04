/*
 *  COptionRecord.h
 *  
 *
 *  Created by William Wang on 01/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef COPTION_RECORD_H
#define COPTION_RECORD_H

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <ql/quantlib.hpp>
#include "COptionPrice.h"

using std::string;
using std::vector;
using QuantLib::Date;

class COptionRecord {
 public:
  explicit COptionRecord(const string& sLine);
  ~COptionRecord();

  double getStrikePrice();
  double getSpotPrice();

  double price();

 private:
  void tokenizeLine( const string& sLine, const char* pSeparator, vector<string>& vsItems );
  void retrieveItems(const vector<string> & vsItems);
  Date parseDate(const string& sDate) const;

 private:
  enum RecordColumnIndex {
    COLUMN_DealId        = 0,
    COLUMN_DealDate      = 1,
    COLUMN_DealQuantity  = 3,
    COLUMN_EndDate       = 5,
    COLUMN_StrikePrice   = 6,
    COLUMN_BuyOrSell     = 7,
    COLUMN_OptionType    = 8,
    COLUMN_ExerciseType  = 9,
    COLUMN_Currency      = 10,
    COLUMN_ISIN          = 11,
    COLUMN_Portfolio     = 12,
    COLUMN_CounterParty  = 13,
    COLUMN_FrontOfficePV = 14,
    COLUMN_Volatility    = 15,
    COLUMN_SpotPrice     = 21
  };

  int       m_nDealId;
  Date      m_tDealDate;
  int       m_nDealQuantity;
  Date      m_tEndDate;
  double    m_dStrikePrice;
  string    m_sBuyOrSell;
  string    m_sOptionType;
  string    m_sExerciseType;
  string    m_sCurrency;
  string    m_sISIN;
  string    m_sPortfolio;
  string    m_sCounterParty;
  double    m_dFrontOfficePV;
  double    m_dVolatility;
  double    m_dSpotPrice;

  boost::shared_ptr<COptionPrice> m_pPriceStrategy;
};

#endif //COPTION_RECORD_H
