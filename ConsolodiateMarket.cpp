#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <map>
#include <array>
#include "inc\ConsolodiateMarket.h"
#include "inc\ConsException.h"
#include "inc\Feeder.h"

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

	// Calculating income or expense
	auto totalPrice = executeOrder(order);

	// Printing the output
	printOutput(order, totalPrice);
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
		order_it = bids_.insert(std::pair<Order::Limit_Price, Order::Quantity>(order.price, order.size));
	else
		order_it = asks_.insert(std::pair<Order::Limit_Price, Order::Quantity>(order.price, order.size));
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

orderbook::Order::Limit_Price orderbook::BookConsolidator::executeOrder(Order &order) {
	Order::Quantity total_size = 0;
	Order::Limit_Price possible_income_expense = 0;
	Order::Limit_Price total = 0;
	switch (order.side) {
	case orderbook::BUY:
		for (auto buy_item : bids_) {
			Order::Limit_Price price = buy_item.first;
			Order::Quantity size = buy_item.second;
			if (target_size <= total_size + size) {
				possible_income_expense += price * (target_size - total_size);
				total = possible_income_expense;
				break;
			}
			else {
				total_size += size;
				possible_income_expense += price * size;
			}
		}
		break;
	case orderbook::SELL:
		for (auto buy_item : asks_) {
			Order::Limit_Price price = buy_item.first;
			Order::Quantity size = buy_item.second;
			if (target_size <= total_size + size) {
				possible_income_expense += price * (target_size - total_size);
				total = possible_income_expense;
				break;
			}
			else {
				total_size += size;
				possible_income_expense += price * size;
			}
		}
		break;
	default:
		throw executeOrderException();
	}
	return total;
}

void orderbook::BookConsolidator::printOutput(Order &order, Order::Limit_Price &totalPrice) {
	double total_in_dollar = static_cast<double>(totalPrice) / 100;
	if (order.side == orderbook::BUY && buy_previous_trade_total != totalPrice) {
		buy_previous_trade_total = totalPrice;
		std::cout << order.timestamp << " " << kSellLiteral << " ";
		if (totalPrice != 0)
			std::cout << std::fixed << std::setprecision(2) << total_in_dollar << std::endl;
		else
			std::cout << "NA" << std::endl;
	}
	else if (order.side == orderbook::SELL && sell_previous_trade_total != totalPrice) {
		sell_previous_trade_total = totalPrice;
		std::cout << order.timestamp << " " << kBuyLiteral << " ";
		if (totalPrice != 0)
			std::cout << std::fixed << std::setprecision(2) << total_in_dollar << std::endl;
		else
			std::cout << "NA" << std::endl;
	}
}

void orderbook::BookConsolidator::setTargetSize(Order::Quantity targetSize) {
	target_size = targetSize;
}