#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include "cRunWatch.h"

std::vector<int> theCoupons;
std::vector<int> theItems;
int theMoney;

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

std::vector<sBuy> thePlan;

void gen1()
{
    theMoney = 30;
    theItems = {12, 20, 15, 10};
    theCoupons = {9, 6, 8, 7};
}
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

void buy(int itemIndex, int couponIndex)
{
    int cost = theItems[itemIndex] - theCoupons[couponIndex];
    if (cost > theMoney)
        return;
    theMoney -= cost;
    thePlan.emplace_back(itemIndex, couponIndex, cost, theMoney);
}

void solve()
{
    std::sort(theItems.begin(), theItems.end());
    std::sort(theCoupons.begin(), theCoupons.end(),
              [](int a, int b)
              { return a > b; });
    for (int itemIndex = 0; itemIndex < theItems.size(); itemIndex++)
        for (int couponIndex = 0; couponIndex < theCoupons.size(); couponIndex++)
            buy(itemIndex, couponIndex);
}
void display()
{
    for (auto &b : thePlan)
        b.text();
    std::cout << "Items bought " << thePlan.size() << "\n";
}

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

void performace()
{
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

    raven::set::cRunWatch::Report();
}

main()
{
    test();

    performace();

    return 0;
}
