/*
 * Auther :-Sekhar Barua
 * Date : - 15-07-2019
 *
 */

#ifndef FEEDER_PARSER_H
#define FEEDER_PARSER_H

#include <string>
#include <sstream>
#include <array>
#include "order.h"
#include "ConsException.h"
#include "ConsolodiateMarket.h"

namespace orderbook {

	class Feeder {
	public:
		// Basic Market Order Feeder
		static orderbook::Order parse(const std::string &input);
	};

} // orderbook namespace

#endif //FEEDER_PARSER_H

inline orderbook::Order orderbook::Feeder::parse(const std::string &input) {
	// Split the line into different tokens
	std::istringstream input_string_stream(input);
	std::array<std::string, 7> tokens;
	std::copy(std::istream_iterator<std::string>(input_string_stream),
		std::istream_iterator<std::string>(),
		tokens.begin());

	Order order;

	// timestamp
	auto timestamp = tokens[0];
	order.timestamp = (Order::Timestamp) std::stoul(timestamp);

	// identifier
	auto order_identifier = tokens[1];
	if (order_identifier.compare(kAddNewOrderIdentifier) == 0)
		order.type = NEW_ORDER;
	else if (order_identifier.compare(kCancelOrderIdentifier) == 0)
		order.type = CANCEL_ORDER;
	else if (order_identifier.compare(kModifyOrderIdentifier) == 0)
		order.type = MODIFY_ORDER;
	else
		throw ParseException();

	// order-id
	auto order_id = tokens[2];
	order.id = order_id;

	if (order.type == MODIFY_ORDER || order.type == NEW_ORDER) 
	{ // Add Order

	  // side
		auto sideLiteral = tokens[3];
		if (sideLiteral == kBuyLiteral)
			order.side = BUY;
		else if (sideLiteral == kSellLiteral)
			order.side = SELL;
		else
			throw ParseException();

		// price (in cents)
		order.price = static_cast<Order::Limit_Price>(100 * std::stof(tokens[4]));

		// size
		order.size = std::stoul(tokens[5]);

	}
	else if (order.type == CANCEL_ORDER) 
	{ // Reduce Order
   // size
		order.size = std::stoul(tokens[3]);
	}
	else 
	{
		throw ParseException();
	}

	auto eProduct = tokens[6];
	order.mProduct = eProduct;

	return order;
}