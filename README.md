# Football Club Management System

A desktop application for managing football club data, built with C/C++ using the Qt framework and an SQLite database backend. The app provides a GUI for adding, editing and removing players and staff, creating match fixtures, maintaining a transfer list, selecting starting XIs, simulating upcoming matches and viewing team statistics.

Technologies: C++, Qt (Qt Widgets), QSql (SQLite), SQLite (DB Browser for SQLite for manual DB edits)

## Key features
- Manage players and staff: add, edit, remove records stored in an SQLite database.
- Create and edit match fixtures and transfer lists.
- Choose starting XIs for both your team and opponents.
- Simulate matches and generate predicted scores using player statistics.
- View team statistics and simple reports driven by SQL queries.
- Automated unit tests (Qt Test) located in the `Tests/` folder.

## Project layout (important files/directories)
- `Football_Club_Management_System.pro` — Qt project file (root project).
- `main.cpp` — application entry point.
- `*.cpp`, `*.h`, `*.ui` — application source, widgets and UI forms.
- `Tests/` — unit tests using Qt Test framework.
- `images/` — GUI assets and screenshots used by the app.

## Database details
- The application uses Qt's `QSQLITE` driver (connection name used in code: `DB1`).
- Tests (see `Tests/maintests.cpp`) add a `QSQLITE` database and reference a database file by path; by default the test helper points to a development path (for example a sample path in the repository developer's environment). Update that path or create a database in the working directory before running tests.
- To inspect or modify the database manually, use DB Browser for SQLite and open the DB file used by the app/tests.

## Building and running (recommended: Qt Creator)
1. Install Qt (Qt 6 recommended) and a matching compiler (MinGW or MSVC). Ensure the Qt `qmake`/`cmake` tools are available in your PATH or use Qt Creator.
2. Open `Football_Club_Management_System.pro` with Qt Creator.
3. Configure a build kit (e.g., Desktop Qt 6.x MinGW) and build the project.
4. Run the application from Qt Creator. Make sure the working directory or the database path points to the SQLite DB file you intend to use.

### Command-line (example with qmake & MinGW on Windows)
```
"C:\Path\To\Qt\6.x\mingw\bin\qmake.exe" Football_Club_Management_System.pro
mingw32-make
./release/Football_Club_Management_System.exe
```
Replace the qmake path with your Qt installation path. Alternatively configure a CMake-based workflow if you prefer.

## Running tests
- Open the `Tests/Tests.pro` project in Qt Creator and run it. Tests depend on the `DB1` SQLite database; update `Tests/maintests.cpp` to point to your local DB file or create a DB at the referenced path.
- From the command line you can build the `Tests` project the same way as the main project and run the produced test executable.

## Usage notes and tips
- The codebase frequently uses the named database connection `DB1` (search for `QSqlDatabase::database("DB1")` to see usages). Ensure the connection is added at app startup before widgets expect it.
- If you get database connection errors, confirm the DB file exists and that the app's working directory or the hard-coded path in tests points to it.
- `images/` contains UI assets referenced by some forms; if you move the project, update absolute image paths in the code (some files contain absolute paths used during development).

## Testing and data
- The project includes a set of Qt Test cases in `Tests/` that exercise player/staff add/edit/remove functionality and match simulation. These tests create/expect records in the SQLite DB.
- The original project included ~100 player/staff records managed in a DB during development; your copy may not include that DB — create or import one as needed using DB Browser for SQLite.

## Contributing
- To contribute: fork the repository, create a branch, implement changes and open a pull request. If adding features that require DB schema changes, include a migration/export file or SQL script to create the required tables.

## Contact
- For questions about this codebase, inspect the `Tests/` and `maintests.cpp` to see how the database connection is configured, and open an issue or contact the project maintainer via the GitHub repo.

Enjoy exploring and extending the Football Club Management System!
