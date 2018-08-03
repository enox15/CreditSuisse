// ConsoleApplication4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <map>
#include <string>

using namespace std;

class order
{
public:
	string side;
	double price;
	int quantity;
	
	order(string _side, double _price, int _quantity)
	{
		side = _side;
		price = _price;
		quantity = _quantity;
	};

	bool operator<(const order &A)
	{
		return (quantity < A.quantity);	
	}
};


class Listener
{

private:

	double NQF;
	double COV_Bid;
	double COV_Offer;
	double POV_Bid_Min;
	double POV_Bid_Max;
	double POV_Offer_Min;
	double POV_Offer_Max;
	map <int, order> orders;

public:

	// These two callbacks represent client requests.
	// Indicates the client has sent a new order request to the market. Exactly one
	// callback will follow:
	// * 'OnOrderRequestAcknowledged', in which case order 'id' is active in the market; or
	// * 'OnRequestRejected', in which case the order was never active in the market.
	virtual void OnInsertOrderRequest(int id, string side,  double price, int quantity) /* 'B' for bid, 'O' for offer*/
	{
		order temp_order(side, price, quantity);
		orders.insert(id,temp_order);

		if (side == "B") { POV_Bid_Max += price*quantity; }
		if (side == "O") { POV_Offer_Max += price*quantity; }
	};

	// Indicates the client has sent a request to change the quantity of an order.
	// Exactly one callback will follow:
	// * 'OnRequestAcknowledged', in which case the order quantity was modified and the
	// order is now tracked by ID 'newId'; or
	// * 'OnRequestRejected', in which case the order was not modified and remains
	// tracked by ID 'oldId'.

	virtual void OnReplaceOrderRequest(
		int oldId, // The existing order to modify
		int newId, // The new order ID to use if the modification succeeds
		int deltaQuantity) = 0; // How much the quantity should be increased/decreased

								// These three callbacks represent market confirmations.
								// Indicates the insert or modify request was accepted.
	virtual void OnRequestAcknowledged(int id)
	{
		order temp_order = orders[id];
		if (temp_order.side == "B") 
		{ 
			POV_Bid_Min += temp_order.price*temp_order.quantity;  
			COV_Bid += temp_order.price*temp_order.quantity;
		}
		if (temp_order.side == "O") 
		{ 
			POV_Offer_Min += temp_order.price*temp_order.quantity; 
			COV_Offer += temp_order.price*temp_order.quantity;
		}
	};

	// Indicates the insert or modify request was rejected.
	virtual void OnRequestRejected(
		int id) = 0;

	// Indicates that the order quantity was reduced (and filled) by 'quantityFilled'.
	virtual void OnOrderFilled(
		int id,
		int quantityFilled) = 0;
};

int main()
{

	return 0;
}


