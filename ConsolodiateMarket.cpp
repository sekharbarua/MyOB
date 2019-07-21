#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <map>
#include <array>
#include "inc\ConsolodiateMarket.h"


void orderbook::BookConsolidator::consolodiateFeed(exchangeFeed &fed) 
{
	orderLocator book_order;
	double newBidSize = fed.bidSize;
	double newOfferSize = fed.offerSize;

	for (auto itr = bids_.find(fed.bidPrice); itr != bids_.end(); itr++)
	{
		newBidSize = bids_.find(fed.bidPrice)->second + fed.bidSize;
		
	}
	bids_.erase(fed.bidPrice);
	bids_.insert(std::pair<Order::Limit_Price, Order::Quantity>(fed.bidPrice, newBidSize));
	for (auto itr = asks_.find(fed.offerPrice); itr != asks_.end(); itr++)
	{
		newOfferSize = asks_.find(fed.offerPrice)->second + fed.offerSize;
		
	}
	asks_.erase(fed.offerPrice);
	asks_.insert(std::pair<Order::Limit_Price, Order::Quantity>(fed.offerPrice, newOfferSize));
}

void orderbook::BookConsolidator::processExFeed(const std::string &input)
{
	exchangeFeed feed;
	// Parsing
	try {
		feed = Feeder::parseExFeed(input);
	}
	catch (const ParseException&) {
		std::cerr << "***WARNING*** Input format incorrect." << std::endl;
		std::cerr << "Malformed input message: " << input << std::endl;
		return;
	}
	try {
		consolodiateFeed(feed);
	}
	catch (const ParseException&) {
		std::cerr << "Error on Feed Consolodations";
	}
}

void orderbook::BookConsolidator::processOrder(const std::string &input) {
	Order order;


	// Parsing
	try {
		order = Feeder::parse(input);
	}
	catch (const ParseException&) {
		std::cerr << "***WARNING*** Input format incorrect." << std::endl;
		std::cerr << "Malformed input message: " << input << std::endl;
		return;
	}

	// Updating the Orderbooks
	try {
		updateBooks(order);
	}
	catch (const updateBooksException&) {
		std::cerr << "Error on adding order to the orderbooks";
	}
}

void orderbook::BookConsolidator::updateBooks(Order &order) {
	if (order.type == NEW_ORDER || order.type == MODIFY_ORDER)
		addToBooks(order);
	else if (order.type == CANCEL_ORDER)
		reduceFromBooks(order);
	else
		throw updateBooksException();
}

void orderbook::BookConsolidator::addToBooks(Order &order) {
	orderLocator book_order;
	book_order.side = order.side;
	auto locator_it = orders_locator_.insert_or_assign(order.id, book_order).first;
	std::multimap<Order::Limit_Price, Order::Quantity>::iterator order_it;
	
	if (order.side == orderbook::BUY)
	{
		double newBidSize = order.size;
		for (auto itr = bids_.find(order.price); itr != bids_.end(); itr++)
			newBidSize = bids_.find(order.price)->second + order.size;
		bids_.erase(order.price);
		bids_.insert(std::pair<Order::Limit_Price, Order::Quantity>(order.price, newBidSize));
		order_it = bids_.begin();
	}
	else
	{
		double newOfferSize = order.size;
		for (auto itr = asks_.find(order.price); itr != asks_.end(); itr++)
			newOfferSize = asks_.find(order.price)->second + order.size;
		asks_.erase(order.price);
		asks_.insert(std::pair<Order::Limit_Price, Order::Quantity>(order.price, newOfferSize));
		order_it = asks_.begin();
	}
	locator_it->second.order_it = order_it;
}

void orderbook::BookConsolidator::reduceFromBooks(Order &order) {
	auto locator_it = orders_locator_.find(order.id);
	if (locator_it != orders_locator_.end()) {
		auto book_order = &locator_it->second;
		order.side = book_order->side;
		auto book_it = book_order->order_it;
		auto item_size_ptr = &book_it->second;
		auto new_size = *item_size_ptr - order.size;

		if (new_size <= 0) {  // remove from orderbook
			orders_locator_.erase(locator_it);
			if (order.side == orderbook::BUY)
				bids_.erase(book_it);
			else
				asks_.erase(book_it);
		}
		else {  // update the orderbook
			*item_size_ptr = new_size;
		}
	}
}

void orderbook::BookConsolidator::setTargetSize(Order::Quantity targetSize) {
	target_size = targetSize;
}

void orderbook::BookConsolidator::printTop5()
{

	if (bids_.empty() || asks_.empty())
	{
		std::cout << "Nothing to Show.Empty Order Book" << std::endl;
		std::cout << "Please choose Option 2 -To Process the order" << std::endl;
	}
	else
	{
		std::cout << "Bid Size" << " " << "Bid Price " <<  " "<< "Offer Price" << " " << "Offer Size" << endl;
		for (auto it_m1 = bids_.cbegin(), end_m1 = bids_.cend(),
			it_m2 = asks_.cbegin(), end_m2 = asks_.cend();
			it_m1 != end_m1 || it_m2 != end_m2;)
		{
			if ((it_m1 != end_m1) && (it_m2 != end_m2))
			{
				std::cout << "  " << it_m1->second << "\t\t" << it_m1->first << "\t" << it_m2->first << "\t" << it_m2->second << endl;
				++it_m1;
				++it_m2;
			}
			else
				break;
		}
	}
	std::cout << endl;
}


