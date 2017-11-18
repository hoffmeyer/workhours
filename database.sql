CREATE TABLE IF NOT EXISTS work (id uuid PRIMARY KEY NOT NULL, start timestamp NOT NULL, duration real, break real );
CREATE TABLE IF NOT EXISTS dbVersion (version integer PRIMARY KEY NOT NULL);

DO
$do$
BEGIN
IF NOT EXISTS (SELECT 1 FROM dbVersion) THEN
  INSERT INTO dbVersion(version) VALUES(1);
  ALTER TABLE work ADD lunch real;
-- ELIF EXISTS (SELECT 1 FROM dbVersion WHERE version=1)
END IF;
END
$do$
