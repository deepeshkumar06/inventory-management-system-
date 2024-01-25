#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <limits>
#include <iomanip>
using namespace std;

class Product
{
    private:
        int id;
        string name;
        string category;
        double price;
        int quantity;

    public:
        Product(int id, string name, string category, double price, int quantity)
        {
            this->id=id;
            this->name=name;
            this->category=category;
            this->price=price;
            this->quantity=quantity;
        }

        int getId() const{
            return id;
        }

        void setId(int id){
            this->id=id;
        }

        string getName() const{
            return name;
        }

        void setName(string name){
            this->name=name;
        }

        string getCategory() const{
            return category;
        }

        void setCategory(string category){
            this->category=category;
        }

        double getPrice() const{
            return price;
        }

        void setPrice(double price){
            this->price=price;
        }

        int getQuantity() const{
            return quantity;
        }

        void setQuantity(int quantity){
            this->quantity=quantity;
        }
};

class Inventory
{
    private:
        vector<Product> products;

    public:
        void addProduct(Product product)
        {
        bool found = false;

        for (auto& p : products)
        {
          if (p.getId() == product.getId())
            {
                cout << "Id already Exist." << endl;
                found = true;
                break;
            }
        }

        if (!found)
        {
            products.push_back(product);
            cout << "-----------------------------------------------------------" << endl;
            cout << "              Product added successfully✔✔✔" << endl;
            cout << "-----------------------------------------------------------" <<endl<<endl;

        }
    }

        void removeProduct(int id)
        {
            bool found = false;
            for (auto i = products.begin(); i != products.end(); i++)
            {
                if (i->getId() == id)
                {
                    products.erase(i);
                    found = true;
                    cout << "-----------------------------------------------------------" <<endl;
                    cout << "            Product removed successfully✔✔✔" << endl;
                    cout << "-----------------------------------------------------------" <<endl<<endl;
                    break;
                }
            }
            if (!found)
            {
                cout << "Id does not exist" << endl<<endl;
            }
        }

        Product* findProduct(int id)
        {
            for(auto i=products.begin();i!=products.end();i++)
            {
                if(i->getId()==id)
                {
                    return &(*i);
                }
            }
            return nullptr;
        }

