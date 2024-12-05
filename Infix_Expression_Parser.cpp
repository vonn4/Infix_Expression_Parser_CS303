#include <iostream>
#include <stack>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <cctype>
#include <cmath>
#include <stdexcept>
#include <algorithm>

using namespace std;

class Evaluator {
    map<string, int> precedence;  // Operator precedences

    void initializePrecedence() {
        precedence["!"] = 8;
        precedence["++"] = 8;
        precedence["--"] = 8;
        precedence["-"] = 8;  // unary minus
        precedence["^"] = 7;
        precedence["*"] = 6;
        precedence["/"] = 6;
        precedence["%"] = 6;
        precedence["+"] = 5;
        precedence["-"] = 5;  // binary minus
        precedence[">"] = 4;
        precedence[">="] = 4;
        precedence["<"] = 4;
        precedence["<="] = 4;
        precedence["=="] = 3;
        precedence["!="] = 3;
        precedence["&&"] = 2;
        precedence["||"] = 1;
    }

    bool isOperator(const string& token) {
        return precedence.find(token) != precedence.end();
    }

    int applyOperator(const string& op, int a, int b) {
        if (op == "+") return a + b;
        if (op == "-") return a - b;
        if (op == "*") return a * b;
        if (op == "/") {
            if (b == 0) throw runtime_error("Division by zero");
            return a / b;
        }
        if (op == "%") return a % b;
        if (op == "^") return pow(a, b);
        if (op == ">") return a > b;
        if (op == ">=") return a >= b;
        if (op == "<") return a < b;
        if (op == "<=") return a <= b;
        if (op == "==") return a == b;
        if (op == "!=") return a != b;
        if (op == "&&") return a && b;
        if (op == "||") return a || b;
        throw runtime_error("Unknown operator: " + op);
    }

    vector<string> tokenize(const string& expression) {
        vector<string> tokens;
        string token;

        for (size_t i = 0; i < expression.size(); ++i) {
            char ch = expression[i];
            if (isspace(ch)) {
                continue;
            }
            else if (isdigit(ch)) {
                token += ch;
                while (i + 1 < expression.size() && isdigit(expression[i + 1])) {
                    token += expression[++i];
                }
                tokens.push_back(token);
                token.clear();
            }
            else {
                token += ch;
                if (i + 1 < expression.size()) {
                    char next = expression[i + 1];
                    if ((ch == '&' && next == '&') || (ch == '|' && next == '|') || (ch == '=' && next == '=') || (ch == '!' && next == '=')) {
                        token += next;
                        ++i;
                    }
                }
                tokens.push_back(token);
                token.clear();
            }
        }

        return tokens;
    }

    string toPostfix(const vector<string>& tokens) {
        stack<string> opStack;
        string postfix;

        for (const auto& token : tokens) {
            if (isdigit(token[0])) {
                postfix += token + " ";
            }
            else if (isOperator(token)) {
                while (!opStack.empty() && precedence[opStack.top()] >= precedence[token]) {
                    postfix += opStack.top() + " ";
                    opStack.pop();
                }
                opStack.push(token);
            }
            else if (token == "(") {
                opStack.push(token);
            }
            else if (token == ")") {
                while (!opStack.empty() && opStack.top() != "(") {
                    postfix += opStack.top() + " ";
                    opStack.pop();
                }
                if (opStack.empty()) throw runtime_error("Mismatched parentheses");
                opStack.pop();
            }
            else {
                throw runtime_error("Invalid token: " + token);
            }
        }

        while (!opStack.empty()) {
            if (opStack.top() == "(") throw runtime_error("Mismatched parentheses");
            postfix += opStack.top() + " ";
            opStack.pop();
        }

        return postfix;
    }

    int evaluatePostfix(const string& postfix) {
        stack<int> evalStack;
        istringstream iss(postfix);
        string token;

        while (iss >> token) {
            if (isdigit(token[0])) {
                evalStack.push(stoi(token));
            }
            else if (isOperator(token)) {
                if (evalStack.size() < 2) throw runtime_error("Invalid expression");
                int b = evalStack.top(); evalStack.pop();
                int a = evalStack.top(); evalStack.pop();
                evalStack.push(applyOperator(token, a, b));
            }
            else {
                throw runtime_error("Invalid token in postfix: " + token);
            }
        }

        if (evalStack.size() != 1) throw runtime_error("Invalid expression");
        return evalStack.top();
    }

    bool isLogicalOperator(const string& token) {
        return token == "&&" || token == "||" || token == ">" || token == "<" ||
            token == ">=" || token == "<=" || token == "==" || token == "!=";
    }

    bool containsLogicalOperators(const vector<string>& tokens) {
        for (const string& token : tokens) {
            if (isLogicalOperator(token)) {
                return true;
            }
        }
        return false;
    }

public:
    Evaluator() {
        initializePrecedence();
    }

    string eval(const string& expression) {
        auto tokens = tokenize(expression);
        auto postfix = toPostfix(tokens);
        int result = evaluatePostfix(postfix);

        // Check if the expression contains only logical operators
        if (containsLogicalOperators(tokens)) {
            return result ? "true" : "false";
        }

        // Otherwise, return numerical result as a string
        return to_string(result);
    }
};

void displayMenu() {

    cout << "Welcome to the Infix Expression Parser!\n";
    cout << "You can perform mathematical and logical evaluations.\n";
    cout << "Supported operations:\n";
    cout << "  +   Addition (e.g., 3 + 2 = 5)\n";
    cout << "  -   Subtraction (e.g., 5 - 3 = 2)\n";
    cout << "  *   Multiplication (e.g., 4 * 3 = 12)\n";
    cout << "  /   Division (e.g., 8 / 2 = 4)\n";
    cout << "  %   Modulus (e.g., 5 % 2 = 1)\n";
    cout << "  ^   Power (e.g., 2 ^ 3 = 8)\n";
    cout << "  >   Greater than (e.g., 5 > 3 = true)\n";
    cout << "  <   Less than (e.g., 3 < 5 = true)\n";
    cout << "  >=  Greater than or equal to (e.g., 5 >= 5 = true)\n";
    cout << "  <=  Less than or equal to (e.g., 4 <= 5 = true)\n";
    cout << "  ==  Equality (e.g., 5 == 5 = true)\n";
    cout << "  !=  Not equal (e.g., 5 != 3 = true)\n";
    cout << "  &&  Logical AND (e.g., 1 && 0 = false)\n";
    cout << "  ||  Logical OR (e.g., 1 || 0 = true)\n";
    cout << "  !   Logical NOT (e.g., !1 = false)\n";
    cout << "\nYou can use multiple operations in a single expression.\n";
    cout << "Example: (3 + 2) * 4 > 10 && 1 == 1 evaluates to true.\n";
    cout << "Type 'quit' to exit the program.\n";
}

int main() {
    Evaluator evaluator;
    string input;

    displayMenu();

    while (true) {
        cout << "\nEnter an expression (or 'quit' to exit): ";
        getline(cin, input);

        transform(input.begin(), input.end(), input.begin(), ::tolower);

        if (input == "quit") {
            cout << "Goodbye!\n";
            break;
        }

        else {
            try {
                string result = evaluator.eval(input);
                cout << "Result: " << result << endl;
            }
            catch (const exception& ex) {
                cerr << "Error: " << ex.what() << endl;
            }
        }
    }
    return 0;
}


