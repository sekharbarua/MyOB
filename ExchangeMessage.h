/*
 * Auther :-Sekhar Barua
 * Date : - 15-07-2019
 *
 */

#ifndef EXCHANGE_H
#define EXCHANGE_H

#include <string>
#include <iostream>

using namespace std;
namespace orderbook {

	
	// Market exchangeFeed
	class  exchangeFeed
	{
	public:
		typedef std::string Symbol;
		typedef double Quantity;       
     public:
        
		Quantity bidSize = 0;
		Quantity bidPrice = 0;
		Quantity offerSize = 0;
		Quantity offerPrice = 0;
		Symbol mProduct = std::string("");
			
     
	public:
	
		exchangeFeed() = default;
		exchangeFeed(const exchangeFeed& feed) : bidSize(feed.bidSize),bidPrice(feed.bidPrice),offerSize(feed.offerSize),offerPrice(feed.offerPrice),
			                                    mProduct(feed.mProduct)
		{ }

		exchangeFeed(const Quantity&& bidSize, const Quantity&& bidPrice, const Quantity&& offerSize, const Quantity&& offerPrice,
			const Symbol&& mProduct):bidSize(std::move(bidSize)), bidPrice(std::move(bidPrice)),
			offerSize(std::move(offerSize)), offerPrice(std::move(offerPrice)), mProduct(std::move(mProduct))

		{}

		exchangeFeed& operator=(const exchangeFeed& other)
		{
			bidSize = other.bidSize;
			bidPrice = other.bidPrice;
			offerSize = other.offerSize;
			offerPrice = other.offerPrice;
			mProduct = other.mProduct;
			return *this;
		}
		exchangeFeed& operator=(exchangeFeed&& other)
		{
			bidSize = std::move(other.bidSize);
			bidPrice = std::move(other.bidPrice);
			offerSize = std::move(other.offerSize);
			offerPrice = std::move(other.offerPrice);
			mProduct = std::move(other.mProduct);
			return *this;
		}
		exchangeFeed f(exchangeFeed a) { return a; }
	
	};



} // orderbook namespace

#endif //EXCHANGE_H