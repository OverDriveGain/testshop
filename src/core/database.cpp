#include "database.hpp"


neroshop::db::Sqlite3::Sqlite3() : handle(nullptr), opened(false) {}
////////////////////
neroshop::db::Sqlite3::Sqlite3(const std::string& filename) : Sqlite3()
{
	if(!open(filename)) {
		throw std::runtime_error(std::string("sqlite3_open: ") + std::string(sqlite3_errmsg(handle)));
    }
}
////////////////////
neroshop::db::Sqlite3::~Sqlite3() {
    close();
}
////////////////////
std::unique_ptr<neroshop::db::Sqlite3> neroshop::db::Sqlite3::singleton (nullptr);//(std::make_unique<neroshop::db::Sqlite3>());
////////////////////
// SQLite database should only need to be opened once per application session and closed once when the application is terminated
bool neroshop::db::Sqlite3::open(const std::string& filename)
{
    if(opened) {
        neroshop::print("database is already opened", 2);
        return true;
    }
	if(sqlite3_open(filename.c_str(), &handle) != SQLITE_OK) {
		close();
		return false;
	}
	// Enable Write-Ahead Log. This will prevent the database from being locked
	if(get_text("PRAGMA journal_mode;") != "wal") {
	    execute("PRAGMA journal_mode = WAL;"); // requires version 3.7.0 (2010-07-21)
	}
	// Enable Foreign keys
	if(get_integer("PRAGMA foreign_keys;") != 1) {
	    execute("PRAGMA foreign_keys = ON;"); // requires version 3.6.19 (2009-10-14)
	}
	// Initialize the neroshop database
	//initialize();
	opened = true;
	return true;
}
////////////////////
void neroshop::db::Sqlite3::close() {
    if(!handle) {
        return;
	}
	sqlite3_close(handle);
	handle = nullptr;
    opened = false;
    neroshop::print("database is now closed");
}
////////////////////
void neroshop::db::Sqlite3::execute(const std::string& command) 
{
    if(!handle) throw std::runtime_error("database is not connected");
	char * error_message = 0;
	int result = sqlite3_exec(handle, command.c_str(), neroshop::db::Sqlite3::callback, 0, &error_message);
	if (result != SQLITE_OK) {
		neroshop::print("sqlite3_exec: " + std::string(error_message), 1);
		sqlite3_free(error_message);
	}
}
////////////////////
void neroshop::db::Sqlite3::execute_params(const std::string& command, const std::vector<std::string>& args) {
    if(!handle) throw std::runtime_error("database is not connected");
    // Prepare statement
    sqlite3_stmt * statement = nullptr;
    int result = sqlite3_prepare_v2(handle, command.c_str(), -1, &statement, nullptr);
    if(result != SQLITE_OK) {
        neroshop::print("sqlite3_prepare_v2: " + std::string(sqlite3_errmsg(handle)), 1);
        // Since we don't prepare a statement here, there is no need to finalise it
        return;
    }
    // Bind user-defined parameter arguments
    for(int i = 0; i < args.size()/*sqlite3_bind_parameter_count(statement)*/; i++) {
        result = sqlite3_bind_text(statement, i + 1, args[i].c_str(), args[i].length(), SQLITE_STATIC);
        if(result != SQLITE_OK) {
            neroshop::print("sqlite3_bind_*: " + std::string(sqlite3_errmsg(handle)), 1);
            sqlite3_finalize(statement);
            return;
        }
    }
    // Evaluate the statement
    result = sqlite3_step(statement);
    if(result != SQLITE_DONE) {
        neroshop::print("sqlite3_step: " + std::string(sqlite3_errmsg(handle)), 1);
        sqlite3_finalize(statement);
        return;
    }    
    // Finalize (destroy) the prepared statement
    result = sqlite3_finalize(statement);
    if(result != SQLITE_OK) {
        neroshop::print("sqlite3_finalize: " + std::string(sqlite3_errmsg(handle)), 1);
        sqlite3_finalize(statement);
        return;
    }        
    // We are not returning anything, just setting executing queries
}
////////////////////
////////////////////
////////////////////
std::string neroshop::db::Sqlite3::get_sqlite_version() {
    return sqlite3_libversion();
}
////////////////////
sqlite3 * neroshop::db::Sqlite3::get_handle() const {
    return handle;
}
////////////////////
neroshop::db::Sqlite3 * neroshop::db::Sqlite3::get_singleton() {
    if(!singleton.get()) {
        singleton = std::make_unique<neroshop::db::Sqlite3>();
    }
    return singleton.get();
}
////////////////////
void * neroshop::db::Sqlite3::get_blob(const std::string& command) {
    if(!handle) throw std::runtime_error("database is not connected");
    sqlite3_stmt * statement = nullptr;
    int result = sqlite3_prepare_v2(handle, command.c_str(), -1, &statement, nullptr);
    if(result != SQLITE_OK) {
        neroshop::print("sqlite3_prepare_v2: " + std::string(sqlite3_errmsg(handle)), 1);
        return nullptr;
    }
    result = sqlite3_step(statement);
    if (result != SQLITE_ROW) {
        neroshop::print("sqlite3_step: " + std::string(sqlite3_errmsg(handle)), 1);
        sqlite3_finalize(statement);
        return nullptr;
    }
    int column_type = sqlite3_column_type(statement, 0);
    if(column_type != SQLITE_BLOB) {
        neroshop::print("sqlite3_column_type: invalid column type", 1);
        sqlite3_finalize(statement);
        return nullptr;
    }
    void * blob = const_cast<void *>(sqlite3_column_blob(statement, 0));//reinterpret_cast<const char *>(sqlite3_column_text16(stmt, 0)); // utf-16
    sqlite3_finalize(statement);
    return blob;
}
////////////////////
void * neroshop::db::Sqlite3::get_blob_params(const std::string& command, const std::vector<std::string>& args) {
	if(!handle) throw std::runtime_error("database is not connected");
    sqlite3_stmt * statement = nullptr;
    int result = sqlite3_prepare_v2(handle, command.c_str(), -1, &statement, nullptr);
    if(result != SQLITE_OK) {
        neroshop::print("sqlite3_prepare_v2: " + std::string(sqlite3_errmsg(handle)), 1);
        return nullptr;
    }
    // Bind user-defined parameter arguments
    for(int i = 0; i < args.size()/*sqlite3_bind_parameter_count(statement)*/; i++) {
        result = sqlite3_bind_text(statement, i + 1, args[i].c_str(), args[i].length(), SQLITE_STATIC);
        if(result != SQLITE_OK) {
            neroshop::print("sqlite3_bind_*: " + std::string(sqlite3_errmsg(handle)), 1);
            sqlite3_finalize(statement);
            return nullptr;
        }
    }    
    sqlite3_step(statement); // Don't check for error or it'll keep saying: "another row available" or "no more rows available"
    int column_type = sqlite3_column_type(statement, 0);
    if(column_type != SQLITE_BLOB) {
        neroshop::print("sqlite3_column_type: invalid column type", 1);
        sqlite3_finalize(statement);
        return nullptr;
    }
    void * blob = const_cast<void *>(sqlite3_column_blob(statement, 0));//reinterpret_cast<const char *>(sqlite3_column_text16(stmt, 0)); // utf-16
    sqlite3_finalize(statement);
    return blob;
}
////////////////////
std::string neroshop::db::Sqlite3::get_text(const std::string& command) {//const {
    if(!handle) throw std::runtime_error("database is not connected");
    sqlite3_stmt * stmt = nullptr;
    int result = sqlite3_prepare_v2(handle, command.c_str(), -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        neroshop::print("sqlite3_prepare_v2: " + std::string(sqlite3_errmsg(handle)), 1);
        return "";
    }
    result = sqlite3_step(stmt);
    if (result != SQLITE_ROW) {
        neroshop::print("sqlite3_step: " + std::string(sqlite3_errmsg(handle)), 1);
        sqlite3_finalize(stmt);
        return "";
    }
    int column_type = sqlite3_column_type(stmt, 0);
    if(column_type != SQLITE_TEXT) {
        neroshop::print("sqlite3_column_type: invalid column type", 1);
        sqlite3_finalize(stmt);
        return "";
    }
    std::string text = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));//reinterpret_cast<const char *>(sqlite3_column_text16(stmt, 0)); // utf-16
    sqlite3_finalize(stmt);
    return text;
}
////////////////////
std::string neroshop::db::Sqlite3::get_text_params(const std::string& command, const std::vector<std::string>& args) {//const {
    if(!handle) throw std::runtime_error("database is not connected");
    // Prepare statement
    sqlite3_stmt * statement = nullptr;
    int result = sqlite3_prepare_v2(handle, command.c_str(), -1, &statement, nullptr);
    if(result != SQLITE_OK) {
        neroshop::print("sqlite3_prepare_v2: " + std::string(sqlite3_errmsg(handle)), 1);
        return "";
    }
    // Bind user-defined parameter arguments
    for(int i = 0; i < args.size()/*sqlite3_bind_parameter_count(statement)*/; i++) {
        result = sqlite3_bind_text(statement, i + 1, args[i].c_str(), args[i].length(), SQLITE_STATIC);
        if(result != SQLITE_OK) {
            neroshop::print("sqlite3_bind_*: " + std::string(sqlite3_errmsg(handle)), 1);
            sqlite3_finalize(statement);
            return "";
        }
    }
    // Evaluate statement
    sqlite3_step(statement); // Don't check for error or it'll keep saying: "another row available" or "no more rows available"
    // Check the type of the statement's return value
    int column_type = sqlite3_column_type(statement, 0);
    if(column_type != SQLITE_TEXT) {
        neroshop::print("sqlite3_column_type: invalid column type", 1);
        sqlite3_finalize(statement);
        return "";
    }
    // Finalize (destroy) the prepared statement
    std::string text = reinterpret_cast<const char *>(sqlite3_column_text(statement, 0));//reinterpret_cast<const char *>(sqlite3_column_text16(stmt, 0)); // utf-16
    sqlite3_finalize(statement);
    return text;
}
////////////////////
int neroshop::db::Sqlite3::get_integer(const std::string& command) {
    if(!handle) throw std::runtime_error("database is not connected");
    sqlite3_stmt * statement = nullptr;
    int result = sqlite3_prepare_v2(handle, command.c_str(), -1, &statement, nullptr);
    if(result != SQLITE_OK) {
        neroshop::print("sqlite3_prepare_v2: " + std::string(sqlite3_errmsg(handle)), 1);
        return 0;
    }
    result = sqlite3_step(statement);
    if (result != SQLITE_ROW) {
        neroshop::print("sqlite3_step: " + std::string(sqlite3_errmsg(handle)), 1);
        sqlite3_finalize(statement);
        return 0;
    }
    int column_type = sqlite3_column_type(statement, 0);
    if(column_type != SQLITE_INTEGER) {
        neroshop::print("sqlite3_column_type: invalid column type", 1);
        sqlite3_finalize(statement);
        return 0;
    }
    int number = sqlite3_column_int64(statement, 0);//sqlite3_column_int(statement, 0);
    sqlite3_finalize(statement);
    return number;
}
////////////////////
int neroshop::db::Sqlite3::get_integer_params(const std::string& command, const std::vector<std::string>& args) {
    if(!handle) throw std::runtime_error("database is not connected");
    // Prepare statement
    sqlite3_stmt * statement = nullptr;
    int result = sqlite3_prepare_v2(handle, command.c_str(), -1, &statement, nullptr);
    if(result != SQLITE_OK) {
        neroshop::print("sqlite3_prepare_v2: " + std::string(sqlite3_errmsg(handle)), 1);
        return 0;
    }
    // Bind user-defined parameter arguments
    for(int i = 0; i < args.size()/*sqlite3_bind_parameter_count(statement)*/; i++) {
        result = sqlite3_bind_text(statement, i + 1, args[i].c_str(), args[i].length(), SQLITE_STATIC);
        if(result != SQLITE_OK) {
            neroshop::print("sqlite3_bind_*: " + std::string(sqlite3_errmsg(handle)), 1);
            sqlite3_finalize(statement);
            return 0;
        }
    }
    // Evaluate statement
    sqlite3_step(statement); // Don't check for error or it'll keep saying: "another row available" or "no more rows available"
    // Check the type of the statement's return value
    int column_type = sqlite3_column_type(statement, 0);
    if(column_type != SQLITE_INTEGER) {
        neroshop::print("sqlite3_column_type: invalid column type", 2);
        sqlite3_finalize(statement);
        return 0;
    }
    // Finalize (destroy) the prepared statement
    int number = sqlite3_column_int64(statement, 0);//sqlite3_column_int(statement, 0);
    sqlite3_finalize(statement);
    return number;
}
////////////////////
double neroshop::db::Sqlite3::get_real(const std::string& command) {
    if(!handle) throw std::runtime_error("database is not connected");
    sqlite3_stmt * stmt = nullptr;
    int result = sqlite3_prepare_v2(handle, command.c_str(), -1, &stmt, nullptr);
    if(result != SQLITE_OK) {
        neroshop::print("sqlite3_prepare_v2: " + std::string(sqlite3_errmsg(handle)), 1);
        return 0.0;
    }
    result = sqlite3_step(stmt);
    if (result != SQLITE_ROW) {
        neroshop::print("sqlite3_step: " + std::string(sqlite3_errmsg(handle)), 1);
        sqlite3_finalize(stmt);
        return 0.0;
    }
    int column_type = sqlite3_column_type(stmt, 0);
    if(column_type != SQLITE_FLOAT) {
        neroshop::print("sqlite3_column_type: invalid column type", 1);
        sqlite3_finalize(stmt);
        return 0.0;
    }
    double number = sqlite3_column_double(stmt, 0);
    sqlite3_finalize(stmt);
    return number;
}
////////////////////
double neroshop::db::Sqlite3::get_real_params(const std::string& command, const std::vector<std::string>& args) {
    if(!handle) throw std::runtime_error("database is not connected");
    // Prepare statement
    sqlite3_stmt * statement = nullptr;
    int result = sqlite3_prepare_v2(handle, command.c_str(), -1, &statement, nullptr);
    if(result != SQLITE_OK) {
        neroshop::print("sqlite3_prepare_v2: " + std::string(sqlite3_errmsg(handle)), 1);
        return 0;
    }
    // Bind user-defined parameter arguments
    for(int i = 0; i < args.size()/*sqlite3_bind_parameter_count(statement)*/; i++) {
        result = sqlite3_bind_text(statement, i + 1, args[i].c_str(), args[i].length(), SQLITE_STATIC);
        if(result != SQLITE_OK) {
            neroshop::print("sqlite3_bind_*: " + std::string(sqlite3_errmsg(handle)), 1);
            sqlite3_finalize(statement);
            return 0;
        }
    }
    // Evaluate statement
    sqlite3_step(statement); // Don't check for error or it'll keep saying: "another row available" or "no more rows available"
    // Check the type of the statement's return value
    int column_type = sqlite3_column_type(statement, 0);
    if(column_type != SQLITE_FLOAT) {
        neroshop::print("sqlite3_column_type: invalid column type", 2);
        sqlite3_finalize(statement);
        return 0;
    }
    // Finalize (destroy) the prepared statement
    double number = sqlite3_column_double(statement, 0);
    sqlite3_finalize(statement);
    return number;
}
////////////////////
////////////////////
////////////////////
////////////////////
bool neroshop::db::Sqlite3::is_open() const {
    return (opened == true);
}
////////////////////
bool neroshop::db::Sqlite3::table_exists(const std::string& table_name) {
    std::string command = "SELECT count(*) FROM sqlite_master WHERE type = 'table' AND name = $1;";
    return get_integer_params(command, { table_name });
}
////////////////////
/*bool neroshop::db::Sqlite3::rowid_exists(const std::string& table_name, int rowid) {
     int rowid = database->get_integer_params("SELECT id FROM $1 WHERE id = $2", { table_name, rowid });
     return (rowid != 0);
}*/
////////////////////
////////////////////
////////////////////
int neroshop::db::Sqlite3::callback(void *not_used, int argc, char **argv, char **az_col_name)
{
    int i;
    for(i = 0; i < argc; i++) {
	    std::cout << SQLITE3_TAG << az_col_name[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;  // printf("%s = %s\n", azcolname[i], argv[i] ? argv[i] : "nullptr");
    }
    std::cout << std::endl;
    return 0;	
}
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
////////////////////
