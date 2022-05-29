#include<iostream>
#include<list>
#include<iterator>
#include<sstream>
#include<vector>
#include<utility>
#include<map>
#include<algorithm>
using namespace std;
class Order {
private:
    string      SaleType;
    string      OrderType;
    string      OrderId;
    int         Price;
    int         Quantity;

public:
    /*
     * Going to maintain a static price vector which will have SELL prices
     * if there is sell price then there should be buy list to be part of match
     * having a sell price stored here, i can have match with buy list whose
     * price are >= selling price.
     * Keep the above vector accessible to all objects
     */
    static vector<int> SellPriceVector;
    static vector<int> BuyPriceVector;
    /*
     * If match made then set this boolean, no need to execute when
     * no input is entered.
     */
    static bool IsMatchMade;
    Order(): SaleType(""),OrderType(""), OrderId(""), Price(0), Quantity(0){
    }
    ~Order() {}
    void setSaleType(string SaleType) {
        this->SaleType = SaleType;
    }
    string getSaleType() {
        return this->SaleType;
    }

    void setOrderType(string OrderType) {
        #if DEBUG
            cout<<"Order type " << OrderType <<endl;
        #endif // DEBUG
        this->OrderType = OrderType;
    }
    string getOrderType() {
        return this->OrderType;
    }

    void setOrderId(string OrderId) {
        #if DEBUG
            cout<<"Order ID " <<OrderId<<endl;
        #endif // DEBUG
        this->OrderId = OrderId;
    }
    string getOrderId() {
        return this->OrderId;
    }

    void setPrice(int Price) {
        this->Price = Price;
        /*
         * add unique Prices alone
        */
        #if DEBUG
            cout<<"Incoming Price " <<Price <<endl;
        #endif
    }

    int getPrice() {
        return this->Price;
    }

    void setQuantity(int Quantity) {
        this->Quantity = Quantity;
    }
    int getQuantity() {
        return this->Quantity;
    }

    /*
     * Overload this operator '<', so user defined Class Order list
     * can be compared.  This will help to sort list in descending order.
     * Return true if current Price > other against during comparisson
     */
    bool operator < (const Order & order)const {
        return (this->Price > order.Price);
    }

    /*
     * Overload operator "==", so find can be used for Class order list
     */
    bool operator == (const Order & order)const {
        return (this->Price == order.Price);
    }
};

vector <int> Order::SellPriceVector;
vector <int> Order::BuyPriceVector;
bool Order::IsMatchMade = false;

void OperationsBuy(vector<string> & Input, list<Order> & BuyList) {
            Order o;
            o.setSaleType("BUY");           /*Sale Type*/
            o.setOrderType(Input[1]);       /*Order Type*/
            o.setPrice(stoi(Input[2]));     /*Price*/
            o.setQuantity(stoi(Input[3]));  /*Quantity*/
            o.setOrderId(Input[4]);         /*OrderId*/
            /*
             *if(Input[4].compare("") !=0)
             *validate OrderId and if not null push into list
             */
            BuyList.push_back(o);
            /*
             * Updating the Buy Price Vector
             */
            if (find(Order::BuyPriceVector.begin(), Order::BuyPriceVector.end(), stoi(Input[2])) == Order::BuyPriceVector.end()) {
                #if DEBUG
                    cout<<"Updated Price "<<Price<<"Vector size " << BuyPriceVector.size()<<endl;
                #endif
                Order::BuyPriceVector.push_back(stoi(Input[2]));
            }
}

void OperationsSell(vector<string> & Input, list<Order> & SellList) {
            Order o;
            o.setSaleType("SELL");          /*Sale Type*/
            o.setOrderType(Input[1]);       /*Order Type*/
            o.setPrice(stoi(Input[2]));     /*Price*/
            o.setQuantity(stoi(Input[3]));  /*Quantity*/
            o.setOrderId(Input[4]);         /*OrderId*/
            SellList.push_back(o);

            /*
             * Updating the sale Price Vector
             */
            if (find(Order::SellPriceVector.begin(), Order::SellPriceVector.end(), stoi(Input[2])) == Order::SellPriceVector.end()) {
            #if DEBUG
                cout<<"Updated Price "<<Price<<"Vector size " << SellPriceVector.size()<<endl;
            #endif
            Order::SellPriceVector.push_back(stoi(Input[2]));
        }

}


void OperationsCancel(vector<string> & Input, list<Order> & Orderlist) {

}

