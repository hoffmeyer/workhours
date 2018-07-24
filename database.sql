CREATE EXTENSION IF NOT EXISTS "uuid-ossp";
CREATE TABLE IF NOT EXISTS work (id uuid PRIMARY KEY NOT NULL, start timestamp NOT NULL, duration real, break real );
CREATE TABLE IF NOT EXISTS dbVersion (version integer PRIMARY KEY NOT NULL);

DO
$do$
BEGIN
IF NOT EXISTS (SELECT 1 FROM dbVersion) THEN
  
  INSERT INTO dbVersion(version) VALUES(1);
  ALTER TABLE work ADD lunch real;
  UPDATE work set lunch=0.5 where duration>=4;
  UPDATE work set lunch=0 where duration <4;

END IF;

IF EXISTS (SELECT 1 FROM dbVersion WHERE version=1) THEN

  CREATE TABLE users (id uuid PRIMARY KEY NOT NULL, name text, username text, password text, workHoursPerWeek real );
  INSERT INTO users(id, username, password, name, workhoursPerWeek) VALUES(uuid_generate_v4(), 'flemming', 'JhFoefi82', 'Flemming', 37);
  INSERT INTO users(id, username, password, name, workhoursPerWeek) VALUES(uuid_generate_v4(), 'heidi', '1treehill', 'Heidi', 32);
  ALTER TABLE work ADD userid uuid;
  UPDATE work SET userid=subquery.id FROM (SELECT * FROM users WHERE username='flemming') AS subquery;
  DELETE FROM dbVersion;
  INSERT INTO dbVersion(version) VALUES(2);

END IF;
END
$do$
