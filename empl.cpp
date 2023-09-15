#include <crow.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;
using namespace crow;

struct Employee {
    string id;
    string name;
    string city;
    
};

map<string, Employee> employees;

int main() {
    SimpleApp app;


    CROW_ROUTE(app, "/employee/add")
        .methods("POST"_method)
        ([&](const request& req, response& res) {
            auto json = crow::json::load(req.body);
            if (!json) {
                return crow::response(400);
            }
            Employee employee;
            employee.id = json["id"].s();
            employee.name = json["name"].s();
            employee.city = json["city"].s();
            employees[employee.id] = employee;
            return crow::response(201);
        });

   
    CROW_ROUTE(app, "/employee/<string>")
        .methods("GET"_method)
        ([&](const request& req, response& res, const string& id) {
            auto it = employees.find(id);
            if (it != employees.end()) {
                return crow::response(200, crow::json::dump(it->second));
            } else {
                return crow::response(404);
            }
        });

    CROW_ROUTE(app, "/employees")
        .methods("GET"_method)
        ([&](const request& req, response& res) {
            vector<Employee> employeeList;
            for (const auto& pair : employees) {
                employeeList.push_back(pair.second);
            }
            return crow::response(200, crow::json::dump(employeeList));
        });

 
    app.port(8080).multithreaded().run();

    return 0;
}
