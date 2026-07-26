#include "CommandParser.h"
#include "CacheSerializer.h"
#include <iostream>
#include <iomanip>

CommandParser::CommandParser(Cache &cache) :cache(cache) {}

CommandResponse CommandParser::process(const std::string &line) {
    std::stringstream ss(line);
    std::ostringstream out;
    std::string command;
    if(!(ss >> command))
        return {};
    if (command == "SET")
        return handleSet(ss);
    else if (command == "GET")
        return handleGet(ss);
    else if (command == "EXISTS")
        return handleExists(ss);
    else if (command == "DELETE")
        return handleDelete(ss);
    else if (command == "RESIZE")
        return handleResize(ss);
    else if (command == "EXPIRE")
        return handleExpire(ss);
    else if (command == "PERSIST")
        return handlePersist(ss);
    else if (command == "TTL")
        return handleTTL(ss);
    else if (command == "SAVE")
        return handleSave(ss);
    else if (command == "LOAD")
        return handleLoad(ss);
    else if (command == "CLEAR")
        return handleClear(ss);
    else if (command == "INFO") {
        cache.cleanupExpired();
        size_t entries = cache.getSize();
        size_t cap = cache.getCapacity();
        double usage = (static_cast<double> (entries)/cap)*100;
        out <<  "\n\tENTRIES: " << entries << "\n\tCAPACITY: " << cap << "\n\tUSAGE: " << std::fixed << std::setprecision(1) << usage << "%\n" <<  "\n";
        out << std::defaultfloat;
        return {out.str()};
    }
    else if (command == "HELP")
        return {std::string(HELP_MESSAGE)};
    else if (command == "EXIT")
        return {"", true};
    else 
        out <<  "\t\"" << command << "\" is not a recognised command.\n\tType HELP to view command usage." <<  "\n";
    return {out.str()};
}

CommandResponse CommandParser::handleSet(std::stringstream &ss) {
    std:: string key, value, command, extra;
    std::ostringstream out;
    int ttl;
    if ((!(ss >> key >> value))) {
        out <<  "\tUsage: SET <key> <value> [TTL <seconds>]" <<  "\n";
        return {out.str()};
    }
    if (!(ss >> command)) {
        cache.set(key, value);
    }
    else {
        if (command != "TTL") {
            out <<   "\tUsage: SET <key> <value> [TTL <seconds>]" <<  "\n";
            return {out.str()};
        }
        if (!(ss >> ttl) || ttl<=0 || (ss >> extra)) {
            out <<  "\tUsage: SET <key> <value> [TTL <seconds>]" <<  "\n";
            return {out.str()};
        }
        cache.set(key, value, ttl);
    }
    out <<  "\tStored successfully." <<  "\n";
    return {out.str()};
}

CommandResponse CommandParser::handleGet(std::stringstream &ss) {
    std::string key, extra;
    std::ostringstream out;
    if ((!(ss >> key)) || (ss >> extra)) {
        out <<  "\tUsage: GET <key>" <<  "\n";
        return {out.str()};
    }
    auto s = cache.get(key);
    if (s.has_value())
        out <<  "\t" << s.value() << "\n";
    else
        out <<  "\tKEY: \""<< key << "\" not found" <<  "\n";
    return {out.str()};
}

CommandResponse CommandParser::handleExists(std::stringstream &ss) {
    std::string key, extra;
    std::ostringstream out;
    if ((!(ss >> key)) || (ss >> extra)) {
        out <<  "\tUsage: EXISTS <key>" <<  "\n";
        return {out.str()};
    }
    if (cache.exists(key))
        out <<  "\tKEY: \"" << key << "\" exists" <<  "\n";
    else
        out <<  "\tKEY: \"" << key << "\" does not exist" <<  "\n";
    return {out.str()};
}

CommandResponse CommandParser::handleDelete(std::stringstream &ss) {
    std::string key, extra;
    std::ostringstream out;
    if ((!(ss >> key)) || (ss >> extra)) {
        out <<  "\tUsage: DELETE <key>" <<  "\n";
        return {out.str()};
    }
    if(cache.eraseKey(key))
        out <<  "\tDeleted successfully." <<  "\n";
    else
        out <<  "\tKEY: \"" << key << "\" not found." <<  "\n";
    return {out.str()};
}

