/*
 *  COptionRecord.cpp
 *  
 *
 *  Created by William Wang on 01/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>
#include <exception>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include "COptionRecord.h"
#include "COptionPrice.h"
#include "CBadRecord.h"


using namespace std;
using QuantLib::Date;
using QuantLib::Month;

COptionRecord::COptionRecord(const string& sline)
{
    const char* EXERCISE_TYPE_EUROPEAN = "European";
    const char* EXERCISE_TYPE_AMERICAN = "American";

    vector<string> vItems;
    tokenizeLine( sline, ";", vItems );

    try {
        retrieveItems( vItems );
    }
    catch (...) {
        // re-throw the exception
        throw; 
    }

    if ( m_sExerciseType.compare(EXERCISE_TYPE_EUROPEAN) == 0 ) {
        m_pPriceStrategy = boost::shared_ptr<COptionPrice>(new CEuropeanOptionPrice());
    } else {
        m_pPriceStrategy = boost::shared_ptr<COptionPrice>(new CAmericanOptionPrice());
    }
}

COptionRecord::~COptionRecord()
{
}

double 
COptionRecord::getSpotPrice()
{
    return m_dSpotPrice;
}

double 
COptionRecord::getStrikePrice()
{
    return m_dStrikePrice;
}

double 
COptionRecord::price()
{
    int key_width = 16;
    int val_width = 16;
    cout << setw(key_width) << left << "DEAL ID "           << setw(val_width) << left << m_nDealId        << endl;
    cout << setw(key_width) << left << "DEAL DATE "         << setw(val_width) << left << m_tDealDate      << endl;
    cout << setw(key_width) << left << "QUANTITY "          << setw(val_width) << left << m_nDealQuantity  << endl;
    cout << setw(key_width) << left << "END DATE "          << setw(val_width) << left << m_tEndDate       << endl;
    cout << setw(key_width) << left << "STRIKE "            << setw(val_width) << left << m_dStrikePrice   << endl;
    cout << setw(key_width) << left << "BUY OR SELL "       << setw(val_width) << left << m_sBuyOrSell     << endl;
    cout << setw(key_width) << left << "OPTION TYPE "       << setw(val_width) << left << m_sOptionType    << endl;
    cout << setw(key_width) << left << "EXERCISE TYPE "     << setw(val_width) << left << m_sExerciseType  << endl;
    cout << setw(key_width) << left << "CURRENCY "          << setw(val_width) << left << m_sCurrency      << endl;
    cout << setw(key_width) << left << "ISIN "              << setw(val_width) << left << m_sISIN          << endl;
    cout << setw(key_width) << left << "PORTFOLIO "         << setw(val_width) << left << m_sPortfolio     << endl;
    cout << setw(key_width) << left << "COUNTERPARTY "      << setw(val_width) << left << m_sCounterParty  << endl;
    cout << setw(key_width) << left << "FRONT OFFICE PV "   << setw(val_width) << left << m_dFrontOfficePV << endl;
    cout << setw(key_width) << left << "VOLATILITY "        << setw(val_width) << left << m_dVolatility    << endl;
    cout << setw(key_width) << left << "SPOT VALUE "        << setw(val_width) << left << m_dSpotPrice     << endl;
    cout << endl;
	
    return m_pPriceStrategy->getPrice(m_tDealDate, m_tEndDate, m_dSpotPrice, m_dStrikePrice, m_dVolatility, m_sOptionType);
}

void
COptionRecord::retrieveItems(const vector<string>& vsItems) 
{
    int nCurrentIndex;

    try {
        nCurrentIndex       =  COLUMN_DealId;
        m_nDealId           = boost::lexical_cast<int>(vsItems[COLUMN_DealId]);
        nCurrentIndex       =  COLUMN_DealDate;
        m_tDealDate         = parseDate(vsItems[ COLUMN_DealDate]);
        nCurrentIndex       =  COLUMN_DealQuantity;
        m_nDealQuantity     = boost::lexical_cast<int>( vsItems[COLUMN_DealQuantity]);
        nCurrentIndex       =  COLUMN_EndDate;
        m_tEndDate          = parseDate(vsItems[ COLUMN_EndDate]);
        nCurrentIndex       =  COLUMN_StrikePrice;
        m_dStrikePrice      = boost::lexical_cast<double>(vsItems[COLUMN_StrikePrice]);
        m_sBuyOrSell        = vsItems[ COLUMN_BuyOrSell];
        m_sOptionType       = vsItems[ COLUMN_OptionType];
        m_sExerciseType     = vsItems[ COLUMN_ExerciseType];
        m_sCurrency         = vsItems[ COLUMN_Currency];
        m_sISIN             = vsItems[ COLUMN_ISIN];
        m_sPortfolio        = vsItems[ COLUMN_Portfolio];
        m_sCounterParty     = vsItems[ COLUMN_CounterParty];
        nCurrentIndex       =  COLUMN_FrontOfficePV;
        m_dFrontOfficePV    = boost::lexical_cast<double>(vsItems[COLUMN_FrontOfficePV]);
        nCurrentIndex       =  COLUMN_Volatility;
        m_dVolatility       = boost::lexical_cast<double>(vsItems[COLUMN_Volatility]);
        nCurrentIndex       =  COLUMN_SpotPrice;
        m_dSpotPrice        = boost::lexical_cast<double>(vsItems[COLUMN_SpotPrice]);
    }catch (const boost::bad_lexical_cast &e ) {
        cout << e.what() << endl;
        cout << " Error parsing this data record with deal id " << vsItems[0];
        cout << " at column " << nCurrentIndex + 1 << " with value " <<  vsItems[nCurrentIndex] << endl;
        cout << " Please check input data record!" << endl;
        throw CBadRecord(nCurrentIndex + 1);
    }catch (exception &e) {
        cout << " Error parsing this data record with deal id " << vsItems[0];
        cout << " at column " << nCurrentIndex + 1 << " with value " <<  vsItems[nCurrentIndex] << endl;
        cout << " Please check input data record!" << endl;
        throw CBadRecord(nCurrentIndex + 1);
	}
}

void
COptionRecord::tokenizeLine(
    const string& sLine, 
    const char* pSeparator, 
    vector<string>& vsItems )
{
    typedef boost::tokenizer<boost::char_separator<char> > boost_tokenizer;
	
    boost::char_separator<char> charSeparator(pSeparator, "", boost::keep_empty_tokens);
    boost_tokenizer tokens(sLine, charSeparator);
	
    for (boost_tokenizer::iterator tok_iter=tokens.begin(); tok_iter!=tokens.end(); ++tok_iter){ 
        vsItems.push_back(*tok_iter);
    }
}

Date
COptionRecord::parseDate(const string& sDate) const
{
    try {
        int day   = boost::lexical_cast<int>(sDate.substr(0,2));
        int month = boost::lexical_cast<int>(sDate.substr(3,2));
        int year  = boost::lexical_cast<int>(sDate.substr(6,4));
		
        Date tDate(day, static_cast<Month>(month), year);
        return tDate;
    }
    catch (const boost::bad_lexical_cast &e) {
        cout << " Error parsing date: " << sDate << endl;
        throw e;
    }
    catch (exception &e) {
        cout << " Invalid Date: " << sDate << ", ";
        cout << e.what() << endl;
        throw e;
	}
}