void OperationsModify(vector<string> & Input, list<Order> & Orderlist) {

   /*
    * find the order ID, then modify the existing list
    * if the OrderType is IOC or OrderId is null, return
    */
    list<Order>::iterator it;

    for(it = Orderlist.begin(); it!=Orderlist.end(); it++) {
        /*
         * If the sale type is IOC or
         * entered OrderId is null
         */
         #if DEBUG
                    cout << "Sale Type      " << it->getSaleType() <<endl;
                    cout << "Order Type     " << it->getOrderType() <<endl;
                    cout << "Get Price      " << it->getPrice() <<endl;
                    cout << "Get Quantity   " << it->getQuantity()<<endl;
                    cout << "Get OrderId    " << it->getOrderId() <<endl;
        #endif // DEBUG
        if ((it->getOrderType().compare("IOC") == 0) ||
            (it->getOrderId().compare("") == 0)) {

                #if DEBUG
                    cout << "Sale Type      " << it->getSaleType() <<endl;
                    cout << "Order Type     " << it->getOrderType() <<endl;
                    cout << "Get Price      " << it->getPrice() <<endl;
                    cout << "Get Quantity   " << it->getQuantity()<<endl;
                    cout << "Get OrderId    " << it->getOrderId() <<endl;
                #endif // DEBUG
                return;
        }
        if((it->getOrderId().compare(Input[1]) == 0)) {
            /*
             *  OrderId matches here, Input argument
             *  Remove the order and push it back
             *  Erase in iterator,
             *  Set the Price, Quantity, OrderId etc..
             */
            #if DEBUG
                cout<<"MODIFY "<<it->getOrderId()<<endl;
            #endif // DEBUG
                Order temp = *it;
                it = Orderlist.erase(it);
                temp.setSaleType(Input[2]);
                temp.setPrice(stoi(Input[3]));
                temp.setQuantity(stoi(Input[4]));
                temp.setOrderId(Input[1]);
                Orderlist.push_back(temp);
                break;

        }
    }
}

void MatchEngine(list<Order> & Orderlist) {
    /* Ideally, I should have used a map < int, order>, but have some issues.*/
    /* having a Price and order, I would easily iterate o(1)*/

    /*Now maintain two lists Buy and Sell*/
    list<Order> SellList;
    list<Order> BuyList;
    list<Order>:: iterator SellIt;
    list<Order>:: iterator BuyIt;


    #if DEBUG
        cout<<"Match made " << Order::IsMatchMade <<endl;
    #endif
    /*
     * Match is made, so return
     */
    if (Order::IsMatchMade == true)
        return;

    #if DEBUG
        cout<<"Sell list Size " <<SellList.size() << "Buy list size "<<BuyList.size()<<endl;
    #endif // DEBUG
    for (auto PriceIt = Order::SellPriceVector.begin(); PriceIt != Order::SellPriceVector.end(); PriceIt++) {
        string temp= "";
        #if DEBUG
            cout<<"Price It "<<*Priceit<<endl;
        #endif
        for (SellIt = Orderlist.begin(); SellIt != Orderlist.end(); SellIt++) {
            if((SellIt->getPrice() == *PriceIt) &&
                (SellIt->getSaleType().compare("SELL") == 0) ) {
                    #if DEBUG
                        cout<<"Inside Sell it " << endl;
                    #endif
                break;
            }
        }

        for (BuyIt = Orderlist.begin(); BuyIt != Orderlist.end(); BuyIt ++) {
            if ((BuyIt->getPrice() >= *PriceIt) &&
                (BuyIt->getSaleType().compare("BUY") == 0)){
                    #if DEBUG
                        cout<<"Inside Buy it " << endl;
                        cout<< "Sell Quantity " << SellIt->getQuantity()<< "   BuyIt Quantity "<< BuyIt->getQuantity()<<endl;
                    #endif //DEBUG
                    auto Quantity = (SellIt->getQuantity() > BuyIt->getQuantity()) ?
                                     BuyIt->getQuantity() : SellIt->getQuantity();
                    #if DEBUG
                        cout<<"Quantity  " <<Quantity << "  Sell qunatity " << SellItem.getQuantity()<<endl;
                    #endif
                    temp = "TRADE " + BuyIt->getOrderId() + " " +
                            to_string(BuyIt->getPrice()) + " " +
                            to_string(Quantity) + " " +
                            SellIt->getOrderId() + " " +
                            to_string(SellIt->getPrice()) + " " +
                            to_string(Quantity);
                    /*Update the sell items*/
                    SellIt->setQuantity(SellIt->getQuantity()- Quantity);
                    /*Update Buy Quantity*/
                    if( SellIt->getQuantity() > BuyIt->getQuantity() <= 0)
                         BuyIt->setQuantity(0);
                    cout<< temp <<endl;
                }


        } //end of for loop BuyIt
        #if DEBUG
            cout<< "Updated sell items quantity " << SellIt->getQuantity()<<endl;
        #endif //DEBUG
    } //end of for Priceit
    Order::IsMatchMade  = true;
}

