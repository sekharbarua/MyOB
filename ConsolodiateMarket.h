
#pragma once

#ifndef BOOKCONSOLIDATOR_H_
#define BOOKCONSOLIDATOR_H_

#include <string>
#include <unordered_map>
#include <map>
#include <functional>
#include "order.h"
#include <iostream>

const std::string kBuyLiteral = "B";
const std::string kSellLiteral = "S";
const std::string kAddNewOrderIdentifier = "A";
const std::string kModifyOrderIdentifier = "A";
const std::string kCancelOrderIdentifier = "R";


namespace orderbook {

	/**
	 * BookConsolidator class
	 */
	class BookConsolidator {

	public:
		BookConsolidator() : orders_locator_(), bids_(), asks_() {}
		void processOrder(const std::string &input);
		void setTargetSize(orderbook::Order::Quantity targetSize);

	private:
		void updateBooks(Order &order);
		void addToBooks(Order &order);
		void reduceFromBooks(Order &order);
		Order::Limit_Price executeOrder(Order &order);
		void printOutput(Order &order, Order::Limit_Price &totalPrice);

		struct orderLocator {
			orderbook::OrderSide side;
			std::multimap<Order::Limit_Price, Order::Quantity>::iterator order_it;
		};

		// An ordered map for bids in descending order,
		std::multimap<Order::Limit_Price, Order::Quantity, std::greater<>> bids_;

		// An ordered map for asks in ascending order
		std::multimap<Order::Limit_Price, Order::Quantity> asks_;

		// A hash map for locating order in the bids or asks map by it's order-id
		std::unordered_map<std::string, orderLocator> orders_locator_;

		Order::Quantity target_size = 0;
		Order::Limit_Price buy_previous_trade_total = 0;
		Order::Limit_Price sell_previous_trade_total = 0;
	};

} // orderbook namespace

#endif  // #define BOOKCONSOLIDATOR_H_

