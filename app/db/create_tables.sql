--
-- File generated with SQLiteStudio v3.2.1 on Sun Apr 21 18:47:31 2019
--
-- Text encoding used: System
--
PRAGMA foreign_keys = off;
BEGIN TRANSACTION;

-- Table: globalsettings
CREATE TABLE globalsettings (setting_key TEXT PRIMARY KEY UNIQUE, setting_value TEXT);

-- Table: stats
CREATE TABLE stats (id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE NOT NULL, stamp BIGINT NOT NULL, category TEXT NOT NULL, "action" STRING NOT NULL, used_cloudinary BOOLEAN, transformation_config INTEGER REFERENCES transformationconfigs (id) ON DELETE SET NULL);

-- Table: transformationconfigs
CREATE TABLE transformationconfigs (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, user_defined_name TEXT UNIQUE NOT NULL, save_locally BOOLEAN NOT NULL, overwrite_local BOOLEAN NOT NULL, created_file_suffix TEXT, uses_preset BOOLEAN NOT NULL DEFAULT (0), preset_name TEXT, uses_named_trans BOOLEAN, named_trans TEXT, uses_trans_string BOOLEAN NOT NULL DEFAULT (0), trans_string TEXT, uses_outgoing_trans BOOLEAN NOT NULL, outgoing_trans_string TEXT, store_original BOOLEAN NOT NULL DEFAULT (0), delete_cloud_after_download BOOLEAN NOT NULL DEFAULT (0));

COMMIT TRANSACTION;
PRAGMA foreign_keys = on;