CommandResponse CommandParser::handleResize(std::stringstream &ss) {
    long long newCapacity;
    std::string extra;
    std::ostringstream out;
    if ((!(ss >> newCapacity)) || (ss >> extra) ) {
        out <<  "\tUsage: RESIZE <integer_value>" <<  "\n";
        return {out.str()};
    }
    if (newCapacity <= 0) {
        out <<  "\tINVALID: Capacity should be atleast 1" <<  "\n";
        return {out.str()};
    }
    int changed = cache.resize(static_cast<size_t>(newCapacity));
    out <<  "\tCapacity changed to " << newCapacity << ".\n\tEvicted " << changed << " entries." <<  "\n";
    return {out.str()};
}

CommandResponse CommandParser::handleExpire(std::stringstream &ss) {
    std::string key, extra;
    std::ostringstream out;
    int ttl;
    if ((!(ss >> key >> ttl)) || ttl<=0 || (ss >> extra)) {
        out <<  "\tUsage: EXPIRE <key> <seconds>" <<  "\n";
        return {out.str()};
    }
    if(cache.setExpiry(key, ttl))
        out <<  "\tKEY: \"" << key << "\" set to expire in " << ttl << " seconds." <<  "\n";
    else
        out <<  "\tKEY: \"" << key << "\" not found." <<  "\n";
    return {out.str()};
}

CommandResponse CommandParser::handlePersist(std::stringstream &ss) {
    std::string key, extra;
    std::ostringstream out;
    if ((!(ss >> key)) || (ss >> extra)) {
        out <<  "\tUsage: PERSIST <key>" <<  "\n";
        return {out.str()};
    }
    if(cache.persist(key))
        out <<  "\tKEY: \"" << key << "\" expiry removed." <<  "\n";
    else
        out <<  "\tKEY: \"" << key << "\" not found." <<  "\n";
    return {out.str()};
}

CommandResponse CommandParser::handleTTL(std::stringstream &ss) {
    std::string key, extra;
    std::ostringstream out;
    if ((!(ss >> key)) || (ss >> extra)) {
        out <<  "\tUsage: TTL <key>" <<  "\n";
        return {out.str()};
    }
    int ttl = cache.getTTL(key);
    if (ttl == -2)
        out <<  "\tKEY: \"" << key << "\" not found." <<  "\n";
    else if (ttl == -1)
        out <<  "\tKEY: \"" << key << "\" has no expiry." <<  "\n";
    else
        out <<  "\tKEY: \"" << key << "\" expires in " << ttl << " seconds." <<  "\n";
    return {out.str()};
}

CommandResponse CommandParser::handleSave(std::stringstream &ss) {
    std::string filename, extra;
    std::ostringstream out;
    if ((!(ss >> filename)) || (ss >> extra)) {
        out <<  "\tUsage: SAVE <filename>" <<  "\n";
        return {out.str()};
    }
    if (CacheSerializer::save(cache, filename)) 
        out <<  "\tCache stored to ../data/" << filename <<  "\n";
    else 
        out <<  "\tERROR: Could not open " << filename << "." <<  "\n";
    return {out.str()};
}

CommandResponse CommandParser::handleLoad(std::stringstream &ss) {
    std::string filename, extra;
    std::ostringstream out;
    if ((!(ss >> filename)) || (ss >> extra)) {
        out <<  "\tUsage: LOAD <filename>" <<  "\n";
        return {out.str()};
    }
    if (CacheSerializer::load(cache, filename)) 
        out <<  "\tCache loaded from ../data/" << filename <<  "\n";
    else 
        out <<  "\tERROR: Could not open " << filename << "." <<  "\n";
    return {out.str()};
}

CommandResponse CommandParser::handleClear(std::stringstream &ss) {
    std::string extra;
    std::ostringstream out;
    if (ss >> extra) {
        out <<  "\tUsage: CLEAR" <<  "\n";
        return {out.str()};
    }
    cache.clear();
    out <<  "\tCache cleared." <<  "\n";
    return {out.str()};
}