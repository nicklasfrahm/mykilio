DROP TABLE IF EXISTS signature_schemes (
  id UUID PRIMARY KEY,
  name TEXT NOT NULL,
  algorithm TEXT NOT NULL,
  curve TEXT NOT NULL,
  hash TEXT NOT NULL
);