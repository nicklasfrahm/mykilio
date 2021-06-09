CREATE TABLE certificates (
  id uuid NOT NULL,
  pem text NOT NULL,
  thing_id uuid NOT NULL,
  created_at timestamp DEFAULT NOW(),
  updated_at timestamp DEFAULT NOW(),
  PRIMARY KEY(id),
  FOREIGN KEY (thing_id) REFERENCES things(id) ON DELETE CASCADE
);
