CREATE TABLE IF NOT EXISTS signature_schemes (
  id UUID PRIMARY KEY,
  name TEXT NOT NULL,
  algorithm TEXT NOT NULL,
  curve TEXT NOT NULL,
  hash TEXT NOT NULL
);

CREATE UNIQUE INDEX CONCURRENTLY signature_schemes_name 
ON signature_schemes (name);

ALTER TABLE signature_schemes 
ADD CONSTRAINT unique_name 
UNIQUE USING INDEX signature_schemes_name;

CREATE UNIQUE INDEX CONCURRENTLY signature_schemes_algorithm_curve_hash
ON signature_schemes (algorithm, curve, hash);

ALTER TABLE signature_schemes 
ADD CONSTRAINT unique_algoritm_curve_hash 
UNIQUE USING INDEX signature_schemes_algorithm_curve_hash;