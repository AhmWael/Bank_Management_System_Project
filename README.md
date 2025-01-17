# Bank_Management_System_Project
By: Ahmad Wael, Asser Hanafy, Ahmad Zaki, Seif Ahmed and Abd Elrahman Agha

## Description
This **Bank Management System** is a C programming project developed as part of the final assignment for Alexandria University's CSE126 course. The project provides an interface for managing bank accounts, including features like account creation, modification, transaction management, and advanced search functionality.

## Features
- **User Authentication**:
  - Secure login system with credentials validated against a `users.txt` file.
  
- **Account Management**:
  - Add, delete (zero-balance accounts only), and modify accounts.
  - View account details by account number or search with keywords.
  
- **Transactions**:
  - Perform withdrawals, deposits, and transfers between accounts.
  - Automatically maintain transaction history in individual account logs.

- **Sorting & Reporting**:
  - Sort accounts by name, balance, or date opened.
  - Generate and print transaction history reports.

- **Data Persistence**:
  - Account data is stored in a `accounts.txt` file.
  - Updates are saved with user confirmation.

## Project Highlights
- Menu-driven interface for easy navigation.
- Comprehensive error handling and data validation.
- Modular functions for clear structure and maintainability.
- Individual transaction logs for each account.

## File Structure
- `users.txt`: Stores login credentials (username and password).
- `accounts.txt`: Contains account details in a CSV format:
- `{AccountNumber}.txt`: Logs transaction history for each account.

## How to Run
1. Clone the repository:
```bash
 git clone https://github.com/AhmWael/Bank_Management_System_Project.git
```
2. Navigate to the project directory
```bash
 cd bank-management-system
```
3. Compile the program
```bash
 gcc main.c -o bank_system
```
4. Run the program:
```bash
 ./bank_system
```

## Validation Rules
- Account Number: Must be a unique 10-digit numeric value.
- Email: Must be in a valid email format.
- Transaction Limits: Maximum $10,000 for withdrawals and deposits.

## Contributing
Contributions are welcome!
Feel free to contribute by submitting a pull request. For major changes, please open an issue first to discuss what you would like to change.
