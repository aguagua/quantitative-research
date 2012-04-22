/*
 * Copyright (c) 2011 Wang & Company
 * All rights reserved
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Wei Wang
 */

#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

class Shares {
private:
    double stampDutyRate;
    double transactionFee;
    int    numShares;
    double buyPrice;
    
  public:
    Shares(double _duty, double _fees):stampDutyRate(_duty),transactionFee(_fees), numShares(0){};	
    int getNumOfShares(double investmentAmount, double sharePrice);
    int getFees(double investmentAmount, double sharePrice);
    int getPayoff();
};

int Shares::getNumOfShares(double investmentAmount, double sharePrice)
{
    buyPrice = sharePrice;
    numShares = floor((investmentAmount - transactionFee)/(sharePrice * (1+stampDutyRate)));
    return numShares;
}

int Shares::getFees(double investmentAmount, double sharePrice)
{
    cout << endl;	
    cout << "Cost breakdown of " << investmentAmount << " pounds investment: " << endl;
    cout << " - " << numShares << " shares" << " cost " << sharePrice*numShares << " pounds" << endl;
	cout << " - Stamp duty cost "<< sharePrice*numShares*stampDutyRate << " pounds" << endl;
    cout << " - Transaction cost " << transactionFee  << " pounds"<< endl;
    cout << endl;
}

int Shares::getPayoff()
{
    double cost = numShares*buyPrice + transactionFee*2 + buyPrice*numShares*stampDutyRate;
    double sellPrice;
    double pnl;
    vector<double> y_value;
    
    //cout << "Payoff from 1% to 10%: " << endl;
    cout << setw(5) << "Payoff" << setw(10) << "TP" << setw(10) << "PnL" << endl;
    
    // payoff from -10% to -1%
    for (double i = -0.1; i<-0.01; i+=0.01) {
        sellPrice = cost * (1+ i)/numShares;
        pnl = i*cost;
        cout << setw(5) << i*100 <<"% "<< setw(10) << sellPrice << setw(10) << pnl << endl;
    }

    cout << endl;
    // payoff from 1% to 10%
    for (double i = 0.01; i<0.1; i+=0.01) {
        sellPrice = cost * (1+ i)/numShares;
        pnl = i*cost;
        cout << setw(5) << i*100 <<"% "<< setw(10) << sellPrice << setw(10) << pnl << endl;
    }

    cout << endl;
    //cout << "Payoff from 10% to 100%: " << endl;
    // payoff from 10% to 100%
    for (double i = 0.1; i<=1; i+=0.1) { 
        sellPrice = cost * (1+ i)/numShares;
        pnl = i*cost;
        cout << setw(5) << i*100 <<"% "<< setw(10) << sellPrice << setw(10) << pnl << endl;
    }
    cout << endl;
    
    //xsystem("set key inside left top vertical Right noreverse enhanced autotitles box linetype -1 linewidth 1.000; set samples 100,100; plot [1,100] cost*x/numShares");
}

int main(int argc, const char * argv[])
{
    if (argc<=2) {
        cout << "Usage: " << argv[0] << 
        " investAmount(pounds), sharePrice(pounds)" << endl;
        return 1;
    }
    
    double investmentAmount = atof(argv[1]);
    double sharePrice = atof(argv[2]);
    
    int numShares;
    
    // fixed for a broker hsbc
    // UK stamp duty is 0.5%
    // hshc invest direct trading fee is £12.95
    Shares hsbcInvestDirect(0.005, 12.95); 
    
    // fixed for a certain share price and amount of investment
    numShares = hsbcInvestDirect.getNumOfShares(investmentAmount,sharePrice);
    
    // get the fees
    hsbcInvestDirect.getFees(investmentAmount,sharePrice);
    
    // get pay off
    hsbcInvestDirect.getPayoff();
}