void OperationsPrint(list<Order> & Orderlist) {
        list <Order>::iterator temp;

        vector<int> SellPriceVector = Order::SellPriceVector;
        vector<int> BuyPriceVector = Order::BuyPriceVector;
        vector<int>::iterator Pv;
        #if DEBUG
            cout<<"Number of different Sell Prices " << SellPriceVector.size() <<endl;
            cout<<"Number of different Buy Prices " << BuyPriceVector.size() <<endl;
        #endif // DEBUG
        /* Run Match Engine */
        MatchEngine(Orderlist);
        /*
         * Sort the Price vector in descending so, we can iterator for each Price
         * make use of greater<int> member function
         * For matching Price, sum Quantity based on SELL/BUY and GFD types.
         */
        sort(SellPriceVector.begin(), SellPriceVector.end(), greater <int>());
        sort(BuyPriceVector.begin(), BuyPriceVector.end(), greater <int>());
        /* Make sure all are in descending order */
        Orderlist.sort();

        cout <<"SELL:" <<endl;
        /*
         * Iterator over the list of orders
         * Print SELL details which are GFD
         * Skip others
         */
        for (auto sPv = SellPriceVector.begin(); sPv != SellPriceVector.end(); sPv++) {
            int Quantity = 0;
            for (temp = Orderlist.begin(); temp != Orderlist.end(); temp ++) {
                if ((temp->getOrderType().compare("GFD") == 0) &&
                    (temp->getSaleType().compare("SELL") == 0) &&
                    (temp->getQuantity() > 0 )){
                        if (*sPv == temp->getPrice())
                            Quantity += temp->getQuantity();
                    #if DEBUG
                        cout << "Sale Type      " << temp->getSaleType() <<endl;
                        cout << "Order Type     " << temp->getOrderType() <<endl;
                        cout << "Get Price      " << temp->getPrice() <<endl;
                        cout << "Get Quantity   " << temp->getQuantity()<<endl;
                        cout << "Get OrderId    " << temp->getOrderId() <<endl;
                    #endif
                }
            }
            if(Quantity)
                cout<<*sPv<< " " << Quantity <<endl;
        }
        cout <<"BUY:" <<endl;
        #if DEBUG
            cout <<"Price vector " << BuyPriceVector.size()<<endl;
        #endif
        for (auto bPv = BuyPriceVector.begin(); bPv != BuyPriceVector.end(); bPv++) {
                int Quantity = 0;
            for (temp = Orderlist.begin(); temp != Orderlist.end(); temp ++) {
                if ((temp->getOrderType().compare("GFD") == 0) &&
                    (temp->getSaleType().compare("BUY") == 0) &&
                    (temp->getQuantity() > 0 )){
                        if (*bPv == temp->getPrice())
                            Quantity += temp->getQuantity();

                    #if DEBUG
                        cout << "Order Type     " << temp->getOrderType() <<endl;
                        cout << "Get Price      " << temp->getPrice() <<endl;
                        cout << "Get Quantity   " << temp->getQuantity()<<endl;
                        cout << "Get OrderId    " << temp->getOrderId() <<endl;
                    #endif
                }
             }
             if(Quantity)
                cout<<*bPv<< " " << Quantity <<endl;
        }
}

void process_Inputs() {
    string      operation = "";
    string      SaleType = "";
    string      OrderType = "";
    string      OrderId = "";
    int         Price;
    int         Quantity;

    list <Order> Orderlist;
    /*
     * Maintain the order objects in list.
     * Easy to iterate and able to modify */

    vector<string> InputString;
    /*
     * Input gets stored in InputString vector*/

    /* Process the Input */
    for (string line; getline(cin, line); ) {
        /*
         * Process Inputs separated by space, push to vector
         */
        if(line.empty()) {
            /*
            * When nothing is entered, show the trade output
            */
            #if DEBUG
                cout << "Line is empty " <<endl;
            #endif
            MatchEngine(Orderlist);
            break;
        }
        /*Process the line*/
        istringstream iss(line);
        for(string s; iss>>s;) {
            /*
             * Using isstringstream and pushing words separated
             * by space into InputString vector
             */
            InputString.push_back(s);
        }
        #if DEBUG
            cout<<InputString[0]<<InputString[0].length()<<endl;
        #endif
        if(InputString[0].compare("BUY") == 0) {
            #if DEBUG
                cout<<"Operations Buy is called " <<endl;
            #endif
            OperationsBuy(InputString, Orderlist);
            InputString.clear();
            /*
             * make sure the InputString cleared
             * for next iteration. */
        } else if (InputString[0].compare("SELL") == 0) {
            #if DEBUG
                cout<<"Operations Sell is called " <<endl;
            #endif
            OperationsSell(InputString, Orderlist);
            InputString.clear();
        } else if (InputString[0].compare("CANCEL") == 0) {
            #if DEBUG
                cout<<"Operations Cancel is called " <<endl;
            #endif
            OperationsCancel(InputString, Orderlist);
            InputString.clear();
        } else if (InputString[0].compare("MODIFY") == 0) {
            #if DEBUG
                cout<<"Operations Modify is called " <<endl;
            #endif
            OperationsModify(InputString, Orderlist);

            InputString.clear();
        } else if (InputString[0].compare("PRINT") == 0) {
            #if DEBUG
                cout<<"Operations Print is called " <<endl;
            #endif
            OperationsPrint(Orderlist);
            InputString.clear();
        }
    } /* End of for loop */
}
int main () {
    /*
    * Process Inputs.. all processing done here
    */
    process_Inputs();
return 0;
}
