#include <crails/odb/connection.hpp>

void install_plugin_database()
{
  Crails::Odb::Connection database;

  database.transaction.require("odb");
  database.execute("CREATE TABLE \"festival_Artist\" ("
    "\"id\" BIGSERIAL NOT NULL PRIMARY KEY,"
    "\"deleted\" BOOLEAN NOT NULL DEFAULT FALSE,"
    "\"created_at\" BIGINT NULL,"
    "\"updated_at\" BIGINT NULL,"
    "\"title\" TEXT NOT NULL,"
    "\"slug\" TEXT NOT NULL,"
    "\"body\" TEXT NOT NULL,"
    "\"description\" TEXT NOT NULL,"
    "\"thumbnail\" TEXT NOT NULL);");
  database.execute("CREATE TABLE \"festival_Event\" ("
    "\"id\" BIGSERIAL NOT NULL PRIMARY KEY,"
    "\"deleted\" BOOLEAN NOT NULL DEFAULT FALSE,"
    "\"created_at\" BIGINT NULL,"
    "\"updated_at\" BIGINT NULL,"
    "\"title\" TEXT NOT NULL,"
    "\"slug\" TEXT NOT NULL,"
    "\"body\" TEXT NOT NULL,"
    "\"description\" TEXT NOT NULL,"
    "\"thumbnail\" TEXT NOT NULL,"
    "\"programme\" TEXT NOT NULL,"
    "\"date_json\" TEXT NOT NULL,"
    "\"order\" INTEGER NOT NULL);");
  database.execute("CREATE TABLE \"festival_Festival\" ("
    "\"id\" BIGSERIAL NOT NULL PRIMARY KEY,"
    "\"deleted\" BOOLEAN NOT NULL DEFAULT FALSE,"
    "\"created_at\" BIGINT NULL,"
    "\"updated_at\" BIGINT NULL,"
    "\"title\" TEXT NOT NULL,"
    "\"slug\" TEXT NOT NULL,"
    "\"body\" TEXT NOT NULL,"
    "\"description\" TEXT NOT NULL,"
    "\"thumbnail\" TEXT NOT NULL,"
    "\"start_date\" BIGINT NOT NULL,"
    "\"end_date\" BIGINT NOT NULL);");
  database.execute("CREATE TABLE \"festival_Festival_events\" ("
    "\"object_id\" BIGINT NOT NULL,"
    "\"index\" BIGINT NOT NULL,"
    "\"value\" BIGINT NULL,"
    "CONSTRAINT \"object_id_fk\""
    " FOREIGN KEY (\"object_id\")"
    " REFERENCES \"festival_Festival\" (\"id\")"
    " ON DELETE CASCADE);");
  database.execute("CREATE INDEX \"festival_Festival_events_object_id_i\""
    " ON \"festival_Festival_events\" (\"object_id\");");
  database.execute("CREATE INDEX \"festival_Festival_events_index_i\""
    " ON \"festival_Festival_events\" (\"index\");");
  database.execute("ALTER TABLE \"festival_Festival_events\""
    " ADD CONSTRAINT \"value_fk\""
    " FOREIGN KEY (\"value\")"
    " REFERENCES \"festival_Event\" (\"id\")"
    " INITIALLY DEFERRED;");
  database.commit();
}

void uninstall_plugin_database()
{
  Crails::Odb::Connection database;

  database.transaction.require("odb");
  database.execute("DROP TABLE IF EXISTS \"festival_Artist\" CASCADE;");
  database.execute("DROP TABLE IF EXISTS \"festival_Event\" CASCADE;");
  database.execute("DROP TABLE IF EXISTS \"festival_Festival_events\" CASCADE;");
  database.execute("DROP TABLE IF EXISTS \"festival_Festival\" CASCADE;");
  database.commit();
}
