# Project description #

This is a project aimed to create an automated algorithmic trading platform. The system consists of market data feed, trading strategy, order management and risk management modules.

Initially, the system interfaces with Interactive Broker C++ API for market data feed and order management. The trading strategy boxes and risk management modules are developed using QuantLib. The initial goal of the system is to trade equity options and index options in the US and UK.

The system should be easily extended to trade multiple asset classes and support multiple brokers. C++ is selected as the language to implemented the trading system, due to the potential to go ultra low latency and high frequency with the same code base, among other reasons such as skills and library considerations.

Orthogonal to the software implementation of the project, I've also implemented computation intensive financial models in HDL (Hardware Description Language) to accelerate the algorithms in hardware, such as FPGA and ASIC. An article I wrote to describe the implementation of Black-Scholes is available here on HFT Review http://www.hftreview.com/mod/file/download.php?file_guid=56074 or in the Downloads as published on ACCU Overload August 2012 http://accu.org/var/uploads/journals/overload110.pdf.