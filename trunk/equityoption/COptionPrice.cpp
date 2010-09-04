/*
 *  COptionPrice.cpp
 *  
 *
 *  Created by William Wang on 01/09/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "COptionPrice.h"
#include <iostream>
#include <ql/quantlib.hpp>

using namespace QuantLib;

double
CEuropeanOptionPrice::getPrice(
    Date tSettlementDate, 
    Date tMaturityDate,
    double dSpotPrice, 
    double dStrikePrice, 
    double dVolatility,
    string& sOptionType)
{
 
    try {
        // set up dates
        Calendar calendar = TARGET();
        Date todaysDate(26, January, 2005);
        Date settlementDate(tSettlementDate);
		
        Settings::instance().evaluationDate() = todaysDate;
		
        // our options
        Option::Type type;
        if (sOptionType.compare("CALL")==0) {
            type = Option::Call;
        }else{
            type = Option::Put;
        }
		
        Real underlying = dSpotPrice;
        Real strike = dStrikePrice;
        Spread dividendYield = 0.00;   // non divident paying
        Rate riskFreeRate = 0.01;      // euro interest rate jan 2005
        Volatility volatility = dVolatility;
        Date maturity(tMaturityDate);
        DayCounter dayCounter = Actual365Fixed();
		
        // printout
        std::string method;
        Size widths[] = { 35, 14 };
        std::cout << std::setw(widths[0]) << std::left << "Method"
                  << std::setw(widths[1]) << std::left << "European"
                  << std::endl;
		
        boost::shared_ptr<Exercise> europeanExercise(
            new EuropeanExercise(maturity));
		
        Handle<Quote> underlyingH(
            boost::shared_ptr<Quote>(new SimpleQuote(underlying)));
		
        // bootstrap the yield/dividend/vol curves
        Handle<YieldTermStructure> flatTermStructure(
            boost::shared_ptr<YieldTermStructure>(
                new FlatForward(settlementDate, riskFreeRate, dayCounter)));
        Handle<YieldTermStructure> flatDividendTS(
            boost::shared_ptr<YieldTermStructure>(
                new FlatForward(settlementDate, dividendYield, dayCounter)));
        Handle<BlackVolTermStructure> flatVolTS(
            boost::shared_ptr<BlackVolTermStructure>(
                new BlackConstantVol(settlementDate, calendar, volatility,
                                     dayCounter)));
        boost::shared_ptr<StrikedTypePayoff> payoff(
            new PlainVanillaPayoff(type, strike));
		
        boost::shared_ptr<BlackScholesMertonProcess> bsmProcess(
            new BlackScholesMertonProcess(underlyingH, flatDividendTS,
                                          flatTermStructure, flatVolTS));
		
        // options
        VanillaOption europeanOption(payoff, europeanExercise);
		
        // Analytic formulas:
		
        // Black-Scholes for European
        method = "Black-Scholes";
        europeanOption.setPricingEngine(boost::shared_ptr<PricingEngine>(
                                            new AnalyticEuropeanEngine(bsmProcess)));
        std::cout << std::setw(widths[0]) << std::left << method
                  << std::fixed
                  << std::setw(widths[1]) << std::left << europeanOption.NPV()
                  << std::endl;
		
        // Integral
        method = "Integral";
        europeanOption.setPricingEngine(boost::shared_ptr<PricingEngine>(
                                            new IntegralEngine(bsmProcess)));
        std::cout << std::setw(widths[0]) << std::left << method
                  << std::fixed
                  << std::setw(widths[1]) << std::left << europeanOption.NPV()
                  << std::endl;
		
        // Finite differences
        Size timeSteps = 801;
        method = "Finite differences";
        europeanOption.setPricingEngine(boost::shared_ptr<PricingEngine>(
                                            new FDEuropeanEngine<CrankNicolson>(bsmProcess,
                                                                                timeSteps,timeSteps-1)));
        std::cout << std::setw(widths[0]) << std::left << method
                  << std::fixed
                  << std::setw(widths[1]) << std::left << europeanOption.NPV()
                  << std::endl;
		
        // Binomial method: Jarrow-Rudd
        method = "Binomial Jarrow-Rudd";
        europeanOption.setPricingEngine(boost::shared_ptr<PricingEngine>(
                                            new BinomialVanillaEngine<JarrowRudd>(bsmProcess,timeSteps)));
        std::cout << std::setw(widths[0]) << std::left << method
                  << std::fixed
                  << std::setw(widths[1]) << std::left << europeanOption.NPV()
                  << std::endl;
		
        // Binomial Cox-Ross-Rubinstein
        method = "Binomial Cox-Ross-Rubinstein";
        europeanOption.setPricingEngine(boost::shared_ptr<PricingEngine>(
                                            new BinomialVanillaEngine<CoxRossRubinstein>(bsmProcess,
                                                                                         timeSteps)));
        std::cout << std::setw(widths[0]) << std::left << method
                  << std::fixed
                  << std::setw(widths[1]) << std::left << europeanOption.NPV()
                  << std::endl;
		
        // Binomial method: Additive equiprobabilities
        method = "Additive equiprobabilities";
        europeanOption.setPricingEngine(boost::shared_ptr<PricingEngine>(
                                            new BinomialVanillaEngine<AdditiveEQPBinomialTree>(bsmProcess,
                                                                                               timeSteps)));
        std::cout << std::setw(widths[0]) << std::left << method
                  << std::fixed
                  << std::setw(widths[1]) << std::left << europeanOption.NPV()
                  << std::endl;
		
        // Binomial method: Binomial Trigeorgis
        method = "Binomial Trigeorgis";
        europeanOption.setPricingEngine(boost::shared_ptr<PricingEngine>(
                                            new BinomialVanillaEngine<Trigeorgis>(bsmProcess,timeSteps)));
        std::cout << std::setw(widths[0]) << std::left << method
                  << std::fixed
                  << std::setw(widths[1]) << std::left << europeanOption.NPV()
                  << std::endl;
		
        // Binomial method: Binomial Tian
        method = "Binomial Tian";
        europeanOption.setPricingEngine(boost::shared_ptr<PricingEngine>(
                                            new BinomialVanillaEngine<Tian>(bsmProcess,timeSteps)));
        std::cout << std::setw(widths[0]) << std::left << method
                  << std::fixed
                  << std::setw(widths[1]) << std::left << europeanOption.NPV()
                  << std::endl;
		
        // Binomial method: Binomial Leisen-Reimer
        method = "Binomial Leisen-Reimer";
        europeanOption.setPricingEngine(boost::shared_ptr<PricingEngine>(
                                            new BinomialVanillaEngine<LeisenReimer>(bsmProcess,timeSteps)));
        std::cout << std::setw(widths[0]) << std::left << method
                  << std::fixed
                  << std::setw(widths[1]) << std::left << europeanOption.NPV()
                  << std::endl;
		
        // Binomial method: Binomial Joshi
        method = "Binomial Joshi";
        europeanOption.setPricingEngine(boost::shared_ptr<PricingEngine>(
                                            new BinomialVanillaEngine<Joshi4>(bsmProcess,timeSteps)));
        std::cout << std::setw(widths[0]) << std::left << method
                  << std::fixed
                  << std::setw(widths[1]) << std::left << europeanOption.NPV()
                  << std::endl;
		
        // Monte Carlo Method: MC (crude)
        timeSteps = 1;
        method = "MC (crude)";
        Size mcSeed = 42;
        boost::shared_ptr<PricingEngine> mcengine1;
        mcengine1 = MakeMCEuropeanEngine<PseudoRandom>(bsmProcess)
            .withSteps(timeSteps)
            .withAbsoluteTolerance(0.02)
            .withSeed(mcSeed);
        europeanOption.setPricingEngine(mcengine1);
        // Real errorEstimate = europeanOption.errorEstimate();
        std::cout << std::setw(widths[0]) << std::left << method
                  << std::fixed
                  << std::setw(widths[1]) << std::left << europeanOption.NPV()
                  << std::endl;
		
        // Monte Carlo Method: QMC (Sobol)
        method = "QMC (Sobol)";
        Size nSamples = 32768;  // 2^15
        boost::shared_ptr<PricingEngine> mcengine2;
        mcengine2 = MakeMCEuropeanEngine<LowDiscrepancy>(bsmProcess)
            .withSteps(timeSteps)
            .withSamples(nSamples);
        europeanOption.setPricingEngine(mcengine2);
        std::cout << std::setw(widths[0]) << std::left << method
                  << std::fixed
                  << std::setw(widths[1]) << std::left << europeanOption.NPV()
                  << std::endl;
		
        // return
        std::cout << std::endl;
        return 0;
		
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "unknown error" << std::endl;
        return 1;
    }
}

double 
CAmericanOptionPrice::getPrice(
    Date tSettlementDate, 
    Date tMaturityDate,
    double dSpotPrice, 
    double dStrikePrice, 
    double dVolatility,
    string& sOptionType)
{
    try {
        // set up dates
        Calendar calendar = TARGET();
        Date todaysDate(26, January, 2005);
        Date settlementDate(tSettlementDate);
		
        Settings::instance().evaluationDate() = todaysDate;
		
        // option parameters
        Option::Type type;
        if (sOptionType.compare("CALL")==0) {
            type = Option::Call;
        }else{
            type = Option::Put;
        }
        Real underlying = dSpotPrice;
        Real strike = dStrikePrice;
        Spread dividendYield = 0.00;  // non divident paying
        Rate riskFreeRate = 0.01;     // euro interest rate jan 2005
        Volatility volatility = dVolatility;
        Date maturity(tMaturityDate);
        DayCounter dayCounter = Actual365Fixed();
		
        // pricing method
        std::string method;
		
        // write column headings
        Size widths[] = { 35, 14 };
        std::cout << std::setw(widths[0]) << std::left << "Method"
                  << std::setw(widths[1]) << std::left << "American"
                  << std::endl;
		
        // american exercise option
        boost::shared_ptr<Exercise> americanExercise(
            new AmericanExercise(settlementDate,
                                 maturity));
		
        Handle<Quote> underlyingH(
            boost::shared_ptr<Quote>(new SimpleQuote(underlying)));
		
        // bootstrap the yield/dividend/vol curves
        Handle<YieldTermStructure> flatTermStructure(
            boost::shared_ptr<YieldTermStructure>(
                new FlatForward(settlementDate, riskFreeRate, dayCounter)));
        Handle<YieldTermStructure> flatDividendTS(
            boost::shared_ptr<YieldTermStructure>(
                new FlatForward(settlementDate, dividendYield, dayCounter)));
        Handle<BlackVolTermStructure> flatVolTS(
            boost::shared_ptr<BlackVolTermStructure>(
                new BlackConstantVol(settlementDate, calendar, volatility,
                                     dayCounter)));
        boost::shared_ptr<StrikedTypePayoff> payoff(
            new PlainVanillaPayoff(type, strike));
		
        boost::shared_ptr<BlackScholesMertonProcess> bsmProcess(
            new BlackScholesMertonProcess(underlyingH, flatDividendTS,
                                          flatTermStructure, flatVolTS));
		
        // options
        VanillaOption americanOption(payoff, americanExercise);
		
        // Analytic formulas:
		
        // Barone-Adesi and Whaley approximation for American
        method = "Barone-Adesi/Whaley";
        americanOption.setPricingEngine(boost::shared_ptr<PricingEngine>(
                                            new BaroneAdesiWhaleyApproximationEngine(bsmProcess)));
        std::cout << std::setw(widths[0]) << std::left << method
                  << std::fixed
                  << std::setw(widths[1]) << std::left << americanOption.NPV()
                  << std::endl;
		
        // Bjerksund and Stensland approximation for American
        method = "Bjerksund/Stensland";
        americanOption.setPricingEngine(boost::shared_ptr<PricingEngine>(
                                            new BjerksundStenslandApproximationEngine(bsmProcess)));
        std::cout << std::setw(widths[0]) << std::left << method
                  << std::fixed
                  << std::setw(widths[1]) << std::left << americanOption.NPV()
                  << std::endl;
		
        // Finite differences
        Size timeSteps = 801;
        method = "Finite differences";
        americanOption.setPricingEngine(boost::shared_ptr<PricingEngine>(
                                            new FDAmericanEngine<CrankNicolson>(bsmProcess,
                                                                                timeSteps,timeSteps-1)));
        std::cout << std::setw(widths[0]) << std::left << method
                  << std::fixed
                  << std::setw(widths[1]) << std::left << americanOption.NPV()
                  << std::endl;
		
        // Binomial method: Jarrow-Rudd
        method = "Binomial Jarrow-Rudd";
        americanOption.setPricingEngine(boost::shared_ptr<PricingEngine>(
                                            new BinomialVanillaEngine<JarrowRudd>(bsmProcess,timeSteps)));
        std::cout << std::setw(widths[0]) << std::left << method
                  << std::fixed
                  << std::setw(widths[1]) << std::left << americanOption.NPV()
                  << std::endl;
		
        // Binomial Cox-Ross-Rubinstein
        method = "Binomial Cox-Ross-Rubinstein";
        americanOption.setPricingEngine(boost::shared_ptr<PricingEngine>(
                                            new BinomialVanillaEngine<CoxRossRubinstein>(bsmProcess,
                                                                                         timeSteps)));
        std::cout << std::setw(widths[0]) << std::left << method
                  << std::fixed
                  << std::setw(widths[1]) << std::left << americanOption.NPV()
                  << std::endl;
		
        // Binomial method: Additive equiprobabilities
        method = "Additive equiprobabilities";
        americanOption.setPricingEngine(boost::shared_ptr<PricingEngine>(
                                            new BinomialVanillaEngine<AdditiveEQPBinomialTree>(bsmProcess,
                                                                                               timeSteps)));
        std::cout << std::setw(widths[0]) << std::left << method
                  << std::fixed
                  << std::setw(widths[1]) << std::left << americanOption.NPV()
                  << std::endl;
		
        // Binomial method: Binomial Trigeorgis
        method = "Binomial Trigeorgis";
        americanOption.setPricingEngine(boost::shared_ptr<PricingEngine>(
                                            new BinomialVanillaEngine<Trigeorgis>(bsmProcess,timeSteps)));
        std::cout << std::setw(widths[0]) << std::left << method
                  << std::fixed
                  << std::setw(widths[1]) << std::left << americanOption.NPV()
                  << std::endl;
		
        // Binomial method: Binomial Tian
        method = "Binomial Tian";
        americanOption.setPricingEngine(boost::shared_ptr<PricingEngine>(
                                            new BinomialVanillaEngine<Tian>(bsmProcess,timeSteps)));
        std::cout << std::setw(widths[0]) << std::left << method
                  << std::fixed
                  << std::setw(widths[1]) << std::left << americanOption.NPV()
                  << std::endl;
		
        // Binomial method: Binomial Leisen-Reimer
        method = "Binomial Leisen-Reimer";
        americanOption.setPricingEngine(boost::shared_ptr<PricingEngine>(
                                            new BinomialVanillaEngine<LeisenReimer>(bsmProcess,timeSteps)));
        std::cout << std::setw(widths[0]) << std::left << method
                  << std::fixed
                  << std::setw(widths[1]) << std::left << americanOption.NPV()
                  << std::endl;
		
        // Binomial method: Binomial Joshi
        method = "Binomial Joshi";
        americanOption.setPricingEngine(boost::shared_ptr<PricingEngine>(
                                            new BinomialVanillaEngine<Joshi4>(bsmProcess,timeSteps)));
        std::cout << std::setw(widths[0]) << std::left << method
                  << std::fixed
                  << std::setw(widths[1]) << std::left << americanOption.NPV()
                  << std::endl;
		
        // Monte Carlo Method: MC (Longstaff Schwartz)
        method = "MC (Longstaff Schwartz)";
        Size mcSeed = 42;
        boost::shared_ptr<PricingEngine> mcengine3;
        mcengine3 = MakeMCAmericanEngine<PseudoRandom>(bsmProcess)
            .withSteps(100)
            .withAntitheticVariate()
            .withCalibrationSamples(4096)
            .withAbsoluteTolerance(0.02)
            .withSeed(mcSeed);
        americanOption.setPricingEngine(mcengine3);
        std::cout << std::setw(widths[0]) << std::left << method
                  << std::fixed
                  << std::setw(widths[1]) << std::left << americanOption.NPV()
                  << std::endl;
		
        // return
        std::cout << std::endl;
        return 0;		
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "unknown error" << std::endl;
        return 1;
    }
	
}
