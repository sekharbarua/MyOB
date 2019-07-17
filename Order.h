/*
 * Auther :-Sekhar Barua
 * Date : - 15-07-2019
 *
 */

#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <iostream>

using namespace std;
namespace orderbook {

	enum OrderSide {BUY, SELL };
	enum OrderType { NEW_ORDER, CANCEL_ORDER, MODIFY_ORDER };



	
	// Market Order
	class  Order
	{
	public:
		typedef std::string Symbol;                 // Name of the exchange
		typedef std::string Order_Id;               // Unique Order Id
		typedef unsigned int Timestamp;             // Milliseconds from midnight
		typedef unsigned long int Limit_Price;      // Price in cents
		typedef unsigned long int Quantity;         // Size of the Order
     public:
        Timestamp timestamp = 0;
		OrderType type = NEW_ORDER;
		Order_Id id = "";
		OrderSide side = BUY;
		Limit_Price price = 0;
		Quantity size = 0;
		Symbol mProduct = std::string("");
			
     
	public:
	
		Order() = default;
		Order(const Order& ord) :timestamp(ord.timestamp), type(ord.type), id(ord.id), side(ord.side), price(ord.price), 
			size(ord.size), mProduct(ord.mProduct) 
		{ }

		Order(const Timestamp&& itime, const OrderType&& itype, const Order_Id &&iid,
			OrderSide iside, const Limit_Price&& iprice, const Quantity&& isize,
			const Symbol&& iProduct)
			: timestamp(std::move(itime)),
			type(std::move(itype)),id(std::move(iid)), side(std::move(iside)),
			price(std::move(iprice)),size(std::move(isize)), mProduct(std::move(iProduct))
		{}

		Order& operator=(const Order& other)
		{
			timestamp = other.timestamp;
			type = other.type;
			id = other.id;
			side = other.side;
			price = other.price;
			size = other.size;
			mProduct = other.mProduct;
			return *this;
		}
		Order& operator=(Order&& other)
		{
			timestamp = std::move(other.timestamp);
			type = std::move(other.type);
			id = std::move(other.id);
			side = std::move(other.side);
			price = std::move(other.price);
			size = std::move(other.size);
			mProduct = std::move(other.mProduct);
			return *this;
		}
		Order f(Order a) { return a; }
		 
		bool operator==(Order& rhs)
		{
			if (timestamp != rhs.timestamp  || type != rhs.type || id.compare(rhs.id) != 0 && 
				side != rhs.side || price != rhs.price || size != rhs.size || mProduct.compare(rhs.mProduct) != 0)
				return false;
			else
				return true;
		}
		friend ostream& operator<<(ostream &dout, Order obj)
		{
			std::cout << "--------------------------" << endl;
			dout << "Exchange Info:-" << obj.mProduct << "\n" 
				<< "Order Id :- " << obj.id << "\n"
				<< "OrderType :- " << obj.type << "\n"
				<< "OrderSide :- " << obj.side << "\n"
				<< "Limit Price :- " << obj.price << "\n"
				<< "Quantity :- " << obj.size << "\n"
				<< "Timestamp :-" << obj.timestamp << std::endl;
			return dout;
		}
     
	};



} // orderbook namespace

#endif //ORDER_H