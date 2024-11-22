# Simple DBMS Console Application 📋

This project is a **console-based Database Management System (DBMS)** created in **C++** during the first semester. It provides basic database functionalities such as creating, deleting, and managing databases and tables.

---

## Features ✨

- **Database Management**: Create, use, drop, and list databases.
- **Table Management**: Create tables with columns, insert data, and delete rows.
- **Command Execution**: Execute queries with predefined commands.
- **Data Operations**: Show table data, update values, and delete rows.
- **Console Interface**: Interactive text-based interface.

---

## Tech Stack 🛠️

- **Language**: C++
- **Core Concepts**: Structures, Arrays, Loops, and Modular Programming.

---

## How It Works ⚙️

### Commands
This project supports a set of predefined commands to interact with the DBMS. Examples:
- `use <database>`: Switch to a specific database.
- `create <database>`: Create a new database.
- `createtable <table>`: Create a new table within the current database.
- `insertinto <table>`: Insert rows into a table.
- `showtables`: List all tables in the current database.
- `showtabledata <table>`: Display the data in a table.
- `updateinto <table>`: Update rows in a table.
- `deletefrom <table>`: Delete rows from a table.
- `dropdatabase <database>`: Delete a database.
- `exit`: Exit the program.

### Data Structures
- **`Database`**: Holds multiple tables and tracks database metadata.
- **`Table`**: Represents a table with columns, data types, and rows.

### Input Flow
1. User enters a command in the console.
2. The program parses the command and validates it.
3. Executes the requested operation if valid, else displays an error.