        void updateProduct(int id, string name, string category, double price, int quantity)
        {
            bool found = false;
            for (auto i = products.begin(); i != products.end(); i++)
            {
                if (i->getId() == id)
                {
                    i->setName(name);
                    i->setCategory(category);
                    i->setPrice(price);
                    i->setQuantity(quantity);
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                cout << "ID does not exist." << endl<< endl;
            }
        }

        void printProduct() const
        {
            if (products.empty())
            {
                cout << "-----------------------------------------------------------" << endl;
                cout << "          No data available. Inventory is empty." << endl;
                cout << "-----------------------------------------------------------" << endl<< endl;
            }
            else
            {
                for (auto i = products.begin(); i != products.end(); i++)
                {
                    cout << "ID"<<setw(8)<<"Name"<<setw(8)<<"Category"<<setw(8)<<"Price"<<setw(8)<<"Quantity"<< endl<<endl;
                    cout << i->getId()<<setw(8) << i->getName()<<setw(8)<< i->getCategory()<<setw(8)<< i->getPrice()<<setw(8)<< i->getQuantity()<< endl<<endl;
                }
            }
        }


        void saveInventoryToFile(string filename)
        {
            if (products.empty())
            {
                cout << "Warning: Inventory is empty. Nothing to save." << endl;
                return;
            }

            ifstream fileCheck(filename);
            if (fileCheck)
            {
                // Check if the data to be saved already exists in the file
                string line;
                vector<Product> existingProducts;

                while (getline(fileCheck, line))
                {
                    stringstream ss(line);
                    string idStr, name, category, priceStr, quantityStr;
                    getline(ss, idStr, ',');
                    getline(ss, name, ',');
                    getline(ss, category, ',');
                    getline(ss, priceStr, ',');
                    getline(ss, quantityStr);

                    int id = stoi(idStr);
                    double price = stod(priceStr);
                    int quantity = stoi(quantityStr);

                    existingProducts.emplace_back(id, name, category, price, quantity);
                }

                fileCheck.close();

                for (const auto &newProduct : products)
                {
                    bool exists = false;

                    for (auto &existingProduct : existingProducts)
                    {
                        if (existingProduct.getName() == newProduct.getName() && existingProduct.getCategory() == newProduct.getCategory())
                        {
                            // Update the quantity if the product name is the same
                            existingProduct.setQuantity(existingProduct.getQuantity() + newProduct.getQuantity());
                            exists = true;
                            break;
                        }
                    }

                    if (!exists)
                    {
                        // Add the new product if it doesn't exist
                        existingProducts.push_back(newProduct);
                    }
                }

                // Write the updated data to the file
                ofstream file;
                file.open(filename, ios::out | ios::trunc);

                if (file.is_open())
                {
                    for (const auto &existingProduct : existingProducts)
                    {
                        file << existingProduct.getId() << "," << existingProduct.getName() << ","
                            << existingProduct.getCategory() << "," << existingProduct.getPrice() << ","
                            << existingProduct.getQuantity() << endl;
                    }

                    file.close();
                    cout << "-----------------------------------------------------------" << endl;
                    cout << "              Inventory saved to file✔✔✔" << endl;
                    cout << "-----------------------------------------------------------" << endl<< endl;
                    return;
                }
                else
                {
                    cout << "Error: Could not open file " << filename << endl;
                    return;
                }
            }

            // If the file doesn't exist, write the data to the file directly
            ofstream file;
            file.open(filename, ios::out | ios::app);

            if (file.is_open())
            {
                for (int i = 0; i < products.size(); i++)
                {
                    Product p = products[i];
                    file << p.getId() << "," << p.getName() << "," << p.getCategory() << "," << p.getPrice() << ","
                        << p.getQuantity() << endl;
                }

                file.close();
                cout << "-----------------------------------------------------------" << endl;
                cout << "                Inventory saved to file✔✔✔" << endl;
                cout << "-----------------------------------------------------------" << endl<< endl;
            }
            else
            {
                cout << "Error: Could not open file " << filename << endl;
            }
        }

        void loadInventoryFromFile(string filename)
        {
            if (!products.empty())
            {
                cout << "Warning: Inventory already contains data. Loading new data will overwrite existing data." << endl;
                cout << "Do you want to continue? (y/n): ";
                char choice;
                cin >> choice;
                if (choice != 'y' && choice != 'Y')
                {
                    cout << "Loading aborted." << endl;
                    return;
                }
                // Clear existing data if the user chooses to continue
                products.clear();
            }

            ifstream file;
            file.open(filename);

            if (file.is_open())
            {
                if (file.peek() == std::ifstream::traits_type::eof())
                {
                    // File is empty
                    cout << "-----------------------------------------------------------" << endl;
                    cout << "            Warning: Data set is empty in the file." << endl;
                    cout << "-----------------------------------------------------------" << endl<< endl;
                    file.close();
                    return;
                }

                string line;
                while (getline(file, line))
                {
                    stringstream ss(line);
                    string idStr, name, category, priceStr, quantityStr;
                    getline(ss, idStr, ',');
                    getline(ss, name, ',');
                    getline(ss, category, ',');
                    getline(ss, priceStr, ',');
                    getline(ss, quantityStr);

                    int id = stoi(idStr);
                    double price = stod(priceStr);
                    int quantity = stoi(quantityStr);

                    // Check if the product with the same ID already exists in the inventory
                    bool exists = false;
                    for (const auto &existingProduct : products)
                    {
                        if (existingProduct.getId() == id)
                        {
                            exists = true;
                            break;
                        }
                    }

                    if (!exists)
                    {
                        Product p(id, name, category, price, quantity);
                        products.push_back(p);
                    }
                }

                file.close();
                cout << "-----------------------------------------------------------" << endl;
                cout << "            Inventory loaded from file✔✔✔" << endl;
                cout << "-----------------------------------------------------------" << endl<< endl;
            }
            else
            {
                cout << "Error: Could not open file " << filename << endl;
            }
        }
};

int main() {
    Inventory inventory;
    cout << "-----------------------------------------------------------" <<endl;
    cout << "-----------------------------------------------------------" <<endl;
    cout << "---------------Inventory Management System ----------------" <<endl;
    cout << "-----------------------------------------------------------" <<endl;
    cout << "------------------------- Welcome! ------------------------" <<endl;
    cout << "-----------------------------------------------------------" <<endl;
    cout << "-----------------------------------------------------------" <<endl<< endl;
    cout<<"NOTE : If Already Data Exist In The Data Set Please Choose The Option (7) To Load The Existing Data!!"<<endl<<endl;
    char choice;
    do {
        cout<<"There are several option to choose..............."<<endl;
        cout << "1. Add a product" << endl;
        cout << "2. Remove a product" << endl;
        cout << "3. Find a product" << endl;
        cout << "4. Update a product" << endl;
        cout << "5. View all products" << endl;
        cout << "6. Save inventory to file" << endl;
        cout << "7. Load Inventory from file" << endl;
        cout << "Q. Quit" << endl;
        cout << "Please choose an option: ";
        cin >> choice;
        cout<<endl;
        switch (choice) {
        case '1': {
            int id;
            string name, category;
            double price;
            int quantity;
            cout << "Enter product id: ";
            if (!(cin >> id) || id < 0)
            {
                cout << "Invalid input for ID. Please enter a non-negative integer." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            cout << "Enter product name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter product category: ";
            //cin.ignore();
            getline(cin, category);
            cout << "Enter product price: ₹ ";
            if (!(cin >> price) || price < 0)
            {
                cout << "Invalid input for price. Please enter a non-negative number." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            cout << "Enter product quantity (in kg): ";
            if (!(cin >> quantity) || quantity < 0)
            {
                cout << "Invalid input for quantity. Please enter a non-negative integer." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            cout<< endl;
            Product product(id, name, category, price, quantity);
            inventory.addProduct(product);
            break;
        }

        case '2': {
            int id;
            cout << "Enter product id: ";
            cin >> id;
            inventory.removeProduct(id);
            break;
        }

        case '3': {
            int id;
            cout << "Enter product id: ";
            cin >> id;
            Product* product = inventory.findProduct(id);
            if (product) {
                cout << "-----------------------------------------------------------" <<endl;
                cout << "Name: " << product->getName() << endl;
                cout << "Category: " << product->getCategory() << endl;
                cout << "Price: ₹ " << product->getPrice() << endl;
                cout << "Quantity: " << product->getQuantity() << endl;
                cout << "-----------------------------------------------------------" <<endl<< endl;
            }
            else {
                cout << "-----------------------------------------------------------" <<endl;
                cout << "                   Product not found." << endl;
                cout << "-----------------------------------------------------------" <<endl<< endl;
            }
            break;
        }

        case '4': {
            int id;
            string name, category;
            double price;
            int quantity;
            cout << "Enter product id: ";
            if (!(cin >> id) || id < 0)
            {
                cout << "Invalid input for ID. Please enter a non-negative integer." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            cout << "Enter product name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter product category: ";
            //cin.ignore();
            getline(cin, category);
            cout << "Enter product price: ₹ ";
            if (!(cin >> price) || price < 0)
            {
                cout << "Invalid input for price. Please enter a non-negative number." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            cout << "Enter product quantity (in kg): ";
            if (!(cin >> quantity) || quantity < 0)
            {
                cout << "Invalid input for quantity. Please enter a non-negative integer." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            cout<< endl;
            inventory.updateProduct(id, name, category, price, quantity);
            cout << "-----------------------------------------------------------" <<endl;
            cout << "             Product updated successfully✔✔✔" << endl;
            cout << "-----------------------------------------------------------" <<endl<< endl;
            break;
        }

        case '5': {
            inventory.printProduct();
            break;
        }
        case '6': {
            inventory.saveInventoryToFile("inventory.csv");
            break;
        }
        case '7': {
            inventory.loadInventoryFromFile("inventory.csv");

            break;
        }
        case 'q':
        case 'Q':
            cout << "-----------------------------------------------------------" <<endl;
            cout << "                     Goodbye!!!" << endl;
            cout << "-----------------------------------------------------------" <<endl<< endl;
            return 0;

        default:
            cout << "-----------------------------------------------------------" <<endl;
            cout << "           Invalid Choice. Please Try again!!!" << endl;
            cout << "-----------------------------------------------------------" <<endl;
            break;
        }
    } while (true);

    return 0;
}
