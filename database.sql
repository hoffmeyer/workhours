CREATE EXTENSION IF NOT EXISTS "uuid-ossp";
CREATE EXTENSION IF NOT EXISTS "pgcrypto";
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
  ALTER TABLE work ADD userid uuid;
  UPDATE work SET userid=subquery.id FROM (SELECT * FROM users WHERE username='flemming') AS subquery;
  DELETE FROM dbVersion;
  INSERT INTO dbVersion(version) VALUES(2);

END IF;

IF EXISTS (SELECT 1 FROM dbVersion WHERE version=2) THEN
  ALTER TABLE users ADD balanceFrom timestamp;
  UPDATE users set balanceFrom='2019-01-01 00:00:00';
  ALTER TABLE users ALTER COLUMN balanceFrom SET DEFAULT '2019-01-01 00:00:00';
  ALTER TABLE users ALTER COLUMN balanceFrom SET NOT NULL;
  DELETE FROM dbVersion;
  INSERT INTO dbVersion(version) VALUES(3);
END IF;

IF EXISTS (SELECT 1 FROM dbVersion WHERE version=3) THEN
  UPDATE users SET password = crypt(password, gen_salt('bf'));
  DELETE FROM dbVersion;
  INSERT INTO dbVersion(version) VALUES(4);
END IF;

END
$do$
