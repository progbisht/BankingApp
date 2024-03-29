# BankingApp (Developed in C++ and Later in Node,Express and Handlebars)

## C++ Version
The banking application emulates the banking system and the services provided by the banks. Just like an online banking system which provides various functionalities
such as deposits, withdrawls, transfers, updating profiles, and deleting accounts. This is a minor application (console-based application) developed in C++ programming language
and Object Oriented Programming concepts to learn the implementation of programming concepts in real-world situations.

### Complete source code is on the master branch

# Setup
1. gcc compiler
2. vsCode/Code Blocks (modern IDE)
3. C++ v.11 or later
Necessary Header Files - iostream, iomanip, fstream, string, vector, algorithm, utility, sstream, ctime, cmath, regex

# How to run
Click on the BankingApp.exe and the application will start.

# NodeJS Version 
This application is later upgraded with Node and Express at the backend and handlebars (template engine) are used at the front end. The functionalities are similar to the console
based application but this one is built using proper authentication requiring an access token to access the data.

### Complete source code is on the main branch.

# Setup
1. npm
2. vsCode/modern IDE
Necessary Header Files - can be downloaded using npm install

# How to run
Nodejs code type node index to launch the server.


# Working
- The application starts with a master menu that provides user to either create a 'New Account' or 'Login'.
- A user can register himself by selecting 'New Account' or existing user can select 'Login'. 
- On selecting 'New Account' a form will appear to register a new user. 
- After entering the details and submitting it user have to make minimum deposit. 
- And as soon as the account creation process finishes can login to his/her account.
- Whenever a user logs in a menu appears that contains the facilities provided by the BankingApp.

-> For more details please check 'working images' folder to understand the working.

Note : 
Input only allowed values.
Avoid using whitespaces
