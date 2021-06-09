CREATE TABLE IF NOT EXISTS things (
  id uuid NOT NULL,
  name text NOT NULL,
  created_at timestamp DEFAULT NOW(),
  updated_at timestamp DEFAULT NOW(),
  PRIMARY KEY(id),
  UNIQUE(NAME)
);
