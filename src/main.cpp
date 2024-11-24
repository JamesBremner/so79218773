#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include "cRunWatch.h"

// input data
std::vector<int> theCoupons;
std::vector<int> theItems;
int theMoney;

// details of buying an item with a coupon
struct sBuy
{
    int itemIndex;
    int couponIndex;
    int cost;
    int money;

    sBuy(int i, int c, int s, int m)
        : itemIndex(i),
          couponIndex(c),
          cost(s),
          money(m)
    {
    }

    void text()
    {
        std::cout
            << "Buy item " << theItems[itemIndex]
            << " with coupon " << theCoupons[couponIndex]
            << " cost " << cost
            << " wallet " << money
            << "\n";
    }
};

// the planned buys calculated by the solver
std::vector<sBuy> thePlan;

// generated the problem in the original stack overflow question
// https://stackoverflow.com/q/79218773/16582
void gen1()
{
    theMoney = 30;
    theItems = {12, 20, 15, 10};
    theCoupons = {9, 6, 8, 7};
}

/// @brief generate random problem of set size
/// @param count of items and coupons
void gen(int count)
{
    srand(time(NULL));
    theItems.clear();
    theCoupons.clear();
    for (int i = 0; i < count; i++)
    {
        theItems.push_back(rand() % 10 + 10);
        theCoupons.push_back(rand() % 9 + 1);
    }
    theMoney = count * 20;
}

/// @brief record a buy
/// @param itemIndex 
/// @param couponIndex 
void buy(int itemIndex, int couponIndex)
{
    int cost = theItems[itemIndex] - theCoupons[couponIndex];
    if (cost > theMoney)
        return;
    theMoney -= cost;
    thePlan.emplace_back(itemIndex, couponIndex, cost, theMoney);
}

/// @brief apply algorithm to solve problem
void solve()
{
    // sort items into increasing cost
    std::sort(theItems.begin(), theItems.end());

    // sort coupons into decreasing value
    std::sort(theCoupons.begin(), theCoupons.end(),
              [](int a, int b)
              { return a > b; });

    // buy items of increasing cost
    for (int itemIndex = 0; itemIndex < theItems.size(); itemIndex++)
        // apply coupon of decreasing value
        for (int couponIndex = 0; couponIndex < theCoupons.size(); couponIndex++)
            buy(itemIndex, couponIndex);
}

// display plenned purchases
void display()
{
    for (auto &b : thePlan)
        b.text();
    std::cout << "Items bought " << thePlan.size() << "\n";
}

// unit test
void test()
{
    gen1();
    solve();
    display();
    if (thePlan.size() != 8)
    {
        std::cout << "TEST FAILED !!!";
        exit(1);
    }
}

// performance test
void performace()
{
    // start timimng profiler
    raven::set::cRunWatch::Start();
    gen(1000);
    {
        raven::set::cRunWatch aWatcher("N=1000");
        solve();
    }
    std::cout << "N=1000, Items bought " << thePlan.size() << "\n";
    gen(10000);
    {
        raven::set::cRunWatch aWatcher("N=10000");
        solve();
    }
    std::cout << "N=10000, Items bought " << thePlan.size() << "\n";
    gen(100000);
    {
        raven::set::cRunWatch aWatcher("N=100000");
        solve();
    }
    std::cout << "N=100000, Items bought " << thePlan.size() << "\n";

    // display timing profiler report
    raven::set::cRunWatch::Report();
}

main()
{
    // run unit test
    test();

    // run performance test
    performace();

    return 0;
}
